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

#include "LoginDialog.h"
#include "ui_LoginDialog.h"

#include "minecraft/auth/YggdrasilTask.h"

#include <QtWidgets/QPushButton>

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent), ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    ui->progressBar->setVisible(false);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

// Stage 1.1: User interaction + test for Polycraft account
void LoginDialog::accept()
{
    setUserInputsEnabled(false);
    ui->progressBar->setVisible(true);

    // check for special Polycraft logins
    manager = new QNetworkAccessManager(this);

    QUrl url(BuildConfig.PCW_VERSION_URL + "/portal/exp_account/");
    //QUrl url("http://10.163.43.159:8000/portal/exp_account/");
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrlQuery params;

    params.addQueryItem("token", "DpBmRTBBp7P4X0Pn6qmM6NEhkbweM3R3O1k5N2mJAHCMrgwVjKqw5NRjgmTHOeqM");
    params.addQueryItem("polycraft_user_name", ui->userTextBox->text());
    params.addQueryItem("polycraft_pass", ui->passTextBox->text());
    // etc

    QObject::connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(checkForPolycraftAccount(QNetworkReply *)));

    manager->post(request, params.query().toUtf8());
}

// Stage 1.2: Minecraft account auth
void LoginDialog::accept2()
{
    // Setup the login task and start it
    m_account = MojangAccount::createFromUsername(ui->userTextBox->text());
    m_loginTask = m_account->login(nullptr, ui->passTextBox->text());
    connect(m_loginTask.get(), &Task::failed, this, &LoginDialog::onTaskFailed);
    connect(m_loginTask.get(), &Task::succeeded, this,
            &LoginDialog::onTaskSucceeded);
    connect(m_loginTask.get(), &Task::status, this, &LoginDialog::onTaskStatus);
    connect(m_loginTask.get(), &Task::progress, this, &LoginDialog::onTaskProgress);
    m_loginTask->start();
}

void LoginDialog::checkForPolycraftAccount(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError){
        QString strReply = (QString)reply->readAll();
        //parse json
        qDebug() << "Response:" << strReply;
        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());

        QJsonObject jsonObj = jsonResponse.object();

        //qDebug() << "email:" << jsonObj["email"].toString();
        //qDebug() << "minecraft_pass:" << jsonObj["minecraft_pass"].toString();
        ui->userTextBox->setText(jsonObj["email"].toString());
        ui->passTextBox->setText(jsonObj["minecraft_pass"].toString());
    }
    accept2();
}

void LoginDialog::setUserInputsEnabled(bool enable)
{
    ui->userTextBox->setEnabled(enable);
    ui->passTextBox->setEnabled(enable);
    ui->buttonBox->setEnabled(enable);
}

// Enable the OK button only when both textboxes contain something.
void LoginDialog::on_userTextBox_textEdited(const QString &newText)
{
    ui->buttonBox->button(QDialogButtonBox::Ok)
        ->setEnabled(!newText.isEmpty() && !ui->passTextBox->text().isEmpty());
}
void LoginDialog::on_passTextBox_textEdited(const QString &newText)
{
    ui->buttonBox->button(QDialogButtonBox::Ok)
        ->setEnabled(!newText.isEmpty() && !ui->userTextBox->text().isEmpty());
}

void LoginDialog::onTaskFailed(const QString &reason)
{
    // Set message
    ui->label->setText("<span style='color:red'>" + reason + "</span>");

    // Re-enable user-interaction
    setUserInputsEnabled(true);
    ui->progressBar->setVisible(false);
}

void LoginDialog::onTaskSucceeded()
{
    QDialog::accept();
}

void LoginDialog::onTaskStatus(const QString &status)
{
    ui->label->setText(status);
}

void LoginDialog::onTaskProgress(qint64 current, qint64 total)
{
    ui->progressBar->setMaximum(total);
    ui->progressBar->setValue(current);
}

// Public interface
MojangAccountPtr LoginDialog::newAccount(QWidget *parent, QString msg)
{
    LoginDialog dlg(parent);
    dlg.ui->label->setText(msg);
    if (dlg.exec() == QDialog::Accepted)
    {
        return dlg.m_account;
    }
    return 0;
}
