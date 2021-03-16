#ifndef SPEEDDIALDIALOG_H
#define SPEEDDIALDIALOG_H

#include <QDialog>

#include "dataprocessing.h"
#include "dbaccess.h"

namespace Ui {
class DlgSpeedDial;
}

class DlgSpeedDial: public QDialog
{
    Q_OBJECT

public:
    explicit DlgSpeedDial(QWidget *parent = nullptr);
    ~DlgSpeedDial();

    void getValues(SpeedDialData &sd);
    void setPos(const int x, const int y);

    //    void setSpeedDialData(SpeedDialData &sd);
    void updateAndShow(QString extnName);

private slots:
    void on_pbOk_clicked();

    void on_pbCancel_clicked();

private:
    Ui::DlgSpeedDial *ui;
    SpeedDialData sd;
    DataProcessing dp;

    QMap<QString,Extension *> extensions;

    QMap<QString, QString>extnNameToNum;

    QString myExtnName;
    QString myExtnNum;
};

#endif // SPEEDDIALDIALOG_H
