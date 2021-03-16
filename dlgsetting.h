#ifndef DLGSETTING_H
#define DLGSETTING_H

#include "dataprocessing.h"
#include "dlglog.h"

#include <QDialog>

namespace Ui {
class DlgSetting;
}

class DlgSetting : public QDialog
{
    Q_OBJECT

public:
    explicit DlgSetting(QWidget *parent = nullptr);
    ~DlgSetting();


    void setLogWindow(DlgLog *dlg);
    DataProcessing dp;

    PhoneData pd;

private slots:
    void on_pbCreate_clicked();

private:
    Ui::DlgSetting *ui;
    DlgLog *dlgLog=nullptr;
};

#endif // DLGSETTING_H
