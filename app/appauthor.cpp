/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       电枢测试仪主页
*******************************************************************************/
#include "appauthor.h"

AppAuthor::AppAuthor(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void AppAuthor::initUI()
{
    initSkin();
    initLayout();
    initTitleBar();
    initButtonBar();
}

void AppAuthor::initSkin()
{
    QString style = ".QFrame{border-image:url(:/logo_arm.png)}";
    this->setStyleSheet(style);
}

void AppAuthor::initLayout()
{
    QVBoxLayout *frameLayout = new QVBoxLayout;

    topLayout = new QHBoxLayout;
    frameLayout->addLayout(topLayout);
    frameLayout->addStretch();

    btnLayout = new QHBoxLayout;
    btnLayout->setMargin(0);
    btnLayout->setSpacing(5);
    frameLayout->addLayout(btnLayout);
    frameLayout->addStretch();

    frameLayout->setStretch(1, 1);
    frameLayout->setContentsMargins(0, 9, 0, 30);

    QFrame *frame = new QFrame(this);
    frame->setLayout(frameLayout);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(frame);
    layout->setMargin(0);
    layout->setSpacing(0);

    this->setLayout(layout);
}

void AppAuthor::initTitleBar()
{
    version = new QLabel(this);
    topLayout->addStretch();
    topLayout->addWidget(version);
}

void AppAuthor::initButtonBar()
{
    btnLayout->addStretch();

    QPushButton *btnSystem = new QPushButton(this);
    btnSystem->setText("系统设置");
    btnSystem->setMinimumHeight(65);
    btnSystem->setObjectName("system");
    connect(btnSystem, SIGNAL(clicked(bool)), this, SLOT(clickButton()));
    btnLayout->addWidget(btnSystem);

    QPushButton *btnConfig = new QPushButton(this);
    btnConfig->setText("型号管理");
    btnConfig->setMinimumHeight(65);
    btnConfig->setObjectName("config");
    connect(btnConfig, SIGNAL(clicked(bool)), this, SLOT(clickButton()));
    btnLayout->addWidget(btnConfig);

    QPushButton *btnRecord = new QPushButton(this);
    btnRecord->setText("数据管理");
    btnRecord->setMinimumHeight(65);
    btnRecord->setObjectName("record");
    connect(btnRecord, SIGNAL(clicked(bool)), this, SLOT(clickButton()));
    btnLayout->addWidget(btnRecord);

    QPushButton *btnTester = new QPushButton(this);
    btnTester->setText("进入测试");
    btnTester->setMinimumHeight(65);
    btnTester->setObjectName("tester");
    connect(btnTester, SIGNAL(clicked(bool)), this, SLOT(clickButton()));
    btnLayout->addWidget(btnTester);

    btnLayout->addStretch();
    for (int i=0; i < btnLayout->count(); i++)
        btnLayout->setStretch(i, 1);
}

void AppAuthor::clickButton()
{
    tmpMap.insert("enum", Qt::Key_Display);
    tmpMap.insert("text", QObject::sender()->objectName());
    emit sendAppMap(tmpMap);
    tmpMap.clear();
}

void AppAuthor::recvAppMap(QVariantMap msg)
{  // 版本信息存储在0x0000
    switch (msg.value("enum").toInt()) {
    case Qt::Key_Option:
        version->setText(msg["0"].toString());
        break;
    default:
        break;
    }
}
