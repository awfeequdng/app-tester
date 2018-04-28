/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       介电强度配置界面
*******************************************************************************/
#include "typsetacw.h"

TypSetAcw::TypSetAcw(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void TypSetAcw::initUI()
{
    initLayout();
    initInrView();
    initAcwView();
    initButtons();
    initDelegate();
}

void TypSetAcw::initLayout()
{
    boxLayout = new QVBoxLayout;
    boxLayout->addStretch();

    QGroupBox *box = new QGroupBox(this);
    box->setLayout(boxLayout);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(box);
}

void TypSetAcw::initInrView()
{
    QStringList headers;
    headers << tr("测试") << tr("项目") << tr("电压V") << tr("时间s")
            << tr("上限MΩ") << tr("下限MΩ") << tr("补偿MΩ") << tr("异常报警");

    QStringList names;
    names << tr("绝缘电阻");

    mInrView = new BoxQModel(this);
    mInrView->setRowCount(names.size());
    mInrView->setColumnCount(headers.size());
    mInrView->setHorizontalHeaderLabels(headers);

    for (int i=0; i < names.size(); i++) {
        for (int j=0; j < headers.size(); j++) {
            mInrView->setData(mInrView->index(i, j), "", Qt::DisplayRole);
        }
        mInrView->item(i, 0)->setCheckable(true);
        mInrView->item(i, 1)->setText(names.at(i));
    }
    //    connect(mView, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(autoInput()));

    inrView = new QTableView(this);
    inrView->setFixedHeight(80);
    inrView->setModel(mInrView);
    inrView->verticalHeader()->hide();
    inrView->horizontalHeader()->setFixedHeight(30);
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    inrView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    inrView->verticalHeader()->setResizeMode(QHeaderView::Stretch);
    inrView->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
#else
    inrView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    inrView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    inrView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
#endif
    inrView->setColumnWidth(0, 56);
    inrView->setEditTriggers(QAbstractItemView::AllEditTriggers);

    boxLayout->addWidget(inrView);
}

void TypSetAcw::initAcwView()
{
    QStringList headers;
    headers << tr("测试") << tr("项目") << tr("电压V") << tr("时间s")
            << tr("上限mA") << tr("下限mA") << tr("补偿mA") << tr("电弧侦测");

    QStringList names;
    names << tr("轴铁耐压") << tr("轴线耐压") << tr("线铁耐压");

    mAcwView = new BoxQModel;
    mAcwView->setRowCount(names.size());
    mAcwView->setColumnCount(headers.size());
    mAcwView->setHorizontalHeaderLabels(headers);

    for (int i=0; i < names.size(); i++) {
        for (int j=0; j < headers.size(); j++) {
            mAcwView->setData(mAcwView->index(i, j), "", Qt::DisplayRole);
        }
        mAcwView->item(i, 0)->setCheckable(true);
        mAcwView->item(i, 1)->setText(names.at(i));
    }
    //    connect(mView, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(autoInput()));

    acwView = new QTableView(this);
    acwView->setFixedHeight(180);
    acwView->setModel(mAcwView);
    acwView->verticalHeader()->hide();
    acwView->horizontalHeader()->setFixedHeight(30);
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    acwView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    acwView->verticalHeader()->setResizeMode(QHeaderView::Stretch);
    acwView->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
#else
    acwView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    acwView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    acwView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
#endif
    acwView->setColumnWidth(0, 56);
    acwView->setEditTriggers(QAbstractItemView::AllEditTriggers);

    boxLayout->addWidget(acwView);
}

void TypSetAcw::initButtons()
{
    boxLayout->addStretch();
    QHBoxLayout *layout = new QHBoxLayout;
    boxLayout->addLayout(layout);
    layout->addStretch();

    QPushButton *btnAdd = new QPushButton(this);
    btnAdd->setFixedSize(97, 40);
    btnAdd->setText(tr("保存"));
    layout->addWidget(btnAdd);
    connect(btnAdd, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
}

void TypSetAcw::initDelegate()
{
    BoxDouble *volt = new BoxDouble;
    volt->setDecimals(0);
    volt->setMaxinum(5000);

    BoxDouble *time = new BoxDouble;
    time->setDecimals(1);
    time->setMaxinum(999.9);

    BoxDouble *insr = new BoxDouble;
    insr->setDecimals(0);
    insr->setMaxinum(500);

    BoxDouble *curr = new BoxDouble;
    curr->setDecimals(3);
    curr->setMaxinum(25);

    BoxDouble *comp = new BoxDouble;
    comp->setDecimals(0);
    comp->setMaxinum(60000);

    inrView->setItemDelegateForColumn(0, new BoxQItems);
    acwView->setItemDelegateForColumn(0, new BoxQItems);
    inrView->setItemDelegateForColumn(1, new BoxQItems);
    acwView->setItemDelegateForColumn(1, new BoxQItems);
    inrView->setItemDelegateForColumn(2, volt);
    acwView->setItemDelegateForColumn(2, volt);
    inrView->setItemDelegateForColumn(3, time);
    acwView->setItemDelegateForColumn(3, time);
    inrView->setItemDelegateForColumn(4, insr);
    acwView->setItemDelegateForColumn(4, curr);
    inrView->setItemDelegateForColumn(5, insr);
    acwView->setItemDelegateForColumn(5, curr);
    inrView->setItemDelegateForColumn(6, comp);
    acwView->setItemDelegateForColumn(6, curr);
}

void TypSetAcw::initSettings()
{
    QString addr;
    addr = QString::number(0x0470);
    mInrView->item(0, 0)->setCheckState(config[addr] == "1" ? Qt::Checked : Qt::Unchecked);
    addr = QString::number(0x0480);
    mAcwView->item(0, 0)->setCheckState(config[addr] == "1" ? Qt::Checked : Qt::Unchecked);
    addr = QString::number(0x0490);
    mAcwView->item(1, 0)->setCheckState(config[addr] == "1" ? Qt::Checked : Qt::Unchecked);
    addr = QString::number(0x04A0);
    mAcwView->item(2, 0)->setCheckState(config[addr] == "1" ? Qt::Checked : Qt::Unchecked);

    for (int i= 0x0471; i < 0x0471+mInrView->columnCount()-2; i++) {
        mInrView->item(0, i%0x0471+2)->setText(config[QString::number(i)].toString());
    }
    for (int i= 0x0481; i < 0x0481+mInrView->columnCount()-2; i++) {
        mAcwView->item(0, i%0x0481+2)->setText(config[QString::number(i)].toString());
    }
    for (int i= 0x0491; i < 0x0491+mInrView->columnCount()-2; i++) {
        mAcwView->item(1, i%0x0491+2)->setText(config[QString::number(i)].toString());
    }
    for (int i= 0x04A1; i < 0x04A1+mInrView->columnCount()-2; i++) {
        mAcwView->item(2, i%0x04A1+2)->setText(config[QString::number(i)].toString());
    }
}

void TypSetAcw::saveSettings()
{
    QString addr;
    addr = QString::number(0x0470);
    config[addr] = mInrView->item(0, 0)->checkState() == Qt::Checked ? 1 : 0;
    addr = QString::number(0x0480);
    config[addr] = mAcwView->item(0, 0)->checkState() == Qt::Checked ? 1 : 0;
    addr = QString::number(0x0490);
    config[addr] = mAcwView->item(1, 0)->checkState() == Qt::Checked ? 1 : 0;
    addr = QString::number(0x04A0);
    config[addr] = mAcwView->item(2, 0)->checkState() == Qt::Checked ? 1 : 0;

    for (int i= 2; i < mInrView->columnCount(); i++) {
        config[QString::number(0x0470 + i -1)] = mInrView->item(0, i)->text();
    }
    for (int i= 2; i < mAcwView->columnCount(); i++) {
        config[QString::number(0x0480 + i -1)] = mAcwView->item(0, i)->text();
    }
    for (int i= 2; i < mAcwView->columnCount(); i++) {
        config[QString::number(0x0490 + i -1)] = mAcwView->item(1, i)->text();
    }
    for (int i= 2; i < mAcwView->columnCount(); i++) {
        config[QString::number(0x04A0 + i -1)] = mAcwView->item(2, i)->text();
    }
    config.insert("enum", Qt::Key_Option);
    emit sendAppMap(config);
}

void TypSetAcw::recvAppMap(QVariantMap msg)
{
    switch (msg.value("enum").toInt()) {
    case Qt::Key_Option:
        for (int i=0x0460; i < 0x0460+0x0050; i++) {  // 介电强度存放在0x0460
            config[QString::number(i)] = msg[QString::number(i)];
        }
        break;
    default:
        break;
    }
}

void TypSetAcw::showEvent(QShowEvent *e)
{
    this->setFocus();
    initSettings();
    e->accept();
}

