#pragma once

#include <QWidget>

namespace Ui
{
class PlayPolycraft;
}

class PlayPolycraft : public QWidget
{
    Q_OBJECT

public:
    explicit PlayPolycraft(QWidget *parent = 0);
    ~PlayPolycraft();
    void initialize(QStringList instances);

public slots:
    void updateInstances();

private slots:
    void on_pushButton_clicked();

private:
    Ui::PlayPolycraft *ui;
};

