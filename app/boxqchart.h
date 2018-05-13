/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       环形显示
*******************************************************************************/
#ifndef BOXQCHART_H
#define BOXQCHART_H

#include <QRectF>
#include <QTimer>
#include <QLabel>
#include <QWidget>
#include <QDebug>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QDateTime>

#include <cmath>

#include "appdefine.h"

#define PI 3.1415926
#define SCALE 200.0

class BoxQChart : public QLabel
{
    Q_OBJECT
public:
    explicit BoxQChart(QWidget *parent = 0);
signals:
    void sendClick();
public:
    void setNum(int num);
    void setStr(QString text);
protected:
    void mouseReleaseEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *event);
    void drawPie(QPainter *painter);
    void drawCrown(QPainter *painter);
    void drawTitle(QPainter *painter);
    void drawScaleNum(QPainter *painter);
private:
    QString m_title;
    int m_count;
    int m_timer;
    int m_lenth;
};

#endif // BOXQCHART_H
