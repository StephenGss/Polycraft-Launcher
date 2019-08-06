/* Copyright 2013-2018 PolycraftLauncher Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "AboutDialog.h"
#include "ui_AboutDialog.h"
#include <QIcon>
#include "PolycraftLauncher.h"
#include "BuildConfig.h"

#include <net/NetJob.h>

#include "HoeDown.h"

// Credits
// This is a hack, but I can't think of a better way to do this easily without screwing with QTextDocument...
static QString getCreditsHtml(QStringList patrons)
{
    QString creditsHtml = QObject::tr(
        "<!DOCTYPE HTML PUBLIC '-//W3C//DTD HTML 4.0//EN' 'http://www.w3.org/TR/REC-html40/strict.dtd'>"
        "<html>"
        ""
        "<head>"
        "<meta name='qrichtext' content='1' />"
        "<style type='text/css'>"
        "p { white-space: pre-wrap; margin-top:2px; margin-bottom:2px; }"
        "</style>"
        "</head>"
        ""
        "<body style=' font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;'>"
        ""
        "<h3>PolycraftLauncher Developers</h3>"
        "<p>Stephen Goss &lt;<a href='mailto:stephen@polycraftworld.com'>stephen@polycraftworld.com</a>&gt;</p>"
        ""
        "<h3>With thanks to</h3>"
        "<p>Eric Kildebeck &lt;&gt;</p>"
        "<p>Walter Voit &lt;&gt;</p>"
        ""
        "<h3></h3>"
        "%1"
        ""
        "</body>"
        "</html>");
    if (patrons.isEmpty())
        return creditsHtml.arg(QObject::tr("<p></p>"));
    else
    {
        QString patronsStr;
        for (QString patron : patrons)
        {
            patronsStr.append(QString("<p>%1</p>").arg(patron));
        }

        return creditsHtml.arg(patronsStr);
    }
}

static QString getLicenseHtml()
{
    HoeDown hoedown;
    QFile dataFile(":/documents/COPYING.md");
    dataFile.open(QIODevice::ReadOnly);
    QString output = hoedown.process(dataFile.readAll());
    return output;
}

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent), ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    QString chtml = getCreditsHtml(QStringList());
    ui->creditsText->setHtml(chtml);

    QString lhtml = getLicenseHtml();
    ui->licenseText->setHtml(lhtml);

    ui->urlLabel->setOpenExternalLinks(true);

    ui->icon->setPixmap(MMC->getThemedIcon("logo").pixmap(64));
    ui->title->setText("PolycraftLauncher");

    ui->versionLabel->setText(tr("Version") +": " + BuildConfig.printableVersionString());
    ui->platformLabel->setText(tr("Platform") +": " + BuildConfig.BUILD_PLATFORM);

    if (BuildConfig.VERSION_BUILD >= 0)
        ui->buildNumLabel->setText(tr("Build Number") +": " + QString::number(BuildConfig.VERSION_BUILD));
    else
        ui->buildNumLabel->setVisible(false);

    if (!BuildConfig.VERSION_CHANNEL.isEmpty())
        ui->channelLabel->setText(tr("Channel") +": " + BuildConfig.VERSION_CHANNEL);
    else
        ui->channelLabel->setVisible(false);

    connect(ui->closeButton, SIGNAL(clicked()), SLOT(close()));

    connect(ui->aboutQt, &QPushButton::clicked, &QApplication::aboutQt);

    //loadPatronList();
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::loadPatronList()
{
    netJob.reset(new NetJob("Patreon Patron List"));
    netJob->addNetAction(Net::Download::makeByteArray(QUrl("https://files.polycraftlauncher.org/patrons.txt"), &dataSink));
    connect(netJob.get(), &NetJob::succeeded, this, &AboutDialog::patronListLoaded);
    netJob->start();
}

void AboutDialog::patronListLoaded()
{
    QString patronListStr(dataSink);
    dataSink.clear();
    QString html = getCreditsHtml(patronListStr.split("\n", QString::SkipEmptyParts));
    ui->creditsText->setHtml(html);
}

