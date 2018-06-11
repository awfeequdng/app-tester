/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       匝间配置界面
*******************************************************************************/
#ifndef TYPSETIMP_H
#define TYPSETIMP_H

#include <QLabel>
#include <QDebug>
#include <QLayout>
#include <QWidget>
#include <QVariant>
#include <QCheckBox>
#include <QGroupBox>
#include <QComboBox>
#include <QLineEdit>
#include <QTableView>
#include <QHeaderView>
#include <QShowEvent>
#include <QMessageBox>
#include <QPushButton>
#include <QDoubleSpinBox>

#include "boxqmodel.h"
#include "boxqitems.h"
#include "boxdouble.h"
#include "boxqlabel.h"

#include "appdefine.h"
#include "typconfig.h"

const int AddrIMPSC = 0x00;  // 测试
const int AddrIMPSN = 0x01;  // 名称
const int AddrIMPSV = 0x02;  // 电压
const int AddrIMPST = 0x03;  // 次数
const int AddrIMPSH = 0x04;  // 上限
const int AddrIMPSL = 0x05;  // 下限
const int AddrIMPSO = 0x06;  // 补偿
const int AddrIMPSA = 0x07;  // 间隔
const int AddrIMPSF = 0x09;  // 频率

#define PI 3.1415926

class TypSetImp : public QWidget
{
    Q_OBJECT
public:
    explicit TypSetImp(QWidget *parent = 0);
signals:
    void sendAppMsg(QTmpMap msg);
private slots:
    void initUI();
    void initLayout();
    void initViewBar();
    void initWaveBar();
    void initButtons();
    void initDelegate();
    void initSettings();
    void saveSettings();
    void drawImpWave();
    void waveUpdate();
    void recvUpdate(QTmpMap msg);
    void recvAppMsg(QTmpMap msg);
    virtual void showEvent(QShowEvent *e);
private:
    QVBoxLayout *boxLayout;
    BoxQLabel *impView;
    QTableView *view;
    BoxQModel *mView;
    QLabel *text;
    QList<int> tmpWave;
    QList<int> impWave;
    int time;
    QTmpMap tmpSet;
    QTmpMap tmpMsg;
};

#endif // TYPSETIMP_H
