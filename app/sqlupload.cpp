/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       数据上传
*******************************************************************************/
#include "sqlupload.h"

SqlUpload::SqlUpload(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void SqlUpload::initUI()
{
    initLayout();
    initBoxText();
    initBoxCtrl();
}

void SqlUpload::initLayout()
{
    layout = new QHBoxLayout(this);
}

void SqlUpload::initBoxText()
{
    QGroupBox *group = new QGroupBox(this);
    group->setTitle(tr("上传配置"));
    layout->addWidget(group);

    QVBoxLayout *boxLayout = new QVBoxLayout;
    group->setLayout(boxLayout);

    QStringList names;
    names << tr("上传模式") << tr("网络地址") << tr("登录用户")<< tr("登录密码")
          << tr("数据库名") << tr("网络端口");

    for (int i=0; i < names.size(); i++) {
        QHBoxLayout *box = new QHBoxLayout;
        boxLayout->addLayout(box);
        box->addWidget(new QLabel(names.at(i), this));
        QLineEdit *input = new QLineEdit(this);
        input->setFixedHeight(35);
        box->addWidget(input);
        texts.append(input);
        if (i == 3) {
            input->setEchoMode(QLineEdit::Password);
        }
    }
    boxLayout->addStretch();
}

void SqlUpload::initBoxCtrl()
{
    QGroupBox *group = new QGroupBox(this);
    group->setTitle(tr("备用配置"));
    layout->addWidget(group);

    QVBoxLayout *boxLayout = new QVBoxLayout;
    group->setLayout(boxLayout);

    QStringList names;
    names << tr("上传模式") << tr("网络地址") << tr("登录用户")<< tr("登录密码")
          << tr("数据库名") << tr("网络端口");

    for (int i=0; i < names.size(); i++) {
        QHBoxLayout *box = new QHBoxLayout;
        boxLayout->addLayout(box);
        box->addWidget(new QLabel(names.at(i), this));
        QLineEdit *input = new QLineEdit(this);
        input->setFixedHeight(35);
        box->addWidget(input);
        ctrls.append(input);
        if (i == 3) {
            input->setEchoMode(QLineEdit::Password);
        }
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

void SqlUpload::initSettings()
{
    for (int i=0; i < texts.size(); i++) {  // 上传配置存放在0x0120~0x0127
        texts.at(i)->setText(config[QString::number(i+0x00E0)].toString());
    }
    for (int i=0; i < ctrls.size(); i++) {  // 备用配置存放在0x0128~0x012F
        ctrls.at(i)->setText(config[QString::number(i+0x00E0+8)].toString());
    }
}

void SqlUpload::saveSettings()
{
    for (int i=0; i < texts.size(); i++) {  // 上传配置存放在0x0120~0x0127
        config[QString::number(i+0x00E0)] = texts.at(i)->text();
    }
    for (int i=0; i < ctrls.size(); i++) {  // 备用配置存放在0x0128~0x012F
        config[QString::number(i+0x00E0+8)] = ctrls.at(i)->text();
    }
//    config.insert("enum", Qt::Key_Save);
//    config.insert(AddrText, "aip_system");
//    emit sendAppMap(config);
}

void SqlUpload::recvAppMap(QVariantMap msg)
{
    switch (msg.value("enum").toInt()) {
    case Qt::Key_Copy:
        for (int i=0; i < 0x10; i++) {  // 上传配置存放在0x0120~0x012F
            config[QString::number(i+0x00E0)] = msg[QString::number(i+0x00E0)];
        }
        break;
    default:
        break;
    }
}

void SqlUpload::showEvent(QShowEvent *e)
{
    this->setFocus();
    initSettings();
    e->accept();
}


