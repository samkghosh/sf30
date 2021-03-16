#include "dlgextensionconf.h"
#include "ui_dlgextensionconf.h"

#include <QComboBox>
#include <QStringList>
#include <QMessageBox>

DlgExtensionConf::DlgExtensionConf(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgExtensionConf)
{
    ui->setupUi(this);

    QStringList listYesNo;
    QStringList listRecMode;

    listYesNo << "no" << "yes";
    ui->cbAdminMode->addItems(listYesNo);
    ui->cbMediaEncry->addItems(listYesNo);

    listRecMode << "no" << "Channel Recording" << "Conversation Recorind" << "Both";
    ui->cbRecMode->addItems(listRecMode);

    ui->cbAdminMode->setCurrentIndex(0);
    ui->cbMediaEncry->setCurrentIndex(0);
    ui->cbRecMode->setCurrentIndex(3);

    ui->leExtionsion->setReadOnly(false);
}

DlgExtensionConf::~DlgExtensionConf()
{
    delete ui;
}

void DlgExtensionConf::on_pbClear_clicked()
{
    ui->leExtionsion->setReadOnly(false);

    ui->leExtionsion->setText("");
    ui->leName->setText("");
    ui->lePassword->setText("");
    ui->leCodec->setText("");
    ui->leContext->setText("");
    ui->cbRecMode->setCurrentIndex(3);
    ui->cbMediaEncry->setCurrentIndex(1);
    ui->cbAdminMode->setCurrentIndex(1);

}



