#include <QtCore>
#include <QMessageBox>

#include "dlgsetting.h"
#include "ui_dlgsetting.h"

#include "dataprocessing.h"


DlgSetting::DlgSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgSetting)
{

    ui->setupUi(this);


    dp.readPhoneData(pd);
    ui->lePwd->setText(pd.auth);
    ui->leURI->setText(pd.uri);
    ui->leExtn->setText(pd.extn);
    ui->leName->setText(pd.name);
    ui->leProxy->setText(pd.proxy);
    ui->leScheme->setText(pd.scheme);
    ui->leServer->setText(pd.serverName);

}

DlgSetting::~DlgSetting()
{
    delete ui;
}


void DlgSetting::on_pbCreate_clicked()
{
    pd.extn = ui->leExtn->text();
    pd.auth = ui->lePwd->text();
    pd.name = ui->leName->text();
    pd.uri = ui->leURI->text();
    pd.proxy = ui->leProxy->text();
    pd.serverName = ui->leServer->text();
    pd.scheme = ui->leScheme->text();


    if ( dlgLog)
        dlgLog->log("Extn: "+ pd.extn);
    if ( dp.savePhoneData(pd) > 0 )
    {
        QMessageBox::information(this,"Setting", "Phone login information saved successfully");
        this->hide();
    }


}


void DlgSetting::setLogWindow(DlgLog *dlg)
{
    dlgLog = dlg;
}
