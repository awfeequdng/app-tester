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
    void setRow(int num);
    void setPie(int num);
    void setRun(int num);
    void setTurn(int turn);
protected:
    void mouseReleaseEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *event);
    void drawRow(QPainter *painter);
    void drawPie(QPainter *painter);
    void drawTurn(QPainter *painter);
    void drawCrown(QPainter *painter);
    void drawTitle(QPainter *painter);
    void drawScaleNum(QPainter *painter);
private slots:
    void setStr();
private:
    QString m_title;
    int m_width;
    int m_count;
    int m_timer;
    int m_lenth;
    int m_start;
    int m_turn;
    QList<int> pies;
    QList<int> rows;
};

#endif // BOXQCHART_H
