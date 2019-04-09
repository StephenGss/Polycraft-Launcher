#pragma once

#include "BaseProfiler.h"

#include "polycraftlauncher_logic_export.h"

class POLYCRAFTLAUNCHER_LOGIC_EXPORT JVisualVMFactory : public BaseProfilerFactory
{
public:
    QString name() const override { return "JVisualVM"; }
    void registerSettings(SettingsObjectPtr settings) override;
    BaseExternalTool *createTool(InstancePtr instance, QObject *parent = 0) override;
    bool check(QString *error) override;
    bool check(const QString &path, QString *error) override;
};
