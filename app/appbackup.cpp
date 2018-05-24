/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       后台管理
*******************************************************************************/
#include "appbackup.h"

AppBackup::AppBackup(QWidget *parent) : QWidget(parent)
{
    initUI();
    initNetworks();
}

void AppBackup::initUI()
{
    initLayout();
    initBoxText();
    initMacText();
}

void AppBackup::initLayout()
{
    boxGroup = new QGroupBox(this);
    boxGroup->setTitle(tr("后台配置"));

    macGroup = new QGroupBox(this);
    macGroup->setTitle(tr("本机信息"));

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(boxGroup);
    layout->addWidget(macGroup);
}

void AppBackup::initBoxText()
{
    QVBoxLayout *layout = new QVBoxLayout;
    boxGroup->setLayout(layout);

    QStringList names;
    names << tr("产品型号") << tr("产品编号") << tr("评审编号") << tr("出厂日期")
          << tr("语言支持") << tr("工位数量") << tr("最高电压") << tr("自动测试")
          << tr("测试延时") << tr("公司网址") << tr("网络端口");

    for (int i=0; i < names.size(); i++) {
        QHBoxLayout *box = new QHBoxLayout;
        layout->addLayout(box);
        QString tmp = QString("%1").arg(i+1, 2, 10, QChar('0'));
        box->addWidget(new QLabel(tmp + " " + names.at(i) + ":", this));
        QLineEdit *input = new QLineEdit(this);
        input->setFixedHeight(30);
        box->addWidget(input);
        txts.append(input);
    }
    layout->addStretch();

    QHBoxLayout *btn = new QHBoxLayout;
    layout->addLayout(btn);
    btn->addStretch();

    QPushButton *save = new QPushButton(this);
    save->setText(tr("保存"));
    save->setFixedSize(97, 44);
    btn->addWidget(save);
    connect(save, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));

}

void AppBackup::initMacText()
{
    QVBoxLayout *layout = new QVBoxLayout;
    macGroup->setLayout(layout);

    QStringList names;
    names << tr("硬件地址1") << tr("硬件地址2") << tr("硬件地址3")
          << tr("本机地址1") << tr("本机地址2") << tr("本机地址3")
          << tr("电阻板版本") << tr("高压板版本") << tr("匝间板版本");
    for (int i=0; i < names.size(); i++) {
        QHBoxLayout *box = new QHBoxLayout;
        layout->addLayout(box);
        QString tmp = QString("%1").arg(i+1, 2, 10, QChar('0'));
        box->addWidget(new QLabel(tmp + " " + names.at(i) + ":", this));
        QLineEdit *input = new QLineEdit(this);
        input->setFixedHeight(30);
        box->addWidget(input);
        nets.append(input);
    }
    layout->addStretch();

    names.clear();
    names << tr("硬盘") << "Nand" << tr("SD卡") << "usb1" << "usb2";

    QStringList headers;
    headers << tr("名称") << tr("容量") << tr("使用");

    view = new QTableWidget(this);
    view->setRowCount(names.size());
    view->setColumnCount(headers.size());
    view->setHorizontalHeaderLabels(headers);
    view->verticalHeader()->hide();
    for (int i=0; i < headers.size(); i++) {
        for (int j=0; j < names.size(); j++) {
            QTableWidgetItem *item = new QTableWidgetItem;
            if (i == 0)
                item->setText(names.at(j));
            else
                item->setText("");
            item->setTextAlignment(Qt::AlignCenter);
            view->setItem(j, i, item);
        }
    }
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    view->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
    view->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    view->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
    view->setColumnWidth(0, 70);
    view->setColumnWidth(2, 70);
    view->setEditTriggers(QAbstractItemView::AllEditTriggers);
    layout->addWidget(view);

    QHBoxLayout *btn = new QHBoxLayout;
    layout->addLayout(btn);
    btn->addStretch();

    QPushButton *recy = new QPushButton(this);
    recy->setText(tr("恢复出厂"));
    recy->setFixedHeight(44);
    btn->addWidget(recy);
    connect(recy, SIGNAL(clicked(bool)), this, SLOT(recovery()));
}

void AppBackup::initSettings()
{
    int r = tmpSet[AddrBack].toInt();
    for (int i=0; i < txts.size(); i++) {
        txts.at(i)->setText(tmpSet[r + i].toString());
    }
}

void AppBackup::saveSettings()
{
    int r = tmpSet[AddrBack].toInt();
    for (int i=0; i < txts.size(); i++) {
        tmpSet[r + i] = txts.at(i)->text();
    }
    tmpSet.insert(AddrEnum, Qt::Key_Save);
    emit sendAppMsg(tmpSet);
}

void AppBackup::initNetworks()
{
    int k = 0;
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    for (int i=0; i < list.size(); i++) {
        if (list[i].hardwareAddress().size() == 17 &&
                list[i].hardwareAddress() != "00:00:00:00:00:00") {
            nets.at(k)->setText(list.at(i).hardwareAddress());
            k++;
        }
    }
    int t = 3;
    QList<QHostAddress> hosts = QNetworkInterface::allAddresses();
    for (int i=0; i < hosts.size(); i++) {
        if (hosts.at(i).protocol() == QAbstractSocket::IPv4Protocol) {
            if (hosts.at(i).toString() != "127.0.0.1") {
                nets.at(t)->setText(hosts.at(i).toString());
                t++;
            }
        }
    }
    QProcess sys;
    sys.start("df -h");
    sys.waitForReadyRead();
    QByteArray msg = sys.readAll();

    QStringList s = QString(msg).split("\n");
    for (int i=0; i < s.size(); i++) {
        if (QString(s.at(i)).endsWith("/")) {
            QStringList t = QString(s.at(i)).split(" ", QString::SkipEmptyParts);
            if (t.size() >= 6) {
                view->item(0, 1)->setText(t.at(1));
                view->item(0, 2)->setText(t.at(4));
            }
        }
        if (QString(s.at(i)).endsWith("/mnt/nandflash")) {
            QStringList t = QString(s.at(i)).split(" ", QString::SkipEmptyParts);
            if (t.size() >= 6) {
                view->item(1, 1)->setText(t.at(1));
                view->item(1, 2)->setText(t.at(4));
            }
        }
        if (QString(s.at(i)).endsWith("/mnt/sdcard")) {
            QStringList t = QString(s.at(i)).split(" ", QString::SkipEmptyParts);
            if (t.size() >= 6) {
                view->item(2, 1)->setText(t.at(1));
                view->item(2, 2)->setText(t.at(4));
            }
        }
        if (QString(s.at(i)).endsWith("/mnt/usb1")) {
            QStringList t = QString(s.at(i)).split(" ", QString::SkipEmptyParts);
            if (t.size() >= 6) {
                view->item(3, 1)->setText(t.at(1));
                view->item(3, 2)->setText(t.at(4));
            }
        }
        if (QString(s.at(i)).endsWith("/mnt/usb2")) {
            QStringList t = QString(s.at(i)).split(" ", QString::SkipEmptyParts);
            if (t.size() >= 6) {
                view->item(4, 1)->setText(t.at(1));
                view->item(4, 2)->setText(t.at(4));
            }
        }
    }
    sys.close();
}

void AppBackup::recovery()
{
    qDebug() << "start recovery";
    QProcess sys;
    sys.start("rm /mnt/nandflash/system.db");
    sys.waitForFinished();
    sys.start("rm /mnt/nandflash/config.db");
    sys.waitForFinished();
    sys.start("rm /mnt/nandflash/record.db");
    sys.waitForFinished();
    sys.start("reboot");
    sys.waitForFinished();
}

void AppBackup::recvAppVer(QTmpMap msg)
{
    if (!msg.value(DataDCRV).isNull()) {
        QByteArray tmp = msg.value(DataDCRV).toByteArray();
        QString v;
        v += QString::number(quint8(tmp.at(6))/16);
        v += ".";
        v += QString::number(quint8(tmp.at(6))%16);
        nets.at(6)->setText(v);
    }
    if (!msg.value(DataACWV).isNull()) {
        QByteArray tmp = msg.value(DataACWV).toByteArray();
        QString v;
        v += QString::number(quint8(tmp.at(6))/16);
        v += ".";
        v += QString::number(quint8(tmp.at(6))%16);
        nets.at(7)->setText(v);
    }
    if (!msg.value(DataIMPV).isNull()) {
        QByteArray tmp = msg.value(DataIMPV).toByteArray();
        QString v;
        v += QString::number(quint8(tmp.at(6))/16);
        v += ".";
        v += QString::number(quint8(tmp.at(6))%16);
        nets.at(8)->setText(v);
    }

}

void AppBackup::recvAppMsg(QTmpMap msg)
{
    int c = msg.value(AddrEnum).toInt();
    switch (c) {
    case Qt::Key_Copy:
        tmpSet = msg;
        break;
    case Qt::Key_News:
        recvAppVer(msg);
        break;
    default:
        break;
    }
}

void AppBackup::showEvent(QShowEvent *e)
{
    this->setFocus();
    initSettings();
    e->accept();
}

