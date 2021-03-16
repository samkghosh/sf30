#ifndef DBACCESS_H
#define DBACCESS_H
#include <QtCore>
#include <QSqlDatabase>

#include "dataprocessing.h"

//insert into ps_aors (id, max_contacts, remove_existing) values (7010, 1, 'yes');
class Aors
{
    //Holds data in ps_aors
public:
    QString extn; //id of aors
    int maxContact; // 1 by default filed max_contacts
    QString removeExisting; //{yes,no} def yes, filed 'remove_existing'
};


//insert into ps_auths (id, auth_type, password, username)
//values (7010, 'userpass', 'pass of 7010, 7010);
class Auths
{
    //Holds data in ps_auths
public:
    QString extn; //id of Auth
    QString authType; //"userpass" always
    QString password; //* password
    QString userName; //username
};


//Endpoint example:insert into ps_endpoints (id, transport, aors, auth, context, disallow, allow,
//direct_media, media_use_received_transport, trust_id_inbound, media_encryption, rtp_symmetric,
//rewrite_contact, force_rport)
//values (7010, 'transport-udp', '7010', '7010', 'outgoingcall', 'all', 'ulaw,alaw',
//'no', 'yes', 'yes', 'no', 'yes', 'yes', 'yes');
class Endpoints
{
    // Holds data in ps_endpoints
public:
    QString extn; // * 'id' of ps_endpoints
    QString transortType; //"transport_udp" allways in filed 'transport'
    QString aors; //id of ps_aors in filed 'aors'
    QString auth; //id of ps_auths in field 'auth'
    QString context; //* text in the filed 'context' 'i.e. outgoingall
    QString disallowCodecs; // "all" always in the filed 'disallow'
    QString allowCodecs; //* in allow field 'allow', i.e. alaw,ulaw
    QString directMedia; //def no, {yes,no}'iled 'direct_media'
    QString mediaUseRcvdTransport; //def 'yes', {yes,no} in the filed 'media_use_received_transport'
    QString trustIdInbound; // {yes,no} def yes, field 'trust_id_inbound
    QString mediaEncryption; //* {yes,no}, def no, field 'media_encryption
    QString rtpSymetric; //{yes,no}, def yes, field 'rtp_symetric
    QString rewiteContact; //{yes,no}, def yes, filed 'rewrite_contact
    QString forceRport; //{yes,no}, def yes, filed 'force_rport
    QString callForward; //{yes,no}
    QString forwardBusy; //"VM" or any extension
    QString forwardNoAns; //"VM" or any extension
    int adminMode; //* {0,1} def no, yes:  bargein or spy/wishper allowed, no: notallowed
    int recMode; //* 0: no recording, 1: chan rec, 2: converation recording, 3: both Chan and Conversation
    QString name; // * name associated with the extension
    int iType;
    int iStatus;
};

class Extension
{
public:
    Endpoints endPoint;
    Aors    aors;
    Auths   auths;
};


//CDR
class CDR
{
public:
    QDateTime dateTime;
    QString from;
    QString to;
    //int direction;
    QString context;
    int duration;
    QString disposition;
    QString recFileName;
};

//CallFwd
class CallFWD
{
public:
    QString extn;
    int callFwdSet; //0,1
    int callFwdOnNoAns; // 0 = no, 1: fwd to VM, 2: to extn
    QString fwdExtnNoAns;

    int callFwdOnBusy; // 0 = no, 1: fwd to VM, 2: to extn
    QString fwdExtnBusy;

};

//followmeFindme
class FollowMe
{
   public:
    QString name;
    int order;
    QString phone;
    int timeOut;
};

//extension status
class extensionStatus
{
public:
    QString extn;
    int iStatus;
    int iType;
    QString extnName;
};


class DBAccess
{
public:
    DBAccess();
    DBAccess(QString conn);
    ~DBAccess();
    void setDBInfo();
    int openDB();
    void closeDB();
    // Holder of Extension info
  //  QMap <QString, Extension> ExtensionList;
    int readExtensionData(QMap <QString, Extension *> &extnist);

    // CDR Data and recording data
    int readCDRData(QList<CDR *> &cdrs, int type);

    // VM inbox count
    int getVMInboxCount(QString extnNum);

    // call forward settings
    int readCFInfo(QString extn, CallFWD &cf);
    int saveCFInfo(CallFWD &cf);

    //find me followme
    int readFollowme(QList<FollowMe *> &fmeList, QString extn);
    int saveFollowme(QList<FollowMe *> &fmeList, QString extn);

    int getExtnStatus(QMap <QString, extensionStatus *> &extnStatus, QString extn="");

private:
    QString dbServer;
    QString dbUID;
    QString dbPWD;
    QString dbDBName;
    QSqlDatabase db;
    QString connectionName;
    int driverLoaded =0;
    // Holder of Extension info
  //  QMap <QString, Extension> ExtensionList;
};

#endif // DBACCESS_H
