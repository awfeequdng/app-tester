#include "typsetacw.h"

TypSetAcw::TypSetAcw(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void TypSetAcw::initUI()
{
    initLayout();
    initBoxInrAG();
    initBoxAcwAC();
    initBoxAcwAL();
    initBoxAcwLC();
    initButtons();
}

void TypSetAcw::initLayout()
{
    boxLayout = new QVBoxLayout(this);
}

void TypSetAcw::initBoxInrAG()
{
    QGroupBox *box = new QGroupBox(this);
    boxLayout->addWidget(box);

    QGridLayout *layout = new QGridLayout;
    box->setLayout(layout);

    QCheckBox *boxInrAG = new QCheckBox(this);
    boxInrAG->setText(tr("绝缘电阻"));
    layout->addWidget(boxInrAG, 0, 0, 1, 4);
    boxCheck.append(boxInrAG);

    layout->addWidget(new QLabel(tr("电压"), this), 1, 0);
    QLineEdit *volt = new QLineEdit(this);
    layout->addWidget(volt, 1, 1);
    layout->addWidget(new QLabel(tr("kV"), this), 1, 2);

    layout->addWidget(new QLabel(tr("下限"), this), 1, 3);
    QLineEdit *min = new QLineEdit(this);
    layout->addWidget(min, 1, 4);
    layout->addWidget(new QLabel(tr("MΩ"), this), 1, 5);

    layout->addWidget(new QLabel(tr("时间"), this), 2, 0);
    QLineEdit *time = new QLineEdit(this);
    layout->addWidget(time, 2, 1);
    layout->addWidget(new QLabel(tr("s"), this), 2, 2);

    layout->addWidget(new QLabel(tr("补偿"), this), 2, 3);
    QLineEdit *offset = new QLineEdit(this);
    layout->addWidget(offset, 2, 4);
    layout->addWidget(new QLabel(tr("MΩ"), this), 2, 5);
}

void TypSetAcw::initBoxAcwAC()
{
    QGroupBox *box = new QGroupBox(this);
    boxLayout->addWidget(box);

    QGridLayout *layout = new QGridLayout;
    box->setLayout(layout);

    QCheckBox *boxInrAG = new QCheckBox(this);
    boxInrAG->setText(tr("轴铁耐压"));
    layout->addWidget(boxInrAG, 0, 0, 1, 4);
    boxCheck.append(boxInrAG);

    layout->addWidget(new QLabel(tr("电压"), this), 1, 0);
    QLineEdit *volt = new QLineEdit(this);
    layout->addWidget(volt, 1, 1);
    layout->addWidget(new QLabel(tr("kV"), this), 1, 2);

    layout->addWidget(new QLabel(tr("下限"), this), 1, 3);
    QLineEdit *min = new QLineEdit(this);
    layout->addWidget(min, 1, 4);
    layout->addWidget(new QLabel(tr("MΩ"), this), 1, 5);

    layout->addWidget(new QLabel(tr("时间"), this), 2, 0);
    QLineEdit *time = new QLineEdit(this);
    layout->addWidget(time, 2, 1);
    layout->addWidget(new QLabel(tr("s"), this), 2, 2);

    layout->addWidget(new QLabel(tr("补偿"), this), 2, 3);
    QLineEdit *offset = new QLineEdit(this);
    layout->addWidget(offset, 2, 4);
    layout->addWidget(new QLabel(tr("MΩ"), this), 2, 5);
}

void TypSetAcw::initBoxAcwAL()
{
    QGroupBox *box = new QGroupBox(this);
    boxLayout->addWidget(box);

    QGridLayout *layout = new QGridLayout;
    box->setLayout(layout);

    QCheckBox *boxInrAG = new QCheckBox(this);
    boxInrAG->setText(tr("轴线耐压"));
    layout->addWidget(boxInrAG, 0, 0, 1, 4);
    boxCheck.append(boxInrAG);

    layout->addWidget(new QLabel(tr("电压"), this), 1, 0);
    QLineEdit *volt = new QLineEdit(this);
    layout->addWidget(volt, 1, 1);
    layout->addWidget(new QLabel(tr("kV"), this), 1, 2);

    layout->addWidget(new QLabel(tr("下限"), this), 1, 3);
    QLineEdit *min = new QLineEdit(this);
    layout->addWidget(min, 1, 4);
    layout->addWidget(new QLabel(tr("MΩ"), this), 1, 5);

    layout->addWidget(new QLabel(tr("时间"), this), 2, 0);
    QLineEdit *time = new QLineEdit(this);
    layout->addWidget(time, 2, 1);
    layout->addWidget(new QLabel(tr("s"), this), 2, 2);

    layout->addWidget(new QLabel(tr("补偿"), this), 2, 3);
    QLineEdit *offset = new QLineEdit(this);
    layout->addWidget(offset, 2, 4);
    layout->addWidget(new QLabel(tr("MΩ"), this), 2, 5);
}

void TypSetAcw::initBoxAcwLC()
{
    QGroupBox *box = new QGroupBox(this);
    boxLayout->addWidget(box);

    QGridLayout *layout = new QGridLayout;
    box->setLayout(layout);

    QCheckBox *boxInrAG = new QCheckBox(this);
    boxInrAG->setText(tr("线铁耐压"));
    layout->addWidget(boxInrAG, 0, 0, 1, 4);
    boxCheck.append(boxInrAG);

    layout->addWidget(new QLabel(tr("电压"), this), 1, 0);
    QLineEdit *volt = new QLineEdit(this);
    layout->addWidget(volt, 1, 1);
    layout->addWidget(new QLabel(tr("kV"), this), 1, 2);

    layout->addWidget(new QLabel(tr("下限"), this), 1, 3);
    QLineEdit *min = new QLineEdit(this);
    layout->addWidget(min, 1, 4);
    layout->addWidget(new QLabel(tr("MΩ"), this), 1, 5);

    layout->addWidget(new QLabel(tr("时间"), this), 2, 0);
    QLineEdit *time = new QLineEdit(this);
    layout->addWidget(time, 2, 1);
    layout->addWidget(new QLabel(tr("s"), this), 2, 2);

    layout->addWidget(new QLabel(tr("补偿"), this), 2, 3);
    QLineEdit *offset = new QLineEdit(this);
    layout->addWidget(offset, 2, 4);
    layout->addWidget(new QLabel(tr("MΩ"), this), 2, 5);
}

void TypSetAcw::initButtons()
{
    QGroupBox *box = new QGroupBox(this);
    boxLayout->addWidget(box);

    QHBoxLayout *layout = new QHBoxLayout;
    box->setLayout(layout);
    layout->addStretch();

    QPushButton *btnAdd = new QPushButton(this);
    btnAdd->setFixedSize(97, 40);
    btnAdd->setText(tr("保存"));
    layout->addWidget(btnAdd);
    connect(btnAdd, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
}

void TypSetAcw::initSettings()
{
    for (int i=0; i < boxCheck.size(); i++) {
        boxCheck.at(i)->setChecked(config[QString::number(0x0470+i*0x10)] == 1 ? true : false);
    }
}

void TypSetAcw::saveSettings()
{
    for (int i=0; i < boxCheck.size(); i++) {
        config[QString::number(0x0470+i*0x10)] = boxCheck.at(i)->isChecked() ? 1 : 0;
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

