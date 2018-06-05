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
    void setData(QVector<double> values, int mode);
    void setWave(QVector<double> values, int mode);
private slots:
    void setLine(QPainter *painter, QVector<double> r);
    void linearSmooth(QPainter *painter, QVector<double> r);
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *e);
private:
    QVector<double> std1;
    QVector<double> std2;
    QVector<double> real;
    QVector<double> quan;
    quint8 mode;
};

#endif // BOXQLABEL_H
