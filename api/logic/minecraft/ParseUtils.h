#pragma once
#include <QString>
#include <QDateTime>

#include "polycraftlauncher_logic_export.h"

/// take the timestamp used by S3 and turn it into QDateTime
POLYCRAFTLAUNCHER_LOGIC_EXPORT QDateTime timeFromS3Time(QString str);

/// take a timestamp and convert it into an S3 timestamp
POLYCRAFTLAUNCHER_LOGIC_EXPORT QString timeToS3Time(QDateTime);
