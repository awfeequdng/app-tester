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
#include <QMessageBox>
#include <QPushButton>
#include <QTextStream>
#include <QTextBrowser>

class AppLogger : public QWidget
{
    Q_OBJECT
public:
    static AppLogger *_instance;
    static AppLogger *instance();
    explicit AppLogger(QWidget *parent = 0);
signals:
    void sendAppMap(QVariantMap msg);
public slots:
    void output(QtMsgType type, const QString &msg);
private slots:
    void initUI();
    void initText();
    void initButton();
    void initLayout();
    void saveSettings();
    void changeHandle(int t);
    void recvAppMap(QVariantMap msg);
private:
    QComboBox *combo;
    QTextBrowser *text;
    QHBoxLayout *btnLayout;
    QVariantMap config;
};

#endif // APPLOGGER_H
