#ifndef DLGFOLLOWME_H
#define DLGFOLLOWME_H

#include <QDialog>

#include "dbaccess.h"


namespace Ui {
class DlgFollowme;
}

class DlgFollowme : public QDialog
{
    Q_OBJECT

public:
    explicit DlgFollowme(QWidget *parent = nullptr);
    ~DlgFollowme();

    //void setPos(int x, int y);
    void showFollowme(QString extn);

private slots:
    void on_pbCancel_clicked();

    void on_pbSave_clicked();

    void on_pbClear_clicked();

private:
    Ui::DlgFollowme *ui;

    QList<FollowMe *> fmeList;
    QMap<QString,Extension *> extensions;

    QString myExtnName;
    QString myExtnNum;
};

#endif // DLGFOLLOWME_H
