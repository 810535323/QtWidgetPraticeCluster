#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "server/dialog.h"
#include "client/timeclient.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
    Dialog *server;
    QPushButton *serverBtn;
    QPushButton *clientBtn;
    QPushButton *quitBtn;
    
public slots:
    void serverShow();
    void clientShow();
    void serverDialogClosed();
};
#endif // MAINWINDOW_H
