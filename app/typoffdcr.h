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

class TypOffDcr : public QWidget
{
    Q_OBJECT
public:
    explicit TypOffDcr(QWidget *parent = 0);
signals:
    void sendAppMap(QVariantMap msg);
};

#endif // TYPOFFDCR_H
