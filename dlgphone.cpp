#include "dlgphone.h"
#include "ui_dlgphone.h"
#include <QMessageBox>
#include "dlgsetting.h"
#include <QTimer>
#include <QThread>
#include "const.h"
#include "commapi.h"

using namespace  risip;

DlgPhone::DlgPhone(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DlgPhone)
{

    ui->setupUi(this);


    this->setGeometry(500,0,425, 275);

    dlgLog = new DlgLog();

    dlgSpeed = new DlgSpeedDial();

    dlgSetting = new DlgSetting();

    dlgChat = new DlgChat();

    dlgPresence = new DlgPresence();

    dlgPhonebook = new DlgPhonebook();
    dlgPhonebook->setDlgLog(dlgLog);

    dlgIntercom = new DlgIntercom();

    dlgReport = new DlgReport();

    bAccountRegistered=false;
    logConsole("Started");

    rsip = Risip::instance();  

    activeCall = new RisipCall();

    activeCallMgr =  RisipCallManager::instance();

    pj_log_set_level(0);

 //   rsipMsg = new RisipMessage();

    bHoldStatus = false;
    bMute = false;
    lMicVol=0;
    bDialRequest =false;


    if (dp.readPhoneData(pd) > 0 )
    {
   /*     cfg.setUri("sip:1000@arcanemobile.com");
        cfg.setUserName("1000");
        cfg.setPassword("1000");
        cfg.setScheme("digest");
        cfg.setServerAddress("arcanemobile.com:5065");
        cfg.setProxyServer("");
      */
        cfg.setUri(pd.uri);//("sip:1000@arcanemobile.com");
        cfg.setUserName(pd.name); //("1000");
        cfg.setPassword(pd.auth); //("1000");
        cfg.setScheme (pd.scheme); //"digest");
        cfg.setServerAddress(pd.serverName); //("arcanemobile.com:5065");
        cfg.setProxyServer(pd.proxy);//"");

        logConsole("Name:"+pd.name);
        logConsole("Auth:"+pd.auth);
        logConsole("Server:"+pd.serverName);
        logConsole("Uri:"+pd.uri);

//        this->setWindowTitle("Softphone("+pd.uri+ ")");
        this->setWindowTitle(pd.extn);


    }
    else
    {
        QMessageBox::warning(this,"Phone setting","Unable to read settings");
      //  this->show();
        return;

    }
    // Set signals
    setSignal();

    setButtonState();

    setTimerUpdate();


    rsip->sipEndpoint()->start();

    pj_log_set_level(0);
    pj_log_set_log_func(NULL);
//    pj_log_set_decor(0);
    pj_log_init();

   logConsole("Endpoint Started:+loglevel:"+ QString::number(pj_log_get_level()));


   logConsole("loglevel:"+ QString::number(pj_log_get_level()));

   // start the periodic process
   periodicVMTimer->start(5000);
   periodicStatusTimer->start(1000);

}


DlgPhone::~DlgPhone()
{

    if ( bAccountRegistered == true)
    {
        rsip->sipEndpoint()->stop();
    }
    logConsole("Going to die.");

    delete dlgSpeed;
    delete dlgChat;
    delete dlgPresence;
    delete dlgSetting;
    delete dlgLog;
    delete dlgPhonebook;
    delete dlgReport;

    delete dlgIntercom;
    delete ui;
}

QString DlgPhone::callStatusToMsg(int status)
{
    QString msg;
    switch(status)
    {
        case RisipCall::Status::Null:
            break;
        case RisipCall::Status::CallEarly:
            msg = "Ringing..";
            break;
        case RisipCall::Status::CallConfirmed:
            msg = "Call Connected";
 //           logConsole(msg+ " CID: "+ QString::number(activeCall->callId()) );
            break;
        case RisipCall::Status::CallDisconnected:
            msg = "Call Disconnected";
            break;
        case RisipCall::Status::ConnectingToCall:
            msg = "Call Connecting";
            break;
        case RisipCall::Status::IncomingCallStarted:
            msg = "Incoming Call..";
            break;
        case RisipCall::Status::OutgoingCallStarted:
            msg = "Calling..";
            break;
        default:
            msg = "Unknown:"+QString::number(status);
            break;

    }

    return msg;
}

QString DlgPhone::phoneStatusToMsg(int status)
{
    QString msg;
    QString uri;
    //QMessageBox::information(this,"Acct-Status", QString::number(status));
    switch(status)
    {
        case RisipAccount::NotConfigured:// = 0,
            msg = "Account not Configured.";
            break;
        case RisipAccount::NotCreated:
            msg = "Account not created.";
            break;
        case RisipAccount::Registering:
            msg = "Registering.";
            break;
        case RisipAccount::UnRegistering:
            msg = "Unregistering.";
            break;
        case RisipAccount::SignedIn:
            msg = "Signed in.";

            activeCallMgr->setActiveAccount(m_account);

            bAccountRegistered = true;
            ui->pbRegister->setText("Deregister");
           // m_account->setPresence(RisipBuddy::Away);
            break;
        case RisipAccount::SignedOut:
            msg = "Signed out.";
            bAccountRegistered = false;
            ui->pbRegister->setText("Register");
            break;
        case RisipAccount::AccountError:// = -1
            msg = "Account error.";
            ui->pbRegister->setText("Register");
            bAccountRegistered = false;
            break;
        default:
            msg = "Unknown status:" + QString::number(status);
    }
    return msg;
}

void DlgPhone::onEPStatusChange(int status)
{
 //    QMessageBox::information(this,"Status", QString::number(status));
 //   RisipBuddy *rBuddy = new RisipBuddy;

    displayPhoneStatus("Initialising...");

    logConsole("OnEPStatusChange:"+ QString::number(status));

    if (status == rsip->sipEndpoint()->Started)
    {
        displayPhoneStatus("Registering..");

        logConsole("OnEPStatusChange:Started");

        // Create account and setting as default
       // m_account = rsip->accountForConfiguration(&cfg);
        m_account = rsip->createAccount(&cfg);

        rsip->setDefaultAccount(cfg.uri());

        //TBD
       // activeCall = activeCallMgr->activeCall();

       // activeCall->setAccount(m_account);
        activeCallMgr->setActiveAccount(m_account);
        activeCallMgr->setActiveCall(activeCall);


        // default Account login
        rsip->defaultAccount()->login();
        connect(m_account,SIGNAL(incomingCall()), this, SLOT(acctountIncomingCall()));

        connect(m_account,SIGNAL(incomingMessage(RisipMessage *)),this, SLOT(acctountIncomingMsg(RisipMessage *)));


    }

    if (status == rsip->sipEndpoint()->NotStarted)
    {
        logConsole("OnEPStatusChange:Notstarted");
        displayPhoneStatus("Not Connected..");
    }

}


void DlgPhone::onAcctStatusChange(int status)
{
    QString msg=phoneStatusToMsg(status);
    displayPhoneStatus(msg);

    logConsole("onAcctStatusChange:"+msg);
}

void DlgPhone::acctountIncomingMsg(RisipMessage *msg)
{
    logConsole("Msg Rcvd:"+ msg->messageBody());
}

void DlgPhone::onDefAcctStatusChange(int status)
{
    QString msg="onDefAcctStatusChange:"+QString::number(status) ;

//    QMessageBox::information(this,"Status-DeffAcct", QString::number(status));
    logConsole(msg);

}

void DlgPhone::onIncomingCall(risip::RisipCall *call)
{
    QString CLIP="";
    RisipBuddy *buddy = new RisipBuddy();
   // QQmlListProperty<RisipBuddy> b;
//    QMessageBox::information(this,"CallMgr", "on Incoming call");
    logConsole("onIncomingCall --");

    playRingTone();

    activeCall = call;

//    connect(call, SIGNAL(onBuddyChanged(RisipBuddy *)), this, SLOT(onBuddyChanged(RisipBuddy *)));
//    connect(activeCall, SIGNAL(buddyChanged(RisipBuddy *)), this, SLOT(onBuddyChanged(RisipBuddy *)));


    activeCallMgr->setActiveCall(activeCall);

    if ( (buddy = call->buddy()) == NULL)
    {
        logConsole("Buddy is NULL");
    }
    else
    {
        logConsole("Buddy is not NULL");
    }

    if ( iDND == 1)
    {
        activeCall->hangup();

    }
    int status = activeCall->status();

    displayCallStatus("Incall::"+callStatusToMsg(status));

    // activate Hangup
    ui->pbHangup->setDisabled(false);
    emit onHangupBtn(1);
    ui->pbAnswer->setDisabled(false);
    emit onAnswerBtn(1);
}


void DlgPhone::onCallStatusChanged()
{
 //   QMessageBox::information(this,"Call", "Changed");
    int st= activeCall->status();


    displayCallStatus(callStatusToMsg(st));

    logConsole("onCallStatusChanged:"+ callStatusToMsg(st));
    logConsole("Set Hangup Button:"+QString::number(st));

    if (st == RisipCall::Status::IncomingCallStarted || st == RisipCall::Status::OutgoingCallStarted)
    {
        ui->pbHangup->setDisabled(false);
        emit onHangupBtn(1);
    }

}



void DlgPhone::processKeyPressed(int iKey)
{

    if ( longPressed == 0)
    {
        DialedNumber = DialedNumber+QString::number(iKey);
        ui->leNumber->setText(DialedNumber);
        // send DTMF
        activeCall->sendDTMF(QString::number(iKey));
    }
    else
    {
        processLongKeyPressed(iKey);
    }

}


void DlgPhone::on_pbKey1_clicked()
{
    processKeyPressed(1);
}

void DlgPhone::on_pbKey2_clicked()
{
    processKeyPressed(2);
}

void DlgPhone::on_pbKey3_clicked()
{
    processKeyPressed(3);

}

void DlgPhone::on_pbKey4_clicked()
{
    processKeyPressed(4);

}

void DlgPhone::on_pbKey5_clicked()
{
    processKeyPressed(5);
}

void DlgPhone::on_pbKey6_clicked()
{
    processKeyPressed(6);
}

void DlgPhone::on_pbKey7_clicked()
{
    processKeyPressed(7);
}

void DlgPhone::on_pbKey8_clicked()
{
    processKeyPressed(8);

}

void DlgPhone::on_pbKey9_clicked()
{
    processKeyPressed(9);
}

void DlgPhone::on_pbKeyStar_clicked()
{
    DialedNumber = DialedNumber+"*";
    ui->leNumber->setText(DialedNumber);
    activeCall->sendDTMF("*");

}

void DlgPhone::on_pbKey0_clicked()
{
//    DialedNumber = DialedNumber+"0";
 //   ui->leNumber->setText(DialedNumber);
    processKeyPressed(0);


}

void DlgPhone::on_pbKeyHash_clicked()
{
    DialedNumber = DialedNumber+"#";
    ui->leNumber->setText(DialedNumber);
    activeCall->sendDTMF("#");

}

void DlgPhone::on_pbClear_clicked()
{
    DialedNumber = "";
    ui->leNumber->setText(DialedNumber);

    iCurrContext = CONTEXT_NORMAL;

    dlgIntercom->setContext(iCurrContext);
    displayMsg("");
}

void DlgPhone::on_pbBack_clicked()
{
    DialedNumber = ui->leNumber->text();
    DialedNumber.chop(1);// = remove(DialedNumber);
    ui->leNumber->setText(DialedNumber);
}

void DlgPhone::on_pbDial_clicked()
{

    QRegExp chkNum("[*,0-9]\\d*");
    RisipBuddy *buddy;


    if (!chkNum.exactMatch(ui->leNumber->text()))
    {
        QMessageBox::warning(this,"Dial","Invalid number: "+ ui->leNumber->text() );
        return;
    }

    buddy = new RisipBuddy();



    DialedNumber = ui->leNumber->text();


    RisipCall *call = new RisipCall();
    displayMsg("");

    activeCall = call;

    //connect(call, SIGNAL(onBuddyChanged(RisipBuddy *)), this, SLOT(onBuddyChanged(RisipBuddy *)));
    connect(activeCall, SIGNAL(buddyChanged(RisipBuddy *)), this, SLOT(onBuddyChanged(RisipBuddy *)));

    if (DialedNumber.length() == 0)
    {
        displayMsg("No number to dial.");
    }
    else
    {

        logConsole("Outbound call -- DIaling:"+DialedNumber);

        buddy->setContact(DialedNumber);
        buddy->setAccount(m_account); //rsip->defaultAccount());
        //activeCall->setAccount(m_account);
        //activeCall->setBuddy(&buddy);
        call->setAccount(m_account);
        call->setBuddy(buddy);
//
        //        ui->lblMsg->setText("TBD"+DialedNumber);
        displayCallStatus("Dialing..");

//        activeCall->call();
        call->call();
        ui->pbHangup->setDisabled(false);
        emit onHangupBtn(1);
        bDialRequest = true;

    }
}

void DlgPhone::on_pbSetting_clicked()
{
    dlgSetting->setLogWindow(dlgLog);

    dlgSetting->show();
}

void DlgPhone::on_pbAnswer_clicked()
{
    this->processAnswer();
}

void DlgPhone::processAnswer()
{
    //int callStatus = activeCall->status();
    //QMessageBox::information(this, "ActiveCall", "Status="+QString::number(callStatus));

    activeCall->answer();

    displayCallStatus("Call Answered..");


    ui->pbHangup->setDisabled(false);
    emit onHangupBtn(1);
    ui->pbHold->setDisabled(false);
    bHoldStatus = false;

    logConsole("Clcked Answer Button");
    stopRingTone();
    ui->pbHangup->setStyleSheet(QString("QPushButton {background-color: %1}").arg(COLOUR_RED));
}


void DlgPhone::on_pbHold_clicked()
{
    this->processHold();
}

void DlgPhone::processHold()
{
    int callStatus = activeCall->status();
    //QMessageBox::information(this, "ActiveCall", "Status="+QString::number(callStatus));


        if ( bHoldStatus == false)
        {
            logConsole("Clcked Answer Hold Button");

            if ( callStatus == RisipCall::Status::CallConfirmed)
            {
                displayCallStatus("Call on Hold");

                activeCall->hold(true);

                bHoldStatus = true;

                ui->pbHold->setText("Unhold");
            }
            else
            {
                displayMsg("No Call Connected["+QString::number(callStatus)+"]");

            }
        }
        else
        {
            logConsole("Clcked Answer Unhold Button");

            displayMsg("Conversation.");

//            activeCall->hold(false);
            activeCall->reinvite();

            bHoldStatus = false;

            ui->pbHold->setText("Hold");

        }

}



void DlgPhone::on_pbHangup_clicked()
{
    this->processHangup();
}

void DlgPhone::processHangup()
{
    bHoldStatus = false;
    ui->pbHold->setText("Hold");
    displayMsg("");

    int callStatus = activeCall->status();
   // QMessageBox::information(this, "ActiveCall", "Status="+QString::number(callStatus));
    logConsole("Clcked Hangup Button");

    if ( callStatus == RisipCall::Status::CallConfirmed ||
         callStatus == RisipCall::Status::IncomingCallStarted ||
         callStatus == RisipCall::Status::OutgoingCallStarted)
    {
        displayCallStatus("Disconnected.");
        activeCall->hangup();
        iCurrContext = CONTEXT_NORMAL; // normal dial
        stopRingTone();
        ui->leNumber->setText("");

    }
    else
    {
        displayMsg("No Call Connected["+QString::number(callStatus)+"]");
    }
    ui->pbHangup->setStyleSheet("QPushButton {background-color: white}");
    ui->pbMute->setDisabled(true);
//    setMute(0);

}

void DlgPhone::onCallStatusChanged1(RisipAccount *account)
{
    int Status =account->status();

    QString msg = "onCallStatusChanged1::Status="+QString::number(Status);
    logConsole(msg);

}

void DlgPhone::onBuddyChanged(RisipBuddy *buddy)
{
    QString uri = buddy->uri();
    QString contact= buddy->contact();

    QString msg = "onBuddyChanged::uri->"+uri+" Contact->"+contact;
    logConsole(msg);

//    QMessageBox::information(this,"Call","onBuddyChanged::uri->"+uri+" Contact->"+contact);

}
/*void DlgPhone::onMediaChanged(RisipMedia *media)
{
    QMessageBox::information(this,"Call","Call");

}
*/
void DlgPhone::onCallIdChanged(int callId)
{
    QString msg = "onCallIdChanged"+QString::number(callId);
    logConsole(msg);
//    QMessageBox::information(this,"Call","onCallIdChanged: "+QString::number(callId));

}
void DlgPhone::onCallTypeChanged(int type)
{
//    QMessageBox::information(this,"Call",);
    QString msg = "onCallTypeChanged: "+ QString::number(type);

    logConsole(msg);

}
/*
void DlgPhone::onTimestampChanged(QDateTime timestamp)
{
    QMessageBox::information(this,"Call","Call");

}
*/
void DlgPhone::onCallDirectionChanged(int direction)
{
//    QMessageBox::information(this,"Call","onCallDirectionChanged: "+QString::number(direction));
    QString msg = "onCallDirectionChanged: "+QString::number(direction);

    logConsole(msg);

}
void DlgPhone::onErrorCodeChanged(int code)
{
//    QMessageBox::information(this,"Call","onErrorCodeChanged:"+QString::number(code));
    QString msg ="onErrorCodeChanged:"+QString::number(code);

    logConsole(msg);

}
void DlgPhone::onErrorMessageChanged(const QString &message)
{
//    QMessageBox::information(this,"Call","onErrorMessageChanged: "+message);
    QString msg ="onErrorMessgeChanged:"+message;

    logConsole(msg);

}
void DlgPhone::onErrorInfoChanged(const QString &info)
{
//    QMessageBox::information(this,"Call","onErrorInfoChanged: "+info);
    QString msg ="onErrorInfoChanged: "+info;
    logConsole(msg);

}
void DlgPhone::onlastResponseCodeChanged(int response)
{
//    QMessageBox::information(this,"Call","onlastResponseCodeChanged: "+ QString::number(response));
    QString msg = "onlastResponseCodeChanged: "+ QString::number(response);
    logConsole(msg);

    // handle respone code
    if ( (response >= 400) && (response <= 600))
    {
        if (response == 404)
        {
            displayMsg("Call ended.");
        }
        bDialRequest =false;
//        displayMsg("Unable to perform.["+QString::number(response) + "]");
        ui->pbHangup->setDisabled(true);
        emit onHangupBtn(0);
        ui->leNumber->setText("");
    }
}

//Call Manager
//void DlgPhone::onActiveAccountChanged(RisipAccount *activeAccount)
//{
//    QMessageBox::information(this,"CallManager","onActiveAccountChanged");
//    QString msg = "onActiveAccountChanged";
 //   logConsole(msg);
//
//}

void DlgPhone::onActiveCallChanged(RisipCall *call)
{
    int status = activeCall->status();

//    connect(call, SIGNAL(onBuddyChanged(RisipBuddy *)), this, SLOT(onBuddyChanged(RisipBuddy *)));
    connect(activeCall, SIGNAL(buddyChanged(RisipBuddy *)), this, SLOT(onBuddyChanged(RisipBuddy *)));

    QString msg = "onActiveCallChanged:"+ QString::number(status)+":"+ callStatusToMsg(status) ;


//     displayCallStatus(msg);
     logConsole(msg);
/*     if ( call == nullptr)
     {
         logConsole("Call is nullpte on ActiveCallChd");
     }
     else
     {
         RisipBuddy *buddy = call->buddy();
         QString x=buddy->uri();
     }
     */
//     ui->pbHangup->setDisabled(false);
     if (status == RisipCall::Status::IncomingCallStarted)// || status == RisipCall::Status::OutgoingCallStarted)
     {
         if (iDND == 1)
          {
            activeCall->hangup();
         }
         else
         {
             ui->pbHangup->setDisabled(false);
             emit onHoldBtn(1);
         }
     }
}

/*void DlgPhone::onInIncomingCall(risip::RisipCall *call)
{
    QMessageBox::information(this,"CallManager","onIncoingCall---");

}
*/


void DlgPhone::onOutgoingCall(risip::RisipCall *call)
{
//    QMessageBox::information(this,"CallManager","onOutgoingCall");
logConsole("Outgoing Call Detected");
}


void DlgPhone::on_pbConfCall_clicked()
{
    dialSpecial(CODE_CONF, pd.name, "Conerenceing mode.");
    ui->pbAddCall->setDisabled(false);
}

void DlgPhone::on_pbDND_clicked()
{
    if ( iDND == 0)
    {
        ui->pbDND->setStyleSheet(QString("QPushButton {background-color: %1}").arg(COLOUR_YELLOW));
        iDND=1;
    }
    else
    {
        ui->pbDND->setStyleSheet("QPushButton {background-color: white}");
        iDND = 0;
    }
}

void DlgPhone::on_pbTxfr_clicked()
{
    int status = activeCall->status();

    if (status ==  RisipCall::Status::CallConfirmed)
    {
        dlgIntercom->setContext(1);
        iCurrContext = CONTEXT_TXFR;
        on_pbIntercom_clicked();
        displayMsg("Click on Intercom to Transfer.");
//    dlgIntercom->show();
    }
    else
    {
        QMessageBox::warning(this, "Transfer","No active call to transfer");
    }
}


void DlgPhone::processTransfer(int iSource)
{
    int status = activeCall->status();

    if (status ==  RisipCall::Status::CallConfirmed)
    {
        dlgIntercom->setContext(1);
        iCurrContext = CONTEXT_TXFR;
        if ( iSource ==1)
            on_pbIntercom_clicked();

        displayMsg("Click on Intercom to Transfer.");
//    dlgIntercom->show();
    }
    else
    {
        QMessageBox::warning(this, "Transfer","No active call to transfer");
    }
}



void DlgPhone::on_pbAddCall_clicked()
{
    dlgIntercom->setContext(2);
    iCurrContext = CONTEXT_CONF;
    on_pbIntercom_clicked();
    displayMsg("Click on Intercom to Invite.");

}

void DlgPhone::dialSpecial(QString code, QString num, QString msg)
{


    RisipBuddy *buddy;
    QString NumberToDial;
    QRegExp chkNum("[*,0-9]\\d*");
//    DialedNumber = ui->leNumber->text();

     if (!chkNum.exactMatch(num))
     {
        QMessageBox::warning(this,"Dial","Invalid number: "+num);

     }
     else
     {
        NumberToDial = code + num;

        logConsole("Barge-in DIaling:" + NumberToDial);
        buddy = new RisipBuddy();

        buddy->setContact(NumberToDial);
        buddy->setAccount(m_account); //rsip->defaultAccount());

        activeCall->setAccount(m_account);
        activeCall->setBuddy(buddy);

        displayCallStatus("Dialing..");

        activeCall->call();
        bDialRequest = true;

        displayMsg(msg);
    }
}

void DlgPhone::on_pbBergein_clicked()
{
    this->processBargeIn();
}

void DlgPhone::processBargeIn(int iSource)
{
   DialedNumber = ui->leNumber->text();
   if (DialedNumber.length() == 0)
   {
       displayMsg("Select an Extension to Barge-in mode");
       iCurrContext = CONTEXT_BARGEIN;
       dlgIntercom->setContext(iCurrContext);
        // if request from Intercom panel
       if (iSource == 1)
           on_pbIntercom_clicked();

   }
   else
   {
       displayMsg("Select an Extension to Barge-in mode");
       dialSpecial(CODE_BARGEIN,DialedNumber, "Call Barge-in");
   }
   ui->pbAnswer->setStyleSheet(QString("QPushButton {background-color: %1}").arg(COLOUR_YELLOW));

}

void DlgPhone::on_pbWhisper_clicked()
{
    this->processWhisper();
}

void DlgPhone::processWhisper(int iSource)
{
   DialedNumber = ui->leNumber->text();

   if (DialedNumber.length() == 0)
   {
       displayMsg("Select an Extension to Whisper.");

       iCurrContext = CONTEXT_WHISPER;
       dlgIntercom->setContext(iCurrContext);
       if ( iSource == 1)
           on_pbIntercom_clicked();

   }
   else
    {
        dialSpecial(CODE_WISHPER,DialedNumber," Call in Whisper mode");
    }

}


void DlgPhone::setSignal()
{
    connect(rsip->sipEndpoint(),SIGNAL(statusChanged(int)), this, SLOT(onEPStatusChange(int)) );

    connect(rsip->defaultAccount(), SIGNAL(statusChanged(int)), this, SLOT(onDefAcctStatusChange(int)));

    connect(rsip->createAccount(&cfg), SIGNAL(statusChanged(int)), this, SLOT(onAcctStatusChange(int)));

    connect(activeCall, SIGNAL(statusChanged()), this, SLOT(onCallStatusChanged()));
    connect(activeCall, SIGNAL(accountChanged(RisipAccount *)), this, SLOT(onCallStatusChanged1(RisipAccount *)));
    connect(activeCall, SIGNAL(buddyChanged(RisipBuddy *)), this, SLOT(onBuddyChanged(RisipBuddy *)));
//    connect(activeCall, SIGNAL(mediaChanged(RisipMedia *media)),` this, SLOT(onMediaChanged(RisipMedia *media)));
    connect(activeCall, SIGNAL(callIdChanged(int )), this, SLOT(onCallIdChanged(int )));
    connect(activeCall, SIGNAL(callTypeChanged(int )), this, SLOT(onCallTypeChanged(int )));
  //  connect(activeCall, SIGNAL(timestampChanged(QDateTime timestamp)), this, SLOT(onTimestampChanged(QDateTime timestamp)));
    connect(activeCall, SIGNAL(callDirectionChanged(int )), this, SLOT(onCallDirectionChanged(int )));
    connect(activeCall, SIGNAL(errorCodeChanged(int )), this, SLOT(onErrorCodeChanged(int )));
    connect(activeCall, SIGNAL(errorMessageChanged(const QString &)), this, SLOT(onErrorMessageChanged(const QString &)));
    connect(activeCall, SIGNAL(errorInfoChanged(const QString &)), this, SLOT(onErrorInfoChanged(const QString &)));
    connect(activeCall, SIGNAL(lastResponseCodeChanged(int )), this, SLOT(onlastResponseCodeChanged(int )));



    //Call Manager Signals
    connect(activeCallMgr,  SIGNAL(incomingCall(risip::RisipCall *)),
                     this, SLOT(onIncomingCall(risip::RisipCall *)));

    connect(activeCallMgr, SIGNAL(outgoingcall(risip::RisipCall *)),
                     this, SLOT(onOutGoingCall(risip::RisipCall *)));

    connect(activeCallMgr, SIGNAL(activeCallChanged(RisipCall *)),
                                    this, SLOT(onActiveCallChanged(RisipCall *)));

    connect(activeCallMgr,  SIGNAL(onCLIP(QString)),
                     this, SLOT(onIncomingCallCLIP(QString)));

    /*
     * connect(activeCallMgr, SIGNAL(activeAccountChanged(RisipAccount *activeAccount)),
                                    this, SLOT(onActiveAccountChanged(RisipAccount *activeAccount)));

*/

/*    connect(activeCallMgr, SIGNAL(activeCallHistoryModelChanged(QAbstractItemModel *model)),
                                    this, SLOT(onActiveCallHistoryModelChanged(QAbstractItemModel *model)));

    connect(activeCallMgr, SIGNAL(activeCallHistoryModelChanged(QAbstractItemModel *model)),
                                    this, SLOT(onActiveCallHistoryModelChanged(QAbstractItemModel *model)));

    connect(activeCallMgr, SIGNAL(activeCallHistoryModelChanged(QAbstractItemModel *model)),
                                    this, SLOT(onActiveCallHistoryModelChanged(QAbstractItemModel *model)));
*/

    // rcv data from phonebook

}

void DlgPhone::  acctountIncomingCall()
{
    logConsole("Account in call");
}

void DlgPhone::setButtonState()
{
    ui->pbDND->setDisabled(false);
    ui->pbAddCall->setDisabled(true);
    ui->pbBergein->setDisabled(false);
    ui->pbWhisper->setDisabled(false);
    ui->pbSpeedDial->setDisabled(false);
    ui->pbTxfr->setDisabled(false);
    ui->pbAnswer->setDisabled(true);
    emit onAnswerBtn(0);
    ui->pbHangup->setDisabled(false);
    emit onHangupBtn(1);

    ui->pbHold->setDisabled(true);
    ui->pbConfCall->setDisabled(false);
    ui->pbChat->setDisabled(true);
    ui->pbStatus->hide();
    //ui->pbMinimize->hide();
    ui->pbReport->hide();
    ui->lblPresence->hide();
    //    ui->leNumber->setInputMask("");
    // Configure for speed dial keys
    keyPressedTimer = new QTimer(this);
    periodicVMTimer = new QTimer(this);
    periodicStatusTimer = new QTimer(this);

    ui->pbMute->setDisabled(true);

    ui->pbRadio->hide();
    longPressed = 0;

//    ui->pbVMNext->setDisabled(true);
  //  ui->pbVMPrev->setDisabled(true);

    ui->pbVMNext->hide();
    ui->pbVMPrev->hide();


    connect(ui->pbKey1, SIGNAL(pressed() ), this, SLOT(keyPressed()));
    connect(ui->pbKey1, SIGNAL(released() ), this, SLOT(keyReleased()));
    connect(keyPressedTimer, SIGNAL(timeout()), this, SLOT(timerTimeout()));
    connect(ui->pbKey2, SIGNAL(pressed() ), this, SLOT(keyPressed()));
    connect(ui->pbKey2, SIGNAL(released() ), this, SLOT(keyReleased()));
    connect(ui->pbKey3, SIGNAL(pressed() ), this, SLOT(keyPressed()));
    connect(ui->pbKey3, SIGNAL(released() ), this, SLOT(keyReleased()));
    connect(ui->pbKey4, SIGNAL(pressed() ), this, SLOT(keyPressed()));
    connect(ui->pbKey4, SIGNAL(released() ), this, SLOT(keyReleased()));
    connect(ui->pbKey5, SIGNAL(pressed() ), this, SLOT(keyPressed()));
    connect(ui->pbKey5, SIGNAL(released() ), this, SLOT(keyReleased()));
    connect(ui->pbKey6, SIGNAL(pressed() ), this, SLOT(keyPressed()));
    connect(ui->pbKey6, SIGNAL(released() ), this, SLOT(keyReleased()));
    connect(ui->pbKey7, SIGNAL(pressed() ), this, SLOT(keyPressed()));
    connect(ui->pbKey7, SIGNAL(released() ), this, SLOT(keyReleased()));
    connect(ui->pbKey8, SIGNAL(pressed() ), this, SLOT(keyPressed()));
    connect(ui->pbKey8, SIGNAL(released() ), this, SLOT(keyReleased()));
    connect(ui->pbKey9, SIGNAL(pressed() ), this, SLOT(keyPressed()));
    connect(ui->pbKey9, SIGNAL(released() ), this, SLOT(keyReleased()));


    // present status update
    connect(dlgPresence, SIGNAL(changePresence(int)), this, SLOT(setPresence(int)));

    // Phonebook
    connect(dlgPhonebook, SIGNAL(phoneBookData(QString)), this, SLOT(rcvDataFromPhoneBook(QString)));

    //Intercom panel
    connect(dlgIntercom, SIGNAL(DialNumber(QString)), this, SLOT(autoDial(QString)));

    //vm count periodic process
    connect(periodicVMTimer, SIGNAL(timeout()), this, SLOT(displayVMCount()));

    //vm count periodic process
    connect(periodicStatusTimer, SIGNAL(timeout()), this, SLOT(processPeriodicExtnStatus()));

}



void DlgPhone::on_pbChat_clicked()
{
/*    int x = this->x();
    int y = this->y();
    int ht = this->height();

    dlgChat->setPos(x, y+ht);
    dlgChat->show();
    */
}

void DlgPhone::on_pbMute_clicked()
{

    if ( bMute == false)
    {
        setMute(1);

    }
    else
    {
        setMute(0);
    }
}


void DlgPhone::setMute(int sts)
{
    if ( sts == 1 )
    {

        if (lMicVol <=0)
        {
            lMicVol = activeCall->media()->micVolume();
        }

        QString s = QString::number(lMicVol);

        activeCall->media()->setMicVolume(0);
         ui->pbMute->setText("Unmute");

         bMute = true;
         logConsole("MicVal in Mute:"+s);

    }
    else
    {
        QString s = QString::number(lMicVol);
        logConsole("MicVal in Unmute:"+s);

        if (lMicVol <= 0)
            lMicVol = 19;

        activeCall->media()->setMicVolume(lMicVol);
        ui->pbMute->setText("Mute");
        bMute = false;

    }

}

void DlgPhone::on_pbRegister_clicked()
{

    if (bAccountRegistered== true)
    {
        logConsole("Logout");

        rsip->defaultAccount()->logout();
    }
    else
    {
        logConsole("Login");

        /*
        dp.readPhoneData(pd);


        cfg.setUri(pd.uri);//("sip:1000@arcanemobile.com");
        cfg.setUserName(pd.name); //("1000");
        cfg.setPassword(pd.auth); //("1000");
        cfg.setScheme (pd.scheme); //"digest");
        cfg.setServerAddress(pd.serverName); //("arcanemobile.com:5065");
        cfg.setProxyServer(pd.proxy);//"");

        logConsole("Name:"+pd.name);
        logConsole("Auth:"+pd.auth);
        logConsole("Server:"+pd.serverName);
        logConsole("Uri:"+pd.uri);

        m_account = rsip->createAccount(&cfg);

//        m_account = rsip->accountForConfiguration(&cfg);

        rsip->setDefaultAccount(cfg.uri());

        // default Account login
        */

        rsip->defaultAccount()->login();

     }
}

void DlgPhone::on_pbExit_clicked()
{
    if ( QMessageBox::question(this, "Exit","Do you want to exit the phone?") == QMessageBox::No)
    {
        return;
    }
    else
    {
        QCoreApplication::exit();
    }
}

void DlgPhone::on_pbMinimize_clicked()
{
/*  Qt::WindowFlags x = windowFlags();
  x = x|Qt::WindowMinMaxButtonsHint;

    setWindowFlags( x);
    */
//    ui->pbMinimize->setDisabled(true);

//    logConsole("Callid:"+QString::number(activeCall->callId()));
//    activeCall->sendDTMF("1");

    pj_str_t to;
    pj_str_t text;
    const char *t = "7005";
    const char * x="This is a  message.";

    to = pj_str ((char *)t);
    text = pj_str((char *)x);
    int id = rsip->sipEndpoint()->activeTransportId();

    int ret  = pjsua_im_send(rsip->sipEndpoint()->activeTransportId(), &to, NULL, &text, NULL, NULL );

    logConsole("id:" + QString::number(id) + " SendMsg ret:"+ QString::number(ret));
}

void DlgPhone::setTimerUpdate()
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(UpdateCallStatus()));
    timer->start(1000);
}


// This is called every one sec and updates the status of the Call
void DlgPhone::UpdateCallStatus()
{

    int status = activeCall->status();

    //ui->lblCallStatus->setText(callStatusToMsg(status));
    displayCallStatus(callStatusToMsg(status));

    switch(status)
    {
        case RisipCall::Status::Null:
            ui->pbHold->setDisabled(true);
            emit onHoldBtn(0);

            ui->pbHangup->setDisabled(true);
            emit onHangupBtn(0);

            ui->pbAnswer->setDisabled(true);
            emit onAnswerBtn(0);
            ui->pbAnswer->setStyleSheet("QPushButton {background-color: white}");
            ui->pbHangup->setStyleSheet("QPushButton {background-color: white}");
            //ui->leNumber->setText("");
            ui->pbVMNext->setDisabled(true);
            ui->pbVMPrev->setDisabled(true);

            ui->pbAddCall->setDisabled(true);
            ui->pbTxfr->setDisabled(true);

            stopRingTone();
            break;
        case RisipCall::Status::CallEarly:
            break;
        case RisipCall::Status::CallConfirmed:
            ui->pbHold->setDisabled(false);
            emit onHoldBtn(1);

            ui->pbHangup->setDisabled(false);
            emit onHangupBtn(1);

            ui->pbAnswer->setStyleSheet("QPushButton {background-color: white}");
            ui->pbHangup->setStyleSheet(QString("QPushButton {background-color: %1}").arg(COLOUR_RED));
            ui->pbMute->setDisabled(false);

            ui->pbTxfr->setDisabled(false);

            break;
        case RisipCall::Status::CallDisconnected:
            ui->pbHold->setDisabled(true);
            emit onHoldBtn(0);
            ui->pbHangup->setDisabled(true);
            emit onHangupBtn(0);
            ui->pbAnswer->setDisabled(true);
            emit onAnswerBtn(0);

            ui->pbAnswer->setStyleSheet("QPushButton {background-color: white}");
            ui->pbHangup->setStyleSheet("QPushButton {background-color: white}");
            ui->pbMute->setDisabled(true);
            ui->pbVMNext->setDisabled(true);
            ui->pbVMPrev->setDisabled(true);

            ui->pbAddCall->setDisabled(true);
            ui->pbTxfr->setDisabled(true);

            //setMute(0);

        logConsole("Ans:Setting white");
            iCurrContext= CONTEXT_NORMAL;
            ui->leNumber->setText("");
            displayMsg("");

            break;
        case RisipCall::Status::ConnectingToCall:
            ui->pbHangup->setDisabled(false);
            emit onHangupBtn(1);
            ui->pbAnswer->setDisabled(false);
            emit onAnswerBtn(1);

            ui->pbMute->setDisabled(false);

            //change the colour of answer button
            ui->pbAnswer->setStyleSheet(QString("QPushButton {background-color: %1}").arg(COLOUR_YELLOW));
logConsole("Ans:Setting Yellow");
            break;
        case RisipCall::Status::IncomingCallStarted:
            ui->pbHangup->setDisabled(false);
            emit onHangupBtn(1);

//            ui->pbHangup->setStyleSheet("QPushButton {background-color: white}");

            //DND handled
            break;
        case RisipCall::Status::OutgoingCallStarted:
            ui->pbHangup->setDisabled(false);
            emit onHangupBtn(1);
            ui->pbAnswer->setDisabled(true);
            emit onAnswerBtn(0);
            ui->pbMute->setDisabled(false);

            ui->pbAnswer->setStyleSheet("QPushButton {background-color: white}");
            logConsole("Ans:Setting white");
            break;
        default:
            break;
    }

    status = rsip->defaultAccount()->status();// sipEndpoint()->status();
    //ui->lblPhStatus->setText(phoneStatusToMsg(status));
    displayPhoneStatus(phoneStatusToMsg(status));
}

void DlgPhone::logConsole(QString msg)
{
    dlgLog->log(msg);
}

void DlgPhone::on_pbLog_clicked()
{
    int width = this->width();
    int xPos =this->x();
    int yPos = this->y();

    if ( dlgLog->isHidden() == true)
    {
        dlgLog->setPos((xPos-width), yPos);

        dlgLog->show();
    }
    else
    {
        dlgLog->hide();
    }
}

// Msg display
void DlgPhone:: displayPhoneStatus(QString msg)
{
    ui->lblPhStatus->setText(msg);
    emit this->onDispPhoneStatus(msg);
}

void DlgPhone::displayCallStatus(QString msg)
{
    ui->lblCallStatus->setText(msg);
    emit this->onDispCallStatus(msg);
}


void DlgPhone::displayMsg(QString msg)
{
    ui->lblMsg->setText(msg);
    emit onDispMsg(msg);
}


void DlgPhone::on_pbSpeedDial_clicked()
{
    showSpeedDialDialog();
}


//Speed Dial functions
//void DlgPhone::on_pbKey1_pressed()
//{
//
//}

//
void DlgPhone::keyPressed()
{
    longPressed = 0;
    logConsole("Key Pressed"+QString::number(longPressed));
    keyPressedTimer->start(500);
}

void DlgPhone::timerTimeout()
{
    longPressed = 1;
    keyPressedTimer->stop();
    logConsole("Key Pressed Timeout"+QString::number(longPressed));

}

void DlgPhone::keyReleased()
{
   logConsole("Key RELEASED"+QString::number(longPressed));
    keyPressedTimer->stop();
    if (longPressed != 1)
    {
          longPressed = 0;

    }
    //if (longPressed == 1)
   // {
     //       displayMsg("Long Pressed");
    //}
    //else
   // {
//        displayMsg("Short Pressed");
     //   longPressed = 0;
   //}
}

void DlgPhone::processLongKeyPressed(int iKey)
{
    QString speedNum="";
    SpeedDialData sd;

    dlgSpeed->getValues(sd);

    logConsole("Inside Long Pressed key:"+QString::number(iKey));

    if ( iKey >= 1 && iKey <= 9)
    {
        speedNum = sd.number[iKey-1];

        logConsole("Inside Long Pressed key, Value:"+speedNum);

        if ( speedNum == nullptr || speedNum == "")
        {
            showSpeedDialDialog();
        }
        else
        {
            DialedNumber = speedNum;
            autoDial(DialedNumber);
//            ui->leNumber->setText(speedNum);
            displayMsg("Dialing: "+ sd.name[iKey-1]);
        }
    }
    else
    {
        return;
    }


}


void DlgPhone::showSpeedDialDialog()
{
    //TBD

    int x = this->x();
    int y = this->y();  
    int width = this->width();
    if ( dlgSpeed->isHidden() == true)
    {

        dlgSpeed->setPos(x + width, y);
        dlgSpeed->updateAndShow(pd.extn);
    }
    else
    {
        dlgSpeed->hide();

    }

}

void DlgPhone::on_pbStatus_clicked()
{
    int presence = m_account->presence();

    dlgPresence->setPresence(presence);
    dlgPresence->setModal(true);
    if (dlgPresence->isHidden() == true )
    {
        dlgPresence->show();
    }
    else
    {
        dlgPresence->hide();

    }
    //QMessageBox::information(this,"Msg","After presence");
}


void DlgPhone::setPresence(int iPresence)
{
    logConsole("Got a signal to change Presence");
    m_account->setPresence(iPresence);
    switch(iPresence)
    {
        case 1:
            ui->lblPresence->setText("Online");
           break;
        case 2:
        ui->lblPresence->setText("Offline");
            break;
        case 3:
        ui->lblPresence->setText("Away");
            break;
        case 4:
        ui->lblPresence->setText("Busy");
            break;
    }

}

void DlgPhone::on_pbPhoneBook_clicked()
{

    int x = this->x();
    int y = this->y();
    int width = this->width();
    dlgPhonebook->setPos(x + width, y);

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


//Data from Phonebook
void DlgPhone::rcvDataFromPhoneBook(QString num)
{
    QString dialedNum;
    QMap <QString, Extension *> extnlist;

    QMap <QString, Extension *>::iterator itr;


    commapi capi("192.168.100.127", 9000);


    dialedNum = num;
    if ( capi.readExtensionData(extnlist) > 0)
    {
        for (itr = extnlist.begin(); itr != extnlist.end(); itr++)
        {
//qDebug() <<"Name:" << itr.value()->extnName << "::" << num << ":::" << itr.value()->extn;
            if (itr.value()->endPoint.name == num)
            {
                    dialedNum = itr.value()->endPoint.extn;
                    break;
            }
        }
    }
//        qDebug() << "proc ends";

    logConsole("Data Rcvd from PhoneBook: "+ num + "::" + dialedNum);
 //   ui->leNumber->setText(dialedNum);
    displayMsg("Dialing: "+ num +" ["+ dialedNum+" ]");

    autoDial(dialedNum);
}

void DlgPhone::on_pbIntercom_clicked()
{
    int height = this->height();
    int xPos =this->x();
    int yPos = this->y();
    int rc=0;

    dlgIntercom->setContext(iCurrContext);
    if ( dlgIntercom->isHidden() == true)
    {
        dlgIntercom->setPos((xPos), yPos+height+70);
        rc = dlgIntercom->displayIntercomPage(pd.extn);

        if ( rc == 1)
            dlgIntercom->show();
        else
        {
            QMessageBox::warning(this,"Display","Unable to displa, ret code="+QString::number(rc));
        }
    }
    else
    {
        dlgIntercom->show();
    }

    ui->leNumber->setText("");

}

void DlgPhone::on_pbRadio_clicked()
{
 /*   if ( dlgowspanel == nullptr)
        dlgowspanel = new DlgOWSPanel();

    dlgowspanel->displayMMI(pd.extn);
    dlgowspanel->show();
*/
}

void DlgPhone::on_pbReport_clicked()
{
    dlgReport->setModal(true);

    if ( dlgReport->isHidden() == true)
    {
        dlgReport->displayCDR();
        dlgReport->show();
    }
    else
    {
        dlgReport->hide();

    }
}


void DlgPhone::autoDial(QString phoneNum)
{
    RisipBuddy buddy;
    QString prefixedNumber;

    QRegExp chkNum("[*,0-9]\\d*");
//    DialedNumber = ui->leNumber->text();

     if (!chkNum.exactMatch(phoneNum))
     {
        QMessageBox::warning(this,"Dial","Invalid number: "+ phoneNum);
        return;
     }

     DialedNumber = phoneNum;

     displayMsg("Dialing: "+phoneNum);
//    ui->lblMsg->clear();

    //ui->leNumber->setText(DialedNumber);
    if (DialedNumber.length() == 0)
    {
        displayMsg("No number to dial.");
        dlgIntercom->setContext(0);
    }
    else
    {
        switch(iCurrContext)
        {

            case CONTEXT_TXFR:
                prefixedNumber = CODE_TXFR+DialedNumber;
                activeCall->sendDTMF(prefixedNumber);
      //          dlgIntercom->setContext(0);
                logConsole("Txfr:"+prefixedNumber);

                break;
            case CONTEXT_CONF:
                prefixedNumber = CODE_ADD+DialedNumber;
                activeCall->sendDTMF(prefixedNumber);
                logConsole("Add Conf:"+prefixedNumber);
      //          dlgIntercom->setContext(0);
                break;
            case CONTEXT_BARGEIN:
                dialSpecial(CODE_BARGEIN,DialedNumber, "Call in Barge-in mode.");
                prefixedNumber = CODE_BARGEIN + DialedNumber;

                break;

            case CONTEXT_WHISPER:
                dialSpecial(CODE_WISHPER,DialedNumber, "Call in Whisper mode.");
                prefixedNumber = CODE_WISHPER + DialedNumber;

                break;

            default:
                prefixedNumber="";
        }


        if ( iCurrContext == CONTEXT_NORMAL)
        {
            logConsole("Outbound call -- DIaling:"+DialedNumber);
//            ui->leNumber->setText(DialedNumber);
            buddy.setContact(DialedNumber);
            buddy.setAccount(m_account); //rsip->defaultAccount());

            activeCall->setAccount(m_account);
            activeCall->setBuddy(&buddy);

//        ui->lblMsg->setText("TBD"+DialedNumber);
            displayCallStatus("Dialing..");

            activeCall->call();

            ui->pbHangup->setDisabled(false);
            emit onHangupBtn(1);
            bDialRequest = true;

        }
        else
        {
            ui->leNumber->setText(""); //prefixedNumber);
        }
    }
//    iCurrContext = 0;
    if ( iCurrContext != CONTEXT_CONF)
    {
        iCurrContext = CONTEXT_NORMAL;
        dlgIntercom->setContext(iCurrContext);

    }
}

void  DlgPhone::onIncomingCallCLIP(QString clp)
{
    QString name;
    logConsole(clp);
//    displayMsg("In-call:"+clp);
    name = getExtnName(clp, name);
    qDebug() << "Clip" << clp << name;
    displayMsg("Incall from: "+ name);
}

void DlgPhone::on_pbVM_clicked()
{
    dialSpecial(CODE_VM, pd.name, "Accessing Voice Message");
    ui->pbVMNext->setDisabled(false);
    ui->pbVMPrev->setDisabled(false);

}

// Display VM Count
void DlgPhone::displayVMCount()
{
    int iCount=0;
      commapi capi("192.168.100.127",9000);
//    qDebug() << "pdName: "<<pd.name;

    iCount = capi.getVMInboxCount(pd.name);
        qDebug() << "Vm Count for pdName: "<<pd.name << iCount ;
    if ( iCount == 0)
    {
        ui->pbVM->setStyleSheet("QPushButton {background-color: white}");
        ui->pbVM->setText("VM:0");
    }
    else if (iCount > 0)
    {
        ui->pbVM->setStyleSheet(QString("QPushButton {background-color: %1}").arg(COLOUR_YELLOW));
        ui->pbVM->setText("VM:"+ QString::number(iCount));
    }
    else
    {
        // do nothing
        return;
    }
    emit vmCount(iCount);
}

// periodic jobs
void DlgPhone::processPeriodicExtnStatus()
{

    T_EndpointStatus eps;
    static int procStat=0;
    QMap <QString, extensionStatus *> extnStsList;
    QMap <QString, extensionStatus *>::iterator itr;
    commapi capi("192.168.100.127", 9000);

//qDebug() << "entry" << procStat;

    if ( procStat == 0)
    {
        procStat = 1;
  //      qDebug() << "proc start";

        extensionStatus extnSts;


        if ( capi.getExtnStatus(extnStsList) > 0)
        {

            for (itr = extnStsList.begin(); itr != extnStsList.end(); itr++)
            {
                strcpy(eps.extn, itr.value()->extn.toLatin1().data());
                eps.iType = itr.value()->iType;
                eps.iStatus = itr.value()->iStatus;
                // process each status
//qDebug() << "extnStatus:"<< itr.value()->name;
                emit endpointStatus(&eps);

                dlgIntercom->changeExtnStatus(itr.value()->extn, itr.value()->iType, itr.value()->iStatus);
            }
        }
//        qDebug() << "proc ends";
        procStat =0;
    }
    else
    {
        qDebug() << "skipped";
    }
}

void DlgPhone::on_pbCallFwd_clicked()
{
    if (dlgCallFwd == nullptr)
    {
        dlgCallFwd = new DlgCallFwd();
    }
    dlgCallFwd->showCallFwd(pd.extn);

}

//RingTone
void DlgPhone::playRingTone()
{
    QString RTFileName="/etc/softphone/rt.wav";
    if ( sndRingTone == nullptr)
    {
        sndRingTone = new QSound(RTFileName);
    }
    sndRingTone->setLoops(30);

    sndRingTone->play();

    ui->pbAnswer->setStyleSheet(QString("QPushButton {background-color: %1}").arg(COLOUR_YELLOW));

   logConsole("RingTone Played");
}

void DlgPhone::stopRingTone()
{

    if ( sndRingTone != nullptr)
    {
        sndRingTone->stop();
       logConsole("RingTone stopped");
       delete sndRingTone;
       sndRingTone = nullptr;
       ui->pbAnswer->setStyleSheet("QPushButton {background-color: white}");
    }
}

// followme
void DlgPhone::on_pushButton_clicked()
{
    if (dlgFollowMe == nullptr)
    {
        dlgFollowMe = new DlgFollowme();
    }
    dlgFollowMe->setModal(true);
    dlgFollowMe->showFollowme(pd.extn);
    dlgFollowMe->show();
}

void DlgPhone::on_pbVMPrev_clicked()
{

}

void DlgPhone::on_pbVMNext_clicked()
{

}

QString DlgPhone::getExtnName(QString clip, QString name)
{
    int i;
    QString str1;
    QString str2;
    QMap<QString,Extension *> extnlist;

    i = clip.indexOf("sip:");
    if ( i > 0)
    {
        str1 = clip.mid(0, clip.indexOf('@'));
        str2 = str1.mid(str1.indexOf(':')+1);
//        displayMsg(name);

        // Get extensions and create list
        commapi capi("192.168.100.127", 9000);

        if (capi.readExtensionData(extnlist) > 0)
        {
            name = extnlist.value(str2)->endPoint.name;
         //   qDebug() << "Clip:" << name;
        }
        else
        {
            name = str2;
           // qDebug() << "Clip-else:" << name;
        }
    }
    else
    {
        name=clip;
    }
    return name;
}


QString DlgPhone::getMyOWSNum()
{

     return pd.name;
}

QString DlgPhone::getOWSName()
{
    return pd.extn;

}


void DlgPhone::setDialContext(int ic)
{
    this->iCurrContext = ic;

}

int DlgPhone::getDialContext()
{
    return (this->iCurrContext);
}
