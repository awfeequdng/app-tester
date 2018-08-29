/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       数据上传
*******************************************************************************/
#ifndef SQLUPLOAD_H
#define SQLUPLOAD_H

#include <QLabel>
#include <QLayout>
#include <QWidget>
#include <QVariant>
#include <QGroupBox>
#include <QLineEdit>
#include <QShowEvent>
#include <QPushButton>
#include <QMessageBox>

#include "main.h"

class SqlUpload : public QWidget
{
    Q_OBJECT
public:
    explicit SqlUpload(QWidget *parent = 0);
signals:
    void sendAppMsg(QTmpMap msg);
private slots:
    void initUI();
    void initLayout();
    void initBoxText();
    void initBoxCtrl();
    void initSettings();
    void saveSettings();
    void recvAppMsg(QTmpMap msg);
    virtual void showEvent(QShowEvent *e);
private:
    QHBoxLayout *layout;
    QList<QLineEdit*> texts;
    QTmpMap tmpSet;
};

#endif // SQLUPLOAD_H
