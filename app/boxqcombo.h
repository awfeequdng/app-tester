/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       QComboBox代理
*******************************************************************************/
#ifndef BOXQCOMBO_H
#define BOXQCOMBO_H

#include <QObject>
#include <QComboBox>
#include <QModelIndex>
#include <QItemDelegate>
#include <QStyledItemDelegate>

class BoxQCombo : public QItemDelegate
{
    Q_OBJECT
public:
    explicit BoxQCombo(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QStringList items;
public slots:
    void setItemNames(QStringList names);
};

#endif // BOXQCOMBO_H
