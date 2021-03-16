#include "dlgreport.h"
#include "ui_dlgreport.h"

DlgReport::DlgReport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgReport)
{
    ui->setupUi(this);
}

DlgReport::~DlgReport()
{
    delete ui;

}


void DlgReport::displayCDR()
{
    int iRow=0;
    int iCol=0;
    QStringList hdr;
    QString key=nullptr;

    DBAccess *dba1 = new DBAccess("report1");

    ui->tabWidget->setTabEnabled(0, true);
    this->setWindowTitle("Report on CDR");

    int rows = ui->tblCDR->rowCount();

    for (int i =0; i < rows; i++)
            ui->tblCDR->removeRow(i);
    ui->tblCDR->setRowCount(0);

    hdr << "" <<"From Number" << "To Number" << "context" << "Start Time"<< "Duration";

    ui->tblCDR->setColumnCount(6);


    ui->tblCDR->setHorizontalHeaderLabels(hdr);
    ui->tblCDR->setColumnWidth(0,20);

    qDeleteAll(cdrs.begin(), cdrs.end());
    cdrs.clear();

    int iRecCount = dba1->readCDRData(cdrs,0);

    delete dba1;

    qDebug() << "CDR Record Count:" << iRecCount;

    iRow = 0;
    for(int i=0; i < cdrs.count(); i++)
    {
        iCol=0;

        QTableWidgetItem *select = new QTableWidgetItem();
        QTableWidgetItem *from = new QTableWidgetItem();
        QTableWidgetItem *to= new QTableWidgetItem();
        QTableWidgetItem *context = new QTableWidgetItem();
        QTableWidgetItem *date = new QTableWidgetItem();
        QTableWidgetItem *duration = new QTableWidgetItem();

        from->setText(cdrs[i]->from);
        to->setText(cdrs[i]->to);
        context->setText(cdrs[i]->context);
        date->setText(cdrs[i]->dateTime.toString());
        duration->setText(QString::number(cdrs[i]->duration));

        ui->tblCDR->insertRow(iRow);
        ui->tblCDR->setItem(iRow, iCol++, select);
        select->setCheckState(Qt::Unchecked);

        ui->tblCDR->setItem(iRow, iCol++, from);
        ui->tblCDR->setItem(iRow, iCol++, to);
        ui->tblCDR->setItem(iRow, iCol++, context);
        ui->tblCDR->setItem(iRow, iCol++, date);
        ui->tblCDR->setItem(iRow, iCol++, duration);

        iRow++;

    }
    ui->lblCDRTotal->setText("Total:"+QString::number(iRow));


}

void DlgReport::on_tabWidget_tabBarClicked(int index)
{
    if ( index == 0)
    {
        displayCDR();
    }
    else
    {
        displayRecording();

    }
//    qDebug() << "TabClicked" << index;
}

void DlgReport::displayRecording()
{
    int iRow=0;
    int iCol=0;
    QStringList hdr;
    QString key=nullptr;

    DBAccess *dba1 = new DBAccess("report2");

    ui->tabWidget->setTabEnabled(1, true);
    this->setWindowTitle("Report on Recording");

    int rows = ui->tblRec->rowCount();

    for (int i =0; i < rows; i++)
            ui->tblRec->removeRow(i);

    ui->tblRec->setRowCount(0);

    hdr << "" <<"From Number" << "To Number" << "context" << "Start Time"<< "Duration" << "RecFile";

    ui->tblRec->setColumnCount(7);


    ui->tblRec->setHorizontalHeaderLabels(hdr);
    ui->tblRec->setColumnWidth(0,20);

    qDeleteAll(cdrs.begin(), cdrs.end());
    cdrs.clear();
    int iRecCount = dba1->readCDRData(cdrs, 1);

    delete dba1;

    qDebug() << "CDR Record Count:" << iRecCount;

    iRow = 0;
    for(int i=0; i < cdrs.count(); i++)
    {
        iCol=0;

        QTableWidgetItem *select = new QTableWidgetItem();
        QTableWidgetItem *from = new QTableWidgetItem();
        QTableWidgetItem *to= new QTableWidgetItem();
        QTableWidgetItem *context = new QTableWidgetItem();
        QTableWidgetItem *date = new QTableWidgetItem();
        QTableWidgetItem *duration = new QTableWidgetItem();
        QTableWidgetItem *recFile = new QTableWidgetItem();

        from->setText(cdrs[i]->from);
        to->setText(cdrs[i]->to);
        context->setText(cdrs[i]->context);
        date->setText(cdrs[i]->dateTime.toString());
        duration->setText(QString::number(cdrs[i]->duration));
        recFile->setText(cdrs[i]->recFileName);

        ui->tblRec->insertRow(iRow);
        ui->tblRec->setItem(iRow, iCol++, select);
        select->setCheckState(Qt::Unchecked);

        ui->tblRec->setItem(iRow, iCol++, from);
        ui->tblRec->setItem(iRow, iCol++, to);
        ui->tblRec->setItem(iRow, iCol++, context);
        ui->tblRec->setItem(iRow, iCol++, date);
        ui->tblRec->setItem(iRow, iCol++, duration);
        ui->tblRec->setItem(iRow, iCol++, recFile);

        iRow++;

    }

    ui->lblRecTotal->setText("Total:"+QString::number(iRow));

}
