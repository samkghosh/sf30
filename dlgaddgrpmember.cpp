#include "dlgaddgrpmember.h"
#include "ui_dlgaddgrpmember.h"

#include <QMessageBox>

#include "commapi.h"


DlgAddGrpMember::DlgAddGrpMember(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgAddGrpMember)
{
    ui->setupUi(this);
    setWindowFlag(Qt::WindowStaysOnTopHint);

    connect(ui->lstExtns,SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(sendExtnToMemebr(QListWidgetItem *)));
    connect(ui->lstMembers,SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(sendMemberToExtn(QListWidgetItem *)));


}

DlgAddGrpMember::~DlgAddGrpMember()
{
    delete ui;
}

void DlgAddGrpMember::setMembers()
{
    QMap<QString,Extension *>::iterator itr;

    QListWidgetItem *lstItem;
    QString key;

    QString grpMem;
    QStringList grpList;

    int iExits;

    ui->lstExtns->clear();
    ui->lstMembers->clear();

    members.clear();
 //   empty();

    commapi capi("192.168.100.127", 9000);

    if ( capi.readExtensionData(extnList) < 0)
    {
        QMessageBox::warning(this,"Group Members", "Unable to get List");
        return;
    }

    //TBD::
 /*   if ( dba->readACDMembers(members, acdName) < 0)
    {
        QMessageBox::warning(this,"ACD Members", "Unable to get Existing Members");
        return;
    }

*/
    // add the List into ist list
//    qDebug() << "Members has: " << members.count();

    if ( members.count() > 0)
    {
        grpMem = members[0];
    }

    for (itr = extnList.begin(); itr != extnList.end(); itr++)
    {
        // chek if it is in members
        key = itr.key();
        iExits=0;

        for(int i=0; i < members.count(); i++)
        {
            if (members[i] == key)
            {
                iExits =1;
                break;
            }
        }

        if (iExits == 0)
        {
            // add it here
            lstItem = new QListWidgetItem();
            lstItem->setText(extnList[key]->endPoint.name);
            ui->lstExtns->addItem(lstItem);
        }
    }

    //add the members in the second list
    for (int i=0; i < members.count(); i++)
    {
       // add it here
        lstItem = new QListWidgetItem();
        lstItem->setText(extnList[members[i]]->endPoint.name);
        ui->lstMembers->addItem(lstItem);
    }


}


void DlgAddGrpMember::showMembers(QString name, int iType)
{
    grpName = name;
    iGrpType = iType;

  //  QMessageBox::information(this,"ACD Name", acdName);
    setMembers();
//    ui->lbACDName->setText("ACD/Ring Group Name: "+acdName);
    show();

}



void DlgAddGrpMember::sendExtnToMemebr(QListWidgetItem *item )
{
    QListWidgetItem *listitem = new QListWidgetItem();
    listitem->setText(item->text());
    ui->lstMembers->addItem(listitem);
//    ui->lstExtns->removeItemWidget(item);
    delete item;
}



void DlgAddGrpMember::sendMemberToExtn(QListWidgetItem *item )
{
    QListWidgetItem *listitem = new QListWidgetItem();
    listitem->setText(item->text());
    ui->lstExtns->addItem(listitem);
    delete item;
}

void DlgAddGrpMember::on_pbCancel_clicked()
{
    close();
}
