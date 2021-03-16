#ifndef DLGCHAT_H
#define DLGCHAT_H

#include <QDialog>

#include "const.h"
#include "dlgaddgrpmember.h"

typedef struct
{
    QString extn;
    QString name;
    int iType;
}T_ChatMember;

namespace Ui {
class DlgChat;
}

class DlgChat : public QDialog
{
    Q_OBJECT

public:
    explicit DlgChat(QWidget *parent = nullptr);
    ~DlgChat();

private slots:
    void on_pbCancel_clicked();

    void on_pbDel_clicked();

    void on_pbGrpAdd_clicked();

    void on_tabChat_tabBarClicked(int index);

    void on_tblGroup_cellDoubleClicked(int row, int column);

    void on_pbBack_clicked();

    void on_pbSend_clicked();

    void on_pbClear_clicked();

    void on_pbXfrFile_clicked();

private:
    Ui::DlgChat *ui;

    void processGroup();
    void processChatSession();

    QList<T_Group *> grpList;
    DlgAddGrpMember *dlgAddMember = nullptr;
    QMap<QString,T_ChatMember *>chatMember;
    QMap<QString,Extension *> extnList;

};

#endif // DLGCHAT_H
