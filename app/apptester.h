/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       测试界面
*******************************************************************************/
#ifndef APPTESTER_H
#define APPTESTER_H

#include <qmath.h>
#include <QLabel>
#include <QWidget>
#include <QLayout>
#include <QString>
#include <QSplitter>
#include <QGroupBox>
#include <QTableView>
#include <QLCDNumber>
#include <QMessageBox>
#include <QPushButton>
#include <QHeaderView>
#include <QTextBrowser>
#include <QTableWidget>
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QStandardItemModel>

#include "lib/qcustomplot.h"
#include "boxqchart.h"

#define NUMS 72

const QString StyleOK = "<p style='font-size:10px;color:#666666;line-height:14px;'align='left'>";
const QString StyleNG = "<p style='font-size:10px;color:#FF0000;line-height:14px;'align='left'>";
const QString SmallOK = StyleOK + "<b>%1&nbsp;</b></p>";
const QString SmallNG = StyleNG + "<b>%1&nbsp;</b></p>";

const QString titleOK = "<p style='font-size:11pt;color:#FFFFFF;' align='left'>";
const QString titleNG = "<p style='font-size:11pt;color:#FF0000;' align='left'>";
const QString largeOK = "<p style='font-size:15pt;color:#00FF00;' align='center'>%1</p>";
const QString largeNG = "<p style='font-size:15pt;color:#FF0000;' align='center'>%1</p>";

const QString judgeOK = "<p style='font-size:64pt;color:#00FF00;'align='center'><b>%1</b></p>";
const QString judgeNG = "<p style='font-size:64pt;color:#FF0000;'align='center'><b>%1</b></p>";
const QString judgeON = "<p style='font-size:64pt;color:#FFFF00;'align='center'><b>%1</b></p>";

class AppTester : public QWidget
{
    Q_OBJECT
public:
    explicit AppTester(QWidget *parent = 0);
signals:
    void sendAppMap(QVariantMap msg);
private slots:
    void initUI();
    void initSkin();
    void initLayout();
    void initStatus();
    void initWorker();
    void initResult();
    void initQChart();
    void initButton();
    void initDcrWeld();
    void initDcrChip();
    void initDcrDiag();
    void initDcrWave();
    void drawDcrWave();
    void initInrTextCG();
    void initAcwTextAC();
    void initAcwTextAL();
    void initAcwTextLC();
    void initImpText();
    void initImpWave();
    void drawImpWave();

    void setViewSize();
    void initPieView();
    void initHistogram();
    void drawHistogram();
    void pieResize();
    void dcrResize();
    void impResize();
    void clickButton();
    virtual void showEvent(QShowEvent *e);
private:
    QTableWidget *status;
    QHBoxLayout *vLayout;
    QVBoxLayout *mLayout;
    QVBoxLayout *btnLayout;
    QCustomPlot *dcrView;
    QCustomPlot *impView;
    QTableWidget *view;
    QStandardItemModel *mView;
    QGroupBox *dcrBox;
    QGroupBox *impBox;

    QLabel *textResult;
    QLabel *textWorker;
    QTimer *timer;

    QVariantMap tmpMap;

    QSplitter *hsplitter1;
    QSplitter *hsplitter2;

    QCustomPlot *histogram;
    QList<QCPBars*> bars;
    quint32 countOk;
    quint32 countAll;

    QSplitter *vsp1;
    int tt;
    QList<QLabel*> inrTexts;

    QTextBrowser *textDcrMD;
    QTextBrowser *textDcrCP;
};

#endif // APPTESTER_H
