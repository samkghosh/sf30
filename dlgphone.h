#ifndef DLGPHONE_H
#define DLGPHONE_H

#include <QDialog>
#include <QSound>

#include "dlglog.h"

#include "risip.h"
#include "risipcallmanager.h"
#include "risipmessage.h"


#include "speeddialdialog.h"
#include "dlgsetting.h"

#include "dlgchat.h"
#include "dataprocessing.h"
#include "dlgpresence.h"
#include "dlgphonebook.h"
#include "dlgintercom.h"
#include "dlgreport.h"
#include "dlgcallfwd.h"
#include "dlgfollowme.h"
//#include "dlgowspanel.h"
#include "const.h"



using namespace  risip;



QT_BEGIN_NAMESPACE
namespace Ui { class DlgPhone; class Risip; }
QT_END_NAMESPACE

class DlgPhone : public QDialog
{
    Q_OBJECT

public:
    DlgPhone(QWidget *parent = nullptr);
    ~DlgPhone();
    QString DialedNumber;
    //Phonebook
    DlgPhonebook *dlgPhonebook;

    Risip *rsip;
    RisipEndpoint ep;
    RisipAccount *m_account;
    RisipAccountConfiguration cfg;

    RisipCall *activeCall;

    RisipCallManager *activeCallMgr;

   // Presence
   DlgPresence *dlgPresence;

   QString getMyOWSNum();
   QString getOWSName();
   DlgLog *dlgLog;


   //
   //dial special
   void dialSpecial(QString code, QString num, QString msg);
   void autoDial(QString phoneNum);
   void setDialContext(int ic);
   int getDialContext();
   void processBargeIn(int iSource=1);
   void processWhisper(int iSource=1);
   void processTransfer(int iSource=1);
   void processHangup();
   void processAnswer();
   void processHold();
   void processMute();


private slots:
    void onEPStatusChange(int status);
    void onAcctStatusChange(int status);
    void onDefAcctStatusChange(int status);


    //Calls
    void onCallStatusChanged();
    void onCallStatusChanged1(RisipAccount *account);
    void onBuddyChanged(RisipBuddy *buddy);
//    void onMediaChanged(RisipMedia *media);
    void onCallIdChanged(int callId);
    void onCallTypeChanged(int type);
  //  void onTimestampChanged(QDateTime timestamp);
    void onCallDirectionChanged(int direction);
    void onErrorCodeChanged(int code);
    void onErrorMessageChanged(const QString &message);
    void onErrorInfoChanged(const QString &info);
    void onlastResponseCodeChanged(int response);

    //CallManager
 //   void onActiveAccountChanged(RisipAccount *activeAccount);
    void onActiveCallChanged(RisipCall *call);
    void onOutgoingCall(risip::RisipCall *call);
    void onIncomingCall(risip::RisipCall *call);



    //
    void on_pbKey1_clicked();

    void on_pbKey2_clicked();

    void on_pbKey3_clicked();

    void on_pbKey4_clicked();

    void on_pbKey5_clicked();

    void on_pbKey6_clicked();

    void on_pbKey7_clicked();

    void on_pbKey8_clicked();

    void on_pbKey9_clicked();

    void on_pbKeyStar_clicked();

    void on_pbKey0_clicked();

    void on_pbKeyHash_clicked();

    void on_pbClear_clicked();

    void on_pbBack_clicked();

    void on_pbDial_clicked();

    void on_pbSetting_clicked();

    void on_pbAnswer_clicked();

    void on_pbHold_clicked();

    void on_pbHangup_clicked();

    void on_pbConfCall_clicked();

    void on_pbDND_clicked();

    void on_pbTxfr_clicked();

    void on_pbAddCall_clicked();

    void on_pbWhisper_clicked();

    void on_pbChat_clicked();

    void on_pbMute_clicked();

    void on_pbRegister_clicked();

    void on_pbExit_clicked();

    void on_pbMinimize_clicked();

    void  UpdateCallStatus();

    // used when console log to display
    void on_pbLog_clicked();

    //used when Speed dial button is clicked
    void on_pbSpeedDial_clicked();

  //  void on_pbKey1_pressed();

    // Speed Dial
    void keyPressed();
    void timerTimeout();
    void keyReleased();

    //vm timer
    // Display VM Count
    void displayVMCount();

    //Extension Status
    void processPeriodicExtnStatus();

    void on_pbStatus_clicked();

    //presence
   void setPresence(int presence);

   void on_pbPhoneBook_clicked();

   void on_pbBergein_clicked();

   void rcvDataFromPhoneBook(QString num);
   void on_pbIntercom_clicked();

   void on_pbRadio_clicked();

   void on_pbReport_clicked();


   void onIncomingCallCLIP(QString clp);
   void on_pbVM_clicked();

   void acctountIncomingCall();
   void acctountIncomingMsg(RisipMessage *msg);
   void on_pbCallFwd_clicked();

   void on_pushButton_clicked();

   void on_pbVMPrev_clicked();

   void on_pbVMNext_clicked();

Q_SIGNALS:
    void endpointStatus(T_EndpointStatus *eps);
    void vmCount(int i);
    void incallStatus(int i);
    void onDispPhoneStatus(QString sts);
    void onDispMsg(QString sts);
    void onDispCallStatus(QString sts);
    void onHangupBtn(int iFlag);
    void onAnswerBtn(int iFlag);
    void onHoldBtn(int iFlag);


private:
    Ui::DlgPhone *ui;
    void setSignal();
    void setButtonState();

    // set dialRequest
    bool bDialRequest;
    //Hold Status
    bool bHoldStatus;
    qlonglong lMicVol;
    long int lSpeakerVol;
    bool bMute;
    QString callStatusToMsg(int status);
    void setTimerUpdate();
    QString phoneStatusToMsg(int status);
    bool bAccountRegistered;
    void processKeyPressed(int iKey);

    // Log
    void logConsole(QString msg);

    // Msg display
    void displayPhoneStatus(QString msg);
    void displayCallStatus(QString msg);
    void displayMsg(QString msg);
    DataProcessing dp;

    // Speed dial
    DlgSpeedDial *dlgSpeed;
    QTimer *keyPressedTimer;
    QTimer *periodicVMTimer;
    QTimer *periodicStatusTimer;

    int longPressed;
    void processLongKeyPressed(int iKey);
    void showSpeedDialDialog();
    SpeedDialData sd;
    // settings
     DlgSetting *dlgSetting;
     PhoneData pd;

     //Chat
     DlgChat *dlgChat;


     //Intercom
     DlgIntercom *dlgIntercom;

     // report
//
     DlgReport *dlgReport;

     // DND
     int iDND=0;

     //dialing context
     int iCurrContext =0;//0: normal, 1: transfer, 2: conf

     // PjSip message
     RisipMessage *rsipMsg;


     // call Fwd settings
     DlgCallFwd *dlgCallFwd = nullptr;

     // rington start and stop
     void playRingTone();
     void stopRingTone();
     QSound *sndRingTone = nullptr;

     //Followme
     DlgFollowme *dlgFollowMe = nullptr;

     // OWSpanel
//     DlgOWSPanel *dlgowspanel=nullptr;

     //mute
     void setMute(int sts);

     //get name from number/clip
     QString getExtnName(QString clip, QString name);


};
#endif // DLGPHONE_H
