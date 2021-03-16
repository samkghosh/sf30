#include <QtCore>
#include <QStandardPaths>
#include <QJsonObject>
#include <QtDebug>
#include <QtSql>
#include <QSqlDatabase>


#include "dbaccess.h"


DBAccess::DBAccess()
{
    if ( driverLoaded == 0)
    {
        connectionName = "myconn";

        db = QSqlDatabase::addDatabase("QMYSQL", connectionName);
        driverLoaded = 1;
    }
}

DBAccess::DBAccess(QString conn)
{

    qDebug() << "DBAccess:" << conn;


    if ( driverLoaded == 0)
    {
        connectionName = conn;
        db = QSqlDatabase::addDatabase("QMYSQL", connectionName);
        driverLoaded = 1;
    }

    setDBInfo();

}

DBAccess::~DBAccess()
{

    if (db.isOpen())
        db.close();

//    QSqlDatabase::removeDatabase(connectionName);
}

void DBAccess::setDBInfo()
{
//    dbServer="167.71.230.148";
///    dbUID="commsoft";
// dbPWD = "!Alliance@1986!";
 //   dbDBName = "asterisk";

    DataProcessing dp;
    DBInfo dbi;

    dp.readDBInfo(dbi);
    qDebug() << "Host:" << dbi.dBServer <<"Uid:"<<dbi.dBUser << "pwd:"<<dbi.dBAuth << "name:" << dbi.dBName;
    dbServer = dbi.dBServer;
    dbUID = dbi.dBUser;
    dbPWD = dbi.dBAuth;
    dbDBName = dbi.dBName;

    db.setHostName(dbServer);
    db.setUserName(dbUID);
    db.setPassword(dbPWD);
    db.setDatabaseName(dbDBName);

}

int DBAccess::openDB()
{

    qDebug() << "Conn:" << connectionName << "Drivename:"<<db.driverName();
    setDBInfo();

    db.setHostName(dbServer);
    db.setUserName(dbUID);
    db.setPassword(dbPWD);
    db.setDatabaseName(dbDBName);

 //  db = QSqlDatabase::addDatabase("QMYSQL", connectionName);

    bool bOk = db.open();
    if ( bOk == false)
    {
        QSqlError err = db.lastError();

        if (err.driverText().contains("Driver not loaded"))
        {
            qDebug() << "Found Driver not loaded. Retrying";
            db.addDatabase("QMYSQL");
            bOk = db.open();
            if ( bOk == false)
            {
                qDebug() << "Unable to open DB. Error:" << db.lastError() << "Server:" << dbServer;
            }
        }
        else
        {
            qDebug() << "Unable to open DB. Error:" << db.lastError() << "Server:" << dbServer;
        }
    }

//    connectionName = db.connectionName();

    qDebug() << "ConnectionName" << connectionName;
    return bOk;
}


void DBAccess::closeDB()
{

   if (db.open())
   {
        db.close();
    }
    db.removeDatabase(connectionName);
}

QString getLastExecutedQuery(const QSqlQuery& query)
{
 QString str = query.lastQuery();
 QMapIterator<QString, QVariant> it(query.boundValues());
 while (it.hasNext())
 {
  it.next();
  str.replace(it.key(),it.value().toString());
 }
 return str;
}



int DBAccess::readExtensionData(QMap <QString, Extension *> &extnList)
{
    // if db is not opend open it
    Extension *extn;
    QString selqry;
#if 1
    if (db.isOpen() == false)
    {
        //qDebug() << "Db Is not Opened";

        if (openDB() == false)
        {
            qDebug() << "Unable to open: " << db.lastError();
            return -1;
        }
        else
        {
            qDebug() << "Db Opened successfully";

        }

    }
    else
    {
        qDebug() << "Db Already Opened.";

    }



   selqry = "SELECT ep.id, ep.transport, ep.aors, ep.auth, ep.context, ep.disallow, ep.allow,  ep.direct_media, "
           " ep.media_use_received_transport, ep.trust_id_inbound, ep.media_encryption, ep.rtp_symmetric, "
           " ep.rewrite_contact, ep.force_rport, ep.e_callForward, ep.v_forwardBusy, ep.v_forwardNoAns, "
           " ep.i_adminMode, ep.i_recMode,ep.v_name, aors.max_contacts,aors.remove_existing, auth.auth_type, "
           " auth.password, auth.username FROM ps_endpoints ep, ps_aors aors, ps_auths auth WHERE ep.id = aors.id and ep.id=auth.id";

    QSqlQuery qry(db);

    qry.prepare(selqry);


    if (!qry.exec())
    {
        qDebug() << "Error in select:" <<db.lastError();
        return -1;
    }

    int i=0;
    extnList.empty();

    QSqlRecord rec = qry.record();
    int iRecIndex;
    while(qry.next())
    {
        extn = new Extension;

        iRecIndex = rec.indexOf("id");
        extn->endPoint.extn = QVariant(qry.value(iRecIndex)).toString();

//        qDebug() << "Fetch Extension:" << extn->endPoint.extn;

        iRecIndex = rec.indexOf("transport");
        extn->endPoint.transortType = QVariant(qry.value(iRecIndex)).toString();

        iRecIndex = rec.indexOf("aors");
        extn->endPoint.aors = QVariant(qry.value(iRecIndex)).toString();

        iRecIndex = rec.indexOf("auth");
        extn->endPoint.auth = QVariant(qry.value(iRecIndex)).toString();

        iRecIndex = rec.indexOf("context");
        extn->endPoint.context = QVariant(qry.value(iRecIndex)).toString();

        iRecIndex = rec.indexOf("disallow");
        extn->endPoint.disallowCodecs = QVariant(qry.value(iRecIndex)).toString();

        iRecIndex = rec.indexOf("allow");
        extn->endPoint.allowCodecs = QVariant(qry.value(iRecIndex)).toString();

        iRecIndex = rec.indexOf("direct_media");
        extn->endPoint.directMedia = QVariant(qry.value(iRecIndex)).toString();

        iRecIndex = rec.indexOf("media_use_received_transport");
        extn->endPoint.mediaUseRcvdTransport = QVariant(qry.value(iRecIndex)).toString();

        iRecIndex = rec.indexOf("trust_id_inbound");
        extn->endPoint.trustIdInbound = QVariant(qry.value(iRecIndex)).toString();

        iRecIndex = rec.indexOf("media_encryption");
        extn->endPoint.mediaEncryption = QVariant(qry.value(iRecIndex)).toString();

        iRecIndex = rec.indexOf("rtp_symmetric");
        extn->endPoint.rtpSymetric = QVariant(qry.value(iRecIndex)).toString();

        iRecIndex = rec.indexOf("rewrite_contact");
        extn->endPoint.rewiteContact = QVariant(qry.value(iRecIndex)).toString();

        iRecIndex = rec.indexOf("force_rport");
        extn->endPoint.forceRport = QVariant(qry.value(iRecIndex)).toString();

        iRecIndex = rec.indexOf("e_callForward");
        extn->endPoint.callForward = QVariant(qry.value(iRecIndex)).toString();

        iRecIndex = rec.indexOf("v_forwardBusy");
        extn->endPoint.forwardBusy = QVariant(qry.value(iRecIndex)).toString();

        iRecIndex = rec.indexOf("v_forwardNoAns");
        extn->endPoint.forwardNoAns = QVariant(qry.value(iRecIndex)).toString();

        iRecIndex = rec.indexOf("i_adminMode");
        extn->endPoint.adminMode = QVariant(qry.value(iRecIndex)).toInt();

        iRecIndex = rec.indexOf("i_recMode");
        extn->endPoint.recMode = QVariant(qry.value(iRecIndex)).toInt();

        iRecIndex = rec.indexOf("v_name");
        extn->endPoint.name = QVariant(qry.value(iRecIndex)).toString();

      //  qDebug() << "Exten Name" << extn->endPoint.name;

        iRecIndex = rec.indexOf("max_contacts");
        extn->aors.maxContact = QVariant(qry.value(iRecIndex)).toInt();

        iRecIndex = rec.indexOf("remove_existing");
        extn->aors.removeExisting = QVariant(qry.value(iRecIndex)).toString();

        iRecIndex = rec.indexOf("remove_existing");
        extn->aors.removeExisting = QVariant(qry.value(iRecIndex)).toString();

        iRecIndex = rec.indexOf("auth_type");
        extn->auths.authType = QVariant(qry.value(iRecIndex)).toString();

        iRecIndex = rec.indexOf("password");
        extn->auths.password = QVariant(qry.value(iRecIndex)).toString();

        iRecIndex = rec.indexOf("username");
        extn->auths.userName = QVariant(qry.value(iRecIndex)).toString();

        extnList.insert(extn->endPoint.extn, extn);
        i++;
    }

   qry.finish();

    closeDB();
    return i;
#else
    return 0;
#endif
}


//CDR
int DBAccess::readCDRData(QList<CDR *> &cdrs, int type)
{
//   setDBInfo();
   CDR *cdr;
   QString selqry;

   if (db.isOpen() == false)
   {
       qDebug() << "Db Is not Opened";

       if (openDB() == false)
       {
           qDebug() << "Unable to open: " << db.lastError();
           return -1;
       }
       else
       {
           qDebug() << "Db Opened successfully";

       }

   }
   else
   {
       qDebug() << "Db Already Opened.";

   }


   if ( type == 0)
   {
       selqry = "SELECT calldate, src, dst, duration, disposition, dcontext from cdr order by calldate desc";
   }
   else
   {
       selqry = "SELECT calldate, src, dst, duration, disposition, dcontext,v_voicefileName FROM cdr WHERE "
              " v_voicefileName IS NOT NULL AND v_voicefileName != '' ORDER BY calldate DESC";
   }

   QSqlQuery *qry = new QSqlQuery(db);

   qry->prepare(selqry);


   if (!qry->exec())
   {
       qDebug() << "Error in select:" <<db.lastError();
       return -1;
   }


//   QSqlRecord rec = qry.record();

//   qDebug() << "Query sucess:" << selqry << "Rec Count:" <<rec.count() ;


   int i=0;
   cdrs.empty();


   while(qry->next())
   {
       cdr = new CDR;

//       QString selqry = "SELECT calldate, src, dst, duration, disposition from cdr";
       cdr->dateTime = QVariant(qry->value(0)).toDateTime();
       cdr->from = QVariant(qry->value(1)).toString();
       cdr->to = QVariant(qry->value(2)).toString();
       cdr->duration = QVariant(qry->value(3)).toInt();
       cdr->disposition = QVariant(qry->value(4)).toString();
       cdr->context = QVariant(qry->value(5)).toString();
       if (type == 0)
       {
           cdr->recFileName = "";
       }
       else
       {
           cdr->recFileName = QVariant(qry->value(6)).toString();
       }
//qDebug() << "from:" << cdr->from;
       cdrs.insert(i, cdr);
       i++;
   }

   qry->finish();
   delete qry;

   closeDB();

   return i;
}


// voice mail inbox count
int DBAccess::getVMInboxCount(QString extnNum)
{
    //   setDBInfo();
       QString selqry;

       if (db.isOpen() == false)
       {
           qDebug() << "Db Is not Opened";

           if (openDB() == false)
           {
               qDebug() << "Unable to open: " << db.lastError();
               return -1;
           }
           else
           {
               qDebug() << "Db Opened successfully";

           }

       }
       else
       {
           qDebug() << "Db Already Opened.";

       }

       QString vmInbox;

       selqry = "SELECT COUNT(*) FROM voicemessages WHERE mailboxuser = :extn AND dir like :inboxname";

       QSqlQuery qry = QSqlQuery(db);

       qry.prepare(selqry);

//       vmInbox = "/var/spool/asterisk/voicemail/default/"+ extnNum + "/INBOX";
       vmInbox = "%/INBOX";
         qry.bindValue(":inboxname", vmInbox);
         qry.bindValue(":extn",extnNum);

//       qDebug()<<"VM Count query" << getLastExecutedQuery(qry);

       if (!qry.exec())
       {
           qDebug() << "Error in select:" <<db.lastError();
           closeDB();
           return -1;
       }

       int iCount=0;

       if ( qry.next())
       {
           iCount = QVariant(qry.value(0)).toInt();
       }

       qry.finish();

       closeDB();

       return iCount;

}


//Call fwd
int DBAccess::readCFInfo(QString extn, CallFWD &cf)
{
    // if db is not opend open it
    QString selqry;
    QString fwdExtn;
    int rc = 0;



    if (db.isOpen() == false)
    {
        //qDebug() << "Db Is not Opened";

        if (openDB() == false)
        {
            qDebug() << "Unable to open: " << db.lastError();
            return -1;
        }
        else
        {
            qDebug() << "Db Opened successfully";

        }

    }
    else
    {
        qDebug() << "Db Already Opened.";

    }



   selqry = "SELECT e_callForward, v_forwardBusy, v_forwardNoAns FROM ps_endpoints WHERE id = :id";

    QSqlQuery qry(db);

    qry.prepare(selqry);
    qry.bindValue(":id", extn);

    if (!qry.exec())
    {
        qDebug() << "Error in select:" <<db.lastError();
        return -1;
    }
qDebug() << "CallFwd:" << getLastExecutedQuery(qry);

    if (qry.next())
    {
        cf.extn = extn;
qDebug() << "Got rec" << "Set" << QVariant(qry.value(0)).toString() << "busy" << QVariant(qry.value(1)).toString() << "NAns" << QVariant(qry.value(2)).toString();


        if ( QVariant(qry.value(0)).toString() == "yes")
        {
             cf.callFwdSet = 1;
        }
        else
        {
            cf.callFwdSet = 0;
        }

        fwdExtn = QVariant(qry.value(1)).toString();
        if ( fwdExtn == "VM")
        {
            cf.fwdExtnBusy="";
            cf.callFwdOnBusy = 1;

        }
        else
        {
            if (fwdExtn == nullptr || fwdExtn == "")
            {

                cf.callFwdOnBusy = 0;
                cf.fwdExtnBusy = "";
            }
            else
            {
                cf.callFwdOnBusy = 2;
                cf.fwdExtnBusy = fwdExtn;

            }
        }

        fwdExtn = QVariant(qry.value(2)).toString();
        if ( fwdExtn == "VM")
        {
            cf.fwdExtnNoAns="";
            cf.callFwdOnNoAns = 1;
        }
        else
        {
            if (fwdExtn == nullptr || fwdExtn == "")
            {

                cf.callFwdOnNoAns = 0;
                cf.fwdExtnNoAns = "";
            }
            else
            {
                cf.callFwdOnNoAns = 2;
                cf.fwdExtnNoAns = fwdExtn;

            }
        }

        rc = 1;
    }
    else
    {
        cf.extn = extn;
        cf.fwdExtnBusy="";
        cf.callFwdSet = 0;
        cf.fwdExtnNoAns="";
        cf.callFwdOnBusy=0;
        cf.callFwdOnBusy=0;

        rc = 0;
    }

   qry.finish();

    closeDB();

    qDebug() << "CallFwd" << cf.callFwdSet << "Busy:" << cf.fwdExtnBusy << "No Ans:" << cf.fwdExtnNoAns;

    return rc;
}


int DBAccess::saveCFInfo(CallFWD &cf)
{
    QString strQry;

//qDebug() << "in Save Extn:" << cf.extn << "Fwd Flag:" << cf.callFwdSet << "Fwd Busy:" << cf.callFwdOnBusy << "No Ans:" << cf.callFwdOnNoAns;
    // open database
    if (db.isOpen() == false)
    {
        qDebug() << "Db Is not Opened";

        if (openDB() == false)
        {
            qDebug() << "Unable to open: " << db.lastError();
            return -1;
        }
        else
        {
            qDebug() << "Db Opened successfully";

        }

    }
    else
    {
        qDebug() << "Db Already Opened.";

    }

    QSqlQuery qry(db);

    //Insert Data into ps_endpoints
    strQry ="UPDATE ps_endpoints SET e_callForward = :cfd, v_forwardBusy  = :cfdBusy, v_forwardNoAns = :cfdNoAns WHERE id = :id";

    qry.prepare(strQry);

    qry.bindValue(":id", cf.extn);

    if ( cf.callFwdSet == 0)
    {
        qry.bindValue(":cfd","no");
    }
    else
    {
        qry.bindValue(":cfd","yes");

    }

    if ( cf.callFwdOnBusy == 0)
    {
        qry.bindValue(":cfdBusy","");
    }
    else if (cf.callFwdOnBusy == 1)
    {
        qry.bindValue(":cfdBusy","VM");
    }
    else
    {
        qry.bindValue(":cfdBusy",cf.fwdExtnBusy);

    }

    if ( cf.callFwdOnNoAns == 0)
    {
        qry.bindValue(":cfdNoAns","");
    }
    else if (cf.callFwdOnNoAns == 1)
    {
        qry.bindValue(":cfdNoAns","VM");
    }
    else
    {
        qry.bindValue(":cfdNoAns",cf.fwdExtnNoAns);

    }

   // qDebug() << "Last Qry:" <<getLastExecutedQuery(qry);

    if (!qry.exec())
    {
        qDebug() << "Error in insert:" << strQry <<"Error:"<<db.lastError();
        return -1;
    }


    return 1;

}


//find me followme
int DBAccess::readFollowme(QList<FollowMe *> &fmeList, QString extn)
{
    // if db is not opend open it
    QString selqry;
    QString fwdExtn;
    FollowMe *fme;

    qDebug() << "Followme:" << "Entry";

    if (db.isOpen() == false)
    {
        //qDebug() << "Db Is not Opened";

        if (openDB() == false)
        {
            qDebug() << "Followme read Unable to open: " << db.lastError();
            return -1;
        }
        else
        {
            qDebug() << "Db Opened successfully";

        }

    }
    else
    {
        qDebug() << "Db Already Opened.";

    }


    qDebug() << "Followme:" << "Db open checked";

   //follow me is implemented using queue.
   // There will be intry in queues against FM_<extnNo> and extensions in queue_members

   selqry = "SELECT COUNT(*) from queues WHERE name = :name";

    QSqlQuery qry(db);

    qry.prepare(selqry);
    qry.bindValue(":name", "FM_"+extn);


    if (!qry.exec())
    {
        qDebug() << "Follow me Read Error in select:" <<db.lastError();
        qDebug() << "Followme:" << getLastExecutedQuery(qry);
        return -1;
    }

    //qDebug() << "Followme:" << getLastExecutedQuery(qry);

    int irowCount =0;
    if (qry.next())
    {
        irowCount = QVariant(qry.value(0)).toInt();
    }
    qry.finish();


    if (irowCount == 0)
    {
        // if no entry found, then insert an entry
        qDebug() << "Followme: No entry found";
/*        selqry = "INSERT INTO followme (name, musicclass, context, takecall, declinecall ) VALUES (:name, 'default', 'outgoingcall', '1','2' )";
*/
       selqry = "INSERT INTO queues (name, timeout, ringinuse, retry, strategy) VALUES (:name, '30', 'no', '0','linear' )";

       qry.prepare(selqry);
       qry.bindValue(":name", "FM_"+extn);

       qDebug() << "Followme:" << getLastExecutedQuery(qry);

       if (!qry.exec())
       {
           qDebug() << "Error in Insert :" <<db.lastError();
           return -1;
       }

    }
     else
    {
        qDebug() << "Followme:entry found";

    }
    qry.finish();

    selqry = "SELECT membername FROM queue_members WHERE queue_name = :name order by uniqueid asc";
    qry.prepare(selqry);
    qry.bindValue(":name", "FM_"+extn);

    qDebug() << "Followme:" << getLastExecutedQuery(qry);

    if ( !qry.exec())
    {
        qDebug() << "Error in select:" <<db.lastError();
        return -1;
    }

    fmeList.empty();
    fmeList.clear();

    int i=0;
    while(qry.next())
    {
        fme = new FollowMe;
        fme->name = extn;
        fme->order = i+1;
        fme->phone = QVariant(qry.value(0)).toString();
        fme->timeOut = 0;

        fmeList.insert(i,fme);
        i++;
    }

    qry.finish();

    closeDB();

    qDebug() << "Followme for " << extn << "count is " << i;
    return i;
}




int DBAccess::saveFollowme(QList<FollowMe *> &fmeList, QString extn)
{
    // if db is not opend open it
    QString selqry;
    QString fwdExtn;
    FollowMe *fme;

    if (db.isOpen() == false)
    {
        //qDebug() << "Db Is not Opened";

        if (openDB() == false)
        {
            qDebug() << "Unable to open: " << db.lastError();
            return -1;
        }
        else
        {
            qDebug() << "Db Opened successfully";

        }

    }
    else
    {
        qDebug() << "Db Already Opened.";

    }

    QSqlQuery qry(db);

    selqry = "SELECT COUNT(*) from queues WHERE name = :qname";


     qry.prepare(selqry);
     qry.bindValue(":qname", "FM_"+extn );

//    qDebug() << "Followme:check entry:" << getLastExecutedQuery(qry);
    if (!qry.exec())
    {
        qDebug() << "Error in select:" << db.lastError();
        closeDB();

        return -1;
    }
    else
    {
        qDebug() << "No error";
    }
//    closeDB();

  //  return -1;

    //qDebug() << "CallFwd:" << getLastExecutedQuery(qry);

    int irowCount =0;
    if (qry.next())
    {
        irowCount = QVariant(qry.value(0)).toInt();
    }
    qry.finish();

//    qDebug() << "Count in Save:" << irowCount;
    if (irowCount == 0)
    {
        selqry = "INSERT INTO queues (name, timeout, ringinuse, retry, strategy) VALUES (:name, '30', 'no', '0','linear' )";

        qry.prepare(selqry);
        qry.bindValue(":name", "FM_" + extn);

       if (!qry.exec())
       {
           qDebug() << "Error in select:" <<db.lastError() << "QRY:" << getLastExecutedQuery(qry);
           closeDB();
           return -1;
       }

    }
    qry.finish();

    // remove all entries
    selqry = "DELETE FROM queue_members WHERE queue_name =:name";
    qry.prepare(selqry);
    qry.bindValue(":name","FM_"+extn);

    if (!qry.exec())
    {
        qDebug() << "Error in select:" <<db.lastError() << "QRY:" << getLastExecutedQuery(qry);
        closeDB();
        return -1;
    }

    qry.finish();

    QList<FollowMe *>::ConstIterator iter;

    int i=1;
    for(iter = fmeList.constBegin(); iter != fmeList.constEnd(); iter++)
    {
        fme = *iter;

        selqry = "INSERT INTO queue_members (queue_name, interface, membername) VALUES (:qname, :interface, :member)";

        qry.prepare(selqry);
        qry.bindValue(":qname", "FM_"+fme->name);
        qry.bindValue(":interface", "PJSIP/"+fme->phone);
        qry.bindValue(":member", fme->phone);
        if (!qry.exec())
        {
            qDebug() << "Error in select:" <<db.lastError() << "QRY:" << getLastExecutedQuery(qry);

            closeDB();
            return -1;
        }
        qry.finish();
        i++;
    }

    qry.finish();

    closeDB();

    return i;
}


// voice mail inbox count
int DBAccess::getExtnStatus(QMap <QString, extensionStatus *>&extnStatusL,QString extn)
{
    // if db is not opend open it
    extensionStatus *extnSts;
    QString selqry;
    int i =0;


    if (db.isOpen() == false)
    {
        //qDebug() << "Db Is not Opened";

        if (openDB() == false)
        {
            qDebug() << "Unable to open: " << db.lastError();
            return -1;
        }
        else
        {
            qDebug() << "Db Opened successfully";

        }

    }
    else
    {
        qDebug() << "Db Already Opened.";

    }


    QSqlQuery qry(db);

    if ( extn == "")
    {
        selqry = "SELECT id, i_status,i_type, v_name FROM ps_endpoints";
        qry.prepare(selqry);
    }
    else
    {
        selqry = "SELECT id, i_status, i_type, v_name FROM ps_endpoints WHERE id = :extension";
        qry.prepare(selqry);
        qry.bindValue(":extension",extn);
//        qDebug() << "Extn Status:" << getLastExecutedQuery(qry);
    }



    if (!qry.exec())
    {
        qDebug() << "Error in select:" << db.lastError();
        return -1;
    }

    QSqlRecord rec = qry.record();

    int iRecIndex;

    while(qry.next())
    {
        extnSts = new extensionStatus;

        iRecIndex = rec.indexOf("id");
        extnSts->extn= QVariant(qry.value(iRecIndex)).toString();

        iRecIndex = rec.indexOf("i_status");
        extnSts->iStatus = QVariant(qry.value(iRecIndex)).toInt();

        iRecIndex = rec.indexOf("i_type");
        extnSts->iType = QVariant(qry.value(iRecIndex)).toInt();

        iRecIndex = rec.indexOf("v_name");
        extnSts->extnName = QVariant(qry.value(iRecIndex)).toString();

        extnStatusL.insert(extnSts->extn, extnSts);
       // qDebug() << extn << "Name:" << extnSts->extnName;
        i++;
    }

   qry.finish();

    closeDB();
    return i;
 }
