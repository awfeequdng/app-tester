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
    initButton();
}

void AppAuthor::initSkin()
{
    QString style = ".QFrame{border-image:url(:/logo_aip.bmp)}";
    this->setStyleSheet(style);
}

void AppAuthor::initLayout()
{
    btnLayout = new QHBoxLayout;
    btnLayout->setMargin(0);
    btnLayout->setSpacing(9);

    QVBoxLayout *frameLayout = new QVBoxLayout;
    frameLayout->addStretch();
    frameLayout->addLayout(btnLayout);
    frameLayout->addStretch();
    frameLayout->setStretch(0, 1);
    frameLayout->setContentsMargins(0, 9, 0, 20);

    QFrame *frame = new QFrame(this);
    frame->setLayout(frameLayout);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(frame);
    layout->setMargin(0);

    this->setLayout(layout);
}

void AppAuthor::initButton()
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

    QPushButton *btnSqlite = new QPushButton(this);
    btnSqlite->setText("数据管理");
    btnSqlite->setMinimumHeight(65);
    btnSqlite->setObjectName("sqlite");
    connect(btnSqlite, SIGNAL(clicked(bool)), this, SLOT(clickButton()));
    btnLayout->addWidget(btnSqlite);

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
    tmpMap.insert("enum", QMessageBox::Close);
    tmpMap.insert("winName", QObject::sender()->objectName());
    emit sendAppMap(tmpMap);
    tmpMap.clear();
}

void AppAuthor::showEvent(QShowEvent *e)
{
    tmpMap.insert("enum", QMessageBox::Question);
    tmpMap.insert("txMessage", QString("6004 Conf"));
    emit sendAppMap(tmpMap);
    e->accept();
}
