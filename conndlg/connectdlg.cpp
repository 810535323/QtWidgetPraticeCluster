#include "connectdlg.h"
#include "ui_connectdlg.h"
#include <QSqlDatabase>
#include <QtSql>

ConnDlg::ConnDlg(QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this); //设置用户自定义UI
    QStringList drivers = QSqlDatabase::drivers(); //获取数据库驱动列表
    connect(ui.comboDriver,SIGNAL(currentIndexChanged(const QString &)),this,SLOT(driverChanged(const QString &)));  //当驱动列表改变则触发槽函数
    ui.comboDriver->addItems(drivers); //设置驱动
    ui.status_label->setText(tr("准备连接数据库")); //状态告知
}

ConnDlg::~ConnDlg()
{
}

void ConnDlg::driverChanged(const QString &text)
{
    if(text == "QSQLITE")
    {
        ui.editDatabase->setEnabled(false);
        ui.editUsername->setEnabled(false);
        ui.editPassword->setEnabled(false);
        ui.editHostname->setEnabled(false);
        ui.portSpinBox->setEnabled(false);
    }else{
        ui.editDatabase->setEnabled(true);
        ui.editUsername->setEnabled(true);
        ui.editPassword->setEnabled(true);
        ui.editHostname->setEnabled(true);
        ui.portSpinBox->setEnabled(true);
    }
}

QString ConnDlg::driverName() const{
    return ui.comboDriver->currentText();
}

QString ConnDlg::databaseName() const{
    return ui.editDatabase->text();
}

QString ConnDlg::userName() const{
    return ui.editUsername->text();
}

QString ConnDlg::password() const{
    return ui.editPassword->text();
}

QString ConnDlg::hostName() const{
    return ui.editHostname->text();
}

int ConnDlg::port() const{
    return ui.portSpinBox->value();
}

void ConnDlg::on_okButton_clicked(){
    if(ui.comboDriver->currentText().isEmpty()){
        ui.status_label->setText(tr("请选择一个数据库驱动！"));
        ui.comboDriver->setFocus();
    }else if(ui.comboDriver->currentText() == "QSQLITE"){ //QSQLITE则创建数据库
        addSqliteConnection();//创建数据表，存在则不执行
        creatDB();//创建数据表并插入数据
        accept(); //关闭对话框
    }else{
        QSqlError err = addConnection(driverName(),databaseName(),hostName(),userName(),password(),port());
        if(err.type() != QSqlError::NoError){
            ui.status_label->setText(err.text());
        }else{
            ui.status_label->setText(tr("连接数据库成功"));
            accept();
        }
    }
}

QSqlError ConnDlg::addConnection(const QString &driver, const QString &dbName, const QString &host, const QString &user, const QString &passwd, int port){
    QSqlError err;
    QSqlDatabase db = QSqlDatabase::addDatabase(driver); //与driver数据库连接
    db.setDatabaseName(dbName);
    db.setHostName(host);
    db.setPort(port);
    if(!db.open(user,passwd)){
        err = db.lastError();
    }
    return err;
}

void ConnDlg::addSqliteConnection(){
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE"); //选择数据库驱动
    db.setDatabaseName("databasefile.db"); //选择数据库名称
    if(!db.open()){ //打开数据库
        ui.status_label->setText(db.lastError().text());
        return ;
    }
    ui.status_label->setText(tr("创建sqlite数据库成功!"));
}

void ConnDlg::creatDB(){ //插入数据库内容
    QSqlQuery query;
    query.exec("create table factory (id int primary key,manufactory varchar(40),address varchar(40))");
    query.exec(QObject::tr("insert into factory values(1,'一汽大众','长春')"));
    query.exec(QObject::tr("insert into factory values(2,'二汽神龙','武汉')"));
    query.exec(QObject::tr("insert into factory values(3,'上海大众','上海')"));
    query.exec("create table cars (carid int primary key,name varchar(50),factoryid int,year int,foreign key(factoryid) references factory)");
    query.exec(QObject::tr("insert into cars values(1,'奥迪A6',1,2005)"));
    query.exec(QObject::tr("insert into cars values(2,'捷达',1,1993)"));
    query.exec(QObject::tr("insert into cars values(3,'宝来',1,2000)"));
    query.exec(QObject::tr("insert into cars values(4,'毕加索',2,1999)"));
    query.exec(QObject::tr("insert into cars values(5,'富康',2,2004)"));
    query.exec(QObject::tr("insert into cars values(6,'标致307',2,2001)"));
    query.exec(QObject::tr("insert into cars values(7,'桑塔纳',3,1995)"));
    query.exec(QObject::tr("insert into cars values(8,'帕萨特',3,2000)"));
}
