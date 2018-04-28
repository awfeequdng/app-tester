/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:
*******************************************************************************/
#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QDir>
#include <QFile>
#include <QTimer>
#include <QObject>
#include <QProcess>
#include <QTcpSocket>
#include <QDataStream>
#include <QNetworkInterface>
#include <QCryptographicHash>
#include <QNetworkAccessManager>
#include <QFileInfo>
#include <QDateTime>

#define SIGN_IN     1000  // 登录
#define SIGN_OK     1001  // 登录返回
#define READ_HEAD   1002  // 读取文件
#define READ_FAIL   1003  // 读取失败
#define SEND_HEAD   1004  // 发送文件
#define SEND_FAIL   1005  // 发送失败

#define FILE_HEAD   2000  // 文件头
#define FILE_DATA   2001  // 文件内容
#define FILE_OVER   2002  // 文件发送成功
#define FILE_FAIL   2003  // 文件发送失败
#define SHELL_CMD   2004  // 发送系统命令
#define SHELL_DAT   2005  // 系统命令返回
#define HEAD_FAIL   2009  // 文件接收失败

#define SHOW        3000
#define ADDR        6000
#define BEAT        6000  // 心跳

class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpSocket(QObject *parent = 0);
signals:
    void sendAppMap(QVariantMap msg);
public slots:
    void connectToServer(QVariantMap msg);
    void sendSocket(quint16 addr, quint16 cmd, QByteArray data);
    void sendFileHead(QByteArray data);
public:
    qint64 heart;
private slots:
    void readSocket(void);
    void recvFileHead(QByteArray msg);
    void recvFileData(QByteArray msg);
    void sendFileData(qint64 numBytes);
    void recvSocketCmd(quint16 addr, quint16 cmd, QByteArray data);
    void recvSocketErr(QAbstractSocket::SocketError);
    void recvSocketConnected();
    void sendSocketBeat();
    QString getHardwareAddress();
    void display(QString msg);
private:
    QFile *file;
    qint64 LoadSize;
    qint64 BlockSize;

    qint64 sendFile;
    qint64 sendByte;
    QByteArray PutFileName;

    qint64 recvByte;
    qint64 recvFile;
    QByteArray GetFileMD5;
    QByteArray fileName;

    QVariantMap sign;
    QTimer *timer;

    QProcess *proc;

    QVariantMap tmpMap;
};

#endif // TCPSOCKET_H
