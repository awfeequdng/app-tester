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

#include "appdefine.h"

class DevSerial : public QObject
{
    Q_OBJECT
public:
    explicit DevSerial(QObject *parent = 0);
signals:
    void sendAppMsg(QTmpMap msg);
private slots:
    void initParam();
    void initDevPort();
    void readDevPort();
    void clickButton(int id);
    void recvAppMsg(QTmpMap msg);
private:
    bool upper;
    QSerialPort *com;
    QByteArray tmpByte;
    QTmpMap tmpMsg;
    QMap<QString, int> keys;
};

#endif // DEVSERIAL_H
