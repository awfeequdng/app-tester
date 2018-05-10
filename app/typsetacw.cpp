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

    inrView->setItemDelegateForColumn(AddrSkewHC, new BoxQItems);
    acwView->setItemDelegateForColumn(AddrSkewHC, new BoxQItems);
    inrView->setItemDelegateForColumn(AddrSkewHN, new BoxQItems);
    acwView->setItemDelegateForColumn(AddrSkewHN, new BoxQItems);
    inrView->setItemDelegateForColumn(AddrSkewHV, volt);
    acwView->setItemDelegateForColumn(AddrSkewHV, volt);
    inrView->setItemDelegateForColumn(AddrSkewHT, time);
    acwView->setItemDelegateForColumn(AddrSkewHT, time);
    inrView->setItemDelegateForColumn(AddrSkewHH, insr);
    acwView->setItemDelegateForColumn(AddrSkewHH, curr);
    inrView->setItemDelegateForColumn(AddrSkewHL, insr);
    acwView->setItemDelegateForColumn(AddrSkewHL, curr);
    inrView->setItemDelegateForColumn(AddrSkewHO, new BoxQItems);
    acwView->setItemDelegateForColumn(AddrSkewHO, new BoxQItems);
    inrView->setItemDelegateForColumn(AddrSkewHA, new BoxQItems);
    acwView->setItemDelegateForColumn(AddrSkewHA, new BoxQItems);
}

void TypSetAcw::initSettings()
{
    int skew = 0;
    double t = 0;
    QString addr;
    for (int i=0; i < 4; i++) {
        skew = AddrSkewHC;  // 测试
        addr = QString::number(AddrHighAG + i*0x10 + skew);
        Qt::CheckState s = config[addr] == "1" ? Qt::Checked : Qt::Unchecked;
        if (i == 0)
            mInrView->item(i-0, skew)->setCheckState(s);
        else
            mAcwView->item(i-1, skew)->setCheckState(s);

        skew = AddrSkewHV;  // 电压
        addr = QString::number(AddrHighAG + i*0x10 + skew);
        if (i == 0)
            mInrView->item(i-0, skew)->setText(config[addr].toString());
        else
            mAcwView->item(i-1, skew)->setText(config[addr].toString());

        skew = AddrSkewHT;  // 时间
        addr = QString::number(AddrHighAG + i*0x10 + skew);
        t = config[addr].toString().toDouble();
        t /= 10;
        if (i == 0)
            mInrView->item(i-0, skew)->setText(QString::number(t));
        else
            mAcwView->item(i-1, skew)->setText(QString::number(t));

        skew = AddrSkewHH;  // 上限
        addr = QString::number(AddrHighAG + i*0x10 + skew);
        t = config[addr].toString().toDouble();
        if (i == 0) {
            mInrView->item(i-0, skew)->setText(QString::number(t));
        } else {
            t /= 100;
            mAcwView->item(i-1, skew)->setText(QString::number(t));
        }

        skew = AddrSkewHL;  // 下限
        addr = QString::number(AddrHighAG + i*0x10 + skew);
        t = config[addr].toString().toDouble();
        if (i == 0) {
            mInrView->item(i-0, skew)->setText(QString::number(t));
        } else {
            t /= 100;
            mAcwView->item(i-1, skew)->setText(QString::number(t));
        }

        skew = AddrSkewHO;  // 补偿
        addr = QString::number(AddrHighAG + i*0x10 + skew);
        t = config[addr].toString().toDouble();
        if (i == 0) {
            mInrView->item(i-0, skew)->setText(QString::number(t));
        } else {
            t /= 100;
            mAcwView->item(i-1, skew)->setText(QString::number(t));
        }

        skew = AddrSkewHA;  // 电弧
        addr = QString::number(AddrHighAG + i*0x10 + skew);
        if (i == 0)
            mInrView->item(i-0, skew)->setText(config[addr].toString());
        else
            mAcwView->item(i-1, skew)->setText(config[addr].toString());
    }
}

void TypSetAcw::saveSettings()
{
    int skew = 0;
    double t = 0;
    QString addr;
    for (int i=0; i < 4; i++) {
        skew = AddrSkewHC;  // 测试
        addr = QString::number(AddrHighAG + i*0x10 + skew);
        if (i == 0)
            config[addr] = mInrView->item(i-0, skew)->checkState() == Qt::Checked ? 1 : 0;
        else
            config[addr] = mAcwView->item(i-1, skew)->checkState() == Qt::Checked ? 1 : 0;

        skew = AddrSkewHV;  // 电压
        addr = QString::number(AddrHighAG + i*0x10 + skew);
        if (i == 0)
            config[addr] = mInrView->item(i-0, skew)->text();
        else
            config[addr] = mAcwView->item(i-1, skew)->text();

        skew = AddrSkewHT;  // 时间
        addr = QString::number(AddrHighAG + i*0x10 + skew);
        if (i == 0)
            t = mInrView->item(i-0, skew)->text().toDouble();
        else
            t = mAcwView->item(i-1, skew)->text().toDouble();
        t *= 10;
        config[addr] = QString::number(t);

        skew = AddrSkewHH;  // 上限
        addr = QString::number(AddrHighAG + i*0x10 + skew);
        if (i == 0) {
            t = mInrView->item(i-0, skew)->text().toDouble();
        } else {
            t = mAcwView->item(i-1, skew)->text().toDouble();
            t *= 100;
        }
        config[addr] = QString::number(t);

        skew = AddrSkewHL;  // 下限
        addr = QString::number(AddrHighAG + i*0x10 + skew);
        if (i == 0) {
            t = mInrView->item(i-0, skew)->text().toDouble();
        } else {
            t = mAcwView->item(i-1, skew)->text().toDouble();
            t *= 100;
        }
        config[addr] = QString::number(t);

        skew = AddrSkewHO;  // 补偿
        addr = QString::number(AddrHighAG + i*0x10 + skew);
        if (i == 0) {
            t = mInrView->item(i-0, skew)->text().toDouble();
        } else {
            t = mAcwView->item(i-1, skew)->text().toDouble();
            t *= 100;
        }
        config[addr] = QString::number(t);

        skew = AddrSkewHA;  // 电弧
        addr = QString::number(AddrHighAG + i*0x10 + skew);
        if (i == 0)
            config[addr] = mInrView->item(i-0, skew)->text();
        else
            config[addr] = mAcwView->item(i-1, skew)->text();
    }
    config.insert("enum", Qt::Key_Save);
    emit sendAppMap(config);
}

void TypSetAcw::recvAppMap(QVariantMap msg)
{
    switch (msg.value("enum").toInt()) {
    case Qt::Key_Copy:
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

