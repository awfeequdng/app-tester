/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       介电强度后台配置界面
*******************************************************************************/
#include "typoffacw.h"

TypOffAcw::TypOffAcw(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void TypOffAcw::initUI()
{
    initLayout();
    initBoxINR();
}

void TypOffAcw::initLayout()
{
    inr = new QGroupBox(this);
    inr->setTitle(tr("耐压调试"));

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(inr);
}

void TypOffAcw::initBoxINR()
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addStretch();
    inr->setLayout(layout);
    QStringList title;
    title << tr("测试1") << tr("测试2") << tr("实测1") << tr("实测2") << tr("计算K") << tr("计算b");
    QHBoxLayout *tlay = new QHBoxLayout;
    layout->addLayout(tlay);
    for (int i=0; i < title.size(); i++) {
        tlay->addWidget(new QLabel(title.at(i), this));
    }
    QStringList names;
    names << "500V" << "1000V" << "1M" << "5M" << "5M" << "50M" << "50M" << "500M";
    names << "500V" << "4500V" << "3mA" << "15mA" << "0mA" << "3mA";
    for (int i=0; i < names.size()/2; i++) {
        QHBoxLayout *tlay = new QHBoxLayout;
        layout->addLayout(tlay);
        QPushButton *btnA = new QPushButton(this);
        btnA->setFixedSize(97, 40);
        btnA->setText(names.at(i*2 + 0));
        tlay->addWidget(btnA);
        QPushButton *btnB = new QPushButton(this);
        btnB->setFixedSize(97, 40);
        btnB->setText(names.at(i*2 + 1));
        tlay->addWidget(btnB);
        for (int j=0; j < 4; j++) {
            QLineEdit *txt1 = new QLineEdit(this);
            txt1->setAlignment(Qt::AlignCenter);
            txt1->setFixedHeight(35);
            tlay->addWidget(txt1);
            inrboxs.append(txt1);
        }
    }
    layout->addStretch();

    QHBoxLayout *btn = new QHBoxLayout;
    layout->addLayout(btn);
    btn->addStretch();

    QPushButton *zero = new QPushButton(this);
    zero->setFixedSize(97, 40);
    zero->setText(tr("清零"));
    btn->addWidget(zero);
    connect(zero, SIGNAL(clicked(bool)), this, SLOT(zero()));

    QPushButton *calc = new QPushButton(this);
    calc->setFixedSize(97, 40);
    calc->setText(tr("计算"));
    btn->addWidget(calc);
    connect(calc, SIGNAL(clicked(bool)), this, SLOT(calc()));

    QPushButton *read = new QPushButton(this);
    read->setFixedSize(97, 40);
    read->setText(tr("读取"));
    btn->addWidget(read);
    connect(read, SIGNAL(clicked(bool)), this, SLOT(read()));

    QPushButton *send = new QPushButton(this);
    send->setFixedSize(97, 40);
    send->setText(tr("下发"));
    btn->addWidget(send);
    connect(send, SIGNAL(clicked(bool)), this, SLOT(send()));
}

void TypOffAcw::initSettings()
{
    int addr = 0;
    addr = tmpSet.value(1000 + Qt::Key_4).toInt();  // 耐压后台地址
    for (int i=0; i < inrboxs.size(); i++) {
        inrboxs.at(i)->setText(tmpSet.value(addr + i).toString());
    }
}

void TypOffAcw::zero()
{
    for (int i=0; i < inrboxs.size(); i++) {
        if (i%4 == 2) {
            inrboxs.at(i)->setText("1024");
        }
        if (i%4 == 3) {
            inrboxs.at(i)->setText("500");
        }
    }
}

void TypOffAcw::calc()
{
    int k = 0;
    int b = 0;
    for (int i=0; i < inrboxs.size()/4; i++) {
        int a = inrboxs.at(i*4 + 0)->text().toInt();
        int c = inrboxs.at(i*4 + 1)->text().toInt();
        switch (i) {
        case 0:
            k = (c-a)*1024/500;       //  k = (bn-md)/(bc-ad);  // (U2-U1)*1024/500;
            b = a+500-500*k/1024;     //  b = (mc-an)/(bc-ad);  // U1+500-500*k/1024
            break;
        case 1:
            k = 400*1024/((c-a));
            b = 600-(a)*k/1024;
            break;
        case 2:
            k = 450*1024/((c-a));
            b = 550-(a)*k/1024;
            break;
        case 3:
            k = 450*1024/((c-a));
            b = 550-(a)*k/1024;
            break;
        case 4:
            k = (c-a)*1024/4000;       //  k = (bn-md)/(bc-ad);  // (U2-U1)*1024/500;
            b = a+500-500*k/1024;     //  b = (mc-an)/(bc-ad);  // U1+500-500*k/1024
            break;
        case 5:
            k = (c-a)*1024/(1500-300);
            b = 500+a-k*300/1024;
            break;
        case 6:
            k = (c-a)*1024/(300-10);
            b = 500+a-k*10/1024;
            break;
        default:
            break;
        }
        inrboxs.at(i*4 + 2)->setText(QString::number(k));
        inrboxs.at(i*4 + 3)->setText(QString::number(b));
    }
}

void TypOffAcw::read()
{
    tmpMsg.insert(Qt::Key_0, Qt::Key_Send);
    tmpMsg.insert(Qt::Key_1, Qt::Key_View);  // 后台参数
    tmpMsg.insert(Qt::Key_2, Qt::Key_4);
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void TypOffAcw::send()
{
    for (int i=0; i < inrboxs.size()/4; i++) {
        QByteArray hex = QByteArray::fromHex("060000000000");
        hex[1] = i;
        hex[2] = inrboxs.at(i*4 + 2)->text().toInt() / 256;
        hex[3] = inrboxs.at(i*4 + 2)->text().toInt() % 256;
        hex[4] = inrboxs.at(i*4 + 3)->text().toInt() / 256;
        hex[5] = inrboxs.at(i*4 + 3)->text().toInt() % 256;
        tmpMsg.insert(Qt::Key_0, Qt::Key_Send);
        tmpMsg.insert(Qt::Key_1, Qt::Key_Save);  // 后台参数
        tmpMsg.insert(Qt::Key_2, Qt::Key_4);
        tmpMsg.insert(Qt::Key_3, hex.toHex());
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
    }
}

void TypOffAcw::recvNewMsg(QTmpMap msg)
{
    QByteArray hex = QByteArray::fromHex(msg.value(Qt::Key_1).toByteArray());
    if (hex.size() < 6)
        return;
    int n = quint8(hex.at(1));
    int k = quint8(hex.at(2))*256 + quint8(hex.at(3));
    int b = quint8(hex.at(4))*256 + quint8(hex.at(5));
    if (n < inrboxs.size()/4) {
        inrboxs.at(n*4 + 2)->setText(QString::number(k));
        inrboxs.at(n*4 + 3)->setText(QString::number(b));
    }
}

void TypOffAcw::recvAppMsg(QTmpMap msg)
{
    switch (msg.value(Qt::Key_0).toInt()) {
    case Qt::Key_Copy:
        tmpSet = msg;
        break;
    case Qt::Key_News:
        if (this->isHidden())
            break;
        recvNewMsg(msg);
        break;
    default:
        break;
    }
}

void TypOffAcw::showEvent(QShowEvent *e)
{
    this->setFocus();
    initSettings();
    e->accept();
}

