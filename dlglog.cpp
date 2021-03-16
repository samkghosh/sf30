#include "dlglog.h"
#include "ui_dlglog.h"

DlgLog::DlgLog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgLog)
{
    ui->setupUi(this);



}

DlgLog::~DlgLog()
{
    delete ui;
}

void DlgLog::log(QString msg)
{

    ui->lstLog->addItem(msg);
    ui->lstLog->scrollToBottom();
}

void DlgLog::displayMsg(QString msg)
{
    log(msg);
}


void DlgLog::setPos(const int x, const int y)
{
    this->setGeometry(x, y, this->width(), this->height());
//    this->setFocus();
//    this->setModal(true);
}
