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

#include <cmath>

#define PI 3.1415926
#define SCALE 200.0

#define NUM 72

class BoxQChart : public QLabel
{
    Q_OBJECT
public:
    explicit BoxQChart(QWidget *parent = 0);
signals:
    void sendClick();
protected:
    void mouseReleaseEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *event);
    void drawPie(QPainter *painter);
    void drawCrown(QPainter *painter);
    void drawScaleNum(QPainter *painter);
    void drawParamNum(QPainter *painter);
    void drawParamNum2(QPainter *painter);
    void drawClock(QPainter *painter);
    void drawTitle(QPainter *painter);
    void drawNumericValue(QPainter *painter);
private slots:
    void setTimeOut();
private:
    QString m_units;
    QString m_title;
    int m_scaleMajor;
    QTimer *m_updateTimer;
    int t;
};

#endif // BOXQCHART_H
