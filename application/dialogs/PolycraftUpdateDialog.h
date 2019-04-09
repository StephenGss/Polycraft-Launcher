#ifndef POLYCRAFTUPDATE_H
#define POLYCRAFTUPDATE_H

#include <QDialog>

namespace Ui {
class PolycraftUpdateDialog;
}

class PolycraftUpdateDialog : public QDialog
{
    Q_OBJECT
public:

    struct version{
        QString name;
        QString version;
        QString url;
    };
public:
    explicit PolycraftUpdateDialog(QWidget *parent = nullptr);
    ~PolycraftUpdateDialog();
    void initialize(QList<version> versions);

private slots:
    void on_updateDialog_accepted();

private:
    Ui::PolycraftUpdateDialog *ui;
    QList<version> versions;
};

#endif // POLYCRAFTUPDATE_H
