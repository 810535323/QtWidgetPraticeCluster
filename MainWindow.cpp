#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QGridLayout>
#include <QAbstractItemView>
#include <QHeaderView>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QSqlRecord>
#include <QtDebug>
#include "editdialog/editdialog.h"

extern int uniqueCarId;
extern int uniqueFactoryId;

MainWindow::MainWindow(const QString &factoryTable,const QString &carTable,QFile *carDetails,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    file = carDetails; //保存文件句柄指针
    readCarData(); //读取XML文件信息
    carModel = new QSqlRelationalTableModel(this); //创建一个数据库RelationalTableModel模型
    carModel->setTable(carTable); //设置表名称
    carModel->setRelation(2,QSqlRelation(factoryTable,"id","manufactory"));//当前表的第二列关联factoryTable表下的manufactory表的id
    carModel->select(); //等价于"SELECT * FROM carTable"
    factoryModel = new QSqlTableModel(this);//创建一个数据库TableModel模型
    factoryModel->setTable(factoryTable);//设置表名称
    factoryModel->select();//等价于"SELECT * FROM factoryTable"
    ui->setupUi(this);
    QGroupBox *factory = createFactoryGroupBox(); //创建GroupBox类型保存生产商内容
    QGroupBox *cars = createCarGroupBox(); //创建GroupBox类型保存汽车内容
    QGroupBox *details = createDetailGroupBox(); //创建GroupBox类型保存汽车细节内容
    //布局
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(factory);
    layout->addWidget(cars);
    layout->addWidget(details,0,1,2,1);
    layout->setColumnStretch(1,1);
    layout->setColumnMinimumWidth(0,500);
    QWidget *widget = new QWidget;
    widget->setLayout(layout);
    setCentralWidget(widget);
    createMenuBar();
    resize(850,400);
    setWindowTitle(tr("主从视图"));
    uniqueCarId = carModel->rowCount();
    uniqueFactoryId = factoryModel->rowCount();
}

MainWindow::~MainWindow()
{
    delete ui;
}

QGroupBox* MainWindow::createFactoryGroupBox(){ //布局
    factoryView = new QTableView;
    factoryView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    factoryView->setSortingEnabled(true);
    factoryView->setSelectionBehavior(QAbstractItemView::SelectRows);
    factoryView->setSelectionMode(QAbstractItemView::SingleSelection);
    factoryView->setShowGrid(false);
    factoryView->setAlternatingRowColors(true);
    factoryView->setModel(factoryModel);
    connect(factoryView,SIGNAL(clicked(QModelIndex)),this,SLOT(changeFactory(QModelIndex)));
    QGroupBox *box = new QGroupBox(tr("汽车制造商"));
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(factoryView,0,0);
    box->setLayout(layout);
    return box;
}

QGroupBox* MainWindow::createCarGroupBox(){ //布局
    QGroupBox *box = new QGroupBox(tr("汽车"));
    carView = new QTableView;
    carView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    carView->setSortingEnabled(true);
    carView->setSelectionBehavior(QAbstractItemView::SelectRows);
    carView->setSelectionMode(QAbstractItemView::SingleSelection);
    carView->setShowGrid(false);
    carView->verticalHeader()->hide();
    carView->setAlternatingRowColors(true);
    carView->setModel(carModel);
    connect(carView,SIGNAL(clicked(QModelIndex)),this,SLOT(showCarDetails(QModelIndex)));
    connect(carView,SIGNAL(activated(QModelIndex)),this,SLOT(showCarDetails(QModelIndex)));
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(carView,0,0);
    box->setLayout(layout);
    return box;
}

QGroupBox* MainWindow::createDetailGroupBox(){ //布局
    QGroupBox *box = new QGroupBox(tr("详细信息"));
    profileLabel = new QLabel; 
    profileLabel->setWordWrap(true);
    profileLabel->setAlignment(Qt::AlignBottom);
    titleLabel = new QLabel;
    titleLabel->setWordWrap(true);
    titleLabel->setAlignment(Qt::AlignBottom);
    attribList = new QListWidget;
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(profileLabel,0,0,1,2);
    layout->addWidget(titleLabel,1,0,1,2);
    layout->addWidget(attribList,2,0,1,2);
    layout->setRowStretch(2,1);
    box->setLayout(layout);
    return box;
}

void MainWindow::createMenuBar(){ //布局
    QAction *addAction = new QAction(tr("添加"),this);
    QAction *deleteAction = new QAction(tr("删除"),this);
    QAction *quitAction = new QAction(tr("退出"),this);
    addAction->setShortcut(tr("Ctrl+A")); //设置快捷键
    deleteAction->setShortcut(tr("Ctrl+D"));
    quitAction->setShortcut(tr("Ctrl+Q"));
    QMenu *fileMenu = menuBar()->addMenu(tr("操作菜单")); //创建菜单名称
    fileMenu->addAction(addAction); //绑定行为
    fileMenu->addAction(deleteAction);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAction);
    connect(addAction,SIGNAL(triggered(bool)),this,SLOT(addCar())); //绑定信号与槽
    connect(deleteAction,SIGNAL(triggered(bool)),this,SLOT(delCar()));
    connect(quitAction,SIGNAL(triggered(bool)),this,SLOT(close()));
    return ;
}

void MainWindow::changeFactory(QModelIndex index){
    QSqlRecord record = factoryModel->record(index.row()); //从factortModel获取数据，并获取数据index行
    QString factoryId = record.value("id").toString(); //从index行获得"id"对应的值转为字符类型
    carModel->setFilter("id= '"+ factoryId +"'"); //carModel从car数据库中重新获取id与工厂id相等的Model数据
    showFactoryProfile(index); //更新Label信息
}

void MainWindow::showFactoryProfile(QModelIndex index){
    QSqlRecord record = factoryModel->record(index.row()); //从factortModel获取数据，并获取数据index行
    QString name = record.value("manufactory").toString(); //从index行获得"manufactory"对应的值转为字符类型
    int count = carModel->rowCount(); //获取一共有几个产品
    profileLabel->setText(tr("汽车制造商:%1\n产品数量:%2").arg(name).arg(count)); //更新信息
    profileLabel->show();
    titleLabel->hide();
    attribList->hide();
}

void MainWindow::showCarDetails(QModelIndex index){
    QSqlRecord record = carModel->record(index.row()); //从carModel获取数据，并获取数据index行
    QString factory = record.value("manufactory").toString(); //从index行获得"manufactory"对应的值转为字符类型
    QString name = record.value("name").toString(); //从index行获得"name"对应的值转为字符类型
    QString year = record.value("year").toString(); //从index行获得"year"对应的值转为字符类型
    QString carId = record.value("carid").toString(); //从index行获得"carid"对应的值转为字符类型
    showFactoryProfile(indexofFactory(factory)); //更新Label信息
    titleLabel->setText(tr("品牌：%1(%2)").arg(name).arg(year)); //更新Label信息
    titleLabel->show();
    QDomNodeList cars = carData.elementsByTagName("car"); //从XML文件中获取cars详细信息
    for(int i = 0; i < cars.count(); i++){
        QDomNode car = cars.item(i);
        //qDebug()<<carId<<car.toElement().attribute("id");
        if(car.toElement().attribute("id") == carId){ //将carid与数据库中carid配对
            getAttribList(car.toElement()); //获取详细信息与添加到attribList中
            break ;
        }
    }
    //qDebug()<<attribList->count();
    if(attribList->count()){
        attribList->show(); //显示详细信息
    }
}

void MainWindow::getAttribList(QDomNode car){ //获取详细信息
    attribList->clear(); //清空widget信息
    QDomNodeList attribs = car.childNodes(); //获取所有car下的子属性
    QDomNode node;
    QString attribNumber;
    for(int j = 0; j < attribs.count();j++){
        node = attribs.item(j);
        attribNumber = node.toElement().attribute("number"); //收集子属性
        QListWidgetItem *item = new QListWidgetItem(attribList);
        QString showText(attribNumber + ":" + node.toElement().text()); //重新排版
        //qDebug()<<attribNumber + ":" + node.toElement().text();
        item->setText(tr("%1").arg(showText)); //添加显示条目
    }
}

void MainWindow::addCar(){
    editdialog *dialog = new editdialog(carModel,factoryModel,carData,file,this);
    int accepted = dialog->exec();
    if(accepted == 1){
        int lastRow = carModel->rowCount() - 1;
        carView->selectRow(lastRow);
        carView->scrollToBottom();
        showCarDetails(carModel->index(lastRow,0));
    }
}

void MainWindow::delCar(){
    QModelIndexList selection = carView->selectionModel()->selectedRows(0); //获取选择的行
    if(!selection.empty()){
        QModelIndex idIndex = selection.at(0);
        int id = idIndex.data().toInt();
        QString name = idIndex.sibling(idIndex.row(),1).data().toString(); //获取行对应的名称
        QString factory = idIndex.sibling(idIndex.row(),2).data().toString(); //获取行对应的生产商
        QMessageBox::StandardButton button;
        button = QMessageBox::question(this,tr("删除汽车记录"),QString(tr("确认删除由'%1'生产的'%2'吗？").arg(factory).arg(name)),QMessageBox::Yes | QMessageBox::No); //询问是否删除
        if(button == QMessageBox::Yes){
            removeCarFromFile(id); //从xml文件中移除
            removeCarFromDatabase(idIndex); //从数据库中移除
            decreaseCarCount(indexofFactory(factory)); //减少车数量
        }else{
            QMessageBox::information(this,tr("删除汽车记录"),tr("请选择要删除的记录。"));
        }
    }
}

void MainWindow::removeCarFromFile(int id){
    QDomNodeList cars = carData.elementsByTagName("car"); //找到car id 属性
    for(int i = 0; i < cars.count(); i++){
        QDomNode node = cars.item(i);
        if(node.toElement().attribute("id").toInt() == id){ //获取与carId一致的内容
            carData.elementsByTagName("archive").item(0).removeChild(node);
            break;
        }
    }
}

void MainWindow::removeCarFromDatabase(QModelIndex index){
    carModel->removeRow(index.row());
}

void MainWindow::decreaseCarCount(QModelIndex index){
    int row = index.row();
    int count = carModel->rowCount();
    if(count == 0){
        factoryModel->removeRow(row);
    }
}

void MainWindow::readCarData(){
    if(!file->open(QIODevice::ReadOnly)){
        return ;
    }
    if(!carData.setContent(file)){
        file->close();
        return ;
    }
    file->close();
}

QModelIndex MainWindow::indexofFactory(const QString &factory){
    for(int i = 0; i < factoryModel->rowCount(); i++){
        QSqlRecord record = factoryModel->record(i);
        if(record.value("manufactory") == factory){
            return factoryModel->index(1,1);
        }
    }
    return QModelIndex();
}
