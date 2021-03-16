#ifndef COMMAPI_H
#define COMMAPI_H

#include "dbaccess.h"

class commapi
{
    int iPort;
    QString szIPAddr;
public:
    commapi(){};
    commapi(QString szIP,int port);
//    int getFollowMe(QList<FollowMe *> &fmeList, QString extn);
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

};

#endif // COMMAPI_H
