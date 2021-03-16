
#include <QMessageBox>
#include "dlgowspanel.h"
#include "ui_dlgowspanel.h"
#include "commapi.h"
#include "const.h"

DlgOWSPanel::DlgOWSPanel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgOWSPanel)
{

    ui->setupUi(this);
    setWindowTitle("MMI");

    ui->frm_Rdio->setDisabled(true);
    ui->frm_GSM->setDisabled(true);

    ui->pb_Intercom1->setText("OWS__");
    ui->pb_Intercom1->setDisabled(true);

    ui->pb_Intercom2->setText("OWS__");
    ui->pb_Intercom2->setDisabled(true);

    ui->pb_Intercom3->setText("OWS__");
    ui->pb_Intercom3->setDisabled(true);

    ui->pb_Intercom4->setText("OWS__");
    ui->pb_Intercom4->setDisabled(true);

    ui->pb_Intercom5->setText("OWS__");
    ui->pb_Intercom5->setDisabled(true);

    ui->pb_Intercom6->setText("OWS__");
    ui->pb_Intercom6->setDisabled(true);

    ui->pb_Intercom7->setText("OWS__");
    ui->pb_Intercom7->setDisabled(true);

    ui->pb_Intercom8->setText("OWS__");
    ui->pb_Intercom8->setDisabled(true);

    ui->pb_Intercom9->setText("OWS__");
    ui->pb_Intercom9->setDisabled(true);

    ui->pb_Intercom10->setText("OWS__");
    ui->pb_Intercom10->setDisabled(true);

    ui->pb_Intercom11->setText("OWS__");
    ui->pb_Intercom11->setDisabled(true);

    ui->pb_Intercom12->setText("OWS__");
    ui->pb_Intercom12->setDisabled(true);

    int i =0;

    for(i=0; i < iMaxDevice; i++)
    {
        CPanelButton *panelItem = new CPanelButton();
        panelItem->extnNo = "";
        panelItem->extnType = TYPE_OWS; //extension type
        pbList.insert(i, panelItem);
    }
    i = 0;
    pbList[i++]->pb = ui->pb_Intercom1;

    pbList[i++]->pb = ui->pb_Intercom2;
    pbList[i++]->pb = ui->pb_Intercom3;
    pbList[i++]->pb = ui->pb_Intercom4;
    pbList[i++]->pb = ui->pb_Intercom5;
    pbList[i++]->pb = ui->pb_Intercom6;
    pbList[i++]->pb = ui->pb_Intercom7;
    pbList[i++]->pb = ui->pb_Intercom8;
    pbList[i++]->pb = ui->pb_Intercom9;
    pbList[i++]->pb = ui->pb_Intercom10;
    pbList[i++]->pb = ui->pb_Intercom11;
    pbList[i++]->pb = ui->pb_Intercom12;

    //Radio
    pbList[i]->extnType = TYPE_RADIO;
    pbList[i++]->pb = ui->pb_Radio1;

    pbList[i]->extnType = TYPE_RADIO;
    pbList[i++]->pb = ui->pb_Radio2;

    pbList[i]->extnType = TYPE_RADIO;
    pbList[i++]->pb = ui->pb_Radio3;

    pbList[i]->extnType = TYPE_RADIO;
    pbList[i++]->pb = ui->pb_Radio4;

    pbList[i]->extnType = TYPE_RADIO;
    pbList[i++]->pb = ui->pb_Radio5;

    pbList[i]->extnType = TYPE_RADIO;
    pbList[i++]->pb = ui->pb_Radio6;

    pbList[i]->extnType = TYPE_RADIO;
    pbList[i++]->pb = ui->pb_Radio7;

    pbList[i]->extnType = TYPE_RADIO;
    pbList[i++]->pb = ui->pb_Radio8;

    pbList[i]->extnType = TYPE_RADIO;
    pbList[i++]->pb = ui->pb_Radio9;

    pbList[i]->extnType = TYPE_RADIO;
    pbList[i++]->pb = ui->pb_Radio10;

    //set GSM
    pbList[i]->extnType = TYPE_GSM;
    pbList[i++]->pb = ui->pb_Intercom12;

    // SATCOM
    pbList[i]->extnType = TYPE_SATCOM;
    pbList[i++]->pb = ui->pb_satcom1;

    pbList[i]->extnType = TYPE_SATCOM;
    pbList[i++]->pb = ui->pb_satcom2;

    // C-BAND
    pbList[i]->extnType = TYPE_SATCOM;
    pbList[i++]->pb = ui->pb_cband1;

    pbList[i]->extnType = TYPE_SATCOM;
    pbList[i++]->pb = ui->pb_cband2;
//qDebug() << "Max:" <<iMaxDevice << "Curr" <<i ;

    // CMS
    pbList[i]->extnType = TYPE_CSM;
    pbList[i++]->pb = ui->pb_csm1;
    pbList[i]->extnType = TYPE_CSM;
    pbList[i++]->pb = ui->pb_csm2;


    // add callback to update endpoint status
    dlgPhone = new DlgPhone;

    connect(dlgPhone, SIGNAL(endpointStatus(T_EndpointStatus *)), this, SLOT(updateEndpointSTatus(T_EndpointStatus *)));
    connect(dlgPhone, SIGNAL(vmCount(int)), this, SLOT(setVMCount(int)));
    connect(dlgPhone, SIGNAL(onDispCallStatus(QString)), this, SLOT(onDisplayCallStatus(QString)));
    connect(dlgPhone, SIGNAL(onDispPhoneStatus(QString)), this, SLOT(onDisplayPhoneStatus(QString)));
    connect(dlgPhone, SIGNAL(onDispMsg(QString)), this, SLOT(onDisplayMsg(QString)));
    connect(dlgPhone, SIGNAL(onHangupBtn(int)), this, SLOT(onHangupBtnChange(int)));
    connect(dlgPhone, SIGNAL(onAnswerBtn(int)), this, SLOT(onAnswerBtnChange(int)));
    connect(dlgPhone, SIGNAL(onHoldBtn(int)), this, SLOT(onHoldBtnChange(int)));


    displayMMI(dlgPhone->getMyOWSNum());
    this->setWindowTitle(dlgPhone->getOWSName());

    dlgConf = new DlgConf;
    dlgChat = new DlgChat;
}

DlgOWSPanel::~DlgOWSPanel()
{
    delete ui;
}


void DlgOWSPanel::setNums(QString extnNum, QString name, int iType)
{
    QString pbName;
    CPanelButton *cpb;

   qDebug() << "Maxdevice:" << iMaxDevice;
    for (int i = 0; i < iMaxDevice; i++)
    {
        if ( pbList[i]->extnType == iType && pbList[i]->extnNo == "")
        {
            pbList[i]->extnNo = extnNum;
            pbList[i]->pb->setEnabled(true);
            pbList[i]->pb->setText(name);
            pbList[i]->pbName = name;
            cpb = new CPanelButton;

            cpb->pb = pbList[i]->pb;
            cpb->extnNo = pbList[i]->extnNo;
            cpb->pbName = pbList[i]->pbName;
            cpb->iStatus = pbList[i]->iStatus;
            cpb->extnType = pbList[i]->extnType;
            cpb->pb->setToolTip(extnNum);
            cpb->pb->setText(cpb->pbName);
            cpb->pb->setDisabled(false);
qDebug() << "Extension added:" << extnNum;
            endpointList.insert(extnNum, cpb);
            break;
        }
    }
}


QPushButton * DlgOWSPanel::getPBId(QString extnNum)
{
    int i =0;

    for (i = 0; i < iMaxDevice; i++)
    {
        if ( pbList[i]->extnNo == extnNum)
        {
            break;
        }
    }

    if ( i >= iMaxDevice)
        return nullptr;
    else
        return pbList[i]->pb;
}


void DlgOWSPanel::setIncall(QString clip)
{


}


void DlgOWSPanel::setCallConnected(QString extnNo)
{


}


void DlgOWSPanel::setHangup(QString extnNo)
{

}


int DlgOWSPanel::displayMMI(QString extn)
{

    //int i=0;
    QString extnNum="";
    QMap<QString,Extension *>::iterator itr;
    //QObjectUserData *ud;
    Extension *extnData;
    extensions.empty();



    myExtn = extn;

///    DBAccess *dba = new DBAccess("extn");
    commapi capi("192.168.100.127", 9000);

    if (capi.readExtensionData(extensions) <= 0)
    {
        return -1;
    }

//    delete dba;


    for (itr= extensions.begin(); itr != extensions.end(); itr++)
    {
        extnNum = itr.key();

        if ( extnNum == myExtn)
            continue;

        extnData = itr.value();

        setNums(extnData->endPoint.extn, extnData->endPoint.name,  itr.value()->endPoint.iType); // TBD extension Type
qDebug() << "Set:" << extnData->endPoint.name;
    }
qDebug() << "Ext MMI";
    return 1;
}

void DlgOWSPanel::updateEndpointSTatus(T_EndpointStatus *eps)
{
//qDebug() << "Inside update Endpoints:" << eps->extn;
    QString bkColor;
    int iType;
    int iStatus;
    QString extn;

    extn = QString(eps->extn);
    iType = eps->iType;
    iStatus = eps->iStatus;

    if ( endpointList.find(extn) == endpointList.end())
    {
 //       qDebug() << "Extn:" << extn << " Not there.";
        return;
    }
  //  qDebug() << "Inside update Endpoints" <<extn<<":" << iType << ":" << iStatus << "Endp:" << endpointList[extn]->pb->text();


    if (extensions.count() > 0 )
    {
//        extensions[extn]->endPoint.iType = iType;
        extensions[extn]->endPoint.iStatus= iStatus;

        endpointList[extn]->iStatus = iStatus;

        switch(iStatus)
        {
                case STATUS_NOTCONNECTED: // no registered
                    endpointList[extn]->pb->setStyleSheet("QPushButton {background-color: white}");

                    break;

                case STATUS_NOTINUSE: // Ready
                    bkColor = QString("QPushButton{background-color: %1}").arg(COLOUR_GREEN);
                    endpointList[extn]->pb->setStyleSheet(bkColor);

                    break;

                case STATUS_RINGING: //ringing
                    bkColor = QString("QPushButton {background-color: %1}").arg(COLOUR_YELLOW);
                    endpointList[extn]->pb->setStyleSheet(bkColor);
                    break;

                case STATUS_TALKING: // busy
                     bkColor = QString("QPushButton{background-color: %1}").arg(COLOUR_RED);
                     endpointList[extn]->pb->setStyleSheet(bkColor);

                     break;

            }

        }

}



void DlgOWSPanel::on_pb_setting_clicked()
{
    dlgPhone->show();
}

void DlgOWSPanel::on_pb_ManageConf_clicked()
{
    dlgConf->show();
}

void DlgOWSPanel::on_pb_chat_clicked()
{
    dlgChat->show();
}

void DlgOWSPanel::on_pb_phonebook_clicked()
{
    if ( dlgPhonebook == nullptr)
    {
//        dlgPhonebook = new DlgPhonebook;
        dlgPhonebook = dlgPhone->dlgPhonebook;
        dlgPhonebook->setDlgLog(dlgPhone->dlgLog);

    }
    if ( dlgPhonebook->isHidden() == true)
    {
        dlgPhonebook->show();
        dlgPhonebook->displayContacts();
    }
    else
    {
        dlgPhonebook->hide();

    }
}

void DlgOWSPanel::on_pb_followme_clicked()
{
    if (dlgFollowMe == nullptr)
    {
        dlgFollowMe = new DlgFollowme();
    }
    dlgFollowMe->setModal(true);
    dlgFollowMe->showFollowme(dlgPhone->getOWSName());
    dlgFollowMe->show();

}

void DlgOWSPanel::on_pb_callFwad_clicked()
{
    if (dlgCallFwd == nullptr)
    {
        dlgCallFwd = new DlgCallFwd();
    }
    dlgCallFwd->showCallFwd(dlgPhone->getOWSName());
//    QMessageBox::information(this,"Check", dlgPhone->getMyOWSNum());
}


/******************************** voice mail *******************/

void DlgOWSPanel::setVMCount(int iCount)
{
    if ( iCount == 0)
    {
        ui->pb_vm->setStyleSheet("QPushButton {background-color: white}");
        ui->pb_vm->setText("VM:0");
    }
    else if (iCount > 0)
    {
        ui->pb_vm->setStyleSheet(QString("QPushButton {background-color: %1}").arg(COLOUR_YELLOW));
        ui->pb_vm->setText("VM:"+ QString::number(iCount));
    }

}
void DlgOWSPanel::on_pb_vm_clicked()
{

}

void DlgOWSPanel::on_pb_conference_clicked()
{
    dlgPhone->dialSpecial(CODE_CONF, dlgPhone->getMyOWSNum(), "Conerenceing mode.");

}




void DlgOWSPanel::handlePbClick(QPushButton *pb)
{

    int i=0;
    int idx = -1;
    for (i =0; i < pbList.count(); i++)
    {
        if (pbList[i]->pb == pb )
        {
            idx = i;
            break;
        }
    }

    if ( idx < 0)
    {
        return;
    }


    int iContext = dlgPhone->getDialContext();

    if ( pbList[idx]->extnNo != nullptr)
    {
        qDebug() << "Extn Num=" << pbList[idx]->extnNo;

    }
    else
    {
        QMessageBox::warning(this, "Intercomm", "Device not defined.");
        return;
    }

    int iStatus = extensions[pbList[idx]->extnNo]->endPoint.iStatus;

    if ( iStatus == STATUS_NOTCONNECTED)
    {
        QMessageBox::warning(this, "Intercomm", "Device not connected.");
        return;
    }

    if ( iStatus == STATUS_NOTINUSE)
    {
        if ( iContext == CONTEXT_WHISPER || iContext == CONTEXT_BARGEIN)
        {
            QMessageBox::warning(this,"Intercom","Not Connected.");
            return;
        }

        dlgPhone->autoDial(pbList[idx]->extnNo);
    }
    else
    {
        if (iContext == CONTEXT_BARGEIN || iContext == CONTEXT_WHISPER)
        {
            dlgPhone->autoDial(pbList[idx]->extnNo);
        }
        else
        {
            QMessageBox::warning(this, "Intercomm", "Extension is busy.");
        }
    }
    return;
}


void DlgOWSPanel::on_pb_Intercom1_clicked()
{
    handlePbClick(ui->pb_Intercom1);
}



void DlgOWSPanel::on_pb_Intercom2_clicked()
{
    handlePbClick(ui->pb_Intercom2);

}


void DlgOWSPanel::on_pb_Intercom3_clicked()
{
    handlePbClick(ui->pb_Intercom3);

}


void DlgOWSPanel::on_pb_Intercom4_clicked()
{
    handlePbClick(ui->pb_Intercom4);

}



void DlgOWSPanel::on_pb_Intercom5_clicked()
{
    handlePbClick(ui->pb_Intercom5);

}


void DlgOWSPanel::on_pb_Intercom6_clicked()
{
    handlePbClick(ui->pb_Intercom6);

}



void DlgOWSPanel::on_pb_Intercom7_clicked()
{
    handlePbClick(ui->pb_Intercom7);

}


void DlgOWSPanel::on_pb_Intercom8_clicked()
{
    handlePbClick(ui->pb_Intercom8);

}



void DlgOWSPanel::on_pb_Intercom9_clicked()
{
    handlePbClick(ui->pb_Intercom9);

}


void DlgOWSPanel::on_pb_Intercom10_clicked()
{
    handlePbClick(ui->pb_Intercom10);

}

void DlgOWSPanel::on_pb_Intercom11_clicked()
{
    handlePbClick(ui->pb_Intercom11);

}

void DlgOWSPanel::on_pb_Intercom12_clicked()
{
    handlePbClick(ui->pb_Intercom12);

}


void DlgOWSPanel::onDisplayCallStatus(QString msg)
{
    ui->lblCallStatus->setText(msg);
}

void DlgOWSPanel::onDisplayPhoneStatus(QString msg)
{
    ui->lblPhStatus->setText(msg);

}

void DlgOWSPanel::onDisplayMsg(QString msg)
{
    ui->lblMsg->setText(msg);

}

void DlgOWSPanel::on_pb_bergein_clicked()
{
    dlgPhone->processBargeIn(0);
}

void DlgOWSPanel::on_pb_wishper_clicked()
{
    dlgPhone->processWhisper(0);
}

void DlgOWSPanel::on_pb_transfer_clicked()
{
    dlgPhone->processTransfer(0);
}

void DlgOWSPanel::on_pbAnswer_clicked()
{
    dlgPhone->processAnswer();
}

void DlgOWSPanel::on_pbHangup_clicked()
{
    dlgPhone->processHangup();
}

void DlgOWSPanel::on_pbHold_clicked()
{
    dlgPhone->processHold();

}


void DlgOWSPanel::onAnswerBtnChange(int iFlag)
{
    if (iFlag == 0)
    {
        ui->pbAnswer->setDisabled(true);
        ui->pbAnswer->setStyleSheet(QString("QPushButton {background-color: white}"));

    }
    else
    {
        ui->pbAnswer->setDisabled(false);
        ui->pbAnswer->setStyleSheet(QString("QPushButton {background-color: %1}").arg(COLOUR_YELLOW));

    }
}

void DlgOWSPanel::onHangupBtnChange(int iFlag)
{
    if (iFlag == 0)
    {
        ui->pbHangup->setDisabled(true);
        ui->pbHangup->setStyleSheet(QString("QPushButton {background-color: white}"));

    }
    else
    {
        ui->pbHangup->setDisabled(false);
        ui->pbHangup->setStyleSheet(QString("QPushButton {background-color: %1}").arg(COLOUR_YELLOW));

    }

}

void DlgOWSPanel::onHoldBtnChange(int iFlag)
{
    if (iFlag == 0)
    {
        ui->pbHold->setDisabled(true);

    }
    else
    {
        ui->pbHold->setDisabled(false);

    }

}
