#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include <QtGui>
#include <QDialog>
#include <QtSql>
#include <QtXml>
#include <QtWidgets/QDialogButtonBox>
#include "ui_connectdlg.h"

class editdialog : public QDialog
{
    Q_OBJECT
    
public:
    editdialog(QSqlRelationalTableModel *cars, QSqlTableModel *factory,QDomDocument details, QFile *output,QWidget *parent = 0);
    ~editdialog();
    
private slots:
    void revert();
    void submit();
private:
    int addNewCar(const QString &name, int factoryId);
    int addNewFactory(const QString &factory,const QString &address);
    void addAttributes(int carId, QStringList attribs);
    QDialogButtonBox *createButtons();
    QGroupBox *createInputWidgets();
    int findFactoryId(const QString &factory);
    int generateCarId();
    int generateFactoryId();
    QSqlRelationalTableModel *carModel;
    QSqlTableModel *factoryModel;
    QDomDocument carDetails;
    QFile *outputFile;
    QLineEdit *factoryEditor;
    QLineEdit *addressEditor;
    QLineEdit *carEditor;
    QSpinBox *yearEditor;
    QLineEdit *attribEditor;
};

#endif // EDITDIALOG_H
