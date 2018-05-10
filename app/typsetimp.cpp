/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       匝间配置界面
*******************************************************************************/
#include "typsetimp.h"

TypSetImp::TypSetImp(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void TypSetImp::initUI()
{
    initLayout();
    initViewBar();
    initWaveBar();
    drawImpWave();
    initButtons();
    initDelegate();
}

void TypSetImp::initLayout()
{
    boxLayout = new QVBoxLayout(this);
    boxLayout->addStretch();

    QGroupBox *box = new QGroupBox(this);
    box->setLayout(boxLayout);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(box);
}

void TypSetImp::initViewBar()
{
    QStringList headers;
    headers << tr("测试") << tr("匝间测试") << tr("峰值电压V") << tr("冲击次数")
            << tr("合格上限") << tr("合格下限") << tr("电压补偿") << tr("冲击间隔");

    QStringList names;
    names << tr("匝间测试");

    mView = new BoxQModel(this);
    mView->setRowCount(names.size());
    mView->setColumnCount(headers.size());
    mView->setHorizontalHeaderLabels(headers);

    for (int i=0; i < names.size(); i++) {
        for (int j=0; j < headers.size(); j++) {
            mView->setData(mView->index(i, j), "", Qt::DisplayRole);
        }
        mView->item(i, 0)->setCheckable(true);
        mView->item(i, 1)->setText(names.at(i));
    }
    //    connect(mView, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(autoInput()));

    view = new QTableView(this);
    view->setFixedHeight(80);
    view->setModel(mView);
    view->verticalHeader()->hide();
    view->horizontalHeader()->setFixedHeight(30);
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    view->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    view->verticalHeader()->setResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
#else
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
#endif
    view->setColumnWidth(0, 56);
    view->setEditTriggers(QAbstractItemView::AllEditTriggers);

    boxLayout->addWidget(view);
}

void TypSetImp::initWaveBar()
{
    QGroupBox *box = new QGroupBox(this);
    box->setFixedHeight(200);
    boxLayout->addWidget(box);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    box->setLayout(layout);

    impView = new QCustomPlot(this);
    layout->addWidget(impView);
    impView->setBackground(QBrush(QColor(25, 25, 25))); //设置背景色
    impView->xAxis->grid()->setPen(Qt::NoPen);
    impView->yAxis->grid()->setPen(QPen(Qt::darkGreen, 1, Qt::DotLine));
    impView->xAxis->setTicks(false);
    impView->xAxis->setTickLabels(false);
    impView->yAxis->setTicks(false);
    impView->yAxis->setTickLabels(false);
    impView->axisRect()->setMinimumMargins(QMargins(0, 0, 0, 0));
    impView->axisRect()->setMargins(QMargins(0, 0, 0, 0));
    impView->xAxis->setBasePen(QPen(Qt::darkGreen, 1, Qt::DotLine));
    impView->yAxis->setBasePen(Qt::NoPen);
    impView->xAxis2->setBasePen(Qt::NoPen);
    impView->yAxis2->setBasePen(Qt::NoPen);
    impView->xAxis->setRange(0, 100);
    impView->yAxis->setRange(0, 103);

    impView->xAxis->setTickLabelColor(Qt::white);
    impView->xAxis->setLabelColor(Qt::white);
    impView->xAxis->setTickLabelColor(Qt::white);

    impLine = impView->addGraph();
}

void TypSetImp::initButtons()
{
    boxLayout->addStretch();
    QHBoxLayout *layout = new QHBoxLayout;
    boxLayout->addLayout(layout);

    layout->addStretch();

    QPushButton *btnWave = new QPushButton(this);
    btnWave->setFixedSize(97, 40);
    btnWave->setText(tr("采样"));
    layout->addWidget(btnWave);
    connect(btnWave, SIGNAL(clicked(bool)), this, SLOT(waveUpdate()));

    QPushButton *btnSave = new QPushButton(this);
    btnSave->setFixedSize(97, 40);
    btnSave->setText(tr("保存"));
    layout->addWidget(btnSave);
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
}

void TypSetImp::drawImpWave()
{
    impLine->setPen(QPen(Qt::green, 2));
    if (impWave.isEmpty()) {
        QVector<double> x(1), y(1);
        x[0] = -1;
        y[0] = -1;
        impLine->setData(x, y);
    } else {
        QVector<double> x(400), y(400);
        for (int i=0; i < 400; i++) {
            x[i] = i;
            y[i] = impWave.at(i)*100/0x0400;
        }
        impLine->setData(x, y);
    }
    impView->replot();
}

void TypSetImp::initDelegate()
{
    view->setItemDelegateForColumn(0, new BoxQItems);
    view->setItemDelegateForColumn(1, new BoxQItems);
}

void TypSetImp::initSettings()
{
    QString addr;
    addr = QString::number(AddrSetIMP + AddrSkewIC);  // 测试
    Qt::CheckState s = config[addr] == "1" ? Qt::Checked : Qt::Unchecked;
    mView->item(0, AddrSkewIC)->setCheckState(s);

    addr = QString::number(AddrSetIMP + AddrSkewIV);  // 电压
    mView->item(0, AddrSkewIV)->setText(config[addr].toString());

    addr = QString::number(AddrSetIMP + AddrSkewIT);  // 次数
    mView->item(0, AddrSkewIT)->setText(config[addr].toString());

    addr = QString::number(AddrSetIMP + AddrSkewIH);  // 上限
    mView->item(0, AddrSkewIH)->setText(config[addr].toString());

    addr = QString::number(AddrSetIMP + AddrSkewIL);  // 下限
    mView->item(0, AddrSkewIL)->setText(config[addr].toString());

    addr = QString::number(AddrSetIMP + AddrSkewIO);  // 补偿
    mView->item(0, AddrSkewIO)->setText(config[addr].toString());

    addr = QString::number(AddrSetIMP + AddrSkewIB);  // 间隔
    mView->item(0, AddrSkewIB)->setText(config[addr].toString());
}

void TypSetImp::saveSettings()
{
    QString addr;
    addr = QString::number(AddrSetIMP + AddrSkewIC);  // 测试
    config[addr] = mView->item(0, AddrSkewIC)->checkState() == Qt::Checked ? 1 : 0;

    addr = QString::number(AddrSetIMP + AddrSkewIV);  // 电压
    config[addr] = mView->item(0, AddrSkewIV)->text();

    addr = QString::number(AddrSetIMP + AddrSkewIT);  // 次数
    config[addr] = mView->item(0, AddrSkewIT)->text();

    addr = QString::number(AddrSetIMP + AddrSkewIH);  // 上限
    config[addr] = mView->item(0, AddrSkewIH)->text();

    addr = QString::number(AddrSetIMP + AddrSkewIL);  // 下限
    config[addr] = mView->item(0, AddrSkewIL)->text();

    addr = QString::number(AddrSetIMP + AddrSkewIO);  // 补偿
    config[addr] = mView->item(0, AddrSkewIO)->text();

    addr = QString::number(AddrSetIMP + AddrSkewIB);  // 间隔
    config[addr] = mView->item(0, AddrSkewIB)->text();

    config.insert("enum", Qt::Key_Save);
    emit sendAppMap(config);
}

void TypSetImp::waveUpdate()
{
    tmpMap.insert("enum", Qt::Key_Send);
    tmpMap.insert("text", AddrConfig);
    emit sendAppMap(tmpMap);
    tmpMap.insert("text", AddrSetIMP);
    emit sendAppMap(tmpMap);
    tmpMap.clear();
}

void TypSetImp::recvUpdate(QVariantMap msg)
{
    int addr = msg.value("text").toInt();
    if (addr == AddrSetIMP) {
        impWave.clear();
        for (int i=0; i < 400; i++) {
            impWave.append(msg[QString::number(AddrWaveTP + i)].toInt());
        }
        drawImpWave();
    }
}

void TypSetImp::recvAppMap(QVariantMap msg)
{
    switch (msg.value("enum").toInt()) {
    case Qt::Key_Copy:
        for (int i=AddrSetIMP; i < AddrSetIMP + 0x0010; i++) {  // 匝间参数存放在0x04B0
            config[QString::number(i)] = msg[QString::number(i)];
        }
        break;
    case Qt::Key_News:
        if (this->isHidden())
            break;
        recvUpdate(msg);
        break;
    default:
        break;
    }
}

void TypSetImp::showEvent(QShowEvent *e)
{
    this->setFocus();
    initSettings();
    e->accept();
}

