/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       数据导出
*******************************************************************************/
#ifndef SQLEXPORT_H
#define SQLEXPORT_H

#include <QFile>
#include <QTimer>
#include <QDebug>
#include <QWidget>
#include <QObject>
#include <QProcess>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QElapsedTimer>
#include <QSqlQueryModel>
#include <QTextCodec>
#include <QSqlTableModel>
#include <QMessageBox>

#ifdef __arm__
#define LENTH 10000
#else
#define LENTH 10000
#endif


class SqlExport : public QObject
{
    Q_OBJECT
public:
    explicit SqlExport(QObject *parent = 0);

signals:
    void sendSqlMap(QVariantMap msg);
private slots:
    QByteArray ToGbk(const QString &inStr);
    void recvSqlMap(QVariantMap msg);
    int checkUdisk();
    int exportFile();
    int createFile();
    int selectNumb();
    int selectItem();
    int exportHead();
    int exportData();
    int removeUdisk();
    void showText(QString err);
private:
    QSqlDatabase db;
    int timeOut;
    int quan;
    QString fileName;
    QList<int> numbs;
    QFile *file;
    QVariantMap config;
    QVariantMap tmpMap;
};

#endif // SQLEXPORT_H
