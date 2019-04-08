#include "PlayPolycraft.h"
#include "ui_PlayPolycraft.h"
#include "PolycraftLauncher.h"
#include "InstanceList.h"

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
