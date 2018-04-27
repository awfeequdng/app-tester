/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       系统设置
*******************************************************************************/
#include "appsystem.h"

AppSystem::AppSystem(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void AppSystem::initUI()
{
    initLayout();
    initSystem();
    initDevice();
    initDelegate();
}

void AppSystem::initLayout()
{
    layout = new QHBoxLayout(this);
}

void AppSystem::initSystem()
{
    QGroupBox *group = new QGroupBox(this);
    group->setTitle(tr("系统设置"));
    layout->addWidget(group);

    QVBoxLayout *boxLayout = new QVBoxLayout;
    group->setLayout(boxLayout);

    QStringList names;
    names << tr("语言设置") << tr("测试模式") << tr("启动方式") << tr("亮度设定")
          << tr("音量设定") << tr("条码起始") << tr("条码长度") << tr("合格报警")
          << tr("报警提示");
    for (int i=0; i < names.size(); i++) {
        QHBoxLayout *box = new QHBoxLayout;
        boxLayout->addLayout(box);
        QString tmp = QString("%1").arg(i+1, 2, 10, QChar('0'));
        box->addWidget(new QLabel(tmp + " " + names.at(i) + ":", this));
        QComboBox *input = new QComboBox(this);
        input->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        input->setView(new QListView);
        input->setFixedHeight(35);
        input->setEditable(true);
        box->addWidget(input);
        texts.append(input);
    }
    boxLayout->addStretch();
}

void AppSystem::initDevice()
{
    QGroupBox *group = new QGroupBox(this);
    group->setTitle(tr("本机设置"));
    layout->addWidget(group);

    QVBoxLayout *boxLayout = new QVBoxLayout;
    group->setLayout(boxLayout);

    QStringList names;
    names << tr("自动地址") << tr("默认网关") << tr("网络地址") << tr("子网掩码")
          << tr("挂载地址") << tr("挂载路径") << tr("自动执行") << tr("执行参数");

    for (int i=0; i < names.size(); i++) {
        QHBoxLayout *box = new QHBoxLayout;
        boxLayout->addLayout(box);
        QString tmp = QString("%1").arg(i+1, 2, 10, QChar('0'));
        box->addWidget(new QLabel(tmp + " " + names.at(i) + ":", this));
        QLineEdit *input = new QLineEdit(this);
        input->setFixedHeight(35);
        box->addWidget(input);
        ctrls.append(input);
    }
    boxLayout->addStretch();
    QHBoxLayout *btn = new QHBoxLayout;
    QPushButton *btnSave = new QPushButton(this);
    btnSave->setText(tr("保存"));
    btnSave->setFixedSize(97, 44);
    btn->addStretch();
    btn->addWidget(btnSave);
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
    boxLayout->addLayout(btn);
}

void AppSystem::initDelegate()
{
    QStringList tmp0;
    tmp0 << tr("中文");
    texts.at(0)->addItems(tmp0);

    QStringList tmp1;
    tmp1 << tr("常规模式");
    texts.at(1)->addItems(tmp1);

    QStringList tmp2;
    tmp2 << tr("按钮启动") << tr("滑罩启动") << tr("脚踏启动") << tr("网络启动");
    texts.at(2)->addItems(tmp2);

    QStringList tmp3;
    tmp3 << "0" << "1" << "2" << "3" << "4" << "5";
    texts.at(3)->addItems(tmp3);

    QStringList tmp4;
    tmp4 << "0" << "1" << "2" << "3" << "4" << "5";
    texts.at(4)->addItems(tmp4);
}

void AppSystem::initSettings()
{
    for (int i=0; i < texts.size(); i++) {  // 系统配置存放在0x0020
        if (i < 5) {
            texts.at(i)->setCurrentIndex(config[QString::number(i+0x0020)].toInt());
        } else {
            texts.at(i)->lineEdit()->setText(config[QString::number(i+0x0020)].toString());
        }
    }
    for (int i=0; i < ctrls.size(); i++) {  // 本机设置存放在0x0030
        ctrls.at(i)->setText(config[QString::number(i+0x0030)].toString());
    }
}

void AppSystem::saveSettings()
{
    for (int i=0; i < texts.size(); i++) {  // 系统配置存放在0x0020
        if (i < 5) {
            config[QString::number(i+0x0020)] = QString::number(texts.at(i)->currentIndex());
        } else {
            config[QString::number(i+0x0020)] = texts.at(i)->currentText();
        }
    }
    for (int i=0; i < ctrls.size(); i++) {  // 本机设置存放在0x0030
        config[QString::number(i+0x0030)] = ctrls.at(i)->text();
    }
    config.insert("enum", Qt::Key_Save);
    emit sendAppMap(config);
}

void AppSystem::recvAppMap(QVariantMap msg)
{
    switch (msg.value("enum").toInt()) {
    case Qt::Key_Option:
        for (int i=0x0020; i < 0x40; i++) {  // 后台信息存放在0x0020
            config[QString::number(i)] = msg[QString::number(i)];
        }
        break;
    default:
        break;
    }
}

void AppSystem::showEvent(QShowEvent *e)
{
    this->setFocus();
    initSettings();
    e->accept();
}

