#ifndef DLGCALLFWD_H
#define DLGCALLFWD_H

#include <QDialog>

#include "dbaccess.h"


namespace Ui {
class DlgCallFwd;
}

class DlgCallFwd : public QDialog
{
    Q_OBJECT

public:
    explicit DlgCallFwd(QWidget *parent = nullptr);
    ~DlgCallFwd();

    void showCallFwd(QString extn);

private slots:

    void on_cbCallFwd_stateChanged(int arg1);

    void on_chkNoAnswer_stateChanged(int arg1);

    void on_chkOnBusy_stateChanged(int arg1);

    void on_rbNoAnsVM_clicked();

    void on_rbNoAnsExtn_clicked();

    void on_rbBusyOnVM_clicked();

    void on_rbBusyExtn_clicked();

    void on_pbSave_clicked();

    void on_pbCancel_clicked();

private:
    Ui::DlgCallFwd *ui;
    CallFWD callFwdInfo;
    QString myExtnName;
    QString myExtnNum;
    QMap<QString,Extension *> extensions;

};

#endif // DLGCALLFWD_H
