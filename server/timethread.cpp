#include "timethread.h"
#include <QDateTime>
#include <QByteArray>
#include <QDataStream>

TimeThread::TimeThread(qintptr socketDescriptor,QObject * parent)
    :QThread(parent),socketDescriptor(socketDescriptor){ //继承线程类与TCP套接字类
    
}

void TimeThread::run(){ //重写run方法
    QTcpSocket tcpsocket;
    if(!tcpsocket.setSocketDescriptor(socketDescriptor)){ //创建套接字句柄、绑定端口、使用数据流传输当前时间
        emit error(tcpsocket.error());
        return ;
    }
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_11);
    uint time2u = QDateTime::currentDateTime().toTime_t();
    out<<time2u;
    tcpsocket.write(block);
    tcpsocket.disconnectFromHost();
    tcpsocket.waitForDisconnected();
}
