#include "MainWindow.h"

#include <QApplication>
#include <conndlg/connectdlg.h>
#include <QDialog>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ConnDlg dialog;
    if(dialog.exec() != QDialog::Accepted){
        return -1;
    }
    QFile *carDetails = new QFile("attribs.xml");
    MainWindow w("factory","cars",carDetails);
    w.show();
    return a.exec();
}
