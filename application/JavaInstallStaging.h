#ifndef JAVAINSTALLSTAGING_H
#define JAVAINSTALLSTAGING_H


#include <InstanceTask.h>
#include <ExponentialSeries.h>
#include <QTimer>
#include <JavaInstallTask.h>
#include <widgets/JavaSettingsWidget.h>

class JavaInstallStaging : public Task
{
Q_OBJECT
    const unsigned minBackoff = 1;
    const unsigned maxBackoff = 16;
public:
    JavaInstallStaging (
        JavaSettingsWidget * parent,
        Task * child,
        const QString & stagingPath);


    virtual ~JavaInstallStaging() {}


    // FIXME/TODO: add ability to abort during instance commit retries
    bool abort() override
    {
        if(m_child && m_child->canAbort())
        {
            return m_child->abort();
        }
        return false;
    }
    bool canAbort() const override
    {
        if(m_child && m_child->canAbort())
        {
            return true;
        }
        return false;
    }

protected:
    virtual void executeTask() override
    {
        m_child->start();
    }
    QStringList warnings() const override
    {
        return m_child->warnings();
    }

private slots:
    void childSucceded()
    {
        unsigned sleepTime = backoff();
        if(m_parent->commitStagedJavaInstall())
        {
            emitSucceeded();
            return;
        }
        // we actually failed, retry?
        if(sleepTime == maxBackoff)
        {
            emitFailed(tr("Failed to install java. Please contact support or manually install Java."));
            return;
        }
        qDebug() << "Failed to commit Java Install :: Initiating backoff:" << sleepTime;
        m_backoffTimer.start(sleepTime * 500);
    }
    void childFailed(const QString & reason)
    {
        //m_parent->destroyStagingPath(m_stagingPath);
        emitFailed(reason);
    }

private:
    /*
     * WHY: the whole reason why this uses an exponential backoff retry scheme is antivirus on Windows.
     * Basically, it starts messing things up while PolycraftLauncher is extracting/creating instances
     * and causes that horrible failure that is NTFS to lock files in place because they are open.
     */
    ExponentialSeries backoff;
    QString m_stagingPath;
    JavaSettingsWidget * m_parent;
    unique_qobject_ptr<Task> m_child;
    QTimer m_backoffTimer;
};

#endif // JAVAINSTALLSTAGING_H
