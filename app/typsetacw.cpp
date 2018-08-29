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
    initAcwView();
    initInrView();
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
            << tr("上限MΩ") << tr("下限MΩ") << tr("补偿MΩ") << tr("快速测试");

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

    btnSave = new QPushButton(this);
    btnSave->setFixedSize(97, 40);
    btnSave->setText(tr("保存"));
    layout->addWidget(btnSave);
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
}

void TypSetAcw::initDelegate()
{
    BoxDouble *acwv = new BoxDouble;
    acwv->setDecimals(0);
    acwv->setMaxinum(3000);
    acwView->setItemDelegateForColumn(AddrACWSV, acwv);

    BoxDouble *inrv = new BoxDouble;
    inrv->setDecimals(0);
    inrv->setMaxinum(1000);
    inrView->setItemDelegateForColumn(AddrACWSV, inrv);

    BoxDouble *time = new BoxDouble;
    time->setDecimals(1);
    time->setMaxinum(999.9);
    inrView->setItemDelegateForColumn(AddrACWST, time);
    acwView->setItemDelegateForColumn(AddrACWST, time);

    BoxDouble *insr = new BoxDouble;
    insr->setDecimals(0);
    insr->setMaxinum(500);
    inrView->setItemDelegateForColumn(AddrACWSH, insr);
    inrView->setItemDelegateForColumn(AddrACWSL, insr);

    BoxDouble *curr = new BoxDouble;
    curr->setDecimals(3);
    curr->setMaxinum(25);
    acwView->setItemDelegateForColumn(AddrACWSH, curr);
    acwView->setItemDelegateForColumn(AddrACWSL, curr);

    BoxDouble *comp = new BoxDouble;
    comp->setDecimals(0);
    comp->setMaxinum(60000);

    inrView->setItemDelegateForColumn(AddrACWSC, new BoxQItems);
    acwView->setItemDelegateForColumn(AddrACWSC, new BoxQItems);
    inrView->setItemDelegateForColumn(AddrACWSN, new BoxQItems);
    acwView->setItemDelegateForColumn(AddrACWSN, new BoxQItems);
    inrView->setItemDelegateForColumn(AddrACWSO, new BoxQItems);
    acwView->setItemDelegateForColumn(AddrACWSO, new BoxQItems);
    acwView->setItemDelegateForColumn(AddrACWSA, new BoxQItems);
}

void TypSetAcw::initSettings()
{
    int w = 0;
    double t = 0;
    for (int i=0; i < 4; i++) {
        int s = tmpSet[(4000 + Qt::Key_4) + i].toInt();
        int r = (i == NUMBINRS) ? (i - NUMBINRS) : (i - 0);
        BoxQModel *view = (i == NUMBINRS) ? mInrView : mAcwView;
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
        view->item(r, w)->setText(QString::number((i == NUMBINRS) ? t : t/100));
        w = AddrACWSL;  // 下限
        t = tmpSet[s + w].toString().toDouble();
        view->item(r, w)->setText(QString::number((i == NUMBINRS) ? t : t/100));
        w = AddrACWSO;  // 补偿
        t = tmpSet[s + w].toString().toDouble();
        view->item(r, w)->setText(QString::number((i == NUMBINRS) ? t : t/100));
        w = AddrACWSA;  // 电弧
        view->item(r, w)->setText(tmpSet[s + w].toString());
    }
}

void TypSetAcw::saveSettings()
{
    btnSave->setEnabled(false);
    int w = 0;
    double t = 0;
    for (int i=0; i < 4; i++) {
        int s = tmpSet[(4000 + Qt::Key_4) + i].toInt();
        int r = (i == NUMBINRS) ? (i - NUMBINRS) : (i - 0);
        BoxQModel *view = (i == NUMBINRS) ? mInrView : mAcwView;
        w = AddrACWSC; // 测试
        tmpMsg[s + w] = view->item(r, w)->checkState() == Qt::Checked ? 1 : 0;
        w = AddrACWSN;  // 名称
        tmpMsg[s + w] = view->item(r, w)->text();
        w = AddrACWSV;  // 电压
        tmpMsg[s + w] = view->item(r, w)->text();
        w = AddrACWST;  // 时间
        tmpMsg[s + w] = QString::number(view->item(r, w)->text().toDouble()*10);
        w = AddrACWSH;  // 上限
        t = view->item(r, w)->text().toDouble();
        tmpMsg[s + w] = (i == NUMBINRS) ? QString::number(t) : QString::number(t*100);
        w = AddrACWSL;  // 下限
        t = view->item(r, w)->text().toDouble();
        tmpMsg[s + w] = (i == NUMBINRS) ? QString::number(t) : QString::number(t*100);
        w = AddrACWSO;  // 补偿
        t = view->item(r, w)->text().toDouble();
        tmpMsg[s + w] = (i == NUMBINRS) ? QString::number(t) : QString::number(t*100);
        w = AddrACWSA;  // 电弧
        tmpMsg[s + w] = view->item(r, w)->text();
    }
    tmpMsg.insert(Qt::Key_0, Qt::Key_Save);
    tmpMsg.insert(Qt::Key_1, "aip_config");
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void TypSetAcw::recvAppMsg(QTmpMap msg)
{
    switch (msg.value(Qt::Key_0).toInt()) {
    case Qt::Key_Copy:
        tmpSet = msg;
        btnSave->setEnabled(true);
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

