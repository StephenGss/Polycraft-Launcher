#include "TechnicPage.h"
#include "ui_TechnicPage.h"

#include "PolycraftLauncher.h"
#include "dialogs/NewInstanceDialog.h"

TechnicPage::TechnicPage(NewInstanceDialog* dialog, QWidget *parent)
    : QWidget(parent), ui(new Ui::TechnicPage), dialog(dialog)
{
    ui->setupUi(this);
}

TechnicPage::~TechnicPage()
{
    delete ui;
}

bool TechnicPage::shouldDisplay() const
{
    return true;
}

void TechnicPage::openedImpl()
{
    dialog->setSuggestedPack();
}
