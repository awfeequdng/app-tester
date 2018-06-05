#include "boxqlabel.h"

BoxQLabel::BoxQLabel(QWidget *parent) : QLabel(parent)
{
    setZero();
}

void BoxQLabel::setZero()
{
    mode = 0;
    std1.clear();
    std2.clear();
    real.clear();
}

void BoxQLabel::setData(QVector<double> values, int numb)
{
    quan = values;
    mode = numb;
}

void BoxQLabel::setWave(QVector<double> values, int numb)
{
    switch (numb) {
    case 0:
        real = values;
        break;
    case 1:
        std1 = values;
        break;
    case 2:
        std2 = values;
        mode = 1;
        break;
    default:
        break;
    }
}

void BoxQLabel::setLine(QPainter *painter, QVector<double> r)
{
    if (r.isEmpty())
        return;
    int w = this->width();
    int h = this->height();
    int s = r.size();
    QVector<double> xx(s);
    QVector<double> yy(s);
    for (int i=0; i < s; i++) {
        xx[i] = i * w / (s - 1);
        yy[i] = r.at(i);
    }
    QPoint p1,p2;
    for (int i=0; i < s-1; i++) {
        p1.setX(xx[i + 0]);
        p1.setY(h - yy[i+0] * h / 100);
        p2.setX(xx[i + 1]);
        p2.setY(h - yy[i+1] * h / 100);
        painter->drawLine(p1, p2);
    }
}

void BoxQLabel::linearSmooth(QPainter *painter, QVector<double> r)
{
    if (r.isEmpty())
        return;
    int w = this->width();
    int h = this->height();
    int s = r.size();
    QVector<double> xx(s);
    QVector<double> yy(s);
    QVector<double> out(s);
    for (int i=0; i < s; i++) {
        xx[i] = i * w / s;
        yy[i] = r.at(i);
    }
    out[0] = (13.0*yy[0] + 10.0*yy[1] + 7.0*yy[2] + 4.0*yy[3] + yy[4] - 2.0*yy[5] - 5.0*yy[6]) / 28.0;
    out[1] = (5.0*yy[0] + 4.0*yy[1] + 3*yy[2] + 2*yy[3] + yy[4] - yy[6] ) / 14.0;
    out[2] = (7.0*yy[0] + 6.0*yy [1] + 5.0*yy[2] + 4.0*yy[3] + 3.0*yy[4] + 2.0*yy[5] + yy[6]) / 28.0;

    for (int i=3; i <= s-4; i++) {
        out[i] = (yy[i-3] + yy[i-2] + yy[i-1] + yy[i] + yy[i+1] + yy[i+2] + yy[i+3]) / 7.0;
    }
    out[s-3] = (7.0*yy[s-1] + 6.0*yy [s-2] + 5.0*yy[s-3] + 4.0*yy[s-4] + 3.0*yy[s-5] + 2.0*yy[s-6] + yy[s-7]) / 28.0;
    out[s-2] = (5.0*yy[s-1] + 4.0*yy[s-2] + 3.0*yy[s-3] + 2.0*yy[s-4] + yy[s-5] - yy[s-7] ) / 14.0;
    out[s-1] = (13.0*yy[s-1] + 10.0*yy[s-2] + 7.0*yy[s-3] + 4*yy[s-4] + yy[s-5] - 2*yy[s-6] - 5*yy[s-7] ) / 28.0;

    QPoint p1,p2;
    for (int i=0; i < s-1; i++) {
        p1.setX(xx[i + 0]);
        p1.setY(h - out[i+0] * h / 100);
        p2.setX(xx[i + 1]);
        p2.setY(h - out[i+1] * h / 100);
        painter->drawLine(p1, p2);
    }
}

void BoxQLabel::paintEvent(QPaintEvent *)
{
    int w = this->width();
    int h = this->height();

    QPainter *painter = new QPainter(this);
    painter->setRenderHint(QPainter::Antialiasing, true);

    if (mode == 0 || mode == 1) {  // 匝间与电阻画网络线
        painter->setPen(QPen(Qt::gray, 1, Qt::DotLine));
        int wt = (h > 200) ? 80 : 30;
        int ws = (h % wt == 0) ? h / wt : h / wt + 1;
        int th = (w > 500) ? 200 : 100;
        int wh = (w % th == 0) ? w / th : w / th + 1;
        for (int i=1; i < ws; i++) {  // 横线
            painter->drawLine(QPoint(0, h*i/ws), QPoint(w, h*i/ws));
        }
        for (int i=1; i < wh; i++) {  // 坚线
            painter->drawLine(QPoint(w*i/wh, 0), QPoint(w*i/wh, h));
        }
    }
    if (mode == 0) {  // 画匝间波形
        painter->setPen(QPen(Qt::white, 1, Qt::SolidLine));
        linearSmooth(painter, std1);
        painter->setPen(QPen(Qt::green, 1, Qt::SolidLine));
        linearSmooth(painter, real);
    }
    if (mode == 1) {  // 画电阻波形
        painter->setPen(QPen(Qt::white, 1, Qt::SolidLine));
        setLine(painter, std1);
        setLine(painter, std2);
        painter->setPen(QPen(Qt::green, 1, Qt::SolidLine));
        setLine(painter, real);
    }
    if (mode == 2) {
        double q1 = quan[0];
        double q2 = quan[1];
        double q3 = quan[2];
        QFontMetricsF fm(painter->font());
        double f1 = fm.size(Qt::TextSingleLine, "OK").width();
        double f2 = fm.size(Qt::TextSingleLine, QString::number(q1)).width();
        painter->drawText(1 * w / 6 - f1/2, h-20, "ALL");
        painter->drawText(3 * w / 6 - f1/2, h-20, "OK");
        painter->drawText(5 * w / 6 - f1/2, h-20, "NG");
        painter->drawText(1 * w / 6 - f2/2, h-5, QString::number(q1));
        painter->drawText(3 * w / 6 - f2/2, h-5, QString::number(q2));
        painter->drawText(5 * w / 6 - f2/2, h-5, QString::number(q3));

        painter->setPen(QPen(Qt::blue));
        painter->setBrush(QBrush(Qt::blue));
        painter->drawRect(0 * w / 3 + 5, 5, w / 3 - 5, h * 0.75);

        if (q1 == 0) {
            q1 = 1;
            q2 = 1;
            q3 = 0;
        }

        double x1 = 1 * w / 3 + 5;
        double x2 = 2 * w / 3 + 5;
        double y1 = 5 + (q3) / q1 * 0.75 * h;
        double y2 = 5 + (q2) / q1 * 0.75 * h;
        double h1 = h * 0.75 - y1 + 5;
        double h2 = h * 0.75 - y2 + 5;
        painter->setPen(QPen(Qt::green));
        painter->setBrush(QBrush(Qt::green));
        painter->drawRect(x1, y1, w / 3 - 5, h1);

        painter->setPen(QPen(Qt::red));
        painter->setBrush(QBrush(Qt::red));
        painter->drawRect(x2, y2, w / 3 - 5, h2);
    }

    painter->end();
}

void BoxQLabel::mousePressEvent(QMouseEvent *e)
{
    emit clicked();
    e->accept();
}

