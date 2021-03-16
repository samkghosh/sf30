#ifndef DLGREPORT_H
#define DLGREPORT_H

#include "dbaccess.h"

#include <QDialog>

namespace Ui {
class DlgReport;
}

class DlgReport : public QDialog
{
    Q_OBJECT

public:
    explicit DlgReport(QWidget *parent = nullptr);
    ~DlgReport();
    void displayCDR();

//    DBAccess *dba = nullptr;

    void displayRecording();
private slots:
    void on_tabWidget_tabBarClicked(int index);

private:
    Ui::DlgReport *ui;
    QList<CDR *> cdrs;
};

#endif // DLGREPORT_H
