//#include "dlgphone.h"
#include "dlgowspanel.h"

#include <QApplication>
#include <QLocalServer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    //DlgPhone w;
    DlgOWSPanel ows;
    //w.show();
    ows.show();
    return a.exec();

}
