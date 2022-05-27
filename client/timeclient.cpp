#include "timeclient.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QDataStream>
#include <QMessageBox>

TimeClient::TimeClient(QWidget *parent)
    :QDialog(parent)
{
    setWindowTitle(tr("多线程时间客户端")); //布局
    serverNameLabel = new QLabel("服务器名");
    serverNameLineEdit = new QLineEdit("Localhost");
    portLabel = new QLabel("端口：");
    portLineEdit = new QLineEdit;
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(serverNameLabel,0,0);
    layout->addWidget(serverNameLineEdit,0,1);
    layout->addWidget(portLabel,1,0);
    layout->addWidget(portLineEdit,1,1);
    dateTimeEdit = new QDateTimeEdit(this);
    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addWidget(dateTimeEdit);
    stateLabel = new QLabel("请先运行时间服务器：");
    QHBoxLayout *h2layout = new QHBoxLayout;
    h2layout->addWidget(stateLabel);
    getBtn = new QPushButton(tr("获取时间"));
    getBtn->setDefault(true);
    getBtn->setEnabled(false);
    quitBtn = new QPushButton(tr("退出"));
    QHBoxLayout *h3layout = new QHBoxLayout;
    h3layout->addWidget(getBtn);
    h3layout->addWidget(quitBtn);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(layout);
    mainLayout->addLayout(hlayout);
    mainLayout->addLayout(h2layout);
    mainLayout->addLayout(h3layout);
    connect(serverNameLineEdit,SIGNAL(textChanged(QString)),this,SLOT(enableGetBtn()));
    connect(portLineEdit,SIGNAL(textChanged(QString)),this,SLOT(enableGetBtn()));
    connect(getBtn,SIGNAL(clicked()),this,SLOT(getTime()));
    connect(quitBtn,SIGNAL(clicked()),this,SLOT(close()));
    tcpSocket = new QTcpSocket(this); //创建tcp对象
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(readTime())); //当发送readyRead信号时获取时间
    connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(showError(QAbstractSocket::SocketError)));//当Tcp错误时打印错误原因
    portLineEdit->setFocus();
}

TimeClient::~TimeClient(){
    
}

void TimeClient::enableGetBtn(){ //为空时不可用
    getBtn->setEnabled(!serverNameLineEdit->text().isEmpty() && !portLineEdit->text().isEmpty());
}

void TimeClient::getTime(){ //tcp客户端获取时间
    getBtn->setEnabled(false);
    time2u = 0;
    tcpSocket->abort();//重置套接字 清空缓存
    tcpSocket->connectToHost(serverNameLineEdit->text(),portLineEdit->text().toInt()); //根据输入内容连接到主机
}

void TimeClient::readTime(){
    QDataStream in(tcpSocket); //用DataStream保存tcp套接字缓存
    in.setVersion(QDataStream::Qt_5_11);
    if(time2u == 0){
        if(tcpSocket->bytesAvailable() < (int)sizeof(uint))
            return;
        in>>time2u;
    }
    dateTimeEdit->setDateTime(QDateTime::fromTime_t(time2u)); //设置时间
    getBtn->setEnabled(true);
}

void TimeClient::showError(QAbstractSocket::SocketError socketError){
    switch (socketError){
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this,tr("时间服务客户端"),tr("主机不可用！"));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this,tr("时间服务客户端"),tr("连接被拒绝！"));
        break;
    default:
        QMessageBox::information(this,tr("时间服务客户端"),tr("产生如下错误:%1.").arg(tcpSocket->errorString()));
        break;
    }
    getBtn->setEnabled(true);
}
