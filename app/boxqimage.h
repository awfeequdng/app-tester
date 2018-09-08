/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       QLabel显示波形
*******************************************************************************/
#ifndef BOXQIMAGE_H
#define BOXQIMAGE_H

#include <QDebug>
#include <QLabel>
#include <QPoint>
#include <QColor>
#include <QPainter>
#include <QMouseEvent>

class BoxQImage : public QLabel
{
    Q_OBJECT
public:
    explicit BoxQImage(QWidget *parent = 0);

signals:
    void clicked();
public slots:
    void setClear();
    QList<QVariantMap> getLines();
    QList<QVariantMap> getTexts();
    void setLines(QVariantMap msg);
    void setTexts(QVariantMap msg);
    void setGrids(QVariantMap msg);
    void setBodys(QVariantMap msg);
private slots:
    void drawLines(QPainter *painter);
    void drawTexts(QPainter *painter);
    void drawGrids(QPainter *painter);
    void drawBodys(QPainter *painter);
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
private:
    QList<QVariantMap> lines;
    QList<QVariantMap> texts;
    QList<QVariantMap> grids;
    QList<QVariantMap> bodys;
    QList<QColor> Colors;
};

#endif // BOXQIMAGE_H
