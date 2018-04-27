/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       数据管理
*******************************************************************************/
#ifndef SQLRECORD_H
#define SQLRECORD_H

#include <QLabel>
#include <QDebug>
#include <QWidget>
#include <QLayout>
#include <QGroupBox>
#include <QDateEdit>
#include <QComboBox>
#include <QTableView>
#include <QHeaderView>
#include <QPushButton>
#include <QElapsedTimer>
#include <QMessageBox>
#include <QShowEvent>

#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlTableModel>

class SqlRecord : public QWidget
{
    Q_OBJECT
public:
    explicit SqlRecord(QWidget *parent = 0);
signals:
    void sendAppMap(QVariantMap msg);
private slots:
    void initUI();
    void initLayout();
    void initViewBar();
    void initTextBar();
    void initButtonBar();

    void recvSelect();
    void recvExportAll();
    void recvExportDate();
    void recvDetail();
    void clickIndex(QModelIndex index);
    virtual void showEvent(QShowEvent *e);
private:
    QComboBox *type;
    QDateEdit *from;
    QDateEdit *stop;
    QVBoxLayout *boxLayout;

    QString sqlName;
    QTableView *view;
    QSqlTableModel *mView;

    QTableView *item;
    QSqlTableModel *mItem;

    QVariantMap tmpMap;
};

#endif // SQLRECORD_H
