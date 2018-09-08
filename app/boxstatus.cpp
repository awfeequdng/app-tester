/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       提示框
*******************************************************************************/
#include "boxstatus.h"

BoxStatus::BoxStatus(QWidget *parent) :
    QDialog(parent, Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint)
{
    initUI();
}

void BoxStatus::setLabelText(QString text)
{
    textLabel->setText(text);
}

void BoxStatus::initUI()
{
    this->setStyleSheet("QDialog{border:2px solid gray;}");
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addStretch();

    textLabel = new QLabel(this);
    layout->addWidget(textLabel);

    QHBoxLayout *hh = new QHBoxLayout;
    layout->addLayout(hh);
    hh->addStretch();

    QPushButton *btnOK = new QPushButton(tr("确定"), this);
    hh->addWidget(btnOK);
    connect(btnOK, SIGNAL(clicked(bool)), this, SLOT(hide()));

    this->setMinimumSize(240, 96);
}


