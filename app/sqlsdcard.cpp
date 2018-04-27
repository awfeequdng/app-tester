/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       历史数据
*******************************************************************************/
#include "sqlsdcard.h"

SqlSdcard::SqlSdcard(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void SqlSdcard::initUI()
{
    initLayout();
    initViewBar();
    initButtonBar();
}

void SqlSdcard::initLayout()
{
    boxLayout = new QVBoxLayout;

    QGroupBox *group = new QGroupBox(this);
    group->setTitle(tr("SD卡数据"));
    group->setLayout(boxLayout);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(group);
}

void SqlSdcard::initViewBar()
{
    mView = new QFileSystemModel(this);
    mView->setFilter(QDir::Files | QDir::NoDotAndDotDot);

    view = new QTableView(this);
    view->clearSelection();
    view->setModel(mView);
    view->verticalHeader()->setVisible(false);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    view->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
#ifdef __arm__
    view->setRootIndex(mView->setRootPath("/mnt/sdcard"));
#else
    view->setRootIndex(mView->setRootPath("/home/link/"));
#endif
    boxLayout->addWidget(view);
}

void SqlSdcard::initButtonBar()
{
    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addStretch();

    QPushButton *btnDelete = new QPushButton(this);
    btnDelete->setFixedSize(97, 44);
    btnDelete->setText(tr("删除当前"));
    connect(btnDelete, SIGNAL(clicked(bool)), this, SLOT(recvDelete()));
    btnLayout->addWidget(btnDelete);

    QPushButton *btnExport = new QPushButton(this);
    btnExport->setFixedSize(97, 44);
    btnExport->setText(tr("导出当前"));
    connect(btnExport, SIGNAL(clicked(bool)), this, SLOT(recvExport()));
    btnLayout->addWidget(btnExport);

    boxLayout->addLayout(btnLayout);
}

void SqlSdcard::recvExport()
{
    int row = view->currentIndex().row();
    if (row < 0)
        return;
    QString path = mView->filePath(view->currentIndex());
    tmpMap.insert("enum", Qt::Key_Excel);
    tmpMap.insert("path", path);
    emit sendAppMap(tmpMap);
    tmpMap.clear();
}

void SqlSdcard::recvDelete()
{
    int row = view->currentIndex().row();
    if (row < 0)
        return;
    QString path = mView->filePath(view->currentIndex());
    QFile file(path);
    file.remove();
}

