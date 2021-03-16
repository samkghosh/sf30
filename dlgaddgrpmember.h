#ifndef DLGADDGRPMEMBER_H
#define DLGADDGRPMEMBER_H

#include <QListWidgetItem>

#include <QDialog>

#include "dbaccess.h"

namespace Ui {
class DlgAddGrpMember;
}

class DlgAddGrpMember : public QDialog
{
    Q_OBJECT

public:
    explicit DlgAddGrpMember(QWidget *parent = 0);
    ~DlgAddGrpMember();
    void showMembers(QString name, int iType);

private slots:
    void on_pbCancel_clicked();
    void sendMemberToExtn(QListWidgetItem *item );
    void sendExtnToMemebr(QListWidgetItem *item );

private:
    Ui::DlgAddGrpMember *ui;

    void setMembers();
    QStringList members;
   // QMap<QString,Extension *> extensions;
    QMap<QString,Extension *> extnList;
    QString grpName;
    int iGrpType;



};

#endif // DLGADDGRPMEMBER_H
