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

#include "main.h"
#include "typconfig.h"

#define CACHEIMP 0x04  // 离散数据长度

#define HARDIMPA 0x00  // 版本
#define NUMBIMPA 0x01  // 编号
#define STATIMPA 0x02  // 状态
#define OKNGIMPA 0x03  // 判定

#define VOLTIMPR 0x00  // 电压
#define DATAIMPR 0x01  // 结果
#define FREQIMPR 0x02  // 频率
#define OKNGIMPR 0x03  // 判定

const int AddrIMPSC = 0x00;  // 测试
const int AddrIMPSN = 0x01;  // 名称
const int AddrIMPSV = 0x02;  // 电压
const int AddrIMPST = 0x03;  // 次数
const int AddrIMPSH = 0x04;  // 上限
const int AddrIMPSL = 0x05;  // 下限
const int AddrIMPSO = 0x06;  // 补偿
const int AddrIMPSA = 0x07;  // 间隔
const int AddrIMPSF = 0x09;  // 频率
const int AddrIMPF1 = 0x0A;  // 起点
const int AddrIMPF2 = 0x0B;  // 终点

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
    void initViewDat();
    void initViewBar();
    void initWaveBar();
    void initWaveCtl();
    void initButtons();
    void initDelegate();
    void initSettings();
    void saveSettings();
    void drawImpWave();
    void lineUpdate();
    void waveSwitch();
    void waveUpdate();
    void recvUpdate(QTmpMap msg);
    void recvAppMsg(QTmpMap msg);
    virtual void showEvent(QShowEvent *e);
private:
    QVBoxLayout *boxLayout;
    BoxQLabel *impView;
    QTableView *view;
    BoxQModel *mView;
    QLabel *numbfreq;
    QLabel *numbcurr;
    QList<int> tmpWave;
    QList<int> impWave;
    int time;
    QTmpMap tmpSet;
    QTmpMap tmpMsg;
    QPushButton *btnWave;
    QPushButton *btnSave;
    QSpinBox *numbfrom;
    QSpinBox *numbstop;
};

#endif // TYPSETIMP_H
