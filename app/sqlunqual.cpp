#include "sqlunqual.h"

SqlUnqual::SqlUnqual(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void SqlUnqual::initUI()
{
    initLayout();
    initViewBar();
    initTextBar();
}

void SqlUnqual::initLayout()
{
    boxLayout = new QVBoxLayout;

    QGroupBox *group = new QGroupBox(this);
    group->setTitle(tr("不良分析"));
    group->setLayout(boxLayout);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(group);
}

void SqlUnqual::initViewBar()
{
    view = new QTableView(this);
    view->setSortingEnabled(true);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    view->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    view->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
    boxLayout->addWidget(view);
}

void SqlUnqual::initTextBar()
{
    QHBoxLayout *btnLayout = new QHBoxLayout;

    stop = new QDateEdit(this);
    stop->setFixedSize(125, 44);
    stop->setDate(QDate::currentDate());
    stop->setDisplayFormat("yyyy-MM-dd");
    btnLayout->addWidget(new QLabel(tr("最近日期"), this));
    btnLayout->addWidget(stop);

    btnLayout->addStretch();

    QPushButton *btnSelect = new QPushButton(this);
    btnSelect->setText(tr("重新查询"));
    btnSelect->setFixedSize(97, 44);
    btnLayout->addWidget(btnSelect);
    connect(btnSelect, SIGNAL(clicked(bool)), this, SLOT(selectSqlite()));

    boxLayout->addLayout(btnLayout);
}

void SqlUnqual::createSqlite()
{
    QSqlQuery query(QSqlDatabase::database(sqlName));

    QString cmd;
    cmd = "create table if not exists sql_record (";
    cmd += "id integer primary key, date text, prod integer)";
    if (!query.exec(cmd)) {
        qDebug() << "create table sql_record Error";
    }

    if (!query.exec("delete from sql_record")) {
        qDebug() << "delete table sql_record" << query.lastError();
    }

    query.finish();
    query.clear();
}

void SqlUnqual::updateSqlite()
{
    mView = new QSqlTableModel(this, QSqlDatabase::database(sqlName));
    mView->setTable("sql_record");
    mView->setSort(0, Qt::DescendingOrder);
    mView->select();

    if (mView->columnCount() >= 2) {
        view->setModel(mView);
        view->hideColumn(0);
    }
}

void SqlUnqual::insertSqlite()
{
    QDateTime t1;
    QDateTime t2;
    t1.setDate(stop->date().addDays(1));
    t2.setDate(stop->date().addDays(0));
    QSqlQuery query(QSqlDatabase::database(sqlName));

    QSqlDatabase::database(sqlName).transaction();
    int c = 0;
    for (int i=0; i < 15; i++) {
        query.prepare("select count(*) from aip_record where uuid < ? and uuid > ? and numb=0");
        query.addBindValue(quint64(t1.toMSecsSinceEpoch()) << 20);
        query.addBindValue(quint64(t2.toMSecsSinceEpoch()) << 20);
        if (!query.exec()) {
            qDebug() << query.lastError();
        }
        if (query.next()) {
            c = query.value(0).toInt();
        }
        query.prepare("replace into sql_record values(?,?,?)");
        query.bindValue(0, quint64(t1.toMSecsSinceEpoch()) << 20);
        query.bindValue(1, t2.toString("yyyy-MM-dd"));
        query.bindValue(2, c);
        query.exec();
        t1.setDate(t1.date().addDays(-1));
        t2.setDate(t2.date().addDays(-1));
    }
    QSqlDatabase::database(sqlName).commit();
    query.finish();
    query.clear();
}

void SqlUnqual::selectSqlite()
{
    createSqlite();
    insertSqlite();
    mView->select();
}

void SqlUnqual::showEvent(QShowEvent *e)
{
//    mView->select();
    e->accept();
}

void SqlUnqual::recvSqlMap(QVariantMap msg)
{
    switch (msg.value("enum").toInt()) {
    case QMessageBox::Open:
//        sqlName = msg.value("recordName").toString();
//        createSqlite();
//        insertSqlite();
//        updateSqlite();
        break;
    default:
        break;
    }
}

