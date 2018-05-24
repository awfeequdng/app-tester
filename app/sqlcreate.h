/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       数据初始化
*******************************************************************************/
#ifndef SQLCREATE_H
#define SQLCREATE_H

#include <QDir>
#include <QDebug>
#include <QObject>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlError>
#include <QStringList>
#include <QSqlDatabase>

#include "appdefine.h"

#include "appmaster.h"
#include "appaction.h"
#include "typconfig.h"


#define STEP 0x0010

class SqlCreate : public QObject
{
    Q_OBJECT
public:
    explicit SqlCreate(QObject *parent = 0);
signals:
    void sendAppMsg(QTmpMap msg);
public slots:
    void initSqlDir();
public slots:
    void initTmpDat();
    void openRecord(bool isExist);
    void openSystem(bool isExist);
    void openConfig(bool isExist);
    void initMisc(QSqlQuery query);
    void initBack(QSqlQuery query);
    void initSyst(QSqlQuery query);
    void initInfo(QSqlQuery query);
    void initINRB(QSqlQuery query);
    void initACWB(QSqlQuery query);
    void initIMPB(QSqlQuery query);
    void initLoad(QSqlQuery query);
    void initShow(QSqlQuery query);
    void initUser(QSqlQuery query);
    void initType(QSqlQuery query);
    void initConf(QSqlQuery query);
private:
    QTmpMap tmpSet;
};

#endif // SQLCREATE_H
