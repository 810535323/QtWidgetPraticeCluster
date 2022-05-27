#ifndef DIALOG_H
#define DIALOG_H
#include "timeserver.h"
#include <QDialog>
#include <QLabel>
#include <QPushButton>

class Dialog : public QDialog{
    Q_OBJECT
public:
    Dialog(QWidget *parent = 0);
    ~Dialog();
private:
    QLabel *Label1;
    QLabel *Label2;
    QPushButton *quitBtn;
    TimeServer *timeserver;
    int count;
public slots:
    void slotShow();
    void dlgclose();
signals:
    void closed();
};

#endif // DIALOG_H
