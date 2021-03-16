#ifndef DLGOWSPANEL_H
#define DLGOWSPANEL_H

#include <QDialog>

#include "dbaccess.h"

#include "dlgphone.h"

#include "dlgconf.h"

#include "dlgchat.h"

#include "dlgphonebook.h"
#include "dlgcallfwd.h"
#include "dlgfollowme.h"



namespace Ui {
class DlgOWSPanel;
}

typedef class
{
public:
    QString extnNo;
    int extnType;
    int iStatus;
    QPushButton *pb;
    QString pbName;
}CPanelButton;

#define TYPE_OWS    0
#define TYPE_RADIO  1
#define TYPE_GSM    2
#define TYPE_SATCOM 3
#define TYPE_CBAND  4
#define TYPE_CSM    5

#define MAX_OWS     12
#define MAX_RADIO   10
#define MAX_GSM     1
#define MAX_SATCOM  2
#define MAX_CBAND   2
#define MAX_CSM     2




class DlgOWSPanel : public QDialog
{
    Q_OBJECT

public:
    explicit DlgOWSPanel(QWidget *parent = nullptr);
    ~DlgOWSPanel();

    void setIncall(QString clip);
    void setCallConnected(QString extnNo);
    void setHangup(QString extnNo);
    void setNums(QString extnNum, QString name, int iType);
    QPushButton * getPBId(QString extnNum);
    void setringing(QString extnNum);
    void setIdle(QString extnNum);
    void setTalking(QString extnNum);
    int displayMMI(QString extn);

    //
    void changeExtnStatus(QString extn, int iType, int iStatus);

private slots:
    void on_pb_setting_clicked();

    // update endpoint signal from dlgPhone
    void updateEndpointSTatus(T_EndpointStatus *eps);
    void on_pb_ManageConf_clicked();

    void on_pb_chat_clicked();

    void on_pb_phonebook_clicked();

    void on_pb_followme_clicked();

    void on_pb_callFwad_clicked();

    void on_pb_vm_clicked();

    void setVMCount(int);
    void on_pb_conference_clicked();

    void on_pb_Intercom1_clicked();

    void on_pb_Intercom3_clicked();

    void on_pb_Intercom5_clicked();

    void on_pb_Intercom7_clicked();

    void on_pb_Intercom9_clicked();

    void on_pb_Intercom2_clicked();

    void on_pb_Intercom4_clicked();

    void on_pb_Intercom6_clicked();

    void on_pb_Intercom8_clicked();

    void on_pb_Intercom10_clicked();

    void on_pb_Intercom11_clicked();

    void on_pb_Intercom12_clicked();

    void onDisplayPhoneStatus(QString msg);
    void onDisplayCallStatus(QString msg);
    void onDisplayMsg(QString msg);

    void on_pb_bergein_clicked();

    void on_pb_wishper_clicked();

    void on_pb_transfer_clicked();

private:
    Ui::DlgOWSPanel *ui;
    int iMaxDevice = MAX_OWS + MAX_RADIO + MAX_GSM + MAX_SATCOM + MAX_CBAND + MAX_CSM;
//    int iMaxDevice;
    QList<CPanelButton *> pbList;
    QMap<QString,Extension *> extensions;
    QString myExtn;
    DlgPhone *dlgPhone;
    QMap<QString, CPanelButton *> endpointList;

    // Conf:
    DlgConf *dlgConf = nullptr;
    DlgChat *dlgChat = nullptr;

    // Followme
    DlgFollowme *dlgFollowMe=nullptr;
    DlgCallFwd  *dlgCallFwd = nullptr;
    DlgPhonebook *dlgPhonebook = nullptr;

    void handlePbClick(QPushButton *pb);

};

#endif // DLGOWSPANEL_H
