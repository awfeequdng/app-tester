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
    initBoxACW();
}

void TypOffAcw::initLayout()
{
    inr = new QGroupBox(this);
    inr->setTitle(tr("绝缘调试"));

    acw = new QGroupBox(this);
    acw->setTitle(tr("交耐调试"));

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(inr);
    layout->addWidget(acw);
}

void TypOffAcw::initBoxINR()
{
    QVBoxLayout *layout = new QVBoxLayout;
    inr->setLayout(layout);
    QStringList names;
    names << "500V" << "1000V" << "1M" << "5M" << "5M" << "50M" << "50M" << "500M";
    for (int i=0; i < names.size()/2; i++) {
        QHBoxLayout *btn = new QHBoxLayout;
        QPushButton *btn1 = new QPushButton(this);
        btn1->setFixedHeight(40);
        btn1->setText(names.at(i*2 + 0));
        btn->addWidget(btn1);
        QPushButton *btn2 = new QPushButton(this);
        btn2->setFixedHeight(40);
        btn2->setText(names.at(i*2 + 1));
        btn->addWidget(btn2);
        layout->addLayout(btn);

        QGridLayout *real = new QGridLayout;
        layout->addLayout(real);
        QLineEdit *txt1 = new QLineEdit(this);
        QLineEdit *txt2 = new QLineEdit(this);
        QLineEdit *txt3 = new QLineEdit(this);
        QLineEdit *txt4 = new QLineEdit(this);
        real->addWidget(new QLabel(tr("实测1")), 0 , 0);
        real->addWidget(txt1, 0, 1);
        real->addWidget(new QLabel(tr("实测2")), 0 , 2);
        real->addWidget(txt2, 0, 3);
        real->addWidget(new QLabel(tr("计算K")), 1 , 0);
        real->addWidget(txt3, 1, 1);
        real->addWidget(new QLabel(tr("计算B")), 1 , 2);
        real->addWidget(txt4, 1, 3);
        inrboxs.append(txt1);
        inrboxs.append(txt2);
        inrboxs.append(txt3);
        inrboxs.append(txt4);
    }
    layout->addStretch();
    QHBoxLayout *btn = new QHBoxLayout;
    layout->addLayout(btn);

    QPushButton *zero = new QPushButton(this);
    zero->setFixedHeight(40);
    zero->setText(tr("清零"));
    btn->addWidget(zero);
    connect(zero, SIGNAL(clicked(bool)), this, SLOT(zeroINR()));

    QPushButton *calc = new QPushButton(this);
    calc->setFixedHeight(40);
    calc->setText(tr("计算"));
    btn->addWidget(calc);
    connect(calc, SIGNAL(clicked(bool)), this, SLOT(calcINR()));

    QPushButton *read = new QPushButton(this);
    read->setFixedHeight(40);
    read->setText(tr("读取"));
    btn->addWidget(read);

    QPushButton *send = new QPushButton(this);
    send->setFixedHeight(40);
    send->setText(tr("下发"));
    btn->addWidget(send);
}

void TypOffAcw::initBoxACW()
{
    QVBoxLayout *layout = new QVBoxLayout;
    acw->setLayout(layout);

    QStringList names;
    names << "500V" << "4500V" << "3mA" << "15mA" << "0mA" << "3mA";
    for (int i=0; i < names.size()/2; i++) {
        QHBoxLayout *btn = new QHBoxLayout;
        QPushButton *btn1 = new QPushButton(this);
        btn1->setFixedHeight(40);
        btn1->setText(names.at(i*2 + 0));
        btn->addWidget(btn1);
        QPushButton *btn2 = new QPushButton(this);
        btn2->setFixedHeight(40);
        btn2->setText(names.at(i*2 + 1));
        btn->addWidget(btn2);
        layout->addLayout(btn);

        QGridLayout *real = new QGridLayout;
        layout->addLayout(real);
        QLineEdit *txt1 = new QLineEdit(this);
        QLineEdit *txt2 = new QLineEdit(this);
        QLineEdit *txt3 = new QLineEdit(this);
        QLineEdit *txt4 = new QLineEdit(this);
        real->addWidget(new QLabel(tr("实测1")), 0 , 0);
        real->addWidget(txt1, 0, 1);
        real->addWidget(new QLabel(tr("实测2")), 0 , 2);
        real->addWidget(txt2, 0, 3);
        real->addWidget(new QLabel(tr("计算K")), 1 , 0);
        real->addWidget(txt3, 1, 1);
        real->addWidget(new QLabel(tr("计算B")), 1 , 2);
        real->addWidget(txt4, 1, 3);
        acwboxs.append(txt1);
        acwboxs.append(txt2);
        acwboxs.append(txt3);
        acwboxs.append(txt4);
    }
    layout->addStretch();
    QHBoxLayout *btn = new QHBoxLayout;
    layout->addLayout(btn);

    QPushButton *zero = new QPushButton(this);
    zero->setFixedHeight(40);
    zero->setText(tr("清零"));
    btn->addWidget(zero);
    connect(zero, SIGNAL(clicked(bool)), this, SLOT(zeroACW()));

    QPushButton *calc = new QPushButton(this);
    calc->setFixedHeight(40);
    calc->setText(tr("计算"));
    btn->addWidget(calc);
    connect(calc, SIGNAL(clicked(bool)), this, SLOT(calcACW()));

    QPushButton *read = new QPushButton(this);
    read->setFixedHeight(40);
    read->setText(tr("读取"));
    btn->addWidget(read);

    QPushButton *send = new QPushButton(this);
    send->setFixedHeight(40);
    send->setText(tr("下发"));
    btn->addWidget(send);
}

void TypOffAcw::initSettings()
{
    int addr = 0;
    addr = tmpSet.value(AddrINRB).toInt();
    for (int i=0; i < inrboxs.size(); i++) {
        if (i%4 < 2) {
            inrboxs.at(i)->setText(tmpSet[addr + i/2 + i%4].toString());
        }
    }
    addr = tmpSet.value(AddrACWB).toInt();
    for (int i=0; i < acwboxs.size(); i++) {
        if (i%4 < 2) {
            acwboxs.at(i)->setText(tmpSet[addr + i/2 + i%4].toString());
        }
    }
}

void TypOffAcw::zeroINR()
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

void TypOffAcw::calcINR()
{
    int k = 0;
    int b = 0;
    for (int i=0; i < inrboxs.size()/4; i++) {
        int a = inrboxs.at(i*4 + 0)->text().toInt();
        int c = inrboxs.at(i*4 + 1)->text().toInt();
        if (i == 0) {
            k = (c-a)*1024/500;       //  k = (bn-md)/(bc-ad);  // (U2-U1)*1024/500;
            b = a+500-500*k/1024;     //  b = (mc-an)/(bc-ad);  // U1+500-500*k/1024
        }
        if (i == 1) {
            k = 400*1024/((c-a));
            b = 600-(a)*k/1024;
        }
        if (i == 2) {
            k = 450*1024/((c-a));
            b = 550-(a)*k/1024;
        }
        if (i == 3) {
            k = 450*1024/((c-a));
            b = 550-(a)*k/1024;
        }
        inrboxs.at(i*4 + 2)->setText(QString::number(k));
        inrboxs.at(i*4 + 3)->setText(QString::number(b));
    }
}

void TypOffAcw::zeroACW()
{
    for (int i=0; i < acwboxs.size(); i++) {
        if (i%4 == 2) {
            acwboxs.at(i)->setText("1024");
        }
        if (i%4 == 3) {
            acwboxs.at(i)->setText("500");
        }
    }
}

void TypOffAcw::calcACW()
{
    int k = 0;
    int b = 0;
    for (int i=0; i < acwboxs.size()/4; i++) {
        int a = acwboxs.at(i*4 + 0)->text().toInt();
        int c = acwboxs.at(i*4 + 1)->text().toInt();
        if (i == 0) {
            k = (c-a)*1024/4000;       //  k = (bn-md)/(bc-ad);  // (U2-U1)*1024/500;
            b = a+500-500*k/1024;     //  b = (mc-an)/(bc-ad);  // U1+500-500*k/1024
        }
        if (i == 1) {
            k = (c-a)*1024/(1500-300);
            b = 500+a-k*300/1024;
        }
        if (i == 2) {
            k = (c-a)*1024/(300-10);
            b = 500+a-k*10/1024;
        }
        acwboxs.at(i*4 + 2)->setText(QString::number(k));
        acwboxs.at(i*4 + 3)->setText(QString::number(b));
    }
}

void TypOffAcw::recvAppMsg(QTmpMap msg)
{
    int c = msg.value(AddrEnum).toInt();
    switch (c) {
    case Qt::Key_Copy:
        tmpSet = msg;
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

