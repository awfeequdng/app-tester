/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       系统设置
*******************************************************************************/
#ifndef APPSYSTEM_H
#define APPSYSTEM_H

#include <QLabel>
#include <QLayout>
#include <QWidget>
#include <QVariant>
#include <QListView>
#include <QComboBox>
#include <QGroupBox>
#include <QLineEdit>
#include <QShowEvent>
#include <QMessageBox>
#include <QPushButton>

class AppSystem : public QWidget
{
    Q_OBJECT
public:
    explicit AppSystem(QWidget *parent = 0);
signals:
    void sendAppMap(QVariantMap msg);
private slots:
    void initUI();
    void initLayout();
    void initSystem();
    void initDevice();
    void initDelegate();

    void initSettings();
    void saveSettings();
    void recvAppMap(QVariantMap msg);
    virtual void showEvent(QShowEvent *e);
private:
    QHBoxLayout *layout;
    QList<QComboBox*> texts;
    QList<QLineEdit*> ctrls;
    QVariantMap tmpMap;
    QVariantMap config;
};

#endif // APPSYSTEM_H
