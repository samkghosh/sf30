#ifndef DLGEXTENSIONCONF_H
#define DLGEXTENSIONCONF_H

#include <QDialog>
#include "dbaccess.h"


namespace Ui {
class DlgExtensionConf;
}

class DlgExtensionConf : public QDialog
{
    Q_OBJECT

public:
    explicit DlgExtensionConf(QWidget *parent = nullptr);
    ~DlgExtensionConf();
    int showConfigDialog(Extension *extnData);
    void setExtensionList( QMap<QString,Extension *> *extList){extnList = extList;};

private slots:
    void on_pbClear_clicked();

    void on_pbDefault_clicked();

    void on_pbSave_clicked();

    void on_leExtionsion_textChanged(const QString &arg1);

private:
    Ui::DlgExtensionConf *ui;
    Extension extnData;
    QMap<QString,Extension *> *extnList;
};

#endif // DLGEXTENSIONCONF_H
