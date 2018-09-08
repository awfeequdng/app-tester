/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       开机主页
*******************************************************************************/
#ifndef APPAUTHOR_H
#define APPAUTHOR_H

#include <QDebug>
#include <QLabel>
#include <QTimer>
#include <QLayout>
#include <QWidget>
#include <QVariant>
#include <QShowEvent>
#include <QPushButton>
#include <QApplication>
#include <QDesktopWidget>

#include "main.h"

class AppAuthor : public QWidget
{
    Q_OBJECT
public:
    explicit AppAuthor(QWidget *parent = 0);
signals:
    void sendAppMsg(QTmpMap msg);
    void sendAppMap(QVariantMap msg);
private slots:
    void initUI();
    void initLayout();
    void initTitleBar();
    void initButtonBar();
    void clickButton();
    void recvAppMsg(QTmpMap msg);
    virtual void showEvent(QShowEvent *e);
private:
    QVBoxLayout *layout;
    QWidget *bframe;
    QLabel *tcpStat;
    QLabel *verStat;
    QTmpMap tmpMsg;
    QVariantMap tmpMap;
    QHBoxLayout *topLayout;
    QHBoxLayout *btnLayout;
};

#endif // APPAUTHOR_H
