/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       数据导出
*******************************************************************************/
#include "sqlexport.h"

SqlExport::SqlExport(QObject *parent) : QObject(parent)
{
}

int SqlExport::exportFile(QTmpMap msg)
{
    QElapsedTimer t;
    t.start();
    if (createFile(msg) == Qt::Key_Stop)
        return Qt::Key_Stop;
    if (selectNumb(msg) == Qt::Key_Stop)
        return Qt::Key_Stop;
    if (selectItem(msg) == Qt::Key_Stop)
        return Qt::Key_Stop;
    if (exportHead(msg) == Qt::Key_Stop)
        return Qt::Key_Stop;
    if (exportData(msg) == Qt::Key_Stop)
        return Qt::Key_Stop;
    file->close();
    showText(tr("导出成功,实际用时%1ms").arg(t.elapsed()));
    return 0;
}

int SqlExport::createFile(QTmpMap msg)
{
    QString path = msg.value(Qt::Key_1).toString();
    file = new QFile(QString("%1").arg(path));
    if (!file->open(QFile::WriteOnly)) {
        showText(tr("文件打开失败"));
        return Qt::Key_Stop;
    }
    return Qt::Key_Meta;
}

int SqlExport::selectNumb(QTmpMap msg)
{
    QString name = msg.value(Qt::Key_4).toString();
    quint64 from = msg.value(Qt::Key_9).toLongLong();
    quint64 stop = msg.value(Qt::Key_A).toLongLong();

    QSqlQuery query(QSqlDatabase::database(name));
    QString cmd = tr("select count(*) from aip_record where R_ITEM=65535");
    cmd += tr(" and R_UUID > %1 and R_UUID < %2").arg(from).arg(stop);

    if (!query.exec(cmd)) {
        showText(tr("查询数据失败,%1").arg(query.lastError().text()));
        return Qt::Key_Stop;
    }
    if (!query.next()) {
        showText(tr("查询数据失败,%1").arg(query.lastError().text()));
        return Qt::Key_Stop;
    }
    quan = query.value(0).toInt();
    showText(tr("共找到%1条数据").arg(quan));
    query.clear();

    return Qt::Key_Meta;
}

int SqlExport::selectItem(QTmpMap msg)
{
    QString name = msg.value(Qt::Key_4).toString();
    quint64 from = msg.value(Qt::Key_9).toLongLong();
    quint64 stop = msg.value(Qt::Key_A).toLongLong();

    QSqlQuery query(QSqlDatabase::database(name));
    QString cmd = tr("select distinct R_ITEM from aip_record where R_ITEM < %1").arg(0xFFFF);
    cmd += tr(" and R_UUID > %1 and R_UUID < %2").arg(from).arg(stop);
    if (!query.exec(cmd)) {
        showText(tr("查询数据失败,%1").arg(query.lastError().text()));
        return Qt::Key_Stop;
    }
    numbs.clear();
    while (query.next()) {
        numbs.append(query.value(0).toInt());
    }
    numbs.append(DataFile);
    return Qt::Key_Meta;
}

int SqlExport::exportHead(QTmpMap msg)
{
    quint64 from = msg.value(Qt::Key_9).toLongLong();
    quint64 stop = msg.value(Qt::Key_A).toLongLong();

    QStringList title;

    QString str;
    str.append(tr("导出者编号:"));
    str.append(QString::number(tmpSet.value(DataUser).toInt() - tmpSet.value((2000 + Qt::Key_5)).toInt() + 1));
    str.append("  ");
    str.append(tr("总日期范围:"));
    str.append(QDateTime::fromMSecsSinceEpoch(from >> 20).date().toString("yyyy-MM-dd"));
    str.append("~");
    str.append(QDateTime::fromMSecsSinceEpoch(stop >> 20).date().toString("yyyy-MM-dd"));
    file->write(ToGbk(str));
    file->write("\n");

    int addr = tmpSet.value((3000 + Qt::Key_0)).toInt();

    tmpMsg.insert(DataFile, tr("型号"));
    tmpMsg.insert(DataUser, tr("用户"));
    tmpMsg.insert(tmpSet.value(addr + TEMPDATE).toInt(), tr("日期"));
    tmpMsg.insert(tmpSet.value(addr + TEMPPLAY).toInt(), tr("启动"));
    tmpMsg.insert(tmpSet.value(addr + TEMPSTOP).toInt(), tr("完成"));
    tmpMsg.insert(tmpSet.value(addr + TEMPTEMP).toInt(), tr("温度"));
    tmpMsg.insert(tmpSet.value(addr + TEMPWORK).toInt(), tr("工位"));
    tmpMsg.insert(tmpSet.value(addr + TEMPCODE).toInt(), tr("条码"));
    tmpMsg.insert(tmpSet.value(addr + TEMPISOK).toInt(), tr("判定"));

    addr = tmpSet.value((3000 + Qt::Key_1)).toInt() + CACHEDCR;  // 电阻结果地址
    for(int i=0; i < 72; i++) {
        tmpMsg.insert(addr + i, QString("片间R%1").arg(i+1, 2, 10, QChar('0')));
    }
    addr = tmpSet.value((3000 + Qt::Key_2)).toInt() + CACHEDCR;  // 电阻结果地址
    for(int i=0; i < 72; i++) {
        tmpMsg.insert(addr + i, QString("焊接R%1").arg(i+1, 2, 10, QChar('0')));
    }
    addr = tmpSet.value((3000 + Qt::Key_3)).toInt() + CACHEDCR;  // 电阻结果地址
    for(int i=0; i < 36; i++) {
        tmpMsg.insert(addr + i, QString("跨间R%1").arg(i+1, 2, 10, QChar('0')));
    }
    addr = tmpSet.value((3000 + Qt::Key_4)).toInt() + CACHEACW;  // 轴铁耐压结果地址
    tmpMsg.insert(addr, QString("轴铁耐压"));
    addr = tmpSet.value((3000 + Qt::Key_5)).toInt() + CACHEACW;  // 轴线耐压结果地址
    tmpMsg.insert(addr, QString("轴线耐压"));
    addr = tmpSet.value((3000 + Qt::Key_6)).toInt() + CACHEACW;  // 铁线耐压结果地址
    tmpMsg.insert(addr, QString("铁线耐压"));
    addr = tmpSet.value((3000 + Qt::Key_7)).toInt() + CACHEACW;  // 绝缘电阻结果地址
    tmpMsg.insert(addr, QString("绝缘电阻"));
    addr = tmpSet.value((3000 + Qt::Key_8)).toInt() + CACHEIMP;  // 匝间果地址
    for(int i=0; i < 72; i++) {
        tmpMsg.insert(addr + i, QString("匝间D%1").arg(i+1, 2, 10, QChar('0')));
    }
    addr = tmpSet.value((2000 + Qt::Key_4)).toInt();
    for (int i=0; i < 100; i++) {
        tmpMsg.insert(addr + i, tmpSet.value(addr + i).toString());
    }

    qSort(numbs.begin(), numbs.end());

    title.clear();
    for (int i=0; i < numbs.size(); i++) {
        title.append(tmpMsg.value(numbs.at(i)).toString());
    }
    file->write(ToGbk(title.join(",")));
    file->write("\n");
    return 0;
}

int SqlExport::exportData(QTmpMap msg)
{
    QString name = msg.value(Qt::Key_4).toString();
    quint64 from = msg.value(Qt::Key_9).toLongLong();
    quint64 stop = msg.value(Qt::Key_A).toLongLong();

    QSqlQuery query(QSqlDatabase::database(name));
    quint64 numb = 0;
    QStringList lineBuffer;
    QTmpMap bufstd;
    for (int i=0; i < numbs.size(); i++) {
        bufstd[numbs.at(i)] = QString(" ");
    }
    QTmpMap buftmp = bufstd;
    int t = 0;
    while (1) {
        QString cmd = tr("select * from aip_record where ");
        cmd += tr(" R_UUID > %1 and R_UUID < %2").arg(from).arg(stop);
        if (!query.exec(cmd)) {
            showText(tr("查询数据失败,%1").arg(query.lastError().text()));
            return Qt::Key_Stop;
        }
        while (query.next()) {
            numb = query.value(1).toInt();

            buftmp.insert(numb, query.value(3).toString());
            if (numb == 0xFFFF) {
                buftmp.insert(DataFile, tmpSet.value(query.value(2).toInt()).toString());
                foreach (int n, numbs) {
                    lineBuffer.append(buftmp.value(n).toString());
                }
                file->write(ToGbk(lineBuffer.join(",")));
                file->write("\n");
                lineBuffer.clear();
                buftmp = bufstd;
                t++;
                if (t % LENTH == 0)
                    break;
            }
        }
        tmpMsg.insert(Qt::Key_0, Qt::Key_Word);
        tmpMsg.insert(Qt::Key_2, t*100/quan);
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
        if (t >= quan)
            break;
    }
    return Qt::Key_Meta;
}

QByteArray SqlExport::ToGbk(const QString &inStr)
{
    QTextCodec *gbk = QTextCodec::codecForName("GB18030");
    return gbk->fromUnicode(inStr);
}

void SqlExport::showText(QString err)
{
    tmpMsg.insert(Qt::Key_0, Qt::Key_Word);
    tmpMsg.insert(Qt::Key_1, err);
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void SqlExport::recvAppMsg(QTmpMap msg)
{
    switch (msg.value(Qt::Key_0).toInt()) {
    case Qt::Key_Copy:
        tmpSet = msg;
        break;
    case Qt::Key_Book:
        exportFile(msg);
        break;
    default:
        break;
    }
}

