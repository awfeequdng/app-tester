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
    view = new BoxQLabel(this);
    boxLayout->addWidget(view);
    QVector<double> value(27);
    for (int i=0; i < 9; i++) {
        value[i*3 + 0] = 0;
        value[i*3 + 1] = 0;
        value[i*3 + 2] = 0;
    }
    view->setData(value, 3);
}

void SqlRecord::initTextBar()
{
    QHBoxLayout *layout = new QHBoxLayout;
    boxLayout->addLayout(layout);

    type = new QComboBox(this);
    type->setEditable(true);
    type->setFixedHeight(40);
    layout->addWidget(new QLabel(tr("测试型号"), this));
    layout->addWidget(type);

    from = new QDateEdit(this);
    from->setFixedHeight(40);
    from->setDisplayFormat("yyyy-MM-dd");
    layout->addWidget(new QLabel(tr("起始日期"), this));
    layout->addWidget(from);

    stop = new QDateEdit(this);
    stop->setFixedHeight(40);
    stop->setDate(QDate::currentDate());
    stop->setDisplayFormat("yyyy-MM-dd");
    layout->addWidget(new QLabel(tr("结束日期"), this));
    layout->addWidget(stop);

    layout->addStretch();

    QPushButton *btnSelect = new QPushButton(this);
    btnSelect->setText(tr("查询数据"));
    btnSelect->setFixedSize(97, 44);
    layout->addWidget(btnSelect);
    connect(btnSelect, SIGNAL(clicked(bool)), this, SLOT(recvSelect()));
}

void SqlRecord::initButtonBar()
{
    timeOut = 0;

    QHBoxLayout *layout = new QHBoxLayout;
    boxLayout->addLayout(layout);
#ifdef __arm__
    QPushButton *btnExists = new QPushButton(this);
    btnExists->setText(tr("查询优盘"));
    btnExists->setFixedSize(97, 44);
    layout->addWidget(btnExists);
    connect(btnExists, SIGNAL(clicked(bool)), this, SLOT(existsFlashDisk()));

    QPushButton *btnDelete = new QPushButton(this);
    btnDelete->setText(tr("卸载优盘"));
    btnDelete->setFixedSize(97, 44);
    layout->addWidget(btnDelete);
    connect(btnDelete, SIGNAL(clicked(bool)), this, SLOT(deleteFlashDisk()));

    text = new QLabel(this);
    text->setText("?");
    layout->addWidget(text);
#endif
    layout->addStretch();

    QPushButton *btnUpdate = new QPushButton(this);
    btnUpdate->setText(tr("导出数据"));
    btnUpdate->setFixedSize(97, 44);
    layout->addWidget(btnUpdate);
    connect(btnUpdate, SIGNAL(clicked(bool)), this, SLOT(recvExport()));
}

void SqlRecord::existsFlashDisk()
{
    timeOut = 0;
    QProcess cmddf;
    cmddf.start("df -h");
    if (cmddf.waitForFinished()) {
        QByteArray bytedf = cmddf.readAll();
        QStringList listdf = QString(bytedf).split("\n");
        for (int i=0; i < listdf.size(); i++) {
            if (listdf.at(i).startsWith("/dev/sd")) {
                QStringList tmp = listdf.at(i).split(" ");
                path = tmp.last();
            }
        }
    }
    cmddf.deleteLater();
    if (path.isEmpty()) {
        text->setText(tr("未发现优盘"));
    } else {
        text->setText(tr("发现优盘于%1").arg(path));
    }
}

void SqlRecord::deleteFlashDisk()
{
    QProcess cmddf;
    cmddf.start("df -h");
    if (cmddf.waitForFinished()) {
        QByteArray bytedf = cmddf.readAll();
        QStringList listdf = QString(bytedf).split("\n");
        QString path;
        for (int i=0; i < listdf.size(); i++) {
            if (listdf.at(i).startsWith("/dev/sd")) {
                QStringList tmp = listdf.at(i).split(" ");
                if (tmp.last().startsWith("/mnt/usb")) {
                    path = tmp.first();
                    cmddf.start(tr("umount %1").arg(path));
                    cmddf.waitForFinished();
                }
            }
        }
    }
    path.clear();
    text->setText(tr("优盘已卸载"));
    cmddf.deleteLater();
}

void SqlRecord::recvSelect()
{
    tmpQuan.clear();
    tmpOKNG.clear();
    qint64 t1 = from->date().toJulianDay();
    qint64 t2 = stop->date().toJulianDay();

    QSqlQuery query(QSqlDatabase::database("record"));
    query.prepare("select * from aip_sqlite where R_UUID >= ? and R_UUID <= ?");
    query.addBindValue(t1);
    query.addBindValue(t2);
    if (!query.exec())
        qWarning() << "aip_sqlite:" << query.lastError();
    while (query.next()) {
        int item = query.value(1).toInt();
        int quan = query.value(2).toInt();
        int okng = query.value(3).toInt();
        tmpQuan[item] = tmpQuan[item].toInt() + quan;
        tmpOKNG[item] = tmpOKNG[item].toInt() + okng;
    }
    QVector<double> value(27);
    QList<int> ids;
    ids << Qt::Key_0 << Qt::Key_1 << Qt::Key_2 << Qt::Key_3 << Qt::Key_4
        << Qt::Key_5 << Qt::Key_6 << Qt::Key_7 << Qt::Key_8;
    for (int i=0; i < ids.size(); i++) {
        int addr = tmpSet[4000 + ids.at(i)].toInt();
        int quan = tmpQuan[addr].toInt();
        int okng = tmpOKNG[addr].toInt();
        value[i*3 + 0] = quan;
        value[i*3 + 1] = okng;
        value[i*3 + 2] = quan - okng;
    }
    view->setData(value, 3);

    view->update();
}

void SqlRecord::recvExport()
{
#ifdef __arm__
    existsFlashDisk();
    if (path.isEmpty())
        return;
    path.append("/");
#else
    path.append("./");
#endif
    path.append(tr("%1.csv").arg(QDateTime::currentDateTime().toString("yyyyMMddhhmm")));
    QDateTime t;
    t.setDate(from->date());
    quint64 id_from = quint64(t.toMSecsSinceEpoch()) << 20;
    t.setDate(stop->date().addDays(1));
    quint64 id_stop = quint64(t.toMSecsSinceEpoch()) << 20;
    tmpMsg.insert(Qt::Key_0, Qt::Key_Book);
    tmpMsg.insert(Qt::Key_1, path);
    tmpMsg.insert(Qt::Key_4, "record");
    tmpMsg.insert(Qt::Key_9, id_from);
    tmpMsg.insert(Qt::Key_A, id_stop);
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
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

void SqlRecord::recvAppMsg(QTmpMap msg)
{
    switch (msg.value(Qt::Key_0).toInt()) {
    case Qt::Key_Copy:
        tmpSet = msg;
        break;
    default:
        break;
    }
}

void SqlRecord::showEvent(QShowEvent *e)
{
    this->setFocus();
    recvSelect();
    e->accept();
}
