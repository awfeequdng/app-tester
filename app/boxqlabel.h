/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       QLabel显示波形
*******************************************************************************/
#ifndef BOXQLABEL_H
#define BOXQLABEL_H

#include <QDebug>
#include <QLabel>
#include <QPoint>
#include <QPainter>
#include <QMouseEvent>

class BoxQLabel : public QLabel
{
    Q_OBJECT

public:
    explicit BoxQLabel(QWidget *parent = 0);
signals:
    void clicked();
public slots:
    void setZero();
    void setText(QString text, int mode);
    void setData(QVector<double> values, int mode);
    void setWave(QVector<double> values, int mode);
private slots:
    void setLine(QPainter *painter, QVector<double> r);
    void linearSmooth(QPainter *painter, QVector<double> r);
    void drawDcrWave(QPainter *painter);
    void drawImpWave(QPainter *painter);
    void drawTstCurr(QPainter *painter);
    void drawAllQuan(QPainter *painter);
    void drawAllText(QPainter *painter);
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
private:
    QVector<double> std1;
    QVector<double> std2;
    QVector<double> real;
    QVector<double> quan;
    quint8 mode;
    QString str1,str2,str3,str4;
};

#endif // BOXQLABEL_H
