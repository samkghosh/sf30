#include <QtCore>
#include <QStandardPaths>
#include <QJsonObject>
#include <QtDebug>


#include "dataprocessing.h"

DataProcessing::DataProcessing()
{
    // Get the Home Directory
    filePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);

    filePath = filePath + "/" + ".ph";

    QDir dir(filePath);

    if ( dir.exists() == false)
    {
        dir.mkpath(filePath);
        filePath = dir.absolutePath();
    }
    else
    {
        filePath = dir.absolutePath();
    }

    //set the Phone data file name
    phoneDataFileName = filePath + "/"+ "phonesetting.cfg";

    // set file name for seed dial info

    speedDialFileName = filePath + "/" + "speedDial.cfg";

    phoneBookFileName = filePath + "/"+"phonebook.dat";

    dbConfigName = "/etc/softphone/dbinfo.cfg";
//    qDebug() << phoneDataFileName;
}


//Write Phonedata
int DataProcessing::savePhoneData(PhoneData &pd)
{
    QFile phFile(phoneDataFileName);
    QJsonObject json;
    int iRetVal = 0;

    json["extn"] = pd.extn;
    json["name"] = pd.name;
    json["auth"] = pd.auth;
    json["serverName"] = pd.serverName;
    json["uri"] = pd.uri;
    json["scheme"] = pd.scheme;
    json["proxy"] = pd.proxy;


    qDebug() << "Save Json, Extn:" + pd.extn;

    if (phFile.open(QIODevice::WriteOnly) == true)
    {
        iRetVal = phFile.write(QJsonDocument(json).toJson());
        phFile.close();
    }
    else
    {
        qDebug() << "unable to open the file: "+ phoneDataFileName;
    }

    return iRetVal;
}

int DataProcessing::readPhoneData(PhoneData &pd)
{
    QFile phFile(phoneDataFileName);

    QJsonObject json;
    QByteArray readData;
    int iRetVal =0;


    qDebug() << "Read Json, Extn:" + pd.extn;

    if (phFile.open(QIODevice::ReadOnly) == true)
    {
        readData = phFile.readAll();

        iRetVal = readData.length();

        json = QJsonDocument::fromJson(readData).object();

        pd.extn =  json["extn"].toString();
        pd.name = json["name"].toString();
        pd.auth = json["auth"].toString();
        pd.serverName = json["serverName"].toString();
        pd.uri = json["uri"].toString();
        pd.scheme = json["scheme"].toString();
        pd.proxy = json["proxy"].toString();
        phFile.close();
    }
    else
    {
        qDebug() << "unable to open the file: "+ phoneDataFileName;
    }

    return iRetVal;
}


//////////// Speed Dial Info

int DataProcessing::saveSpeedDialData(SpeedDialData &sd){
    QFile sdFile(speedDialFileName);
    QJsonObject json;
    int iRetVal = 0;

    for(int i =0;i< 9;i++){
        json["Name " + QString::number(i+1)] = sd.name[i];
        json["Number " + QString::number(i+1)] = sd.number[i];
    }
    if (sdFile.open(QIODevice::WriteOnly) == true)
    {
        iRetVal = sdFile.write(QJsonDocument(json).toJson());
        sdFile.close();
    }
    else
    {
        qDebug() << "unable to open the file: "+ speedDialFileName;
    }

    return iRetVal;
}

int DataProcessing::readSpeedDialData(SpeedDialData &sd)
{
    QFile sdFile(speedDialFileName);

    QJsonObject json;
    QByteArray readData;
    int iRetVal =0;



    if (sdFile.open(QIODevice::ReadOnly) == true)
    {
        readData = sdFile.readAll();

        iRetVal = readData.length();

        json = QJsonDocument::fromJson(readData).object();

        for(int i =0;i< 9;i++){
            sd.number[i] = json["Number " + QString::number(i+1)].toString();
            sd.name[i] = json["Name " + QString::number(i+1)].toString();
        }
    }
 //   else
 //   {
 //       qDebug() << "unable to open the file: "+ speedDialFileName;
 //   }

    return iRetVal;
}


//////////////////////////////////////
int DataProcessing::getPhoneBookData(PhoneBookData &pbd)
{
    QFile phbFile(phoneBookFileName);

    QJsonObject json;
    QByteArray readData;
    int iRetVal =0;
    QMultiMap<QString, QString>::iterator itr;
    QString key;
    QStringList keys;
    QString val;
    QStringList lst;

//qDebug() << "Start of phonebook";
    for(itr = pbd.pbEntry.begin();itr != pbd.pbEntry.end(); itr++ )
    {
        key = itr.key();
        pbd.pbEntry.remove(key);
    }
//qDebug() << "After Cleaning phonebook data";

    if (phbFile.open(QIODevice::ReadOnly) == true)
    {
        readData = phbFile.readAll();

        iRetVal = readData.length();

        json = QJsonDocument::fromJson(readData).object();

        keys = json.keys();

        int iCount = json.count();

//    qDebug() << "Phonebook read Count "<< iCount;

        for(int i =0; i< iCount;i++)
        {
            key = keys[i];
    //        qDebug() << "Phonebook After Key:" << key;


            val = json.value(key).toString();

  //          qDebug() << "Phonebook After Val:" << val;


            // now split the String
            lst = val.split(",");

//            qDebug() << "Phonebook After lst";

            for (int j = lst.count()-1; j >= 0; j--)
            {
                qDebug() << "Phonebook After lst" <<lst[j];

                pbd.pbEntry.insert(key,lst[j]);
            }

        }
        iRetVal = pbd.pbEntry.count();
    }
    else
    {
        qDebug() << "unable to open the file: "+ speedDialFileName;
    }

    return iRetVal;
}


// save Phonebook data
int DataProcessing::savePhoneBookData(const PhoneBookData pbd1)
{

    PhoneBookData pbd = pbd1;

    QFile phbFile(phoneBookFileName);

    QJsonObject json;
    int iRetVal = 0;
    QString key;
    QString val;
    QMultiMap<QString, QString>::iterator itr;

    for(itr = pbd.pbEntry.begin();itr != pbd.pbEntry.end(); itr++ )
    {
        key = itr.key();

        QList<QString> nums = pbd.pbEntry.values(itr.key());

        val="";
        for(int i =0; i <nums.size(); i++)
        {
            if ( i == 0)
               val = nums.at(i);
            else
                val = val+","+nums.at(i);

        }
        json.insert(key,val);


    }


    if (phbFile.open(QIODevice::WriteOnly) == true)
    {
            iRetVal = phbFile.write(QJsonDocument(json).toJson());
            phbFile.close();
    }
    else
    {
            qDebug() << "unable to open the file: "+ phoneBookFileName;
    }

     return iRetVal;

}


int DataProcessing::readDBInfo(DBInfo &dinfo)
{
    QSettings dbSetting(dbConfigName, QSettings::NativeFormat);
   // QSettings dbSetting("/etc/softphone/dbinfo.cfg", QSettings::NativeFormat);


//    qDebug() << "DbConfig:" << dbConfigName;
    dinfo.dBServer = dbSetting.value("DBServer").toString();

    dinfo.dBUser   = dbSetting.value("DBUser").toString();
    dinfo.dBAuth   = dbSetting.value("DBPwd").toString();
    dinfo.dBName   = dbSetting.value("DBName").toString();

qDebug() << "Database Server:" << dinfo.dBServer;
qDebug() << "Database Server:" << dinfo.dBUser;
qDebug() << "Database Server:" << dinfo.dBAuth;
qDebug() << "Database Server:" << dinfo.dBName;

    return 1;
}
