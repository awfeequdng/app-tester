/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       系统设置
*******************************************************************************/
#include "appsystem.h"

AppSystem::AppSystem(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void AppSystem::initUI()
{
    initLayout();
    initSystem();
    initDevice();
    initButton();
    initDelegate();
}

void AppSystem::initLayout()
{
    layout = new QHBoxLayout;

    boxLayout = new QVBoxLayout(this);
    boxLayout->addLayout(layout);
}

void AppSystem::initSystem()
{
    QGroupBox *group = new QGroupBox(this);
    group->setTitle(tr("系统设置"));
    layout->addWidget(group);

    QVBoxLayout *boxLayout = new QVBoxLayout;
    group->setLayout(boxLayout);

    QStringList names;
    names << tr("语言设置") << tr("测试模式") << tr("启动方式") << tr("亮度设定")
          << tr("音量设定") << tr("条码起始") << tr("条码长度") << tr("合格报警")
          << tr("报警提示") << tr("气泵延时");
    for (int i=0; i < names.size(); i++) {
        QHBoxLayout *box = new QHBoxLayout;
        boxLayout->addLayout(box);
        QString tmp = QString("%1").arg(i+1, 2, 10, QChar('0'));
        box->addWidget(new QLabel(tmp + " " + names.at(i) + ":", this));
        QComboBox *input = new QComboBox(this);
        input->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        input->setView(new QListView);
        input->setFixedHeight(35);
        if (i > names.indexOf(tr("音量设定")))
            input->setEditable(true);
        box->addWidget(input);
        texts.append(input);
    }
    boxLayout->addStretch();
}

void AppSystem::initDevice()
{
    QGroupBox *group = new QGroupBox(this);
    group->setTitle(tr("本机设置"));
    layout->addWidget(group);

    QVBoxLayout *boxLayout = new QVBoxLayout;
    group->setLayout(boxLayout);

    QStringList names;
    names << tr("自动地址") << tr("默认网关") << tr("网络地址") << tr("子网掩码")
          << tr("挂载地址") << tr("挂载路径") << tr("自动执行") << tr("执行参数");

    for (int i=0; i < names.size(); i++) {
        QHBoxLayout *box = new QHBoxLayout;
        boxLayout->addLayout(box);
        QString tmp = QString("%1").arg(i+1, 2, 10, QChar('0'));
        box->addWidget(new QLabel(tmp + " " + names.at(i) + ":", this));
        QLineEdit *input = new QLineEdit(this);
        input->setFixedHeight(35);
        box->addWidget(input);
        ctrls.append(input);
    }
    boxLayout->addStretch();
}

void AppSystem::initButton()
{
    QHBoxLayout *btn = new QHBoxLayout;
    boxLayout->addLayout(btn);
#ifdef __arm__
    btn->addWidget(new QLabel(tr("系统时间"), this));

    time = new QDateTimeEdit(this);
    time->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
    time->setFixedSize(200, 40);
    btn->addWidget(time);

    QPushButton *btnTime = new QPushButton(this);
    btnTime->setText(tr("设置"));
    btnTime->setFixedSize(97, 44);
    btn->addWidget(btnTime);
    connect(btnTime, SIGNAL(clicked(bool)), this, SLOT(setLocalTime()));
#endif
    btn->addStretch();

    QPushButton *btnSave = new QPushButton(this);
    btnSave->setText(tr("保存"));
    btnSave->setFixedSize(97, 44);
    btn->addWidget(btnSave);
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
}

void AppSystem::initDelegate()
{
    QStringList tmp0;
    tmp0 << tr("中文");
    texts.at(0)->addItems(tmp0);

    QStringList tmp1;
    tmp1 << tr("常规模式");
    texts.at(1)->addItems(tmp1);

    QStringList tmp2;
    tmp2 << tr("按钮启动") << tr("滑罩启动") << tr("脚踏启动") << tr("网络启动");
    texts.at(2)->addItems(tmp2);

    QStringList tmp3;
    tmp3 << "0" << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9";
    texts.at(3)->addItems(tmp3);
    texts.at(4)->addItems(tmp3);
}

void AppSystem::initSettings()
{
    int r = tmpSet[(2000 + Qt::Key_1)].toInt();
    for (int i=0; i < texts.size(); i++) {  // 系统配置存放在0x0020
        if (i < 5) {
            texts.at(i)->setCurrentIndex(tmpSet[r + i].toInt());
        } else {
            texts.at(i)->lineEdit()->setText(tmpSet[r + i].toString());
        }
    }
    int s = tmpSet[(2000 + Qt::Key_2)].toInt();
    for (int i=0; i < ctrls.size(); i++) {  // 本机设置存放在0x0030
        ctrls.at(i)->setText(tmpSet[s + i].toString());
    }
#ifdef __arm__
    time->setDateTime(QDateTime::currentDateTime());
#endif
}

void AppSystem::saveSettings()
{
    int r = tmpSet[(2000 + Qt::Key_1)].toInt();
    for (int i=0; i < texts.size(); i++) {  // 系统配置存放在0x0020
        if (i < 5) {
            tmpSet[r + i] = QString::number(texts.at(i)->currentIndex());
        } else {
            tmpSet[r + i] = texts.at(i)->currentText();
        }
    }
    int s = tmpSet[(2000 + Qt::Key_2)].toInt();
    for (int i=0; i < ctrls.size(); i++) {  // 本机设置存放在0x0030
        tmpSet[s + i] = ctrls.at(i)->text();
    }
    tmpSet.insert(Qt::Key_0, Qt::Key_Save);
    tmpSet.insert(Qt::Key_1, "aip_system");
    emit sendAppMsg(tmpSet);

    QSettings *set = new QSettings("./nandflash/userinfo.txt", QSettings::IniFormat);
    set->beginGroup("LOCAL_MACHINE");
    set->setValue("DHCP", ctrls.at(0)->text());
    set->setValue("DefaultGateway", ctrls.at(1)->text());
    set->setValue("IPAddress", ctrls.at(2)->text());
    set->setValue("SubnetMask", ctrls.at(3)->text());
    set->endGroup();
    set->beginGroup("NFS_SERVER");
    set->setValue("IPAddress", ctrls.at(4)->text());
    set->setValue("Mountpath", ctrls.at(5)->text());
    set->endGroup();
    set->beginGroup("USER_EXE");
    set->setValue("Name", ctrls.at(6)->text());
    set->setValue("Parameters", ctrls.at(7)->text());
    set->endGroup();
}

void AppSystem::setLocalTime()
{
    tmpMsg.insert(Qt::Key_0, Qt::Key_Time);
    tmpMsg.insert(Qt::Key_4, time->dateTime());
    tmpMsg.insert(Qt::Key_1, "time");
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void AppSystem::recvAppMsg(QTmpMap msg)
{
    switch (msg.value(Qt::Key_0).toInt()) {
    case Qt::Key_Copy:
        tmpSet = msg;
        break;
    default:
        break;
    }
}

void AppSystem::showEvent(QShowEvent *e)
{
    this->setFocus();
    initSettings();
    e->accept();
}

