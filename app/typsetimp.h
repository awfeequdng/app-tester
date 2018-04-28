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

#include "lib/qcustomplot.h"

#define PI 3.1415926

class TypSetImp : public QWidget
{
    Q_OBJECT
public:
    explicit TypSetImp(QWidget *parent = 0);
signals:
    void sendAppMap(QVariantMap msg);
private slots:
    void initUI();
    void initLayout();
    void initViewBar();
    void initWaveBar();
    void initButtons();
    void drawImpWave();
    void initDelegate();
    virtual void showEvent(QShowEvent *e);
private:
    QVBoxLayout *boxLayout;
    QCustomPlot *impView;
    QTableView *inrView;
    BoxQModel *mInrView;
};

#endif // TYPSETIMP_H
