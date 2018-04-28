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
}

void AppBackup::initUI()
{
    initLayout();
    initBoxText();
}

void AppBackup::initLayout()
{
    boxLayout = new QHBoxLayout;

    QGroupBox *group = new QGroupBox(this);
    group->setTitle(tr("后台配置"));
    group->setLayout(boxLayout);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(group);
}

void AppBackup::initBoxText()
{
    QVBoxLayout *layout = new QVBoxLayout;
    boxLayout->addLayout(layout);

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
        texts.append(input);
    }
    layout->addStretch();

    QHBoxLayout *btn = new QHBoxLayout;
    QPushButton *save = new QPushButton(this);
    save->setText(tr("保存"));
    save->setFixedSize(97, 44);
    btn->addStretch();
    btn->addWidget(save);
    connect(save, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
    layout->addLayout(btn);
}

void AppBackup::initSettings()
{
    for (int i=0; i < texts.size(); i++) {  // 后台信息存放在0x0010
        texts.at(i)->setText(config[QString::number(i+AddrBackup)].toString());
    }
}

void AppBackup::saveSettings()
{
    for (int i=0; i < texts.size(); i++) {
        config[QString::number(i+AddrBackup)] = texts.at(i)->text();
    }
    config.insert("enum", Qt::Key_Save);
    emit sendAppMap(config);
}

void AppBackup::recvAppMap(QVariantMap msg)
{
    switch (msg.value("enum").toInt()) {
    case Qt::Key_Option:
        for (int i=AddrBackup; i < AddrBackup + 0x10; i++) {  // 后台信息存放在0x0010
            config[QString::number(i)] = msg[QString::number(i)];
        }
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

