#ifndef DLGPRESENCE_H
#define DLGPRESENCE_H

#include <QDialog>

//#include "dlgphone.h"

namespace Ui {
class DlgPresence;
}

class DlgPresence : public QDialog
{
    Q_OBJECT

public:
    explicit DlgPresence(QWidget *parent = nullptr);
    ~DlgPresence();
    void setPresence(int presence);

  //  void setParent(DlgPhone *ph);

private slots:
    void on_pbOk_clicked();

    void on_rbOnline_clicked();

    void on_rbOffline_clicked();

    void on_rbAway_clicked();

    void on_rbBusy_clicked();

   Q_SIGNALS:
    void changePresence(int sts);

private:
    Ui::DlgPresence *ui;
    int presence;
//    DlgPhone *dlgPhone;
};

#endif // DLGPRESENCE_H
