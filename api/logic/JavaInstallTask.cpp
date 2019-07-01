#include "JavaInstallTask.h"
#include "BaseInstance.h"
#include "FileSystem.h"
#include "Env.h"
#include "MMCZip.h"
#include "NullInstance.h"
#include "settings/INISettingsObject.h"
#include "icons/IIconList.h"
#include <QtConcurrentRun>

// FIXME: this does not belong here, it's Minecraft/Flame specific
#include "minecraft/MinecraftInstance.h"
#include "minecraft/ComponentList.h"
#include "modplatform/flame/FileResolvingTask.h"
#include "modplatform/flame/PackManifest.h"
#include "Json.h"

JavaInstallTask::JavaInstallTask(const QUrl sourceUrl)
{
    m_sourceUrl = sourceUrl;
}

void JavaInstallTask::executeTask()
{
    InstancePtr newInstance;

    setStatus(tr("Downloading Java runtime:\n%1").arg(m_sourceUrl.toString()));
    m_downloadRequired = true;

    const QString path = m_sourceUrl.host() + '/' + m_sourceUrl.path();
    auto entry = ENV.metacache()->resolveEntry("general", path);
    entry->setStale(true);
    m_filesNetJob.reset(new NetJob(tr("Java download")));
    m_filesNetJob->addNetAction(Net::Download::makeCached(m_sourceUrl, entry));
    m_archivePath = entry->getFullPath();
    auto job = m_filesNetJob.get();
    connect(job, &NetJob::succeeded, this, &JavaInstallTask::downloadSucceeded);
    connect(job, &NetJob::progress, this, &JavaInstallTask::downloadProgressChanged);
    connect(job, &NetJob::failed, this, &JavaInstallTask::downloadFailed);
    m_filesNetJob->start();
}

void JavaInstallTask::downloadSucceeded()
{
    processZipPack();
    m_filesNetJob.reset();
}

void JavaInstallTask::downloadFailed(QString reason)
{
    emitFailed(reason);
    m_filesNetJob.reset();
}

void JavaInstallTask::downloadProgressChanged(qint64 current, qint64 total)
{
    setProgress(current / 2, total);
}

void JavaInstallTask::processZipPack()
{
    setStatus(tr("Extracting Java"));
    QDir extractDir(m_stagingPath);
    qDebug() << "Attempting to create instance from" << m_archivePath;

    // open the zip and find relevant files in it
    m_packZip.reset(new QuaZip(m_archivePath));
    if (!m_packZip->open(QuaZip::mdUnzip))
    {
        emitFailed(tr("Unable to open supplied modpack zip file."));
        return;
    }

    QString root;

    // make sure we extract just the pack
    m_extractFuture = QtConcurrent::run(QThreadPool::globalInstance(), MMCZip::extractSubDir, m_packZip.get(), root, extractDir.absolutePath());
    connect(&m_extractFutureWatcher, &QFutureWatcher<QStringList>::finished, this, &JavaInstallTask::extractFinished);
    connect(&m_extractFutureWatcher, &QFutureWatcher<QStringList>::canceled, this, &JavaInstallTask::extractAborted);
    m_extractFutureWatcher.setFuture(m_extractFuture);
}

void JavaInstallTask::extractFinished()
{
    m_packZip.reset();
    if (m_extractFuture.result().isEmpty())
    {
        emitFailed(tr("Failed to extract modpack"));
        return;
    }
    QDir extractDir(m_stagingPath);

    qDebug() << "Fixing permissions for extracted pack files...";
    QDirIterator it(extractDir, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        auto filepath = it.next();
        QFileInfo file(filepath);
        auto permissions = QFile::permissions(filepath);
        auto origPermissions = permissions;
        if(file.isDir())
        {
            // Folder +rwx for current user
            permissions |= QFileDevice::Permission::ReadUser | QFileDevice::Permission::WriteUser | QFileDevice::Permission::ExeUser;
        }
        else
        {
            // File +rw for current user
            permissions |= QFileDevice::Permission::ReadUser | QFileDevice::Permission::WriteUser;
        }
        if(origPermissions != permissions)
        {
            if(!QFile::setPermissions(filepath, permissions))
            {
                logWarning(tr("Could not fix permissions for %1").arg(filepath));
            }
            else
            {
                qDebug() << "Fixed" << filepath;
            }
        }
    }

    processJavaInstall();
}

void JavaInstallTask::extractAborted()
{
    emitFailed(tr("Instance import has been aborted."));
    return;
}

void JavaInstallTask::processJavaInstall()
{
    emitSucceeded();
}
