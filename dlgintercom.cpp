#include "dlgintercom.h"
#include "ui_dlgintercom.h"

#include <QMessageBox>
#include <QtCore/QObjectUserData>
#include "dbaccess.h"
#include "const.h"
#include "commapi.h"


DlgIntercom::DlgIntercom(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgIntercom)
{
    ui->setupUi(this);

    iContext = CONTEXT_NORMAL; //dialing

    // add the Pushbuttons
    int i=0;
    interComList.insert(i++, ui->pbExtn1);

    interComList.insert(i++, ui->pbExtn2);
    interComList.insert(i++, ui->pbExtn3);
    interComList.insert(i++, ui->pbExtn4);
    interComList.insert(i++, ui->pbExtn5);
    interComList.insert(i++, ui->pbExtn6);
    interComList.insert(i++, ui->pbExtn7);
    interComList.insert(i++, ui->pbExtn8);
    interComList.insert(i++, ui->pbExtn9);
    interComList.insert(i++, ui->pbExtn10);
    interComList.insert(i++, ui->pbExtn11);
    interComList.insert(i++, ui->pbExtn12);
    interComList.insert(i++, ui->pbExtn13);
    interComList.insert(i++, ui->pbExtn14);
    interComList.insert(i++, ui->pbExtn15);
    interComList.insert(i++, ui->pbExtn16);

    QString name;

    for(i=0; i < interComList.count(); i++)
    {
 //       name = "OWS" + QString::number(i+1);
        name = "OWS??";

        interComList[i]->setText(name);
        interComList[i]->setProperty("data",QVariant(name));
    }


    displayContext();
}

DlgIntercom::~DlgIntercom()
{
    delete ui;
}

void DlgIntercom::setPos(int x, int y)
{

    this->setGeometry(x, y+10, this->width(), this->height());
}


void DlgIntercom::handlePbClick(QPushButton *pb)
{
    QString key;
    Extension *extnData = nullptr;

    key = pb->property("data").toString();

    qDebug() << "Key=" << key;

    extnData = extensions.value(key);

    if ( extnData != nullptr)
    {
        qDebug() << "Extn Num=" << extnData->endPoint.extn;

    }
    else
    {
        QMessageBox::warning(this, "Intercomm", "Device not defined.");
        return;
    }

    if ( extnData->endPoint.iStatus == STATUS_NOTCONNECTED)
    {
        QMessageBox::warning(this, "Intercomm", "Device not connected.");
        return;
    }

    if ( extnData->endPoint.iStatus == STATUS_NOTINUSE)
    {
        if ( iContext == CONTEXT_WHISPER || iContext == CONTEXT_BARGEIN)
        {
            QMessageBox::warning(this,"Intercom","Not Connected.");
            return;
        }

        if ( extnData != nullptr)
        {
            emit DialNumber(extnData->endPoint.extn);//QString number);
        }
        else
        {
             QMessageBox::information(this,"Intercom","Not configured yet");
        }
    }
    else
    {
        if (iContext == CONTEXT_BARGEIN || iContext == CONTEXT_WHISPER)
        {
            if ( extnData != nullptr)
            {
                emit DialNumber(extnData->endPoint.extn);//QString number);
            }
            else
            {
                 QMessageBox::information(this,"Intercom","Not configured yet");
            }
        }
        else
        {
            QMessageBox::warning(this, "Intercomm", "Extension is busy.");
        }
    }
    return;
}

void DlgIntercom::on_pbExtn1_clicked()
{
    handlePbClick(ui->pbExtn1);

}

//

int DlgIntercom::displayIntercomPage(QString extn)
{

    int i=0;
    QString extnNum="";
    QMap<QString,Extension *>::iterator itr;
    //QObjectUserData *ud;
    Extension *extnData;
    extensions.empty();

    myExtn = extn;
    //qDebug() <<"";
    commapi capi("192.168.100.127", 9000);

    if (capi.readExtensionData(extensions) <= 0)
    {
        return -1;
    }


    qDebug() << "Going to Add to List, count=" << extensions.count();
 //   for(i =0; i < interComList.count(); i++)
 //   {
  //      interComList[i]->setText("Unused");
//    }
    qDebug() << "Added to List";

    i =0;
    for (itr= extensions.begin(); itr != extensions.end(); itr++)
    {
        extnNum = itr.key();


        extnData = itr.value();
//qDebug() << "mYextn"<< myExtn << ":" << extnData->endPoint.name;

        if ( extnData->endPoint.name == myExtn)
            continue;

        //   qDebug() << "Extn:" << extnData->endPoint.extn << "Name:" << extnData->endPoint.name;

//        interComList[i]->setText(extnNum)
        interComList[i]->setText(extnData->endPoint.name);

//       interComList[i]->setText(extnData->endPoint.name);
       interComList[i]->setProperty("data",QVariant(extnNum));
       i++;
    }


    return 1;
}

void DlgIntercom::on_pbExtn2_clicked()
{
    handlePbClick(ui->pbExtn2);

}

void DlgIntercom::on_pbExtn3_clicked()
{
    handlePbClick(ui->pbExtn3);

}

void DlgIntercom::on_pbExtn4_clicked()
{
    handlePbClick(ui->pbExtn4);

}

void DlgIntercom::on_pbExtn5_clicked()
{
    handlePbClick(ui->pbExtn5);

}

void DlgIntercom::on_pbExtn6_clicked()
{
    handlePbClick(ui->pbExtn6);

}

void DlgIntercom::on_pbExtn7_clicked()
{
    handlePbClick(ui->pbExtn7);

}

void DlgIntercom::on_pbExtn8_clicked()
{
    handlePbClick(ui->pbExtn8);

}

void DlgIntercom::on_pbExtn9_clicked()
{
    handlePbClick(ui->pbExtn9);

}

void DlgIntercom::on_pbExtn10_clicked()
{
    handlePbClick(ui->pbExtn10);

}

void DlgIntercom::on_pbExtn13_clicked()
{
    handlePbClick(ui->pbExtn13);

}

void DlgIntercom::on_pbExtn11_clicked()
{
    handlePbClick(ui->pbExtn11);

}

void DlgIntercom::on_pbExtn12_clicked()
{
    handlePbClick(ui->pbExtn12);

}

void DlgIntercom::on_pbExtn14_clicked()
{
    handlePbClick(ui->pbExtn14);

}

void DlgIntercom::on_pbExtn15_clicked()
{
    handlePbClick(ui->pbExtn15);

}

void DlgIntercom::on_pbExtn16_clicked()
{
    handlePbClick(ui->pbExtn16);

}


void DlgIntercom::displayContext()
{
    switch(iContext)
    {
        case CONTEXT_TXFR:
            ui->lblState->setText("Click on free extension to transfer.");
            break;
        case CONTEXT_CONF:
            ui->lblState->setText("Click on free extension to add to Conferece.");
            break;
        case CONTEXT_BARGEIN:
            ui->lblState->setText("Click on connected extension to barge-in.");
        case CONTEXT_WHISPER:
            ui->lblState->setText("Click on connected extension to whisper.");

        default:
            ui->lblState->setText("Click to connect.");
    }
}

void DlgIntercom::setContext(int val)
{
    iContext = val;
    displayContext();
}


void DlgIntercom::changeExtnStatus(QString extn, int iType, int iStatus)
{
    QString bkColor;

    if (extensions.count() > 0 )
    {
 //       extensions[extn]->endPoint.iType = iType;
        extensions[extn]->endPoint.iStatus= iStatus;

        for(int i=0; i < interComList.count(); i++)
        {
            if ( interComList[i]->property("data").toString() == extn)
            {
                interComList[i]->setToolTip(extn);

                switch(iStatus)
                {
                    case STATUS_NOTCONNECTED: // no registered
                        interComList[i]->setStyleSheet("QPushButton {background-color: white}");

                        break;

                    case STATUS_NOTINUSE: // Ready
                        bkColor = QString("QPushButton{background-color: %1}").arg(COLOUR_GREEN);
                        interComList[i]->setStyleSheet(bkColor);

                        break;

                    case STATUS_RINGING: //ringing
                        bkColor = QString("QPushButton {background-color: %1}").arg(COLOUR_YELLOW);
                        interComList[i]->setStyleSheet(bkColor);
                        break;

                    case STATUS_TALKING: // busy
//                        interComList[i]->setStyleSheet("QPushButton {background-color: red}");
//                    interComList[i]->setStyleSheet("background-color: rgba(100,0,0,100);");
                         bkColor = QString("QPushButton{background-color: %1}").arg(COLOUR_RED);
                    interComList[i]->setStyleSheet(bkColor);

                         break;

                }

                break;

            }
        }
    }
}
