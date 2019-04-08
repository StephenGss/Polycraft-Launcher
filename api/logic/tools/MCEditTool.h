#pragma once

#include <QString>
#include "settings/SettingsObject.h"
#include "polycraftlauncher_logic_export.h"

class POLYCRAFTLAUNCHER_LOGIC_EXPORT MCEditTool
{
public:
    MCEditTool(SettingsObjectPtr settings);
    void setPath(QString & path);
    QString path() const;
    bool check(const QString &toolPath, QString &error);
    QString getProgramPath();
private:
    SettingsObjectPtr m_settings;
};
