/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       表格代理
*******************************************************************************/
#ifndef BOXQMODEL_H
#define BOXQMODEL_H

#include <QWidget>
#include <QDebug>
#include <QObject>
#include <QStandardItemModel>

class BoxQModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit BoxQModel(QObject *parent = 0);
    explicit BoxQModel(int row, int column, QObject *parent = NULL);
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    void setCheckable(int column);
private:
    QList<int> columns;
    QMap<int, Qt::CheckState> checks;
};

#endif // BOXQMODEL_H
