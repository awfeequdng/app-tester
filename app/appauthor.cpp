/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       开机主页
*******************************************************************************/
#include "appauthor.h"

AppAuthor::AppAuthor(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void AppAuthor::initUI()
{
    initLayout();
    initTitleBar();
    initButtonBar();
}

void AppAuthor::initLayout()
{
    QString str = ".QFrame{border-image:url(:/author.png)}";
    this->setStyleSheet(str);

    QVBoxLayout *mLayout = new QVBoxLayout(this);
    QFrame *frm = new QFrame(this);
    layout = new QVBoxLayout;
    mLayout->addWidget(frm);
    mLayout->setSpacing(0);
    mLayout->setMargin(0);
    frm->setLayout(layout);
    layout->setContentsMargins(0, 9, 9, 30);
}

void AppAuthor::initTitleBar()
{
    QHBoxLayout *topLayout = new QHBoxLayout;
    layout->addLayout(topLayout);
    topLayout->addStretch();

    tcpStat = new QLabel(this);
    tcpStat->setPixmap(QPixmap(":/icon_wifi.png"));
    topLayout->addWidget(tcpStat);
    tcpStat->hide();
    verStat = new QLabel(this);
    verStat->setFixedHeight(40);
    topLayout->addWidget(verStat);
}

void AppAuthor::initButtonBar()
{
    QHBoxLayout *blayout = new QHBoxLayout;
    blayout->addStretch();
    QStringList marks, names;
    if (QApplication::desktop()->width() > 800) {
        marks << tr("退出程序");
        names << "";
    }
    marks << tr("系统设置") << tr("型号管理") << tr("数据管理") << tr("进入测试");
    names << "system" << "config" << "record" << "tester";
    for (int i=0; i < marks.size(); i++) {
        QPushButton *btn = new QPushButton(marks.at(i), this);
        btn->setObjectName(names.at(i));
        connect(btn, SIGNAL(clicked(bool)), this, SLOT(clickButton()));
        btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        blayout->addWidget(btn);
    }
    blayout->addStretch();
    for (int i=0; i < blayout->count(); i++)
        blayout->setStretch(i, 1);
    bframe = new QWidget(this);
    bframe->setLayout(blayout);
    bframe->setVisible(false);
    layout->addStretch();
    layout->addWidget(bframe);
    layout->setStretch(1, 6);
    layout->setStretch(2, 1);
}

void AppAuthor::clickButton()
{
    if (QObject::sender()->objectName().isEmpty()) {
        QApplication::closeAllWindows();
    } else {
        tmpMsg.insert(Qt::Key_0, Qt::Key_View);
        tmpMsg.insert(Qt::Key_1, QObject::sender()->objectName());
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
    }
}

void AppAuthor::recvAppMsg(QTmpMap msg)
{
    switch (msg.value(Qt::Key_0).toInt()) {
    case Qt::Key_Copy:
        verStat->setText(msg.value(DataSoft).toString());
        break;
    case Qt::Key_WLAN:
        tcpStat->setVisible(msg.value(Qt::Key_1).toString() == "show");
        break;
    case Qt::Key_Game:
        QTimer::singleShot(800, bframe, SLOT(show()));
        break;
    default:
        break;
    }
}

void AppAuthor::showEvent(QShowEvent *e)
{
    tmpMap.insert("enum", Qt::Key_View);
    tmpMap.insert("text", QString("6004 Conf"));
    emit sendAppMap(tmpMap);
    tmpMap.clear();
    e->accept();
}
