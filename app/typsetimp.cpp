#include "typsetimp.h"

TypSetImp::TypSetImp(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void TypSetImp::initUI()
{
    initLayout();
    initContent();
    initWaveBar();
    drawImpWave();
    initButtons();
}

void TypSetImp::initLayout()
{
    boxLayout = new QVBoxLayout(this);

    QGroupBox *box = new QGroupBox(this);
    box->setLayout(boxLayout);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(box);
}

void TypSetImp::initContent()
{
    QGroupBox *box = new QGroupBox(this);
    boxLayout->addWidget(box);

    QVBoxLayout *layout = new QVBoxLayout;
    box->setLayout(layout);

    QCheckBox *imp = new QCheckBox(this);
    imp->setText(tr("匝间测试"));
    layout->addWidget(imp);

    layout->addStretch();

    QHBoxLayout *box1 = new QHBoxLayout;
    layout->addLayout(box1);
    box1->addWidget(new QLabel(tr("设定电压"), this));
    QLineEdit *volt = new QLineEdit(this);
    box1->addWidget(volt);
    box1->addWidget(new QLabel(tr("kV"), this));
    box1->addStretch();
    box1->addWidget(new QLabel(tr("冲击次数"), this));
    QLineEdit *time = new QLineEdit(this);
    box1->addWidget(time);
    box1->addWidget(new QLabel(tr("次"), this));
    box1->addStretch();

    QHBoxLayout *box2 = new QHBoxLayout;
    layout->addLayout(box2);
    box2->addWidget(new QLabel(tr("冲击方式"), this));
    QComboBox *mode = new QComboBox(this);
    box2->addWidget(mode);
    box2->addStretch();

    QHBoxLayout *box3 = new QHBoxLayout;
    layout->addLayout(box3);
    box3->addWidget(new QLabel(tr("冲击隔片"), this));
    QLineEdit *count = new QLineEdit(this);
    box3->addWidget(count);
    box3->addStretch();
    box3->addWidget(new QLabel(tr("冲击退格"), this));
    QLineEdit *tmp3 = new QLineEdit(this);
    box3->addWidget(tmp3);
    box3->addStretch();

    QHBoxLayout *box4 = new QHBoxLayout;
    layout->addLayout(box4);
    box4->addWidget(new QLabel(tr("合格上限"), this));
    QLineEdit *max = new QLineEdit(this);
    box4->addWidget(max);
    box4->addWidget(new QLabel(tr("(%)"), this));
    box4->addStretch();
    box4->addWidget(new QLabel(tr("判断方式"), this));
    QComboBox *test = new QComboBox(this);
    box4->addWidget(test);
    QLineEdit *tmp2 = new QLineEdit(this);
    box4->addWidget(tmp2);
    box4->addWidget(new QLabel(tr("次"), this));
    box4->addStretch();

    layout->addStretch();
}

void TypSetImp::initWaveBar()
{
    QGroupBox *box = new QGroupBox(this);
    box->setFixedHeight(200);
    boxLayout->addWidget(box);

    QVBoxLayout *layout = new QVBoxLayout;
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
    QGroupBox *box = new QGroupBox(this);
    boxLayout->addWidget(box);

    QHBoxLayout *layout = new QHBoxLayout;
    box->setLayout(layout);

    layout->addWidget(new QLabel(tr("采样方式"), this));
    QComboBox *mode = new QComboBox(this);
    layout->addWidget(mode);

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

