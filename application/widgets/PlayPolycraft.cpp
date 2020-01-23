#include "PlayPolycraft.h"
#include "ui_PlayPolycraft.h"
#include "PolycraftLauncher.h"
#include "InstanceList.h"
#include "ganalytics.h"

PlayPolycraft::~PlayPolycraft()
{
}

PlayPolycraft::PlayPolycraft(QWidget* parent):
    QWidget(parent),
    ui(new Ui::PlayPolycraft)
{
    ui->setupUi(this);

    connect(MMC->instances().get(), &InstanceList::instancesChanged, this, &PlayPolycraft::updateInstances);
}

void PlayPolycraft::initialize(QStringList instances)
{
//    ui->instances->clear();
//    ui->instances->addItems(instances);
    updateInstances();
}

void PlayPolycraft::on_pushButton_clicked()
{
//    ui->pushButton->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0.478, y1:1, x2:0.483, y2:0, stop:0.357955 rgba(45, 45, 45, 255), stop:1 rgba(198, 198, 198, 255));"
//                                  "background-image: none !important;");

    MMC->SendAnalyticsEvent("Launch", ui->instances->currentText(), "test", QVariant(1));
    MMC->launch(MMC->instances()->getInstanceById(ui->instances->currentText()));
}

void PlayPolycraft::updateInstances(){
    QStringList *instances = new QStringList();
    for(int instCount = MMC->instances()->count()-1; instCount >= 0; instCount--){
        instances->append(MMC->instances()->at(instCount)->id());
    }
    ui->instances->clear();
    ui->instances->addItems(*instances);

    for(int x = 0; x < ui->instances->count(); x++){
        if(!ui->instances->itemText(x).contains("beta"))
            ui->instances->setCurrentIndex(x);
    }
}
