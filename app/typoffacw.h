/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       介电强度后台配置界面
*******************************************************************************/
#ifndef TYPOFFACW_H
#define TYPOFFACW_H

#include <QLabel>
#include <QLayout>
#include <QWidget>
#include <QVariant>
#include <QShowEvent>
#include <QMessageBox>
#include <QPushButton>

#include "appdefine.h"

class TypOffAcw : public QWidget
{
    Q_OBJECT
public:
    explicit TypOffAcw(QWidget *parent = 0);
signals:
    void sendAppMsg(QTmpMap msg);
private slots:
    void recvAppMsg(QTmpMap msg);
private:
    QTmpMap tmpSet;
    QTmpMap tmpMsg;
};

#endif // TYPOFFACW_H
