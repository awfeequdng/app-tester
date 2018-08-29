/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       QComboBox代理
*******************************************************************************/
#include "boxqcombo.h"

BoxQCombo::BoxQCombo(QObject *parent):QItemDelegate(parent)
{
}

QWidget *BoxQCombo::createEditor(QWidget *parent,
                                  const QStyleOptionViewItem &,
                                  const QModelIndex &) const {
    QComboBox *editor = new QComboBox(parent);
    editor->setItemDelegate(new QStyledItemDelegate());
    for (int i=0; i < items.size(); i++)
        editor->addItem(items.at(i));
    return editor;
}

void BoxQCombo::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString text = index.model()->data(index, Qt::EditRole).toString();
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    int tindex = comboBox->findText(text);
    comboBox->setCurrentIndex(tindex);
}

void BoxQCombo::setModelData(QWidget *editor,
                              QAbstractItemModel *model,
                              const QModelIndex &index) const {
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QString text = comboBox->currentText();
    model->setData(index, text, Qt::EditRole);
}

void BoxQCombo::updateEditorGeometry(QWidget *editor,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &) const {
    editor->setGeometry(option.rect);
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    comboBox->showPopup();
}

void BoxQCombo::setItemNames(QStringList names) {
    items = names;
}

