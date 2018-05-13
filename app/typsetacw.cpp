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

    inrView->setItemDelegateForColumn(AddrACWSC, new BoxQItems);
    acwView->setItemDelegateForColumn(AddrACWSC, new BoxQItems);
    inrView->setItemDelegateForColumn(AddrACWSN, new BoxQItems);
    acwView->setItemDelegateForColumn(AddrACWSN, new BoxQItems);
    inrView->setItemDelegateForColumn(AddrACWSV, volt);
    acwView->setItemDelegateForColumn(AddrACWSV, volt);
    inrView->setItemDelegateForColumn(AddrACWST, time);
    acwView->setItemDelegateForColumn(AddrACWST, time);
    inrView->setItemDelegateForColumn(AddrACWSH, insr);
    acwView->setItemDelegateForColumn(AddrACWSH, curr);
    inrView->setItemDelegateForColumn(AddrACWSL, insr);
    acwView->setItemDelegateForColumn(AddrACWSL, curr);
    inrView->setItemDelegateForColumn(AddrACWSO, new BoxQItems);
    acwView->setItemDelegateForColumn(AddrACWSO, new BoxQItems);
    inrView->setItemDelegateForColumn(AddrACWSA, new BoxQItems);
    acwView->setItemDelegateForColumn(AddrACWSA, new BoxQItems);
}

void TypSetAcw::initSettings()
{
    int w = 0;
    double t = 0;
    for (int i=0; i < 4; i++) {
        int s = tmpSet[AddrACWS1 + i].toInt();
        int r = (i == 0) ? (i - 0) : (i - 1);
        BoxQModel *view = (i == 0) ? mInrView : mAcwView;
        w = AddrACWSC; // 测试
        Qt::CheckState c = (tmpSet[s + w]) == "1" ? Qt::Checked : Qt::Unchecked;
        view->item(r, w)->setCheckState(c);
        w = AddrACWSN;  // 名称
        view->item(r, w)->setText(tmpSet[s + w].toString());
        w = AddrACWSV;  // 电压
        view->item(r, w)->setText(tmpSet[s + w].toString());
        w = AddrACWST;  // 时间
        t = tmpSet[s + w].toString().toDouble() / 10;
        view->item(r, w)->setText(QString::number(t));
        w = AddrACWSH;  // 上限
        t = tmpSet[s + w].toString().toDouble();
        view->item(r, w)->setText(QString::number((i == 0) ? t : t/100));
        w = AddrACWSL;  // 下限
        t = tmpSet[s + w].toString().toDouble();
        view->item(r, w)->setText(QString::number((i == 0) ? t : t/100));
        w = AddrACWSO;  // 补偿
        t = tmpSet[s + w].toString().toDouble();
        view->item(r, w)->setText(QString::number((i == 0) ? t : t/100));
        w = AddrACWSA;  // 电弧
        view->item(r, w)->setText(tmpSet[s + w].toString());
    }
}

void TypSetAcw::saveSettings()
{
    int w = 0;
    double t = 0;
    for (int i=0; i < 4; i++) {
        int s = tmpSet[AddrACWS1 + i].toInt();
        int r = (i == 0) ? (i - 0) : (i - 1);
        BoxQModel *view = (i == 0) ? mInrView : mAcwView;
        w = AddrACWSC; // 测试
        tmpSet[s + w] = view->item(r, w)->checkState() == Qt::Checked ? 1 : 0;
        w = AddrACWSN;  // 名称
        tmpSet[s + w] = view->item(r, w)->text();
        w = AddrACWSV;  // 电压
        tmpSet[s + w] = view->item(r, w)->text();
        w = AddrACWST;  // 时间
        tmpSet[s + w] = QString::number(view->item(r, w)->text().toDouble()*10);
        w = AddrACWSH;  // 上限
        t = view->item(r, w)->text().toDouble();
        tmpSet[s + w] = (i == 0) ? QString::number(t) : QString::number(t*100);
        w = AddrACWSL;  // 下限
        t = view->item(r, w)->text().toDouble();
        tmpSet[s + w] = (i == 0) ? QString::number(t) : QString::number(t*100);
        w = AddrACWSO;  // 补偿
        t = view->item(r, w)->text().toDouble();
        tmpSet[s + w] = (i == 0) ? QString::number(t) : QString::number(t*100);
        w = AddrACWSA;  // 电弧
        tmpSet[s + w] = view->item(r, w)->text();
    }
    tmpSet.insert(AddrEnum, Qt::Key_Save);
    emit sendAppMsg(tmpSet);
}

void TypSetAcw::recvAppMsg(QTmpMap msg)
{
    int c = msg.value(0).toInt();
    switch (c) {
    case Qt::Key_Copy:
        tmpSet = msg;
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

