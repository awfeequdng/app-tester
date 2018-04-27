#ifndef SQLUNQUAL_H
#define SQLUNQUAL_H

#include <QLabel>
#include <QDebug>
#include <QWidget>
#include <QLayout>
#include <QGroupBox>
#include <QDateEdit>
#include <QShowEvent>
#include <QTableView>
#include <QHeaderView>
#include <QPushButton>
#include <QMessageBox>

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlTableModel>

#include "sqlsnowid.h"

class SqlUnqual : public QWidget
{
    Q_OBJECT
public:
    explicit SqlUnqual(QWidget *parent = 0);
private slots:
    void initUI();
    void initViewBar();
    void initTextBar();
    void initLayout();
    void createSqlite();
    void updateSqlite();
    void insertSqlite();
    void selectSqlite();
    virtual void showEvent(QShowEvent *e);
    void recvSqlMap(QVariantMap msg);
private:
    QVBoxLayout *boxLayout;
    QDateEdit *stop;

    QString sqlName;
    QTableView *view;
    QSqlTableModel *mView;
};

#endif // SQLUNQUAL_H
