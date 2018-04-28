/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       串口键盘设置
*******************************************************************************/
#include "devserial.h"

DevSerial::DevSerial(QObject *parent) : QObject(parent)
{
    initParam();
    initDevPort();
}

void DevSerial::initParam()
{
    upper = true;
    keys["k1\r\n"] = Qt::Key_Stop;
    keys["k2\r\n"] = Qt::Key_Play;
    keys["k3\r\n"] = Qt::Key_1;
    keys["k4\r\n"] = Qt::Key_2;
    keys["k5\r\n"] = Qt::Key_3;
    keys["k6\r\n"] = Qt::Key_Backspace;
    keys["k7\r\n"] = Qt::Key_4;
    keys["k8\r\n"] = Qt::Key_5;
    keys["k9\r\n"] = Qt::Key_6;
    keys["k10\r\n"] = Qt::Key_Escape;
    keys["k11\r\n"] = Qt::Key_7;
    keys["k12\r\n"] = Qt::Key_8;
    keys["k13\r\n"] = Qt::Key_9;
    keys["k14\r\n"] = Qt::Key_Enter;
    keys["k15\r\n"] = Qt::Key_0;
    keys["k16\r\n"] = Qt::Key_Period;
    keys["k17\r\n"] = Qt::Key_NumLock;
    keys["k18\r\n"] = Qt::Key_Help;
    keys["k19\r\n"] = Qt::Key_Left;
    keys["k20\r\n"] = Qt::Key_Right;
    keys["k21\r\n"] = Qt::Key_Refresh;
    keys["k22\r\n"] = Qt::Key_Down;
    keys["k23\r\n"] = Qt::Key_Up;
    keys["k24\r\n"] = Qt::Key_F3;
    keys["k25\r\n"] = Qt::Key_F4;
    keys["k26\r\n"] = Qt::Key_F1;
    keys["k27\r\n"] = Qt::Key_F2;
}

void DevSerial::initDevPort()
{
    com = new QSerialPort("ttyS3", this);
    if (com->open(QIODevice::ReadWrite)) {
        com->setBaudRate(115200);
        com->setDataBits(QSerialPort::Data8);
        com->setParity(QSerialPort::NoParity);
        com->setStopBits(QSerialPort::OneStop);
        com->setFlowControl(QSerialPort::NoFlowControl);
        com->setDataTerminalReady(true);
        com->setRequestToSend(false);
    }
    connect(com, SIGNAL(readyRead()), this, SLOT(readDevPort()));
}

void DevSerial::readDevPort()
{
    tmpByte.append(com->readAll());
    if (tmpByte.startsWith("k") && tmpByte.endsWith("\r\n")) {
        clickButton(keys[tmpByte]);
        tmpByte.clear();
    }
}

void DevSerial::clickButton(int id)
{
    switch (id) {
    case Qt::Key_Stop:
    case Qt::Key_Play:
        tmpMap.insert("enum", id);
        emit sendAppMap(tmpMap);
        tmpMap.clear();
        break;
    case Qt::Key_Enter :
        break;
    case Qt::Key_CapsLock:
        upper = !upper;
        break;
    default:
#ifdef __arm__
        if (id >= Qt::Key_A && id <= Qt::Key_Z && !upper)
            id += 32;
        QWSServer::sendKeyEvent(id, id, Qt::NoModifier, true, false);
#endif
        break;
    }
}

void DevSerial::recvAppMap(QVariantMap msg)
{
    switch (msg.value("enum").toInt()) {
    case Qt::Key_View:
        if (!msg.value("text").isNull())
            com->write(msg.value("text").toByteArray());
        break;
    default:
        break;
    }
}

