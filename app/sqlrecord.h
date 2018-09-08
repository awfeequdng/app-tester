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
#include <QProcess>
#include <QListView>
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
#include <QSqlError>

#include "main.h"
#include "boxqlabel.h"

class SqlRecord : public QWidget
{
    Q_OBJECT
public:
    explicit SqlRecord(QWidget *parent = 0);
signals:
    void sendAppMsg(QTmpMap msg);
private slots:
    void initUI();
    void initLayout();
    void initViewBar();
    void initTextBar();
    void initButtonBar();
    void initSettings();
    void existsFlashDisk();
    void deleteFlashDisk();
    void recvSelect();
    void recvExport();
    void recvAppMsg(QTmpMap msg);
    virtual void showEvent(QShowEvent *e);
private:
    QVBoxLayout *boxLayout;
    BoxQLabel *view;
    QComboBox *type;
    QDateEdit *from;
    QDateEdit *stop;
    QLabel *text;
    QTmpMap tmpSet;
    QTmpMap tmpQuan;
    QTmpMap tmpOKNG;
    QString path;
    QTmpMap tmpMsg;
    QVariantMap tmpTyp;
};

#endif // SQLRECORD_H
