/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       电阻配置界面
*******************************************************************************/
#ifndef TYPSETDCR_H
#define TYPSETDCR_H

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
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QListView>

class TypSetDcr : public QWidget
{
    Q_OBJECT
public:
    explicit TypSetDcr(QWidget *parent = 0);
signals:
    void sendAppMap(QVariantMap msg);
private slots:
    void initUI();
    void initLayout();
    void initCompBar();
    void initWeldBar();
    void initChipBar();
    void initDiagBar();
    void initViewBar();
    void initButtons();
    void clickButtons();
private:
    QVBoxLayout *boxLayout;
    QVariantMap config;

    QCheckBox *boxTemp;
    QCheckBox *boxWeld;
    QCheckBox *boxChip;
    QCheckBox *boxDiag;
    QDoubleSpinBox *boxTime;
    QDoubleSpinBox *tempMax;
    QDoubleSpinBox *meldMax;
    QDoubleSpinBox *chipMax;
    QDoubleSpinBox *diagMax;
    QDoubleSpinBox *diagMin;
};

#endif // TYPSETDCR_H
