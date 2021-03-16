#include "dlgfollowme.h"
#include "ui_dlgfollowme.h"

#include <QMessageBox>

#include "commapi.h"

DlgFollowme::DlgFollowme(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgFollowme)
{
    ui->setupUi(this);

}

DlgFollowme::~DlgFollowme()
{
    delete ui;
}

void DlgFollowme::on_pbCancel_clicked()
{
    close();
}

void DlgFollowme::on_pbSave_clicked()
{
    // save
    QMap<QString,Extension *>::iterator itr;

    QString phoneNum="";
    QList<FollowMe *> fmeL;
    FollowMe *fme;
   // commapi capi;

    int iOrder=1;

    if ( ui->cbExtn1->currentText() != "")
    {
        for (itr= extensions.begin(); itr != extensions.end(); itr++)
        {
            if ( itr.value()->endPoint.name == ui->cbExtn1->currentText())
            {
                phoneNum = itr.value()->endPoint.extn;
             break;
            }
        }
    }
    else
    {
        phoneNum = "";
    }

    if (phoneNum != "")
    {
        fme = new FollowMe();
        fme->name = myExtnNum;
        fme->order = iOrder++;
        fme->phone = phoneNum;
        fme->timeOut = 15;
        fmeL.insert(iOrder-1, fme);
    }

    //phoneNum = ui->cbExtn2->currentText();

    if ( ui->cbExtn2->currentText() != "")
    {
        for (itr= extensions.begin(); itr != extensions.end(); itr++)
        {
            if ( itr.value()->endPoint.name == ui->cbExtn2->currentText())
            {
                phoneNum = itr.value()->endPoint.extn;
             break;
            }
        }
    }
    else
    {
        phoneNum = "";
    }

    if (phoneNum != "")
    {
        fme = new FollowMe();
        fme->name = myExtnNum;
        fme->order = iOrder++;
        fme->phone = phoneNum;
        fme->timeOut = 15;
        fmeL.insert(iOrder-1, fme);
    }

//    phoneNum = ui->cbExtn3->currentText();
    if ( ui->cbExtn3->currentText() != "")
    {
        for (itr= extensions.begin(); itr != extensions.end(); itr++)
        {
            if ( itr.value()->endPoint.name == ui->cbExtn3->currentText())
            {
                phoneNum = itr.value()->endPoint.extn;
             break;
            }
        }
    }
    else
    {
        phoneNum = "";
    }

    if (phoneNum != "")
    {
        fme = new FollowMe();
        fme->name = myExtnNum;
        fme->order = iOrder++;
        fme->phone = phoneNum;
        fme->timeOut = 15;
        fmeL.insert(iOrder-1, fme);
    }

    commapi capi("192.168.100.127",9000);


    if ( capi.saveFollowme(fmeL, myExtnNum) < 0)
    {
        QMessageBox::warning(this,"Followme Setting","Unable to save Follow me Data.");
    }
    else
    {
        QMessageBox::warning(this,"Followme Setting","Follow me Data saved successfully.");
        close();

    }
    this->close();
    return;
}

void DlgFollowme::on_pbClear_clicked()
{
    QList<FollowMe *> fmeL;
    // delete/clear
    if ( QMessageBox::question(this,"Followme Setting","Do you want to reset Follow me?") == QMessageBox::Yes)
    {
        commapi capi("127.0.0.1",9000);

        if ( capi.saveFollowme(fmeL, myExtnNum) < 0)
        {
            QMessageBox::warning(this,"Followme Setting","Unable to reset Follow me.");
        }
        else
        {
            QMessageBox::warning(this,"Followme Setting","Follow me reset successfully.");
            close();
        }
        return;

    }

}

void DlgFollowme::showFollowme(QString extnName)
{

    QList<QString> listExtn;
    Extension *extnData;

    QMap<QString,Extension *>::iterator itr;
    QString extnNum;

    myExtnName = extnName;

    extensions.empty();
    extensions.clear();
    listExtn.clear();
    listExtn.empty();

    //commapi capi("167.71.230.148",9000);
    commapi capi("192.168.100.127",9000);

    capi.readExtensionData(extensions);

    qDebug() << "show:" << myExtnName << "ExtnName" << extnName;


    listExtn.append("");

    for (itr= extensions.begin(); itr != extensions.end(); itr++)
    {
         extnNum = itr.key();
         extnData = itr.value();
         if ( extnData->endPoint.name == myExtnName)
         {
             myExtnNum = extnNum;
             continue;
         }
         listExtn.append(extnData->endPoint.name);
//       i++;
    }

    if ( capi.readFollowme(fmeList,myExtnNum) < 0 )
    {
        QMessageBox::warning(this,"Follow me setting", "Unable to get Followme information for display.");
        return;
    }
    else
    {
        if (fmeList.count() >= 1 )
        {
            QString ext = fmeList[0]->phone;

            for (itr= extensions.begin(); itr != extensions.end(); itr++)
            {
                if ( itr.value()->endPoint.extn == ext)
                {
                    qDebug() << "FNum:" << itr.value()->endPoint.name;
                    break;
                }
            }

        }



        ui->cbExtn1->clear();
        ui->cbExtn1->addItems(listExtn);

        if (fmeList.count() >= 1 )
        {
            QString ext = fmeList[0]->phone;

            for (itr= extensions.begin(); itr != extensions.end(); itr++)
            {
                if ( itr.value()->endPoint.extn == ext)
                {
                    ui->cbExtn1->setCurrentText(itr.value()->endPoint.name);
                    break;
                }
            }

        }
        ui->cbExtn2->clear();
        ui->cbExtn2->addItems(listExtn);
        if (fmeList.count() >= 2 )
        {
            QString ext = fmeList[1]->phone;
    //qDebug() << "Extension to follow 1"<<ext;
            for (itr= extensions.begin(); itr != extensions.end(); itr++)
            {
                if ( itr.value()->endPoint.extn == ext)
                {
                    ui->cbExtn2->setCurrentText(itr.value()->endPoint.name);
                    break;
                }
            }
        }

        ui->cbExtn3->clear();
        ui->cbExtn3->addItems(listExtn);
        if (fmeList.count() >= 3 )
        {
            QString ext = fmeList[2]->phone;
    //qDebug() << "Extension to follow 1"<<ext;
            for (itr= extensions.begin(); itr != extensions.end(); itr++)
            {
                if ( itr.value()->endPoint.extn == ext)
                {
                    ui->cbExtn3->setCurrentText(itr.value()->endPoint.name);
                    break;
                }
            }
        }
    }

}


