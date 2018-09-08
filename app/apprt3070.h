/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       WIFI设置
*******************************************************************************/
#ifndef APPRT3070_H
#define APPRT3070_H

#include <QFile>
#include <QMovie>
#include <QDebug>
#include <QLabel>
#include <QLayout>
#include <QWidget>
#include <QProcess>
#include <QLineEdit>
#include <QSettings>
#include <QShowEvent>
#include <QHeaderView>
#include <QPushButton>
#include <QTextBrowser>
#include <QTableWidget>
#include <QTimer>
#include <QElapsedTimer>
#include <QCoreApplication>

const QString wifiSG = "<p style='font-size:15pt;color:#FFFFFF;' align='left'>%1</p>";
const QString wifiSR = "<p style='font-size:15pt;color:#FFFFFF;' align='right'>%1</p>";
const QString strWifi = "<a style='font-size:18px;color:cyan;' align='left' href=\"%1\">%2";

#define MAXNET 5

class AppRT3070 : public QWidget
{
    Q_OBJECT
public:
    explicit AppRT3070(QWidget *parent = 0);

signals:

private slots:
    void initUI();
    void initLayout();
    void initViewBar();
    void initTextBar();
    void initWireLess();
    void initViewItem(int row);
    void initSettings();
    void saveWireLess();
    void clickLink(QString str);
    virtual void showEvent(QShowEvent *e);
    void wait(int t);
private:
    QVBoxLayout *layout;
    QTableWidget *view;
    QList<QLabel*> names;
    QList<QLabel*> texts;
    QList<QLabel*> icons;
    QLineEdit *ssid;
    QLineEdit *pswd;
    QPushButton *btnWifi;
    QPushButton *btnShow;
    QTextBrowser *text;
    QVariantMap wifis;
};

#endif // APPRT3070_H
