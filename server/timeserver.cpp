#include "timeserver.h"
#include "timethread.h"
#include "dialog.h"

TimeServer::TimeServer(QObject *parent)
    :QTcpServer(parent){
    dlg = (Dialog *)parent; //传入父对象
}

void TimeServer::incomingConnection(qintptr socketDescriptor){ //重写虚函数，TCP监听到有客户端时调用
    TimeThread *thread = new TimeThread(socketDescriptor,0); //创建线程对象并传入套接字对象
    connect(thread,SIGNAL(finished()),dlg,SLOT(slotShow())); //线程结束启动对应槽函数
    connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()),Qt::DirectConnection); //等待线程结束删除线程
    thread->start(); //开启线程
}
