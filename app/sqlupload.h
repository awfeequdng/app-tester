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

class SqlUpload : public QWidget
{
    Q_OBJECT
public:
    explicit SqlUpload(QWidget *parent = 0);
signals:
    void sendAppMap(QVariantMap msg);
private slots:
    void initUI();
    void initLayout();
    void initBoxText();
    void initBoxCtrl();
    void initSettings();
    void saveSettings();
    void recvAppMap(QVariantMap msg);
    virtual void showEvent(QShowEvent *e);
private:
    QHBoxLayout *layout;
    QList<QLineEdit*> texts;
    QList<QLineEdit*> ctrls;
    QVariantMap config;
};

#endif // SQLUPLOAD_H
