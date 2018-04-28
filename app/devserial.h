/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       串口键盘设置
*******************************************************************************/
#ifndef DEVSERIAL_H
#define DEVSERIAL_H

#include <QDebug>
#include <QObject>
#include <QVariant>
#include <QMessageBox>
#include <QApplication>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#ifdef __arm__
#include <QWSServer>
#endif

class DevSerial : public QObject
{
    Q_OBJECT
public:
    explicit DevSerial(QObject *parent = 0);
signals:
    void sendAppMap(QVariantMap msg);
private slots:
    void initParam();
    void initDevPort();
    void readDevPort();
    void clickButton(int id);
    void recvAppMap(QVariantMap msg);
private:
    bool upper;
    QSerialPort *com;
    QByteArray tmpByte;
    QVariantMap tmpMap;
    QMap<QString, int> keys;
};

#endif // DEVSERIAL_H
