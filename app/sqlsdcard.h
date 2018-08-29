/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       历史数据
*******************************************************************************/
#ifndef SQLSDCARD_H
#define SQLSDCARD_H

#include <QLabel>
#include <QTimer>
#include <QDebug>
#include <QWidget>
#include <QLayout>
#include <QProcess>
#include <QGroupBox>
#include <QDateEdit>
#include <QShowEvent>
#include <QTableView>
#include <QHeaderView>
#include <QPushButton>
#include <QElapsedTimer>
#include <QFileSystemModel>
#include <QMessageBox>

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlTableModel>

#include "main.h"


class SqlSdcard : public QWidget
{
    Q_OBJECT
public:
    explicit SqlSdcard(QWidget *parent = 0);
signals:
    void sendAppMsg(QTmpMap msg);
private slots:
    void initUI();
    void initLayout();
    void initViewBar();
    void initButtonBar();
    void recvExport();
    void recvDelete();
private:
    QTableView *view;
    QFileSystemModel *mView;
    QVBoxLayout *boxLayout;

    QDateEdit *from;
    QDateEdit *stop;
};

#endif // SQLSDCARD_H
