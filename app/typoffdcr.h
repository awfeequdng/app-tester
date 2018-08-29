/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       电阻后台配置界面
*******************************************************************************/
#ifndef TYPOFFDCR_H
#define TYPOFFDCR_H

#include <QLabel>
#include <QLayout>
#include <QWidget>
#include <QVariant>
#include <QShowEvent>
#include <QMessageBox>
#include <QPushButton>

#include "main.h"

class TypOffDcr : public QWidget
{
    Q_OBJECT
public:
    explicit TypOffDcr(QWidget *parent = 0);
signals:
    void sendAppMsg(QTmpMap msg);
private slots:
    void recvAppMsg(QTmpMap msg);
private:
    QTmpMap tmpSet;
    QTmpMap tmpMsg;
};

#endif // TYPOFFDCR_H
