#ifndef DEVSYSTEM_H
#define DEVSYSTEM_H

#include <QWidget>

const int AddrCore = 100;  // 内核版本
const int AddrDisk = 101;  // 本机flash
const int AddrNand = 102;  // 本机nand
const int AddrUsb1 = 103;  // 本机usb1
const int AddrUsb2 = 104;  // 本机usb2
const int AddrCard = 105;  // 本机SD卡
const int AddrNet1 = 106;  // 本机网络1
const int AddrMac1 = 107;  // 本机网卡1
const int AddrNet2 = 108;  // 本机网络2
const int addrMac2 = 109;  // 本机网卡2

class DevSystem : public QWidget
{
    Q_OBJECT
public:
    explicit DevSystem(QWidget *parent = 0);

signals:

public slots:
};

#endif // DEVSYSTEM_H
