/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       数据导入
*******************************************************************************/
#ifndef SQLIMPORT_H
#define SQLIMPORT_H

#include <QObject>
#include <QSqlQuery>
#include <QSqlError>

#include "sqlsnowid.h"
#include "appdefine.h"

class SqlImport : public QObject
{
    Q_OBJECT
public:
    explicit SqlImport(QObject *parent = 0);

signals:

public slots:
    void recvAppDat(QTmpMap msg);
    void recvAppMsg(QTmpMap msg);

};

#endif // SQLIMPORT_H
