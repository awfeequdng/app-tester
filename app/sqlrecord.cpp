/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       数据管理
*******************************************************************************/
#include "sqlrecord.h"

SqlRecord::SqlRecord(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void SqlRecord::initUI()
{
    initLayout();
    initViewBar();
    initTextBar();
    initButtonBar();
}

void SqlRecord::initLayout()
{
    boxLayout = new QVBoxLayout;

    QGroupBox *group = new QGroupBox(this);
    group->setTitle(tr("近期数据"));
    group->setLayout(boxLayout);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(group);
}

void SqlRecord::initViewBar()
{

    QStringList header;
    header << tr("UUID") << tr("GUID") << tr("NUMB")
           << tr("测试日期") << tr("测试时间") << tr("测试型号") << tr("测试判定");
    mView = new QSqlTableModel(this, QSqlDatabase::database("record"));
    mView->setTable("aip_record");
    mView->setFilter("numb=0");
    mView->setSort(0, Qt::DescendingOrder);
    mView->select();
    if (mView->columnCount() >= header.size()) {
        for (int i=0; i < header.size(); i++)
            mView->setHeaderData(i, Qt::Horizontal, header.at(i));
    }

    view = new QTableView(this);
    view->setModel(mView);
    view->verticalHeader()->setVisible(false);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    view->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
    connect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(clickIndex(QModelIndex)));
    boxLayout->addWidget(view);
    boxLayout->setStretch(0, 1);


    QStringList items;
    items << tr("UUID") << tr("GUID") << tr("NUMB")
          << tr("测试项目") << tr("测试参数") << tr("测试结果") << tr("测试判定");
    mItem = new QSqlTableModel(this, QSqlDatabase::database("record"));
    mItem->setTable("aip_record");
    mItem->setFilter("numb>100");
    mItem->select();
    if (mItem->columnCount() >= header.size()) {
        for (int i=0; i < items.size(); i++)
            mItem->setHeaderData(i, Qt::Horizontal, items.at(i));
    }

    item = new QTableView(this);
    item->hide();
    item->setModel(mItem);
    item->verticalHeader()->setVisible(false);
    item->setSelectionMode(QAbstractItemView::SingleSelection);
    item->setSelectionBehavior(QAbstractItemView::SelectRows);
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    item->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    item->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
    boxLayout->addWidget(item);
    boxLayout->setStretch(1, 2);
}

void SqlRecord::initTextBar()
{
    QHBoxLayout *layout = new QHBoxLayout;
    boxLayout->addLayout(layout);

    type = new QComboBox(this);
    type->setEditable(true);
    type->setFixedSize(125, 44);
    layout->addWidget(new QLabel(tr("测试型号"), this));
    layout->addWidget(type);

    from = new QDateEdit(this);
    from->setFixedSize(125, 44);
    from->setDisplayFormat("yyyy-MM-dd");
    layout->addWidget(new QLabel(tr("起始日期"), this));
    layout->addWidget(from);

    stop = new QDateEdit(this);
    stop->setFixedSize(125, 44);
    stop->setDate(QDate::currentDate());
    stop->setDisplayFormat("yyyy-MM-dd");
    layout->addWidget(new QLabel(tr("结束日期"), this));
    layout->addWidget(stop);
}

void SqlRecord::initButtonBar()
{
    QHBoxLayout *layout = new QHBoxLayout;
    boxLayout->addLayout(layout);

    QPushButton *btnSelect = new QPushButton(this);
    btnSelect->setText(tr("查询数据"));
    btnSelect->setFixedSize(97, 44);
    layout->addWidget(btnSelect);
    connect(btnSelect, SIGNAL(clicked(bool)), this, SLOT(recvSelect()));

    QPushButton *btnDetail = new QPushButton(this);
    btnDetail->setText(tr("显示详细"));
    btnDetail->setFixedSize(97, 44);
    layout->addWidget(btnDetail);
    connect(btnDetail, SIGNAL(clicked(bool)), this, SLOT(recvDetail()));

    layout->addStretch();

    QPushButton *btnUpdate = new QPushButton(this);
    btnUpdate->setText(tr("导出当前"));
    btnUpdate->setFixedSize(97, 44);
    layout->addWidget(btnUpdate);
    connect(btnUpdate, SIGNAL(clicked(bool)), this, SLOT(recvExportDate()));

    QPushButton *btnExport = new QPushButton(this);
    btnExport->setText(tr("全部导出"));
    btnExport->setFixedSize(97, 44);
    layout->addWidget(btnExport);
    connect(btnExport, SIGNAL(clicked(bool)), this, SLOT(recvExportAll()));
}

void SqlRecord::recvSelect()
{
    QDateTime t;
    t.setDate(from->date());
    quint64 id_from = quint64(t.toMSecsSinceEpoch()) << 20;
    t.setDate(stop->date().addDays(1));
    quint64 id_stop = quint64(t.toMSecsSinceEpoch()) << 20;
    QString filter = tr("uuid>=%1 and uuid<=%2 and numb=0").arg(id_from).arg(id_stop);
    if (!type->currentText().isEmpty())
        filter.append(tr(" and rslt = '%1'").arg(type->currentText()));
    mView->setFilter(filter);
    mView->setSort(0, Qt::DescendingOrder);
    mView->select();
}

void SqlRecord::recvDetail()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (btn->text() == tr("显示详细")) {
        item->show();
        btn->setText(tr("隐藏详细"));
    } else {
        item->hide();
        btn->setText(tr("显示详细"));
    }
}

void SqlRecord::recvExportAll()
{
    tmpMap.insert("enum", Qt::Key_Excel);
    emit sendAppMap(tmpMap);
    tmpMap.clear();
}

void SqlRecord::recvExportDate()
{
    QDateTime t;
    t.setDate(from->date());
    quint64 id_from = quint64(t.toMSecsSinceEpoch()) << 20;
    t.setDate(stop->date().addDays(1));
    quint64 id_stop = quint64(t.toMSecsSinceEpoch()) << 20;
    tmpMap.insert("enum", Qt::Key_Excel);
    tmpMap.insert("from", id_from);
    tmpMap.insert("stop", id_stop);
    emit sendAppMap(tmpMap);
    tmpMap.clear();
}

void SqlRecord::clickIndex(QModelIndex index)
{
    if (item->isHidden())
        return;
    int row = index.row();
    if (row >= 0) {
        qint64 guid = mView->index(row, 0).data().toLongLong();
        mItem->setFilter(tr("guid=%1 and numb>100").arg(guid));
        mItem->select();
    }
}

void SqlRecord::showEvent(QShowEvent *e)
{
    this->setFocus();
    if (mView->columnCount() > 4) {
        view->hideColumn(0);
        view->hideColumn(1);
        view->hideColumn(2);
    }
    if (mItem->columnCount() > 4) {
        item->hideColumn(0);
        item->hideColumn(1);
        item->hideColumn(2);
    }
    e->accept();
}
