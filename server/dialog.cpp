#include "dialog.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent){
    count = 0;
    timeserver = new TimeServer(this); //创建时间服务器继承与QTCP类，当有连接是会自动调用incomingConnection
    setWindowTitle(tr("多线程时间服务器"));
    Label1 = new QLabel(tr("服务器端口: "));
    Label2 = new QLabel;
    quitBtn = new QPushButton(tr("退出"));
    QHBoxLayout *BtnLayout = new QHBoxLayout;
    BtnLayout->addStretch(1);
    BtnLayout->addWidget(quitBtn);
    BtnLayout->addStretch(1);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(Label1);
    mainLayout->addWidget(Label2);
    mainLayout->addLayout(BtnLayout);
    connect(quitBtn,SIGNAL(clicked()),this,SLOT(close()));
    connect(quitBtn,SIGNAL(clicked()),this,SLOT(dlgclose()));
    if(!timeserver->listen()){ //tcp服务器开启监听失败
        QMessageBox::critical(this,tr("多线程时间服务器"),
                              tr("无法启动服务器:%1.").arg(timeserver->errorString()));
        close();
        return ;
    }
    Label1->setText(tr("服务器端口: %1. ").arg(timeserver->serverPort())); //服务器创建成功
}

Dialog::~Dialog(){
    
}

void Dialog::slotShow(){
    Label2->setText(tr("第%1次请求完毕。").arg(++count));
}

void Dialog::dlgclose(){
    emit closed();
}
