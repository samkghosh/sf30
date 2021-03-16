#include "dlgcallfwd.h"
#include "ui_dlgcallfwd.h"
#include <QMessageBox>

#include "commapi.h"

DlgCallFwd::DlgCallFwd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgCallFwd)
{
    ui->setupUi(this);
    this->setWindowTitle("Call forward Settings");
}

DlgCallFwd::~DlgCallFwd()
{
    delete ui;
}

void DlgCallFwd::showCallFwd(QString extn)
{

    QList<QString> listExtn;
    int i;

    QMap<QString,Extension *>::iterator itr;
    QString extnNum;
    Extension *extnData;

    myExtnName = extn;
    extensions.empty();
    extensions.clear();
    listExtn.clear();
    listExtn.empty();

    // Get extensions and create list
    commapi capi("192.168.100.127", 9000);

    if (capi.readExtensionData(extensions) <= 0)
    {
        QMessageBox::warning(this,"Follow me setting", "Unable to get extension information for display.");

        return;
    }

    listExtn.append("");

    for (itr= extensions.begin(); itr != extensions.end(); itr++)
    {
        extnData = itr.value();

  //       extnNum = itr.key();

         if ( extnData->endPoint.name == myExtnName)
         {
             myExtnNum = extnData->endPoint.extn;
             continue;
         }
         listExtn.append(extnData->endPoint.name);

       i++;
    }
    ui->comboBusyExtn->clear();
    ui->comboBusyExtn->addItems(listExtn);
    ui->comboNoAnsExtn->clear();
    ui->comboNoAnsExtn->addItems(listExtn);

    // retriev the forwarding information of this/my extension
//    myExtn = extn;

//QMessageBox::information(this,"CF", myExtnNum);
//return;
    // read CFwd data
    if (capi.readCFInfo(myExtnNum, callFwdInfo) < 0)
    {
        QMessageBox::warning(this,"Call Forwarding", "Unable to fetch data, check DB Settings");
        close();
    }

    // display screen
    if ( callFwdInfo.callFwdSet == 0)
    {
        ui->cbCallFwd->setCheckState(Qt::Unchecked);

        ui->chkNoAnswer->setDisabled(true);
        ui->rbNoAnsExtn->setDisabled(true);
        ui->rbNoAnsVM->setDisabled(true);
        //ui->leNoAnsExtn->setDisabled(true);
        //ui->leNoAnsExtn->setDisabled(true);

        ui->comboNoAnsExtn->setDisabled(true);

        ui->chkOnBusy->setDisabled(true);
        ui->rbBusyOnVM->setDisabled(true);
        ui->rbBusyExtn->setDisabled(true);
        //ui->leBusyExtn->setDisabled(true);

        ui->comboBusyExtn->setDisabled(true);
    }
    else
    {
//qDebug() << "CF is Set" << "busy:" << callFwdInfo.callFwdOnBusy << "No Ans:"<< callFwdInfo.callFwdOnNoAns;
        ui->cbCallFwd->setCheckState(Qt::Checked);

        if ( callFwdInfo.callFwdOnBusy ==0)
        {
            ui->chkOnBusy->setEnabled(true);
            ui->chkOnBusy->setCheckState(Qt::Unchecked);
            ui->rbBusyOnVM->setDisabled(true);
            ui->rbBusyExtn->setDisabled(true);
          //  ui->leBusyExtn->setDisabled(true);

            ui->comboBusyExtn->setDisabled(true);
        }
        else
        {
            // display busy info
            ui->chkOnBusy->setEnabled(true);
            ui->chkOnBusy->setCheckState(Qt::Checked);

            if ( callFwdInfo.callFwdOnBusy == 1)
            {
                // Check Voice mail
                ui->rbBusyOnVM->setDisabled(false);
                ui->rbBusyOnVM->setChecked(true);

                // Uncheck extension
                ui->rbBusyExtn->setDisabled(false);
               // ui->leBusyExtn->setDisabled(true);
                ui->comboBusyExtn->setDisabled(true);
            }
            else
            {
                // Uncheck voice Message
                ui->rbBusyOnVM->setDisabled(false);
                ui->rbBusyOnVM->setChecked(false);

                // check extension
                ui->rbBusyExtn->setDisabled(false);
                ui->rbBusyExtn->setChecked(true);

               // ui->leBusyExtn->setDisabled(false);
                //ui->leBusyExtn->setText(callFwdInfo.fwdExtnBusy);

                ui->comboBusyExtn->setDisabled(false);

                for (itr= extensions.begin(); itr != extensions.end(); itr++)
                {
                    if ( itr.key() == callFwdInfo.fwdExtnBusy)
                    {
                        ui->comboBusyExtn->setCurrentText(itr.value()->endPoint.name);
                        break;
                    }
                }


                // now fill the extensions
            }
        }
            // display No Ans
//            ui->chkNoAnswer->setEnabled(true);
  //          ui->chkNoAnswer->setCheckState(Qt::Checked);

        // No Ans case
        if ( callFwdInfo.callFwdOnNoAns == 0)
        {
            ui->chkNoAnswer->setEnabled(true);
            ui->chkNoAnswer->setCheckState(Qt::Unchecked);

            ui->rbNoAnsVM->setDisabled(true);
            ui->rbNoAnsExtn->setChecked(true);

            //ui->leNoAnsExtn->setDisabled(true);
            ui->comboNoAnsExtn->setDisabled(true);
        }
        else
        {
            ui->chkNoAnswer->setEnabled(true);
            ui->chkNoAnswer->setCheckState(Qt::Checked);

            ui->rbNoAnsVM->setDisabled(false);
            ui->rbNoAnsVM->setChecked(false);

            if ( callFwdInfo.callFwdOnNoAns == 1)
            {
                // Check Voice mail
                ui->rbNoAnsVM->setDisabled(false);
                ui->rbNoAnsVM->setChecked(true);

                // Uncheck extension
                ui->rbNoAnsExtn->setDisabled(false);
               // ui->leBusyExtn->setDisabled(true);
                ui->comboNoAnsExtn->setDisabled(true);
            }
            else
            {
                // Uncheck voice Message
                ui->rbNoAnsVM->setDisabled(false);
                ui->rbNoAnsVM->setChecked(false);

                // check extension
                ui->rbNoAnsExtn->setDisabled(false);
                ui->rbNoAnsExtn->setChecked(true);

               // ui->leBusyExtn->setDisabled(false);
                //ui->leBusyExtn->setText(callFwdInfo.fwdExtnBusy);

                ui->comboNoAnsExtn->setDisabled(false);
                for (itr= extensions.begin(); itr != extensions.end(); itr++)
                {
                    if ( itr.key() == callFwdInfo.fwdExtnNoAns)
                    {
                        ui->comboNoAnsExtn->setCurrentText(itr.value()->endPoint.name);
                        break;
                    }
                }
            }
        }
    }

    show();
 }

void DlgCallFwd::on_cbCallFwd_stateChanged(int arg1)
{
    if ( arg1 == 0)
    {

        ui->chkNoAnswer->setDisabled(true);
        ui->rbNoAnsExtn->setDisabled(true);
        ui->rbNoAnsVM->setDisabled(true);
       // ui->leNoAnsExtn->setDisabled(true);
        ui->comboNoAnsExtn->setDisabled(true);

        ui->chkOnBusy->setDisabled(true);
        ui->rbBusyOnVM->setDisabled(true);
        ui->rbBusyExtn->setDisabled(true);
        //ui->leBusyExtn->setDisabled(true);
        ui->comboBusyExtn->setDisabled(true);

    }
    else
    {

        ui->chkNoAnswer->setDisabled(false);
        if ( ui->chkNoAnswer->checkState() == true)
        {
            ui->rbNoAnsExtn->setDisabled(false);
            ui->rbNoAnsVM->setDisabled(false);
           // ui->leNoAnsExtn->setDisabled(false);
            ui->comboNoAnsExtn->setDisabled(false);
        }
        else
        {
            ui->rbNoAnsExtn->setDisabled(true);
            ui->rbNoAnsVM->setDisabled(true);
           // ui->leNoAnsExtn->setDisabled(true);
            ui->comboNoAnsExtn->setDisabled(true);
        }

        ui->chkOnBusy->setDisabled(false);

        if ( ui->chkOnBusy->checkState() == true)
        {
            ui->rbBusyOnVM->setDisabled(false);
            ui->rbBusyExtn->setDisabled(false);
            //ui->leBusyExtn->setDisabled(false);

            ui->comboBusyExtn->setDisabled(false);
        }
        else
        {
            ui->rbBusyOnVM->setDisabled(true);
            ui->rbBusyExtn->setDisabled(true);
           // ui->leBusyExtn->setDisabled(true);

            ui->comboBusyExtn->setDisabled(true);
        }

    }
}

void DlgCallFwd::on_chkNoAnswer_stateChanged(int arg1)
{
    if ( arg1 == 0)
    {
        ui->rbNoAnsExtn->setDisabled(true);
        ui->rbNoAnsVM->setDisabled(true);
      //  ui->leNoAnsExtn->setDisabled(true);

        ui->comboNoAnsExtn->setDisabled(true);

    }
    else
    {
//        ui->rbNoAnsVM->setDisabled(false);
        ui->rbNoAnsVM->setDisabled(false);
        ui->rbNoAnsExtn->setDisabled(false);
        if (ui->rbNoAnsExtn->isChecked() == true)
        {
//            ui->leNoAnsExtn->setDisabled(false);

            ui->comboNoAnsExtn->setDisabled(false);

        }
        else
        {
//            ui->leNoAnsExtn->setDisabled(true);

            ui->comboNoAnsExtn->setDisabled(true);
        }
    }

}


void DlgCallFwd::on_chkOnBusy_stateChanged(int arg1)
{
    if ( arg1 == 0)
    {
        ui->rbBusyExtn->setDisabled(true);
        ui->rbBusyOnVM->setDisabled(true);
     //   ui->leBusyExtn->setDisabled(true);
        ui->comboBusyExtn->setDisabled(true);

    }
    else
    {
        ui->rbBusyOnVM->setDisabled(false);
        ui->rbBusyExtn->setDisabled(false);
        if ( ui->rbBusyExtn->isChecked()== true)
        {
         //   ui->leBusyExtn->setDisabled(false);
            ui->comboBusyExtn->setDisabled(false);
        }
        else
        {
           // ui->leBusyExtn->setDisabled(true);
            ui->comboBusyExtn->setDisabled(true);
        }

    }

}

void DlgCallFwd::on_rbNoAnsVM_clicked()
{
//    ui->leNoAnsExtn->setDisabled(true);
    ui->comboNoAnsExtn->setDisabled(true);
}

void DlgCallFwd::on_rbNoAnsExtn_clicked()
{
 //   ui->leNoAnsExtn->setDisabled(false);

    ui->comboNoAnsExtn->setDisabled(false);

}

void DlgCallFwd::on_rbBusyOnVM_clicked()
{
//    ui->leBusyExtn->setDisabled(true);
    ui->comboBusyExtn->setDisabled(true);
}

void DlgCallFwd::on_rbBusyExtn_clicked()
{
//    ui->leBusyExtn->setDisabled(false);

    ui->comboBusyExtn->setDisabled(false);
}

void DlgCallFwd::on_pbSave_clicked()
{
    CallFWD cf;
    QString fwdExtn;
    Extension *extnData;

    QMap<QString,Extension *>::iterator itr;
    QString extnNum;

    cf.extn = myExtnNum;
    cf.callFwdSet=0;
    cf.fwdExtnBusy="";
    cf.fwdExtnNoAns="";
    cf.callFwdOnBusy=0;
    cf.callFwdOnNoAns=0;
    // save here
    if (ui->cbCallFwd->checkState() == Qt::Checked)
    {
        cf.callFwdSet =1;
        // check for no ans
        if ( ui->chkNoAnswer->checkState() == Qt::Checked)
        {
            qDebug() << "Forwrd incase No Ans";

              if (ui->rbNoAnsVM->isChecked() == true)
              {
                    cf.callFwdOnNoAns = 1;
                  //  qDebug() << "Forwrd to VM incase No Ans";

              }
              else
              {
//                  fwdExtn = ui->leNoAnsExtn->text();
                  fwdExtn = ui->comboNoAnsExtn->currentText();

                  for (itr= extensions.begin(); itr != extensions.end(); itr++)
                  {
                      extnData = itr.value();
                      if (extnData->endPoint.name == fwdExtn)
                      {
                          fwdExtn = extnData->endPoint.extn;
                          break;
                      }
                  }


                  if ( fwdExtn == "")
                  {
                        QMessageBox::warning(this,"Call Forwarding", "Forwarding extension cann't be blank");
                        return;
                  }
                  else
                  {
                        cf.callFwdOnNoAns = 2;
                        cf.fwdExtnNoAns = fwdExtn;
                    //    qDebug() << "Forwrd to extn incase No Ans" << fwdExtn;
                  }
              }

        }

        if ( ui->chkOnBusy->checkState() == Qt::Checked)
        {
            // get info of busy
            if (ui->rbBusyOnVM->isChecked() == true)
            {
                  cf.callFwdOnBusy = 1;
            }
            else
            {
//                fwdExtn = ui->leBusyExtn->text();
                fwdExtn = ui->comboBusyExtn->currentText();

                for (itr= extensions.begin(); itr != extensions.end(); itr++)
                {
                    extnData = itr.value();
                    if (extnData->endPoint.name == fwdExtn)
                    {
                        fwdExtn = extnData->endPoint.extn;
                        break;
                    }
                }


                if ( fwdExtn == "")
                {
                      QMessageBox::warning(this,"Call Forwarding", "Forwarding extension cann't be blank");
                      return;
                }
                else
                {
                    cf.callFwdOnBusy = 2;
                    cf.fwdExtnBusy = fwdExtn;
                }
            }

        }

        if ( cf.callFwdOnBusy == 0 && cf.callFwdOnNoAns == 0)
        {
            QMessageBox::warning(this,"Call Forwatd", "No forwarding destination is assigned.");
            return;
        }
    }
    else
    {

       cf.callFwdSet = 0;
    }

    commapi capi("192.168.100.127",9000);

    if ( capi.saveCFInfo(cf) < 0)
    {
        QMessageBox::warning(this,"Call Forward", "Unable to save setting");

    }
    else
    {
        QMessageBox::warning(this,"Call Forward", "Setting saved successfully");

    }

    this->close();
}

void DlgCallFwd::on_pbCancel_clicked()
{
    close();
}
