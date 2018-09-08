/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       权限管理
*******************************************************************************/
#include "apppermit.h"

const int pMark = 0x00;  // 界面名称
const int pRole = 0x01;  // 界面权限
const int pForm = 0x02;  // 界面分组
const int pSize = 0x10;  // 每组界面数

AppPermit::AppPermit(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void AppPermit::initUI()
{
    initLayout();
    initViewBar();
    initButtonBar();
    initItemDelegate();
}

void AppPermit::initLayout()
{
    QVBoxLayout *mlayout = new QVBoxLayout(this);
    QGroupBox *box = new QGroupBox(this);
    mlayout->addWidget(box);
    layout = new QVBoxLayout;
    box->setLayout(layout);
}

void AppPermit::initViewBar()
{
    QStringList headers;
    headers << tr("界面名称") << tr("界面权限") << tr("界面分组");

    view = new QTableWidget(this);
    view->setColumnCount(headers.size());
    view->setHorizontalHeaderLabels(headers);
    view->verticalHeader()->setVisible(false);
    view->horizontalHeader()->setFixedHeight(30);
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    view->verticalHeader()->setResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
    connect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(autoChange()));
    layout->addWidget(view);
}

void AppPermit::initButtonBar()
{
    QHBoxLayout *blayout = new QHBoxLayout;
    layout->addLayout(blayout);

    QString str = tr("权限等级: 管理员 > 技术员 > 操作员 > 所有人;");
    blayout->addWidget(new QLabel(str, this));

    blayout->addStretch();

    QPushButton *btnSave = new QPushButton(this);
    btnSave->setFixedSize(97, 40);
    btnSave->setText(tr("保存"));
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
    blayout->addWidget(btnSave);
}

void AppPermit::initItemDelegate()
{
    isInit = false;
    gRole << tr("超级管理员") << tr("管理员") << tr("技术员") << tr("操作员") << tr("所有人");
    view->setItemDelegate(new BoxQItems);
}

void AppPermit::initSettings()
{
    isInit = false;
    int temp = tmpSet.value(3000 + Qt::Key_0).toInt();  // 临时数据地址
    int sign = tmpSet.value(temp + TEMPSIGN).toInt();  // 是否已登录
    int real = tmpSet.value(DataUser).toInt();  // 当前用户地址
    int gear = (sign == 1) ? tmpSet.value(real + mRole).toInt() : 4;  // 当前权限等级
    int addr = tmpSet.value(2000 + Qt::Key_6).toInt();  // 权限管理地址
    view->setRowCount(screen.keys().size());
    for (int i=0; i < screen.keys().size(); i++) {
        QVariantMap tmp = screen.value(i).toMap();
        int form = tmp.value("form").toInt();  // 界面分组
        int numb = tmp.value("numb").toInt();  // 界面编号
        int role = tmpSet.value(addr + form*pSize + numb).toInt();
        for (int t = 0; t < view->columnCount(); t++) {
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setTextAlignment(Qt::AlignCenter);
            view->setItem(i, t, item);
            if (t == 0)
                item->setText(tmp.value("mark").toString());  // 界面名称
            if (t == 1)
                item->setText(gRole.at(role % gRole.size()));  // 界面权限
            if (t == 2)
                item->setText(QString::number(form));  // 界面分组
        }
        if (role < gear)
            view->hideRow(i);  // 隐藏后台界面
        else
            view->showRow(i);
    }
    isInit = true;
}

void AppPermit::saveSettings()
{
    int addr = tmpSet.value(2000 + Qt::Key_6).toInt();  // 权限管理地址
    for (int i=0; i < screen.keys().size(); i++) {
        QString str = view->item(i, pRole)->text();  // 权限名称
        QVariantMap tmp = screen.value(i).toMap();
        int form = tmp.value("form").toInt();
        int numb = tmp.value("numb").toInt();
        int role = gRole.indexOf(str);
        tmpMsg.insert(addr + form*pSize + numb, role);
    }
    tmpMsg.insert(Qt::Key_0, Qt::Key_Save);
    tmpMsg.insert(Qt::Key_1, "aip_system");
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void AppPermit::autoChange()
{
    int row = view->currentRow();
    int col = view->currentColumn();
    if (col == pRole && isInit) {
        int temp = tmpSet.value(3000 + Qt::Key_0).toInt();  // 临时数据地址
        int sign = tmpSet.value(temp + TEMPSIGN).toInt();  // 是否已登录
        int real = tmpSet.value(DataUser).toInt();  // 当前用户地址
        int gear = (sign == 1) ? tmpSet.value(real + mRole).toInt() : 4;  // 当前权限等级
        QString str = view->item(row, col)->text();
        int role = qMax(gear, (gRole.indexOf(str) + 1) % gRole.size());
        str = (gRole.at(role % gRole.size()) );
        view->item(row, col)->setText(str);
        change();
    }
}

void AppPermit::change()
{
    if (isInit) {  // 初始化完成后才发送界面修改
        tmpMsg.insert(Qt::Key_0, Qt::Key_Plus);
        tmpMsg.insert(Qt::Key_1, this->objectName());
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
    }
}

void AppPermit::recvAppMsg(QTmpMap msg)
{
    switch (msg.value(Qt::Key_0).toInt()) {
    case Qt::Key_Copy:
        tmpSet = msg;
        break;
    case Qt::Key_Memo:
        screen = msg;
        screen.remove(Qt::Key_0);
        break;
    case Qt::Key_Save:
        if (!this->isHidden())
            saveSettings();
        break;
    default:
        break;
    }
}

void AppPermit::showEvent(QShowEvent *e)
{
    initSettings();
    e->accept();
}

void AppPermit::hideEvent(QHideEvent *e)
{
    isInit = false;
    e->accept();
}


