/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       权限管理
*******************************************************************************/
#include "appaction.h"

AppAction::AppAction(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void AppAction::initUI()
{
    initView();
    initLayout();
    initDelegate();
}

void AppAction::initView()
{
    QStringList headers;
    headers << tr("界面名称") << tr("界面权限") << tr("界面分组");

    view = new QTableWidget(this);
    view->setRowCount(A_ROW);
    view->setColumnCount(headers.size());
    view->setHorizontalHeaderLabels(headers);
    for (int i=0; i < headers.size(); i++) {
        for (int j=0; j < A_ROW; j++) {
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setText("");
            item->setTextAlignment(Qt::AlignCenter);
            view->setItem(j, i, item);
        }
    }
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    view->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    view->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
    view->setEditTriggers(QAbstractItemView::AllEditTriggers);
    view->verticalHeader()->hide();
}

void AppAction::initLayout()
{
    QHBoxLayout *btnLayout = new QHBoxLayout;

    btnLayout->addStretch();

    QPushButton *save = new QPushButton(this);
    save->setFixedSize(97, 40);
    save->setText(tr("保存"));
    btnLayout->addWidget(save);
    connect(save, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));

    QVBoxLayout *boxLayout = new QVBoxLayout;
    boxLayout->addWidget(view);
    boxLayout->addLayout(btnLayout);

    QGroupBox *box = new QGroupBox(this);
    box->setLayout(boxLayout);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(box);
}

void AppAction::initDelegate()
{
    roles << tr("管理员") << tr("技术员") << tr("操作员") << tr("所有人");

    BoxQCombo *group = new BoxQCombo;
    group->setItemNames(roles);

    view->setItemDelegateForColumn(0, new BoxQItems);
    view->setItemDelegateForColumn(1, group);
    view->setItemDelegateForColumn(2, new BoxQItems);
}

void AppAction::initSettings()
{
    int s = tmpSet[(2000 + Qt::Key_6)].toInt();
    for (int i=0; i < A_ROW; i++) {
        int t = s + i*4;
        int r = tmpSet[t + AddrRole].toInt();
        if (r > 0) {
            r = qMax(1, r);
            r = qMin(r, roles.size());
            view->item(i, 0)->setText(tmpSet[t + AddrMark].toString());
            view->item(i, 1)->setText(roles.at(r - 1));
            view->item(i, 2)->setText(tmpSet[t + AddrForm].toString());
        } else {
            view->hideRow(i);
        }
    }
}

void AppAction::saveSettings()
{
    int s = (2000 + Qt::Key_6);
    for (int i=0; i < A_ROW; i++) {
        int t = s + i*4;
        QString role = view->item(i, 1)->text();
        if (!role.isEmpty()) {
            tmpSet[t + AddrRole] = QString::number(roles.indexOf(role) + 1);
        }
    }
    tmpSet.insert(Qt::Key_0, Qt::Key_Save);
    tmpSet.insert(Qt::Key_1, "aip_system");
    emit sendAppMsg(tmpSet);
}

void AppAction::recvAppMsg(QTmpMap msg)
{
    switch (msg.value(Qt::Key_0).toInt()) {
    case Qt::Key_Copy:
        tmpSet = msg;
        break;
    default:
        break;
    }
}

void AppAction::showEvent(QShowEvent *e)
{
    this->setFocus();
    initSettings();
    e->accept();
}

