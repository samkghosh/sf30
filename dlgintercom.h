#ifndef DLDINTERCOM_H
#define DLDINTERCOM_H

#include "dlgextensionconf.h"

#include <QDialog>
#include "dbaccess.h"

namespace Ui {
class DlgIntercom;
}



//class extnProp
//{
// public:
//    QPushButton *pb;
//    int iType;
//    int iStatus;
//};



class DlgIntercom : public QDialog
{
    Q_OBJECT

public:
    explicit DlgIntercom(QWidget *parent = nullptr);
    ~DlgIntercom();
    void setPos(int x, int y);
    int displayIntercomPage(QString extn);
    void setContext(int val);
    void changeExtnStatus(QString extn, int iType, int iStatus);
private slots:
    void on_pbExtn1_clicked();

    void on_pbExtn2_clicked();

    void on_pbExtn3_clicked();

    void on_pbExtn4_clicked();

    void on_pbExtn5_clicked();

    void on_pbExtn6_clicked();

    void on_pbExtn7_clicked();

    void on_pbExtn8_clicked();

    void on_pbExtn9_clicked();

    void on_pbExtn10_clicked();

    void on_pbExtn13_clicked();

    void on_pbExtn11_clicked();

    void on_pbExtn12_clicked();

    void on_pbExtn14_clicked();

    void on_pbExtn15_clicked();

    void on_pbExtn16_clicked();


Q_SIGNALS:
    void DialNumber(QString number);

private:

    Ui::DlgIntercom *ui;

    QMap<QString,Extension *> extensions;

    Extension extn;

    QMap<int, QPushButton *> interComList;

    void handlePbClick(QPushButton *pb);

    void displayContext();

    int iContext;
    QString myExtn;

//    DBAccess dba;

};

#endif // DLDINTERCOM_H
