/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       电阻配置界面
*******************************************************************************/
#include "typsetdcr.h"

TypSetDcr::TypSetDcr(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void TypSetDcr::initUI()
{
    initLayout();
    initWeldBar();
    initChipBar();
    initDiagBar();
    initCompBar();
    initViewBar();
    initButtons();
}

void TypSetDcr::initLayout()
{
    boxLayout = new QVBoxLayout;

    QGroupBox *box = new QGroupBox(this);
    box->setLayout(boxLayout);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(box);
}

void TypSetDcr::initCompBar()
{
    QGroupBox *group = new QGroupBox(this);
    group->setTitle(tr("电阻综合"));
    boxLayout->addWidget(group);

    QHBoxLayout *layout = new QHBoxLayout;
    group->setLayout(layout);

    boxTemp = new QCheckBox(this);
    boxTemp->setText(tr("温度折算"));
    boxTemp->setFixedWidth(97);
    layout->addWidget(boxTemp);

    layout->addWidget(new QLabel(tr("折算温度"), this));
    tempMax = new QDoubleSpinBox(this);
    tempMax->setDecimals(3);
    tempMax->setFixedSize(97, 40);
    layout->addWidget(tempMax);
    layout->addWidget(new QLabel(tr("°C"), this));

    layout->addWidget(new QLabel(tr("测试时间"), this));
    boxTime = new QDoubleSpinBox(this);
    boxTime->setDecimals(3);
    boxTime->setFixedSize(97, 40);
    layout->addWidget(boxTime);
    layout->addWidget(new QLabel(tr("s"), this));

    layout->addStretch();
}

void TypSetDcr::initWeldBar()
{
    QGroupBox *group = new QGroupBox(this);
    group->setTitle(tr("片间电阻"));
    boxLayout->addWidget(group);

    QHBoxLayout *layout = new QHBoxLayout;
    group->setLayout(layout);

    boxWeld = new QCheckBox(this);
    boxWeld->setText(tr("片间电阻"));
    boxWeld->setFixedWidth(97);
    layout->addWidget(boxWeld);

    layout->addWidget(new QLabel(tr("离散允差"), this));
    meldMax = new QDoubleSpinBox(this);
    meldMax->setDecimals(3);
    meldMax->setFixedSize(97, 40);
    layout->addWidget(meldMax);
    layout->addWidget(new QLabel(tr("%"), this));

    layout->addStretch();

    QComboBox *box = new QComboBox(this);
    box->setView(new QListView);
    box->addItem(tr("自动排序"));
    box->setFixedSize(97, 40);
    layout->addWidget(box);
}

void TypSetDcr::initChipBar()
{
    QGroupBox *group = new QGroupBox(this);
    group->setTitle(tr("焊接电阻"));
    boxLayout->addWidget(group);

    QHBoxLayout *layout = new QHBoxLayout;
    group->setLayout(layout);

    boxChip = new QCheckBox(this);
    boxChip->setText(tr("焊接电阻"));
    boxChip->setFixedWidth(97);
    layout->addWidget(boxChip);

    layout->addWidget(new QLabel(tr("合格上限"), this));

    chipMax = new QDoubleSpinBox(this);
    chipMax->setDecimals(3);
    chipMax->setFixedSize(97, 40);
    layout->addWidget(chipMax);

    layout->addWidget(new QLabel(tr("mΩ"), this));

    layout->addStretch();
}

void TypSetDcr::initDiagBar()
{
    QGroupBox *group = new QGroupBox(this);
    group->setTitle(tr("跨间电阻"));
    boxLayout->addWidget(group);

    QHBoxLayout *layout = new QHBoxLayout;
    group->setLayout(layout);

    boxDiag = new QCheckBox(this);
    boxDiag->setText(tr("跨间电阻"));
    boxDiag->setFixedWidth(97);
    layout->addWidget(boxDiag);

    layout->addWidget(new QLabel(tr("合格范围"), this));

    diagMin = new QDoubleSpinBox(this);
    diagMin->setDecimals(3);
    diagMin->setFixedSize(97, 40);
    layout->addWidget(diagMin);

    layout->addWidget(new QLabel(tr("~"), this));

    diagMax = new QDoubleSpinBox(this);
    diagMax->setDecimals(3);
    diagMax->setFixedSize(97, 40);
    layout->addWidget(diagMax);

    layout->addWidget(new QLabel(tr("Ω"), this));

    layout->addStretch();

    QComboBox *box = new QComboBox(this);
    box->setView(new QListView);
    box->addItem(tr("挂钩顺序"));
    box->setFixedSize(97, 40);
    layout->addWidget(box);
}

void TypSetDcr::initViewBar()
{
    QTableWidget *view = new QTableWidget(this);
    view->setRowCount(6);
    view->setColumnCount(12);
    view->verticalHeader()->hide();
    view->horizontalHeader()->hide();

    for (int i=0; i < 12; i++) {
        for (int j=0; j < 8; j++) {
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setTextAlignment(Qt::AlignCenter);
            view->setItem(j, i, item);
            if (i%2 == 0) {
                item->setText(QString("%1").arg(i*3 + j + 1, 3, 10, QChar('0')));
            }
        }
    }
    //    connect(view, SIGNAL(cellChanged(int, int)), this, SLOT(autoInput(int, int)));
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    view->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    view->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    //    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    for (int i=0; i < 12; i++) {
        if (i%2 == 0) {
            view->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Fixed);
            view->setColumnWidth(i, 36);
        } else {
            view->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
        }
        view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    }
#endif
    boxLayout->addWidget(view);
}

void TypSetDcr::initButtons()
{
    QHBoxLayout *layout = new QHBoxLayout;
    boxLayout->addLayout(layout);

    QPushButton *prev = new QPushButton(this);
    prev->setFixedSize(97, 40);
    prev->setText(tr("上一页"));
    layout->addWidget(prev);
    connect(prev, SIGNAL(clicked(bool)), this, SLOT(clickButtons()));

    QLineEdit *page = new QLineEdit(this);
    page->setText("1");
    page->setFixedSize(50, 36);
    page->setAlignment(Qt::AlignCenter);
    layout->addWidget(page);

    QPushButton *next = new QPushButton(this);
    next->setFixedSize(97, 40);
    next->setText(tr("下一页"));
    layout->addWidget(next);
    connect(next, SIGNAL(clicked(bool)), this, SLOT(clickButtons()));

    layout->addStretch();

    QPushButton *btnCell = new QPushButton(this);
    btnCell->setFixedSize(97, 40);
    btnCell->setText(tr("采样"));
    layout->addWidget(btnCell);

    QPushButton *btnAdd = new QPushButton(this);
    btnAdd->setFixedSize(97, 40);
    btnAdd->setText(tr("保存"));
    layout->addWidget(btnAdd);
    //    connect(btnAdd, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
}

void TypSetDcr::clickButtons()
{
}

