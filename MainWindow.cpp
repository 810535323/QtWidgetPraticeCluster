#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(400,300);
    server = new Dialog(this); //创建服务器对象
    server->setModal(false);
    setWindowTitle(tr("多线程服务器测试程序"));
    serverBtn = new QPushButton(tr("服务器")); //触发服务器弹窗
    clientBtn = new QPushButton(tr("客户端")); //触发客户端弹窗
    QHBoxLayout *h1Layout = new QHBoxLayout;
    h1Layout->addWidget(serverBtn);
    h1Layout->addWidget(clientBtn);
    quitBtn = new QPushButton(tr("退出"));
    QHBoxLayout *h2Layout = new QHBoxLayout;
    h2Layout->addWidget(quitBtn);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(h1Layout);
    mainLayout->addLayout(h2Layout);
    centralWidget()->setLayout(mainLayout); //布局
    connect(serverBtn,SIGNAL(clicked()),this,SLOT(serverShow())); //信号绑定
    connect(clientBtn,SIGNAL(clicked()),this,SLOT(clientShow()));
    connect(quitBtn,SIGNAL(clicked()),this,SLOT(close()));
    connect(server,SIGNAL(closed()),this,SLOT(serverDialogClosed())); //保证服务器唯一
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::serverShow(){
    serverBtn->setEnabled(false);
    server->show();
}

void MainWindow::clientShow(){
    TimeClient *client = new TimeClient(this);
    client->setModal(false);
    client->show();
}

void MainWindow::serverDialogClosed(){
    serverBtn->setEnabled(true);
}
