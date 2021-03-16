#include "speeddialdialog.h"
#include "ui_speeddialdialog.h"

#include <QMessageBox>

DlgSpeedDial::DlgSpeedDial(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgSpeedDial)
{

    ui->setupUi(this);


}



DlgSpeedDial::~DlgSpeedDial()
{
    delete ui;
}



void DlgSpeedDial::on_pbOk_clicked()
{

   SpeedDialData sdtmp;

   sdtmp.name[0] = ui->comboF1->currentText();
   sdtmp.number[0] = extnNameToNum.value(sdtmp.name[0]);

   sdtmp.name[1] = ui->comboF2->currentText();
   sdtmp.number[1] = extnNameToNum.value(sdtmp.name[1]);

   sdtmp.name[2] = ui->comboF3->currentText();
   sdtmp.number[2] = extnNameToNum.value(sdtmp.name[2]);

   sdtmp.name[3] = ui->comboF4->currentText();
   sdtmp.number[3] = extnNameToNum.value(sdtmp.name[3]);

   sdtmp.name[4] = ui->comboF5->currentText();
   sdtmp.number[4] = extnNameToNum.value(sdtmp.name[4]);

   sdtmp.name[5] = ui->comboF6->currentText();
   sdtmp.number[5] = extnNameToNum.value(sdtmp.name[5]);

   sdtmp.name[6] = ui->comboF7->currentText();
   sdtmp.number[6] = extnNameToNum.value(sdtmp.name[6]);

   sdtmp.name[7] = ui->comboF8->currentText();
   sdtmp.number[7] = extnNameToNum.value(sdtmp.name[7]);

   sdtmp.name[8] = ui->comboF9->currentText();
   sdtmp.number[8] = extnNameToNum.value(sdtmp.name[8]);

   if ( dp.saveSpeedDialData(sdtmp) > 0 )
   {
       QMessageBox::information(this, "Speed Dial", "Data saved successfully.");
       sd = sdtmp;
   }
   else
   {
       QMessageBox::warning(this, "Speed Dial", "Unable to save data.");

   }
}

void DlgSpeedDial::on_pbCancel_clicked()
{
	close();
}

void DlgSpeedDial::getValues(SpeedDialData &sdata)
{

    sdata = sd;
    return;
}


void DlgSpeedDial::setPos(const int x, const int y)
{
    this->setGeometry(x, y, this->width(), this->height());
}


void DlgSpeedDial::updateAndShow(QString extnName)
{
    QList<QString> listExtn;
    QMap<QString,Extension *>::iterator itr;

    DBAccess *dba = new DBAccess("extn");

    if (dba->readExtensionData(extensions) <= 0)
    {
        delete dba;
        QMessageBox::warning(this,"Follow me setting", "Unable to get extension information for display.");

        return;
    }

    delete dba;

    myExtnName = extnName;

    listExtn.clear();
    listExtn.empty();
    listExtn.append("");
    extnNameToNum.clear();
    extnNameToNum.empty();

    for (itr= extensions.begin(); itr != extensions.end(); itr++)
    {
//        qDebug() << "Speed:" << myExtnName << ":" << itr.value()->endPoint.name;
         if ( itr.value()->endPoint.name == myExtnName)
         {
             myExtnNum = itr.key();
             continue;
         }
         listExtn.append(itr.value()->endPoint.name);
         extnNameToNum.insert(itr.value()->endPoint.name, itr.value()->endPoint.extn);
    }

    if (dp.readSpeedDialData(sd) >= 0)
    {
        ui->comboF1->clear();
        ui->comboF1->addItems(listExtn);
        ui->comboF1->setCurrentText(sd.name[0]);
        ui->comboF1->setToolTip(sd.number[0]);

        ui->comboF2->clear();
        ui->comboF2->addItems(listExtn);
        ui->comboF2->setCurrentText(sd.name[1]);
        ui->comboF2->setToolTip(sd.number[1]);

        ui->comboF3->clear();
        ui->comboF3->addItems(listExtn);
        ui->comboF3->setCurrentText(sd.name[2]);
        ui->comboF3->setToolTip(sd.number[2]);

        ui->comboF4->clear();
        ui->comboF4->addItems(listExtn);
        ui->comboF4->setCurrentText(sd.name[3]);
        ui->comboF4->setToolTip(sd.number[3]);

        ui->comboF5->clear();
        ui->comboF5->addItems(listExtn);
        ui->comboF5->setCurrentText(sd.name[4]);
        ui->comboF5->setToolTip(sd.number[4]);

        ui->comboF6->clear();
        ui->comboF6->addItems(listExtn);
        ui->comboF6->setCurrentText(sd.name[5]);
        ui->comboF6->setToolTip(sd.number[5]);

        ui->comboF7->clear();
        ui->comboF7->addItems(listExtn);
        ui->comboF7->setCurrentText(sd.name[6]);
        ui->comboF7->setToolTip(sd.number[6]);

        ui->comboF8->clear();
        ui->comboF8->addItems(listExtn);
        ui->comboF8->setCurrentText(sd.name[7]);
        ui->comboF8->setToolTip(sd.number[7]);

        ui->comboF9->clear();
        ui->comboF9->addItems(listExtn);
        ui->comboF9->setCurrentText(sd.name[8]);
        ui->comboF9->setToolTip(sd.number[8]);
     }
     this->show();
}
