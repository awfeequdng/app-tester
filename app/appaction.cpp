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
    headers << tr("界面名称") << tr("界面名称") << tr("界面组别") << tr("界面权限");

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
    view->hideColumn(0);
    view->setEditTriggers(QAbstractItemView::AllEditTriggers);
}

void AppAction::initLayout()
{
    QHBoxLayout *btnLayout = new QHBoxLayout;

    QPushButton *prev = new QPushButton(this);
    prev->setFixedSize(97, 40);
    prev->setText(tr("上一页"));
    btnLayout->addWidget(prev);
    connect(prev, SIGNAL(clicked(bool)), this, SLOT(clickButtons()));

    page = new QLineEdit(this);
    page->setText("1");
    page->setFixedSize(50, 36);
    page->setAlignment(Qt::AlignCenter);
    btnLayout->addWidget(page);

    QPushButton *next = new QPushButton(this);
    next->setFixedSize(97, 40);
    next->setText(tr("下一页"));
    btnLayout->addWidget(next);
    connect(next, SIGNAL(clicked(bool)), this, SLOT(clickButtons()));

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
    groups << tr("管理员") << tr("技术员") << tr("操作员") << tr("所有人");

    BoxQCombo *group = new BoxQCombo;
    group->setItemNames(groups);

    view->setItemDelegateForColumn(0, new BoxQItems);
    view->setItemDelegateForColumn(1, new BoxQItems);
    view->setItemDelegateForColumn(2, new BoxQItems);
    view->setItemDelegateForColumn(3, group);
}

void AppAction::initSettings()
{
    int p = page->text().toInt() - 1;               // 页码
    int s = AddrAction + A_ROW*A_COL*p+0x40;       // 起始地址,隐藏通用界面
    for (int i=0; i < A_ROW; i++) {
        int t = s + A_COL*i;
        view->item(i, 0)->setText(config[QString::number(t+0)].toString());
        view->item(i, 1)->setText(config[QString::number(t+1)].toString());
        view->item(i, 2)->setText(config[QString::number(t+2)].toString());
        if (!config[QString::number(t+0)].toString().isEmpty()) {
            QString usrgroup = groups.at(config[QString::number(t+3)].toInt()-1);
            view->item(i, 3)->setText(usrgroup);
        } else {
            view->item(i, 3)->setText("");
        }
    }
}

void AppAction::saveSettings()
{
    int p = page->text().toInt() - 1;               // 页码
    int s = AddrAction + A_ROW*A_COL*p + 0x40;     // 起始地址,隐藏通用界面
    for (int i=0; i < A_ROW; i++) {
        int t = s + A_COL*i;
        QString username = view->item(i, 0)->text();
        QString action = view->item(i, 3)->text();
        if (!username.isEmpty() && !action.isEmpty()) {
            config[QString::number(t + 3)] = QString::number(groups.indexOf(action) + 1);
        }
    }
    config.insert("enum", Qt::Key_Save);
    emit sendAppMap(config);
}

void AppAction::clickButtons()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    int p = page->text().toInt();
    if (btn->text() == tr("上一页")) {
        p--;
    }
    if (btn->text() == tr("下一页")) {
        p++;
    }
    p = (p >= 1) ? p : 1;
    p = (p <= 3) ? p : 3;

    page->setText(QString::number(p));
    initSettings();
}

void AppAction::recvAppMap(QVariantMap msg)
{
    switch (msg.value("enum").toInt()) {
    case Qt::Key_Option:
        for (int i=AddrAction; i < AddrAction+0x0100; i++) {
            config[QString::number(i)] = msg[QString::number(i)];
        }
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

