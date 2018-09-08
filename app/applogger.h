/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       调试信息
*******************************************************************************/
#ifndef APPLOGGER_H
#define APPLOGGER_H

#include <QDir>
#include <QFile>
#include <QDebug>
#include <QLayout>
#include <QWidget>
#include <QVariant>
#include <QListView>
#include <QDateTime>
#include <QComboBox>
#include <QGroupBox>
#include <QPushButton>
#include <QTextStream>
#include <QTextBrowser>

#include "main.h"

class AppLogger : public QWidget
{
    Q_OBJECT
public:
    static AppLogger *_instance;
    static AppLogger *instance();
    explicit AppLogger(QWidget *parent = 0);
signals:
    void sendAppMsg(QTmpMap msg);
    void sendAppMap(QVariantMap msg);
public slots:
    void output(QtMsgType type, const QString &msg);
private slots:
    void initUI();
    void initLayout();
    void initTextBar();
    void initButtonBar();
    void saveSettings();
    void changeHandle(int t);
    void recvAppMsg(QTmpMap msg);
private:
    QVBoxLayout *layout;
    QComboBox *combo;
    QTextBrowser *text;
    QHBoxLayout *btnLayout;
    QVariantMap config;
    QTmpMap tmpSet;
    QTmpMap tmpMsg;
};

#endif // APPLOGGER_H
