#include "JavaInstallStaging.h"

JavaInstallStaging::JavaInstallStaging (
        JavaSettingsWidget * parent,
        Task * child,
        const QString & stagingPath)
    : backoff(minBackoff, maxBackoff)
    {
        m_parent = parent;
        m_child.reset(child);
        connect(child, &Task::succeeded, this, &JavaInstallStaging::childSucceded);
        connect(child, &Task::failed, this, &JavaInstallStaging::childFailed);
        connect(child, &Task::status, this, &JavaInstallStaging::setStatus);
        connect(child, &Task::progress, this, &JavaInstallStaging::setProgress);
        m_stagingPath = stagingPath;
        m_backoffTimer.setSingleShot(true);
        connect(&m_backoffTimer, &QTimer::timeout, this, &JavaInstallStaging::childSucceded);
    }
