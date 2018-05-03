/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       等待提示框
*******************************************************************************/
#ifndef BOXDIALOG_H
#define BOXDIALOG_H

#include <QFile>
#include <QDebug>
#include <QLabel>
#include <QDialog>
#include <QLayout>
#include <QShowEvent>
#include <QHideEvent>
#include <QProgressBar>
#include <QApplication>
#include <QPropertyAnimation>

class BoxDialog : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(int pixmap READ pixmap WRITE setPixmap)
public:
    explicit BoxDialog(QWidget *parent = 0);
public slots:
    void setValue(int value);
    void setLabelText(QString text);
private slots:
    void initUI();
    void initAnimation();
    int pixmap() const;
    void setPixmap(const int m_index);
    virtual void showEvent(QShowEvent *e);
    virtual void hideEvent(QHideEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);
private:
    bool isAnimation;
    int m_index;
    QLabel *logoLabel;
    QLabel *textLabel;
    QProgressBar *boxbar;
    QPropertyAnimation *animation;
};

#endif // BOXDIALOG_H
