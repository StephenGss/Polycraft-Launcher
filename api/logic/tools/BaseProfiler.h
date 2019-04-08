#pragma once

#include "BaseExternalTool.h"

#include "polycraftlauncher_logic_export.h"

class BaseInstance;
class SettingsObject;
class LaunchTask;
class QProcess;

class POLYCRAFTLAUNCHER_LOGIC_EXPORT BaseProfiler : public BaseExternalTool
{
    Q_OBJECT
public:
    explicit BaseProfiler(SettingsObjectPtr settings, InstancePtr instance, QObject *parent = 0);

public
slots:
    void beginProfiling(std::shared_ptr<LaunchTask> process);
    void abortProfiling();

protected:
    QProcess *m_profilerProcess;

    virtual void beginProfilingImpl(std::shared_ptr<LaunchTask> process) = 0;
    virtual void abortProfilingImpl();

signals:
    void readyToLaunch(const QString &message);
    void abortLaunch(const QString &message);
};

class POLYCRAFTLAUNCHER_LOGIC_EXPORT BaseProfilerFactory : public BaseExternalToolFactory
{
public:
    virtual BaseProfiler *createProfiler(InstancePtr instance, QObject *parent = 0);
};
