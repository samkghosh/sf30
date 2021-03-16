#include <string.h>
#include "Message.h"
#include "common.h"

#include "commapi.h"

/*********************************************
 * Constructor
 * *********************************************/
commapi::commapi(QString szIP, int port)
{
    QString szServerIP="";
    int iServerPort=9000;
    QSettings iniSetting("/etc/softphone/dbinfo.cfg", QSettings::NativeFormat);

    szServerIP = iniSetting.value("CommServerIP").toString();
    iServerPort = iniSetting.value("CommServerPORT").toInt();

//    qDebug() << "Comm ServerIP:" << szServerIP;
//    qDebug() << "Comm Server Port:" << iServerPort;

    if ( szServerIP == "")
        szIPAddr = szIP;
    else
        szIPAddr = szServerIP;

    if ( iServerPort == 0)
        iPort = port;
    else
        iPort = iServerPort;


}


/***************************************************************************
 * *
 * ***************************************************************************/
int commapi::readFollowme(QList<FollowMe *> &fmeList, QString extn)
{
    char szMsg[255];
   // int iSz;
    int iType;
    char *pch;
    FollowMe *fme;
    int i =0;
    char szIPAddress[32];
    QRegExp rx("(\\,)");
    QStringList values;


    strcpy(szMsg, extn.toLatin1().data());
    strcpy(szIPAddress, szIPAddr.toLatin1().data());


//    qDebug() << "Request for followme for:" << szMsg << "Extn" << extn <<" IP:" << szIPAddress;

    if ( (iType = sendNGetResp(szIPAddress, iPort, szMsg, strlen(szMsg), CMD_GET_FOLLOWME)) < 0)
    {
        qDebug() << "Unable to get followme for:" << szMsg;
        return -1;
    }
    else
    {
        qDebug() << "Msg Rcvd sz:" << strlen(szMsg) << "msg:" << szMsg << "Type:"<< iType;

        fmeList.clear();
        fmeList.empty();

        pch = strtok(szMsg, ";");

        while(pch != NULL)
        {
            values = QString(pch).split(rx);

            fme = new FollowMe;
            fme->name = values[0];
            fme->order = values[1].toInt();
            fme->phone = values[2];
            fme->timeOut = values[3].toInt();

            fmeList.insert(i,fme);
            i++;

            pch = strtok(NULL, ";");
        }
    }
    return i;
}


/********************************************************************************
 *
 * ******************************************************************************/
int commapi::readExtensionData(QMap <QString, Extension *> &extnList)
{
    char szMsg[2048]="";
   // int iSz;
    int iType;
    char *pch;
    int i =0;
    char szIPAddress[32];
    Extension *extn;
    char szBuf[64];
    QRegExp rx("(\\,)");
    QStringList values;

//    strcpy(szMsg, extn.toLatin1().data());
    strcpy(szIPAddress, szIPAddr.toLatin1().data());

    if ( (iType = sendNGetResp(szIPAddress, iPort, szMsg, strlen(szMsg), CMD_GET_EXTN,5)) < 0)
    {
        qDebug() << "Unable to get followme for:" << szMsg;
        return -1;
    }

    extnList.empty();

    pch = strtok(szMsg, ";");
    while(pch != NULL)
    {
        extn = new Extension;

        strcpy(szBuf, pch);
        values = QString(szBuf).split(rx);
//        qDebug() << "values:" << values;
        extn->endPoint.extn = values[0];


        extn->endPoint.name = values[1];

        extn->endPoint.iType = values[2].toInt();

        extnList.insert(extn->endPoint.extn, extn);
        i++;
        pch = strtok(NULL, ";");
    }

    return i;
}


int commapi::saveFollowme(QList<FollowMe *> &fmeList, QString extn)
{
    char szMsg[255]="";
    char szIPAddress[32];

    strcpy(szIPAddress, szIPAddr.toLatin1().data());

    strcat(szMsg,extn.toLatin1().data());
    strcat(szMsg,";");

    for (int i=0; i < fmeList.count(); i++)
    {
        strcat(szMsg, fmeList[i]->phone.toLatin1().data());
        strcat(szMsg,",");
    }
    if ( szMsg[strlen(szMsg)-1] == ',')
        szMsg[strlen(szMsg)-1]='\0';
//qDebug() << "Save Followme:" << szMsg;
    if ( sendNGetResp(szIPAddress, iPort, szMsg, strlen(szMsg), CMD_SET_FOLLOWME) < 0)
    {

        qDebug() << "Unable to save followme for:" << szMsg;
        return -1;
    }
    return 1;
}

// VM inbox count
int commapi::getVMInboxCount(QString extnNum)
{
    char szMsg[255]="";
   // int iSz;
    int iType;
    char szIPAddress[32];
    QRegExp rx("(\\,)");
    QStringList values;

    strcpy(szMsg, extnNum.toLatin1().data());
    strcpy(szIPAddress, szIPAddr.toLatin1().data());


  //  qDebug() << "Request for VM Count:" << szMsg << "Extn" << extnNum <<" IP:" << szIPAddress;

    if ( (iType = sendNGetResp(szIPAddress, iPort, szMsg, strlen(szMsg), CMD_GET_VM_COUNT)) < 0)
    {
        qDebug() << "Unable to get followme for:" << szMsg;
        return -1;
    }
    else
    {
        return(atoi(szMsg));
    }
}



// call forward settings
int commapi:: readCFInfo(QString extn, CallFWD &cf)
{
    char szMsg[255];
   // int iSz;
    int iType;
    char szIPAddress[32];
    QRegExp rx("(\\,)");
    QStringList values;
    int rc;

    strcpy(szMsg, extn.toLatin1().data());
    strcpy(szIPAddress, szIPAddr.toLatin1().data());


//    qDebug() << "Request for followme for:" << szMsg << "Extn" << extn <<" IP:" << szIPAddress;

    if ( (iType = sendNGetResp(szIPAddress, iPort, szMsg, strlen(szMsg), CMD_GET_CALL_FWD)) < 0)
    {
        qDebug() << "Unable to get followme for:" << szMsg;
        return -1;
    }
    else
    {
        if ( iType == CMD_NOK)
        {
            cf.extn = extn;
            cf.callFwdSet = 0;
            cf.callFwdOnBusy=0;
            cf.fwdExtnBusy="";
            cf.callFwdOnNoAns=0;
            cf.fwdExtnNoAns="";

            rc = -1;
        }
        else
        {
            qDebug() << "Msg Rcvd sz:" << strlen(szMsg) << "msg:" << szMsg << "Type:"<< iType;

            values = QString(szMsg).split(rx);

            cf.extn = extn;
            cf.callFwdSet = values[0].toInt();
            cf.callFwdOnBusy = values[1].toInt();
            cf.fwdExtnBusy = values[2];
            cf.callFwdOnNoAns=values[3].toInt();
            cf.fwdExtnNoAns = values[4];
            rc = 1;
        }
    }

    return rc;
}


int commapi::saveCFInfo(CallFWD &cf)
{
    char szMsg[255];
   // int iSz;
    int iType;
    char szIPAddress[32];
    QRegExp rx("(\\,)");
    QStringList values;


    strcpy(szIPAddress, szIPAddr.toLatin1().data());
    sprintf(szMsg, "%s;%d,%d,%s,%d,%s",
                                cf.extn.toLatin1().data(),
                                cf.callFwdSet,
                                cf.callFwdOnBusy,
                                cf.fwdExtnBusy.toLatin1().data(),
                                cf.callFwdOnNoAns,
                                cf.fwdExtnNoAns.toLatin1().data());


/*
    if ( cf.callFwdSet == 0)
    {
        strcat(szMsg,"0,"); //qry.bindValue(":cfd","no");
    }
    else
    {
        strcat(szMsg,"1,"); //qry.bindValue(":cfd","yes");

    }

    if ( cf.callFwdOnBusy == 0)
    {
        strcat(szMsg,","); //qry.bindValue(":cfdBusy","");
    }
    else if (cf.callFwdOnBusy == 1)
    {
        strcat(szMsg,"VM,"); //qry.bindValue(":cfdBusy","VM");
    }
    else
    {
        strcat(szMsg,cf.fwdExtnBusy.toLatin1().data());
        strcat(szMsg,",");

    }

    if ( cf.callFwdOnNoAns == 0)
    {
        strcat(szMsg,""); //qry.bindValue(":cfdBusy","");
    }
    else if (cf.callFwdOnNoAns == 1)
    {
        strcat(szMsg,"VM"); //qry.bindValue(":cfdBusy","");
//        qry.bindValue(":cfdNoAns","VM");
    }
    else
    {
        strcat(szMsg,cf.fwdExtnBusy.toLatin1().data());
    }

*/
//    qDebug() << "Request for CF 9et for:" << szMsg << "Extn" << cf.extn <<" IP:" << szIPAddress;


    if ( (iType = sendNGetResp(szIPAddress, iPort, szMsg, strlen(szMsg), CMD_SET_CALL_FWD)) < 0)
    {
        qDebug() << "Unable to get followme for:" << szMsg;
        return -1;
    }
    else
    {
        return 1;
    }
}


int commapi::getExtnStatus(QMap <QString, extensionStatus *> &extnStatus, QString extn)
{

    char szMsg[2048]="";
   // int iSz;
    int iType;
    char szIPAddress[32];
    QRegExp rx("(\\,)");
    QStringList values;
    //int rc=-1;
    char *pch;
    extensionStatus *extnSts;
    char szBuf[255];
    int i=0;

    strcpy(szMsg, extn.toLatin1().data());
    strcpy(szIPAddress, szIPAddr.toLatin1().data());



    if ( (iType = sendNGetResp(szIPAddress, iPort, szMsg, strlen(szMsg), CMD_EXTN_STATUS)) < 0)
    {
        qDebug() << "Unable to get extension status";
        return -1;
    }
    else
    {
        extnStatus.empty();
//qDebug()<< "Extn Status:" << szMsg;
        pch = strtok(szMsg, ";");
        while(pch != NULL)
        {
            extnSts = new extensionStatus;

            strcpy(szBuf, pch);
            values = QString(szBuf).split(rx);
  //          qDebug() << "values:" << values;
            extnSts->extn = values[0];
//            extnSts->extnName = values[0];
 //           extnSts->iType = values[1].toInt();
            extnSts->iStatus = values[1].toInt();

            extnStatus.insert(extnSts->extn, extnSts);
            i++;
            pch = strtok(NULL, ";");
        }

    }
    return i;
}



