#ifndef DLGPHONEBOOK_H
#define DLGPHONEBOOK_H

#include <QDialog>


#include "dataprocessing.h"
#include "dlglog.h"


namespace Ui {
class DlgPhonebook;
}

class DlgPhonebook : public QDialog
{
    Q_OBJECT

public:
    explicit DlgPhonebook(QWidget *parent = nullptr);
    ~DlgPhonebook();
    void setPos(int x, int y);
    void setDlgLog(DlgLog *dlg);
    void displayContacts();
private slots:
    void on_pbCancel_clicked();

    void on_leName_textChanged(const QString &arg1);

    void on_pbClear_clicked();

    void on_pbSave_clicked();

    void on_leSearch_textChanged(const QString &arg1);

    void onCellClicked(int row, int col);

Q_SIGNALS:
    void phoneBookData(QString item);

private:
    Ui::DlgPhonebook *ui;

    PhoneBookData pbd;
    DataProcessing dp;
    DlgLog *dlgLog;
    void logConsole(QString msg);

};

#endif // DLGPHONEBOOK_H
