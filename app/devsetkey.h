#ifndef DEVSETKEY_H
#define DEVSETKEY_H

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


class DevSetKey : public QObject
{
    Q_OBJECT
public:
    explicit DevSetKey(QObject *parent = 0);
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

#endif // DEVSETKEY_H
