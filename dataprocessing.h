#ifndef DATAPROCESSING_H
#define DATAPROCESSING_H

#include <QtCore>
#define MAX_SPEEDDIAL	9
class PhoneData
{
 public:
    QString extn;
    QString name;
    QString auth;
    QString serverName;
    QString uri;
    QString scheme;
    QString proxy;
};


class SpeedDialData
{
public:
    QString number[MAX_SPEEDDIAL];
    QString name[MAX_SPEEDDIAL];
};



class PhoneBookData
{
  public:
    QMultiMap <QString,QString> pbEntry;
};

class DBInfo
{
public:
    QString dBServer;
    QString dBUser;
    QString dBAuth;
    QString dBName;
};


class DataProcessing
{
public:
    DataProcessing();

    PhoneData phData;
    QString filePath;


    // Phone Login Information
    QString phoneDataFileName;
    int savePhoneData(PhoneData &pd);
    int readPhoneData(PhoneData &pd);

    //speed DIal
    QString speedDialFileName;
    int saveSpeedDialData(SpeedDialData &sd);
    int readSpeedDialData(SpeedDialData &sd);

    //PhoneBook
//    PhoneBookData pbd;
    QString phoneBookFileName;
    int getPhoneBookData(PhoneBookData &pbd);
    int savePhoneBookData(const PhoneBookData pbd);

    //db access
    QString dbConfigName;
    int readDBInfo(DBInfo &dinfo);

};


#endif // DATAPROCESSING_H
