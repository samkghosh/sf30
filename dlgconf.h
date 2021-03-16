#ifndef DLGCONF_H
#define DLGCONF_H

#include <QDialog>

#include "const.h"
#include "dlgaddgrpmember.h"

namespace Ui {
class DlgConf;
}

class DlgConf : public QDialog
{
    Q_OBJECT

public:
    explicit DlgConf(QWidget *parent = nullptr);
    ~DlgConf();

private slots:
    void on_pbCancel_clicked();

    void on_tabConf_tabBarClicked(int index);

    void on_pbAddGrp_clicked();

    void on_pbDel_clicked();

    void on_tblGroup_cellDoubleClicked(int row, int column);

    void on_pbCancel_2_clicked();

    void on_pbDel_2_clicked();

    void on_pbCreate_clicked();

    void on_pbStart_clicked();

private:
    Ui::DlgConf *ui;

    void processGroup();

    void processConfSchedule();

    QList<T_Group *> grpList;
    DlgAddGrpMember *dlgAddMember = nullptr;

    QList<T_Schedule *> schList;

    bool bConfStarted = false;
    void processStatConf(QString grpName);

    QMap<QString,Extension *> extnList;
    int getExtensionList();

};

#endif // DLGCONF_H
