#include <QMessageBox>

#include "dlgchat.h"

#include "ui_dlgchat.h"
#include "commapi.h"

DlgChat::DlgChat(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgChat)
{
    ui->setupUi(this);

    ui->tabChat->setTabEnabled(2,true);

    ui->tabChat->setCurrentIndex(1);
    ui->tabChatSession->show();

    if ( ui->tabGroup->isEnabled())
        processGroup();

    if ( ui->tabChatSession->isEnabled())
        processChatSession();

}

DlgChat::~DlgChat()
{
    delete ui;
}

void DlgChat::on_pbCancel_clicked()
{
    close();
}

void DlgChat::on_pbDel_clicked()
{
    int i=0;

//   qDebug() << "Inside Del";

    for(i=0; i < ui->tblGroup->rowCount(); i++)
    {

      qDebug() << "RowCnt:" << ui->tblGroup->rowCount() <<"Lbl:" <<ui->tblGroup->itemAt(0,i)->text() ;
        if ( ui->tblGroup->item(i,0)->checkState()== Qt::Checked)
        {
    //        qDebug() << "Row:" << i << " Selected.";

              //TBD:: delete from Database
            ui->tblGroup->removeRow(i);

            grpList.removeAt(i);

            QMessageBox::information(this,"Group", "Row:"+QString::number(i));
        }
    }

}

void DlgChat::on_pbGrpAdd_clicked()
{
    QString grpName;
    T_Group *grp;
    int i=0;

    grpName = ui->leAddGrp->text();

    if ( grpName == "")
    {
        QMessageBox::warning(this, "Add Group", "Group Name is blank. Enter a group name.");
        return;
    }

    for( i=0; i < grpList.count(); i++)
    {
           if ( grpName == grpList[i]->szGroupName)
           {
               QMessageBox::warning(this, "Add Group", "Group Name already exists. Enther another one.");
               return;
           }
    }

    /* Not There */
    //TBD: aad the grop in to database

    int iColCount=0;

    int iRowCount = ui->tblGroup->rowCount();
    ui->tblGroup->setRowCount(iRowCount);

    int rc = 0;
    if (rc == 0)
    {
        grp = new T_Group;
        grp->id=grpList[i-1]->id+1;
        grp->iGroupType = GRP_CONF;
        strcpy(grp->szGroupName,grpName.toLatin1().data());

        grpList.insert(i, grp);

        QTableWidgetItem *select = new QTableWidgetItem();
        QTableWidgetItem *name = new QTableWidgetItem();

        name->setText(grpList[i]->szGroupName);

        ui->tblGroup->insertRow(iRowCount);

        ui->tblGroup->setItem(iRowCount, iColCount++, select);
        select->setCheckState(Qt::Unchecked);

        ui->tblGroup->setItem(iRowCount, iColCount++, name);


        QMessageBox::information(this, "Add Group", "Group added successfully.");

    }
    else
    {
        QMessageBox::warning(this, "Add Group", "Unable to Add the group.");

    }

}



void DlgChat::processGroup()
{
    T_Group *grp;
    QStringList hdr;
    int i;


    // get the group information
    grp = new T_Group;
    grp->id=1;
    grp->iGroupType = GRP_CHAT;
    strcpy(grp->szGroupName,"Chat Grp 1");

    grpList.clear();
    grpList.empty();

    grpList.insert(0,grp);
    grp = new T_Group;

    grp->id=2;
    grp->iGroupType = GRP_CHAT;
    strcpy(grp->szGroupName,"Chat Grp 2");
    grpList.insert(1,grp);


    // display group information
    hdr <<"" <<"Group Name";

    int iRowCount = ui->tblGroup->rowCount();

    for(i = 0; i < iRowCount ; i++)
        ui->tblGroup->removeRow(i);
    iRowCount = 0;
    ui->tblGroup->setRowCount(iRowCount);

    ui->tblGroup->setColumnCount(2);
    ui->tblGroup->setHorizontalHeaderLabels(hdr);

    int iColCount;

    for(i=0; i < grpList.count(); i++)
    {
        iColCount = 0;
        QTableWidgetItem *select = new QTableWidgetItem();
        QTableWidgetItem *name = new QTableWidgetItem();

        name->setText(grpList[i]->szGroupName);

        ui->tblGroup->insertRow(i);

        ui->tblGroup->setItem(i, iColCount++, select);
        select->setCheckState(Qt::Unchecked);

        ui->tblGroup->setItem(i, iColCount++, name);
    }
}


void DlgChat::on_tabChat_tabBarClicked(int index)
{
    switch(index)
    {
        case 0:// process Group
            processGroup();
            break;

        case 1://Process Schedule
            break;
    }

}

void DlgChat::on_tblGroup_cellDoubleClicked(int row, int column)
{
    QString grpName = ui->tblGroup->item(row,1)->text();

    if ( dlgAddMember == nullptr)
    {
        dlgAddMember = new DlgAddGrpMember;
    }
    dlgAddMember->showMembers(grpName,GRP_CHAT);

}

void DlgChat::on_pbBack_clicked()
{
    close();
}


void DlgChat::processChatSession()
{
    QMap<QString,Extension *>::Iterator itr;
    QListWidgetItem *lstItem;

    commapi capi("192.168.100.127", 9000);

    if ( capi.readExtensionData(extnList) < 0)
    {
        QMessageBox::warning(this,"Group Members", "Unable to get List");
        return;
    }

    int i =0;
    for (itr = extnList.begin(); itr != extnList.end(); itr++)
    {
        T_ChatMember *chatMbr = new T_ChatMember;

        chatMbr->extn = itr.key();
        chatMbr->name = itr.value()->endPoint.name;
        chatMbr->iType=0;

        chatMember.insert(chatMbr->name, chatMbr);
        lstItem = new QListWidgetItem;
        lstItem->setText(chatMbr->name);

        i++;
        if ( i % 2 == 1)
            lstItem->setBackground(Qt::lightGray);


        ui->lstChatMember->addItem(lstItem);
    }

    //TBD:://get the list of chat group



}


void DlgChat::on_pbSend_clicked()
{
    QString msg;
    int iRowCount;
    QListWidgetItem *lstItem;
    QDateTime currDtTime = QDateTime::currentDateTime();

    QString dtTime = currDtTime.toString("h:m:s ap");




    msg = ui->edSendTxt->toPlainText();

    if ( msg == "")
        return;

    lstItem = new QListWidgetItem;
    lstItem->setBackgroundColor(Qt::gray);
//    lstItem->setFont(Qt::)
    lstItem->setText("[" +dtTime+ "]");
    ui->lstChatMsg->addItem(lstItem);

    lstItem = new QListWidgetItem;
    lstItem->setBackgroundColor(QColor(COLOUR_GREEN));
//    lstItem->setStyleSheet(QString("QPushButton {background-color: %1}").arg(COLOUR_RED));

    lstItem->setText(msg);
    ui->lstChatMsg->addItem(lstItem);
    ui->edSendTxt->clear();

    // test
    QString oParty = "Foo";
    lstItem = new QListWidgetItem;
    lstItem->setBackgroundColor(Qt::gray);
    lstItem->setText(oParty + "   ["+dtTime+"]");
    ui->lstChatMsg->addItem(lstItem);
    lstItem = new QListWidgetItem;
//    lstItem->setBackgroundColor(QColor(COLOUR_GREEN));
//    lstItem->setStyleSheet(QString("QPushButton {background-color: %1}").arg(COLOUR_RED));

    lstItem->setText("Hi I am there.");
    ui->lstChatMsg->addItem(lstItem);
    ui->edSendTxt->clear();

}

void DlgChat::on_pbClear_clicked()
{
    ui->lstChatMsg->clear();
}

void DlgChat::on_pbXfrFile_clicked()
{

}
