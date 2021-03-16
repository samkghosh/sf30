#include "dlgphonebook.h"
#include "ui_dlgphonebook.h"
#include "dataprocessing.h"
#include <QMessageBox>

DlgPhonebook::DlgPhonebook(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgPhonebook)
{
    ui->setupUi(this);

    connect(ui->tblView,SIGNAL(cellDoubleClicked(int,int)), this, SLOT(onCellClicked(int,int)));

    ui->lblPhone2->hide();
    ui->lblPhone3->hide();
    ui->lePhone2->hide();
    ui->lePhone3->hide();

    dp.getPhoneBookData(pbd);


}

DlgPhonebook::~DlgPhonebook()
{
    delete ui;
}

void DlgPhonebook::setPos(int x, int y)
{
    this->setGeometry(x, y, this->width(), this->height());


}

void DlgPhonebook::setDlgLog(DlgLog *dlg)
{
    dlgLog = dlg;

}


void DlgPhonebook::logConsole(QString msg)
{
    dlgLog->log(msg);
}

void DlgPhonebook::onCellClicked(int row, int col)
{
    logConsole("Row:"+QString::number(row)+"Col:"+ QString::number(col));
    QString item;
    //emit signal
    item = ui->tblView->item(row,2)->text();
    logConsole("Cell Value: "+item);

        emit phoneBookData(item) ;

}


void DlgPhonebook::displayContacts()
{
    int iRow=0;
    int iCol=0;
    QStringList lbl;
    QString key=nullptr;

    int rows = ui->tblView->rowCount();

    for (int i =0; i < rows; i++)
        ui->tblView->removeRow(i);
    ui->tblView->setRowCount(0);
    lbl << "" <<"Name" << "Number"; // << "Number 2" << "Number 3";

    ui->tblView->setColumnCount(3);


    ui->tblView->setHorizontalHeaderLabels(lbl);
    ui->tblView->setColumnWidth(0,20);

    QMultiMap<QString, QString>::iterator itr;

    iRow = 0;
    for(itr = pbd.pbEntry.begin();itr != pbd.pbEntry.end(); itr++ )
    {
        iCol=0;
        QTableWidgetItem *select = new QTableWidgetItem();
        QTableWidgetItem *name= new QTableWidgetItem();


        name->setText(itr.key());

        if ( key != nullptr && key == itr.key())
        {
            continue;
        }

        ui->tblView->insertRow(iRow);
        key = itr.key();

        ui->tblView->setItem(iRow, iCol++, select);
        select->setCheckState(Qt::Unchecked);
        ui->tblView->setItem(iRow,iCol++,name);
        name->setFlags(name->flags() & ~Qt::ItemIsEditable);


        QList<QString> nums = pbd.pbEntry.values(itr.key());
        for(int i =0; i <nums.size(); i++)
        {
           QTableWidgetItem *number1 = new QTableWidgetItem();
           number1->setText(nums.at(i));
           number1->setFlags(name->flags() & ~Qt::ItemIsEditable);

           ui->tblView->setItem(iRow,iCol++,number1);

//          break;
        }
        iRow++;
    }


}



void DlgPhonebook::on_pbCancel_clicked()
{
    close();
}

void DlgPhonebook::on_leName_textChanged(const QString &arg1)
{
    QList<QString> nums = pbd.pbEntry.values(arg1);

    if ( nums.count() > 0)
    {
        ui->lePhone1->setText("");
        ui->lePhone2->setText("");
        ui->lePhone3->setText("");

        for(int i =0; i <nums.size(); i++)
        {
            if ( i == 0)
               ui->lePhone1->setText(nums.at(i));
            if ( i == 1)
               ui->lePhone2->setText(nums.at(i));
            if ( i == 2)
               ui->lePhone3->setText(nums.at(i));
        }
    }
}

void DlgPhonebook::on_pbClear_clicked()
{
    ui->leName->setText("");
    ui->lePhone1->setText("");
    ui->lePhone2->setText("");
    ui->lePhone3->setText("");

}

void DlgPhonebook::on_pbSave_clicked()
{
    QString key;
    QString val;

    //get the key , here name
    key = ui->leName->text();

    if ( key == nullptr || key == "")
    {
        QMessageBox::warning(this,"Warning", "No Name is present");
        return;
    }

    pbd.pbEntry.remove(key);

/*    val = ui->lePhone3->text();
    if ( val != nullptr && val != "")
    {
        // add the value
        pbd.pbEntry.insert(key,val);
    }

    val = ui->lePhone2->text();
    if ( val != nullptr && val != "")
    {
        // add the value
        pbd.pbEntry.insert(key,val);
    }
*/
    val = ui->lePhone1->text();
    if ( val != nullptr && val != "")
    {
        // add the value
        pbd.pbEntry.insert(key,val);
    }

    dp.savePhoneBookData(pbd);

    QMessageBox::information(this, "Add Phonebook","Contact added successfully");

    displayContacts();
}

void DlgPhonebook::on_leSearch_textChanged(const QString &arg1)
{
    QTableWidget *t =ui->tblView;

    if ( arg1 == "")
    {
        for (int i =0; i < t->rowCount(); i++)
        {
            t->setRowHidden(i, false);
        }
        return;
    }

    for (int i =0; i < t->rowCount(); i++)
    {
        t->setRowHidden(i, true);
    }

    QList<QTableWidgetItem *> tmpTbl = ui->tblView->findItems(arg1,Qt::MatchContains);
    QTableWidgetItem *rowPtr;
    foreach(rowPtr, tmpTbl)
    {
        int r = rowPtr->row();
        t->setRowHidden(r, false);
   }
}
