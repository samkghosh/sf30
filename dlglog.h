#ifndef DLGLOG_H
#define DLGLOG_H

#include <QDialog>


namespace Ui {
class DlgLog;
}

class DlgLog : public QDialog
{
    Q_OBJECT

public:
    explicit DlgLog(QWidget *parent = nullptr);
    ~DlgLog();
    void log(QString msg);
    void setPos(const int x, const int y);

private slots:
        void displayMsg(QString msg);

private:
    Ui::DlgLog *ui;
};

#endif // DLGLOG_H
