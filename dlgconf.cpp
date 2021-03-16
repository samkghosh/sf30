#include "dlgconf.h"
#include "ui_dlgconf.h"
#include "QMessageBox"
#include "QDebug"
#include "commapi.h"

DlgConf::DlgConf(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgConf)
{
    ui->setupUi(this);

    ui->tabConf->setTabEnabled(1,true);
    ui->tabConf->setCurrentIndex(0);
    ui->tabGroup->show();

    if ( ui->tabGroup->isEnabled())
        processGroup();
    if ( ui->tabSchedule->isEnabled())
        processConfSchedule();
}

DlgConf::~DlgConf()
{
    delete ui;
}

void DlgConf::on_pbCancel_clicked()
{
    close();
}


int DlgConf::getExtensionList()
{

    commapi capi("192.168.100.127", 9000);

    return capi.readExtensionData(extnList);

}


void DlgConf::on_tabConf_tabBarClicked(int index)
{
    switch(index)
    {
        case 0:// process Group
            processGroup();
            break;

        case 1://Process Schedule
            processConfSchedule();
            break;

        case 2:
            break;
    case 3:
        if (bConfStarted == false)
        {
            processStatConf("Test");
//            QMessageBox::warning(this,"Conf Start", "No Conference is initiated. Select Scheduled confernce and then start it.");

        }
        else
        {
            processStatConf("Test");

        }
        break;

    }

}


/*****************************************************/
void DlgConf::processGroup()
{
    T_Group *grp;
    QStringList hdr;
    int i;


    // get the group information
    grp = new T_Group;
    grp->id=1;
    grp->iGroupType = GRP_CONF;
    strcpy(grp->szGroupName,"Conf Grp 1");

    grpList.clear();
    grpList.empty();

    grpList.insert(0,grp);
    grp = new T_Group;

    grp->id=2;
    grp->iGroupType = GRP_CONF;
    strcpy(grp->szGroupName,"Conf Grp 2");
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


/****************************************************/
void DlgConf::processConfSchedule()
{
    T_Schedule *sch;
    QStringList hdr;
    int i;


    // get the group information
    sch = new T_Schedule;
    sch->id=1;

    sch->iMedia = MEDIA_AUDIO;
    strcpy(sch->szName,"Conf Sch 1");
    strcpy(sch->szGroup,"Grop -1");
    sch->iType = CONF_TYPE_IN;
    strcpy(sch->szDate,"03/04/2021 12:30");
    sch->iModeratorLess = 0;
    sch->iConfRoom = 1234;
    sch->iPin=1234;
    sch->iRecConf =1;


    schList.clear();
    schList.empty();

    schList.insert(0,sch);

    sch = new T_Schedule;
    sch->id=2;

    sch->iMedia = MEDIA_VIDEO;
    strcpy(sch->szName,"Conf Sch 2");
    strcpy(sch->szGroup,"Grop -2");
    sch->iType = CONF_TYPE_OUT;
    strcpy(sch->szDate,"03/04/2021 15:30");
    sch->iModeratorLess = 0;
    sch->iConfRoom = 1235;
    sch->iPin=1235;
    sch->iRecConf=0;
    schList.insert(1,sch);


    // display group information
    hdr <<"" <<"Conf Name" << "Group" << "Date" << "Type" << "Mode" << "Record" << "Conf Room" << "Pin";

    int iRowCount = ui->tblSchedule->rowCount();

    for(i = 0; i < iRowCount ; i++)
        ui->tblSchedule->removeRow(i);
    iRowCount = 0;

    ui->tblSchedule->setRowCount(iRowCount);

    ui->tblSchedule->setColumnCount(9);

    ui->tblSchedule->setHorizontalHeaderLabels(hdr);

    int iColCount;

    for(i=0; i < schList.count(); i++)
    {
        iColCount = 0;
        QTableWidgetItem *select = new QTableWidgetItem();
        QTableWidgetItem *name = new QTableWidgetItem();
        QTableWidgetItem *group= new QTableWidgetItem();

        QTableWidgetItem *dt = new QTableWidgetItem();
        QTableWidgetItem *type = new QTableWidgetItem();
        QTableWidgetItem *mode = new QTableWidgetItem();
        QTableWidgetItem *rec = new QTableWidgetItem();
        QTableWidgetItem *room = new QTableWidgetItem();
        QTableWidgetItem *pin = new QTableWidgetItem();

        name->setText(schList[i]->szName);
        group->setText(schList[i]->szGroup);
        dt->setText(schList[i]->szDate);
        if ( schList[i]->iType == CONF_TYPE_IN)
            type->setText("IN");
        else
            type->setText("OUT");

        if ( schList[i]->iMode == MEDIA_AUDIO)
            mode->setText("Audio");
        else
            mode->setText("Video");

        if ( schList[i]->iRecConf == 0)
            rec->setText("No");
        else
            rec->setText("Yes");

        room->setText(QString::number(schList[i]->iConfRoom));
        pin->setText(QString::number(schList[i]->iPin));


        ui->tblSchedule->insertRow(i);

        ui->tblSchedule->setItem(i, iColCount++, select);
        select->setCheckState(Qt::Unchecked);
//        hdr <<"" <<"Conf Name" << "Group" << "Date" << "Type" << "Mode" << "Record" << "Conf Room" << "Pin";

        ui->tblSchedule->setItem(i, iColCount++, name);
        ui->tblSchedule->setItem(i, iColCount++, group);
        ui->tblSchedule->setItem(i, iColCount++, dt);
        ui->tblSchedule->setItem(i, iColCount++, type);
        ui->tblSchedule->setItem(i, iColCount++, mode);
        ui->tblSchedule->setItem(i, iColCount++, rec);
        ui->tblSchedule->setItem(i, iColCount++, room);
        ui->tblSchedule->setItem(i, iColCount++, pin);
    }


}

void DlgConf::on_pbAddGrp_clicked()
{
    QString grpName;
    T_Group *grp;
    int i=0;

    grpName = ui->leGroupName->text();

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

void DlgConf::on_pbDel_clicked()
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

void DlgConf::on_tblGroup_cellDoubleClicked(int row, int column)
{
    QString grpName = ui->tblGroup->item(row,1)->text();

    if ( dlgAddMember == nullptr)
    {
        dlgAddMember = new DlgAddGrpMember;
    }
    dlgAddMember->showMembers(grpName, GRP_CONF);
//    QMessageBox::information(this, "Add Members", "Group"+ui->tblGroup->item(row,1)->text());
}

void DlgConf::on_pbCancel_2_clicked()
{
    close();
}

void DlgConf::on_pbDel_2_clicked()
{

}

void DlgConf::on_pbCreate_clicked()
{

}


void DlgConf::on_pbStart_clicked()
{
    int i=0;
    int iFound =0;
    QString grpName;

    for(i=0; i < ui->tblSchedule->rowCount(); i++)
    {

    //  qDebug() << "RowCnt:" << ui->tblGroup->rowCount() <<"Lbl:" <<ui->tblGroup->itemAt(0,i)->text() ;
        if ( ui->tblSchedule->item(i,0)->checkState()== Qt::Checked)
        {
            iFound = 1;
            break;
        }
    }

    if ( iFound == 0)
    {
        QMessageBox::warning(this,"Conf Sch", "Select a conference to Start.");
        return;
    }

    // Check the date, if conf to be started, else display error message

    // get the group name and take to start conf
    ui->tabSchedule->hide();

    //ui->tabConf->setTabEnabled(4,true);
    ui->tabConf->setCurrentIndex(3);

    ui->tabStart->show();

    bConfStarted = true;
    grpName=ui->tblSchedule->item(i,2)->text();

    processStatConf(grpName);
}


/*******************************************************************
 * *
 * ******************************************************************/
void DlgConf::processStatConf(QString grpName)
{
    QStringList members;
    QStringList hdr;
    int i;

    // get the group information
    if ( extnList.count() ==0)
    {
        if (getExtensionList() <= 0)
        {
            QMessageBox::warning(this,"Start Conf", "Unable to get the extension list. Can start the conference.");
            return;
        }
    }

    //Test data, to be
    members.empty();
    members.insert(0,"OWS2");
    members.insert(1,"OWS3");
    members.insert(2,"OWS4");
    members.insert(3,"OWS5");



    // display group information
    hdr <<"" <<"Conference Members" << "Status";

    int iRowCount = ui->tblStartConf->rowCount();

    for(i = 0; i < iRowCount ; i++)
        ui->tblStartConf->removeRow(i);
    iRowCount = 0;
    ui->tblStartConf->setRowCount(iRowCount);

    ui->tblStartConf->setColumnCount(3);
    ui->tblStartConf->setHorizontalHeaderLabels(hdr);

    int iColCount;

    for(i=0; i < members.count(); i++)
    {
        iColCount = 0;
        QTableWidgetItem *select = new QTableWidgetItem();
        QTableWidgetItem *name = new QTableWidgetItem();
        QTableWidgetItem *status= new QTableWidgetItem();

        name->setText(members[i]);
        status->setText("Not Connected");
        ui->tblStartConf->insertRow(i);

        ui->tblStartConf->setItem(i, iColCount++, select);
        select->setCheckState(Qt::Unchecked);

        ui->tblStartConf->setItem(i, iColCount++, name);
        ui->tblStartConf->setItem(i, iColCount++, status);
    }


}
