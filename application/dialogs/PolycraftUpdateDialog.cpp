#include "PolycraftUpdateDialog.h"
#include "ui_PolycraftUpdateDialog.h"

#include <MultiMC.h>

PolycraftUpdateDialog::PolycraftUpdateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PolycraftUpdateDialog)
{
    ui->setupUi(this);
}


PolycraftUpdateDialog::~PolycraftUpdateDialog()
{
    delete ui;
}

void PolycraftUpdateDialog::initialize(QList<version> versions){
    this->versions = versions;
    QString labelText = "";
    foreach(const struct version & v, versions){
        if(v.name.toLower().compare("release")){
            labelText = "There is a new versions of Polycraft World\nWould you like to update to version:\'" + v.version + "\'?";
        }
    }
    this->ui->label->setText(labelText);
}

void PolycraftUpdateDialog::on_updateDialog_accepted()
{
    //delete old instances and install new instances
    MMC->updatePolycraft(versions);
    close();
}
