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
    headers << tr("测试") << tr("匝间测试") << tr("峰值电压V") << tr("冲击间隔")
            << tr("冲击次数") << tr("合格上限") << tr("采样方式");

    QStringList names;
    names << tr("匝间测试");

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

//    QGroupBox *box = new QGroupBox(this);
//    boxLayout->addWidget(box);

//    QVBoxLayout *layout = new QVBoxLayout;
//    box->setLayout(layout);

//    QCheckBox *imp = new QCheckBox(this);
//    imp->setText(tr("匝间测试"));
//    layout->addWidget(imp);

////    layout->addStretch();

//    QStringList tmp;
//    tmp << tr("峰值电压") << tr("冲击间隔") << tr("合格上限") << tr("采样方式");
//    QStringList unit;
//    unit << "V" << "次" << "%" << "";

//    for (int i=0; i < tmp.size(); i++) {
//        QHBoxLayout *bLayout = new QHBoxLayout;
//        bLayout->addWidget(new QLabel(tmp.at(i), this));
//        QLineEdit *line = new QLineEdit(this);
//        line->setFixedHeight(40);
//        bLayout->addWidget(line);
//        bLayout->addWidget(new QLabel(unit.at(i), this));
//        bLayout->addStretch();
//        layout->addLayout(bLayout);
//    }
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
}

void TypSetImp::initButtons()
{
    boxLayout->addStretch();
    QHBoxLayout *layout = new QHBoxLayout;
    boxLayout->addLayout(layout);

    layout->addStretch();

    QPushButton *btnGet = new QPushButton(this);
    btnGet->setFixedSize(97, 40);
    btnGet->setText(tr("采样"));
    layout->addWidget(btnGet);
//    connect(btnGet, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));

    QPushButton *save = new QPushButton(this);
    save->setFixedSize(97, 40);
    save->setText(tr("保存"));
    layout->addWidget(save);
//    connect(save, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
}

void TypSetImp::drawImpWave()
{
    QCPGraph *graph = impView->addGraph();
    graph->setPen(QPen(Qt::green, 2));
    QVector<double> x(100), y(100);

    for (int i=0; i < 100; i++) {
        x[i] = i;
        y[i] = 50+50*sin(PI*i/24)*(100-i)/100;
    }
    graph->setData(x, y);
    impView->replot();
}

void TypSetImp::initDelegate()
{
    inrView->setItemDelegateForColumn(0, new BoxQItems);
    inrView->setItemDelegateForColumn(1, new BoxQItems);
}

void TypSetImp::showEvent(QShowEvent *e)
{
    this->setFocus();
    e->accept();
}

