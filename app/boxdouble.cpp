/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       QDoubleSpinBox代理
*******************************************************************************/
#include "boxdouble.h"

BoxDouble::BoxDouble(QObject *parent) : QItemDelegate(parent)
{
    min = 0;
    max = 9999.99;
    prec = 2;
}

QWidget *BoxDouble::createEditor(QWidget *parent,
                                       const QStyleOptionViewItem &,
                                       const QModelIndex &) const {
    QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
    editor->setMaximum(max);
    editor->setMinimum(min);
    editor->setDecimals(prec);
    editor->setButtonSymbols(QAbstractSpinBox::NoButtons);
    return editor;
}

void BoxDouble::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    double value = index.model()->data(index, Qt::EditRole).toDouble();
    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->setValue(value);
}

void BoxDouble::setModelData(QWidget *editor,
                                   QAbstractItemModel *model,
                                   const QModelIndex &index) const {
    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->interpretText();
    double value = spinBox->value();
    model->setData(index, value, Qt::EditRole);
}

void BoxDouble::updateEditorGeometry(QWidget *editor,
                                           const QStyleOptionViewItem &option,
                                           const QModelIndex &) const {
    editor->setGeometry(option.rect);
}

void BoxDouble::setMaxinum(double m)
{
    max = m;
}

void BoxDouble::setMininum(double m)
{
    min = m;
}

void BoxDouble::setDecimals(int p)
{
    prec = p;
}


