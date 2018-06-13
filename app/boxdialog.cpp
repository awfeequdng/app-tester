/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       等待提示框
*******************************************************************************/
#include "boxdialog.h"

BoxDialog::BoxDialog(QWidget *parent) :
    QDialog(parent, Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint)
{
    initUI();
    initAnimation();
}

void BoxDialog::setValue(int value)
{
    boxbar->setValue(value);
    isAnimation = false;
    if (value == 100)
        this->hide();
}

void BoxDialog::setLabelText(QString text)
{
    textLabel->setText(text);
}

void BoxDialog::initUI()
{
    isAnimation = true;

    textLabel = new QLabel(this);

    QHBoxLayout *hh = new QHBoxLayout;
    hh->addStretch();
    hh->addWidget(textLabel);
    hh->addStretch();

    boxbar = new QProgressBar(this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addStretch();
    layout->addWidget(boxbar);
    layout->addLayout(hh);
    layout->addStretch();

    this->setMinimumSize(240, 96);
}

void BoxDialog::initAnimation()
{
    animation = new QPropertyAnimation(this, "pixmap");
    animation->setDuration(1200);
    animation->setLoopCount(-1);
    for (int i = 0; i <= 8; ++i) {
        animation->setKeyValueAt(i / 8.0, i);
    }
}

int BoxDialog::pixmap() const
{
    return m_index;
}

void BoxDialog::setPixmap(const int index)
{
    m_index = index;
    QMatrix leftmatrix;
    leftmatrix.rotate((index/2%4)*90%360);
    QString path = (index%2 == 0) ? (":/loading1") : (":/loading2");
    QPixmap pixmap(path);
    QPixmap pix = pixmap.transformed(leftmatrix, Qt::SmoothTransformation);
    logoLabel->setPixmap(pix);
}

void BoxDialog::showEvent(QShowEvent *e)
{
    if (isAnimation) {
        logoLabel->setFixedSize(64, 64);
        animation->start();
    }
    int x = (this->parentWidget()->width() - this->width())/2 + this->parentWidget()->x();
    int y = (this->parentWidget()->height() - this->height())/2 + this->parentWidget()->y();
    this->move(x, y);
    e->accept();
}

void BoxDialog::hideEvent(QHideEvent *e)
{
    animation->stop();
    e->accept();
}

void BoxDialog::mousePressEvent(QMouseEvent *e)
{
    e->accept();
}
