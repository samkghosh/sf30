#include "dlgpresence.h"
#include "ui_dlgpresence.h"

DlgPresence::DlgPresence(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgPresence)
{
    ui->setupUi(this);


}

DlgPresence::~DlgPresence()
{
    delete ui;
}

void DlgPresence::on_pbOk_clicked()
{
    int sts=0;

    if (ui->rbOnline->isChecked())
        sts = 1;
    if (ui->rbOffline->isChecked())
        sts = 2;
    if (ui->rbAway->isChecked())
        sts = 3;
    if (ui->rbBusy->isChecked())
        sts = 4;

    emit changePresence(sts);

    close();
}


void DlgPresence::setPresence(int sts)
{
    switch(sts)
    {
           case 1: //online
                ui->rbOnline->setChecked(true);
                presence = sts;
                break;
            case 2://offline
                ui->rbOffline->setChecked(true);
                presence = sts;
                break;
            case 3://away
                ui->rbAway->setChecked(true);
                presence = sts;
                break;
            case 4://busy
                ui->rbBusy->setChecked(true);
                presence = sts;
                break;
    }
}

void DlgPresence::on_rbOnline_clicked()
{
    presence =1;
}

void DlgPresence::on_rbOffline_clicked()
{
    presence =2;
}

void DlgPresence::on_rbAway_clicked()
{
    presence =3;
}

void DlgPresence::on_rbBusy_clicked()
{
    presence = 4;
}

//void DlgPresence::setParent(DlgPhone *ph)
//{
    //this->dlgPhone = ph;
//}
