/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       只读代理
*******************************************************************************/
#include "boxqitems.h"

BoxQItems::BoxQItems(QObject *parent) : QItemDelegate(parent)
{
}

QWidget *BoxQItems::createEditor(QWidget *,
                                      const QStyleOptionViewItem &,
                                      const QModelIndex &) const {
    return NULL;
}
