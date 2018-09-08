/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       QLabel显示波形
*******************************************************************************/
#include "boxqlabel.h"

BoxQLabel::BoxQLabel(QWidget *parent) : QLabel(parent)
{
    setZero();
}

int BoxQLabel::getFrom()
{
    return from;
}

int BoxQLabel::getStop()
{
    return stop;
}

void BoxQLabel::setZero()
{
    mode = 0;
    from = 1;
    stop = 99;
    std1.clear();
    std2.clear();
    real.clear();
}

void BoxQLabel::setText(QString text, int mode)
{
    switch (mode) {
    case 0:
        str1 = text;
        break;
    case 1:
        str2 = text;
        break;
    case 2:
        str3 = text;
        stop = text.toInt();
        break;
    case 3:
        str4 = text;
        from = text.toInt();
        break;
    default:
        break;
    }
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
    case 4:
        real = values;
        mode = 4;
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
    QPoint p1, p2;
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
    out[0] = (13.0*yy[0] + 10.0*yy[1] + 7.0*yy[2] + 4.0*yy[3] +
            yy[4] - 2.0*yy[5] - 5.0*yy[6]) / 28.0;
    out[1] = (5.0*yy[0] + 4.0*yy[1] + 3*yy[2] + 2*yy[3] + yy[4] - yy[6]) / 14.0;
    out[2] = (7.0*yy[0] + 6.0*yy[1] + 5.0*yy[2] + 4.0*yy[3] +
            3.0*yy[4] + 2.0*yy[5] + yy[6]) / 28.0;

    for (int i=3; i <= s-4; i++) {
        out[i] = (yy[i-3] + yy[i-2] + yy[i-1] + yy[i] + yy[i+1] + yy[i+2] + yy[i+3]) / 7.0;
    }
    out[s-3] = (7.0*yy[s-1] + 6.0*yy[s-2] + 5.0*yy[s-3] + 4.0*yy[s-4] +
            3.0*yy[s-5] + 2.0*yy[s-6] + yy[s-7]) / 28.0;
    out[s-2] = (5.0*yy[s-1] + 4.0*yy[s-2] + 3.0*yy[s-3] + 2.0*yy[s-4] +
            yy[s-5] - yy[s-7]) / 14.0;
    out[s-1] = (13.0*yy[s-1] + 10.0*yy[s-2] + 7.0*yy[s-3] + 4*yy[s-4] +
            yy[s-5] - 2*yy[s-6] - 5*yy[s-7]) / 28.0;

    QPoint p1, p2;
    for (int i=0; i < s-1; i++) {
        p1.setX(xx[i + 0]);
        p1.setY(h - out[i+0] * h / 100);
        p2.setX(xx[i + 1]);
        p2.setY(h - out[i+1] * h / 100);
        painter->drawLine(p1, p2);
    }
}

void BoxQLabel::drawImpLine(QPainter *painter)
{
    int w = this->width();
    int h = this->height();
    int x1 = w * from / IMP_SIZE;
    int x2 = w * stop / IMP_SIZE;
    painter->setPen(QPen(Qt::transparent));
    painter->setBrush(QBrush(QColor("#303000")));
    painter->drawRect(x1, 0, x2-x1, h);
}

void BoxQLabel::drawDcrWave(QPainter *painter)
{
    int w = this->width();
    int h = this->height();
    int wt = (h > 200) ? 80 : 30;
    int ws = (h % wt == 0) ? h / wt : h / wt + 1;
    int th = (w > 500) ? 200 : 100;
    int wh = (w % th == 0) ? w / th : w / th + 1;
    painter->setPen(QPen(Qt::gray, 1, Qt::DotLine));
    for (int i=1; i < ws; i++) {  // 横线
        painter->drawLine(QPoint(0, h*i/ws), QPoint(w, h*i/ws));
    }
    for (int i=1; i < wh; i++) {  // 坚线
        painter->drawLine(QPoint(w*i/wh, 0), QPoint(w*i/wh, h));
    }
    // 画电阻波形
    painter->setPen(QPen(Qt::white, 1, Qt::SolidLine));
    setLine(painter, std1);
    setLine(painter, std2);
    painter->setPen(QPen(Qt::green, 1, Qt::SolidLine));
    setLine(painter, real);
}

void BoxQLabel::drawImpWave(QPainter *painter)
{
    int w = this->width();
    int h = this->height();
    int wt = (h > 200) ? 80 : 30;
    int ws = (h % wt == 0) ? h / wt : h / wt + 1;
    int th = (w > 500) ? 200 : 100;
    int wh = (w % th == 0) ? w / th : w / th + 1;
    painter->setPen(QPen(Qt::gray, 1, Qt::DotLine));
    for (int i=1; i < ws; i++) {  // 横线
        painter->drawLine(QPoint(0, h*i/ws), QPoint(w, h*i/ws));
    }
    for (int i=1; i < wh; i++) {  // 坚线
        painter->drawLine(QPoint(w*i/wh, 0), QPoint(w*i/wh, h));
    }
    // 画匝间波形
    painter->setPen(QPen(Qt::white, 1, Qt::SolidLine));
    linearSmooth(painter, std1);
    painter->setPen(QPen(Qt::green, 1, Qt::SolidLine));
    linearSmooth(painter, real);
}

void BoxQLabel::drawTstCurr(QPainter *painter)
{
    int w = this->width();
    int h = this->height();
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
    painter->drawRect(0 * w / 3 + 5, 5, w / 3 - 5, h - 40);

    if (q1 == 0) {
        q1 = 1;
        q2 = 1;
        q3 = 0;
    }

    double x1 = 1 * w / 3 + 5;
    double x2 = 2 * w / 3 + 5;
    double y1 = 5 + (q3) / q1 * 0.75 * h;
    double y2 = 5 + (q2) / q1 * 0.75 * h;
    double h1 = h - y1 - 35;
    double h2 = h - y2 - 35;
    painter->setPen(QPen(Qt::green));
    painter->setBrush(QBrush(Qt::green));
    painter->drawRect(x1, y1, w / 3 - 5, h1);

    painter->setPen(QPen(Qt::red));
    painter->setBrush(QBrush(Qt::red));
    painter->drawRect(x2, y2, w / 3 - 5, h2);
}

void BoxQLabel::drawAllQuan(QPainter *painter)
{
    int w = this->width();
    int h = this->height();
    QFont font = painter->font();
    font.setPixelSize(10);
    painter->setFont(font);
    QStringList tmp;
    tmp << tr("总数") << "合格" << "不良"
        << tr("片间") << "合格" << "不良"
        << tr("焊接") << "合格" << "不良"
        << tr("跨间") << "合格" << "不良"
        << tr("绝缘") << "合格" << "不良"
        << tr("轴铁") << "合格" << "不良"
        << tr("轴线") << "合格" << "不良"
        << tr("铁线") << "合格" << "不良"
        << tr("匝间") << "合格" << "不良";
    double ww = quan.value(0);
    if (ww != 0) {
        for (int i=0; i < quan.size() / 3; i++) {
            painter->setPen(QPen(Qt::white));
            double q1 = quan[i*3 + 0];
            double q2 = quan[i*3 + 1];
            double q3 = quan[i*3 + 2];
            QFontMetricsF fm(painter->font());
            double f1 = fm.size(Qt::TextSingleLine, tmp.at(0)).width();
            double f2 = fm.size(Qt::TextSingleLine, QString::number(q1)).width();

            painter->drawText((i*6 + 1) * w / quan.size()/2 - f1/2, h-20, tmp.at(i*3 + 0));
            painter->drawText((i*6 + 3) * w / quan.size()/2 - f1/2, h-20, tmp.at(i*3 + 1));
            painter->drawText((i*6 + 5) * w / quan.size()/2 - f1/2, h-20, tmp.at(i*3 + 2));
            painter->drawText((i*6 + 1) * w / quan.size()/2 - f2/2, h-5, QString::number(q1));
            painter->drawText((i*6 + 3) * w / quan.size()/2 - f2/2, h-5, QString::number(q2));
            painter->drawText((i*6 + 5) * w / quan.size()/2 - f2/2, h-5, QString::number(q3));

            double x1 = (i*6 + 0) * w / quan.size()/2 + 2;
            double x2 = (i*3 + 1) * w / quan.size() + 2;
            double x3 = (i*3 + 2) * w / quan.size() + 0;
            double y1 = 5 + (ww - q1) / ww * (h - 40);
            double y2 = 5 + (ww - q2) / ww * (h - 40);
            double y3 = 5 + (ww - q3) / ww * (h - 40);
            double h1 = h - y1 - 34;
            double h2 = h - y2 - 34;
            double h3 = h - y3 - 34;

            painter->setPen(QPen(Qt::blue));
            painter->setBrush(QBrush(Qt::blue));
            painter->drawRect(x1, y1, w / quan.size() - 2, h1);

            painter->setPen(QPen(Qt::green));
            painter->setBrush(QBrush(Qt::green));
            painter->drawRect(x2, y2, w / quan.size() - 3, h2);

            painter->setPen(QPen(Qt::red));
            painter->setBrush(QBrush(Qt::red));
            painter->drawRect(x3, y3, w / quan.size() - 5, h3);
        }
    }
}

void BoxQLabel::drawAllText(QPainter *painter)
{
    QFontMetricsF fm(this->font());
    int w = this->width();
    int h = this->height();
    painter->setPen(QPen(Qt::white));
    if (!str1.isEmpty()) {
        int x = 2;
        int y = 2 + fm.size(Qt::TextSingleLine, str1).height();
        painter->drawText(x, y, str1);
    }
    if (!str2.isEmpty()) {
        int x = w - 2 - fm.size(Qt::TextSingleLine, str2).width();
        int y = 2 + fm.size(Qt::TextSingleLine, str2).height();
        painter->drawText(x, y, str2);
    }
    if (!str3.isEmpty()) {
        str3 = QString::number(stop * 100 / IMP_SIZE) + "%";
        int x = w - 0 - fm.size(Qt::TextSingleLine, str3).width();
        int y = h - 5;
        painter->drawText(x, y, str3);
    }
    if (!str4.isEmpty()) {
        str4 = QString::number(from * 100 / IMP_SIZE) + "%";
        int x = 2;
        int y = h -5;
        painter->drawText(x, y, str4);
    }
}

void BoxQLabel::paintEvent(QPaintEvent *e)
{
    QPainter *painter = new QPainter(this);
    painter->setRenderHint(QPainter::Antialiasing, true);
    switch (mode) {
    case 0:
        drawImpWave(painter);
        break;
    case 1:
        drawDcrWave(painter);
        break;
    case 2:
        drawTstCurr(painter);
        break;
    case 3:
        drawAllQuan(painter);
        break;
    case 4:
        drawImpLine(painter);
        drawImpWave(painter);
        break;
    default:
        break;
    }
    drawAllText(painter);
    painter->end();
    e->accept();
}

void BoxQLabel::mousePressEvent(QMouseEvent *e)
{
    if (mode == 4) {
        int w = this->width();
        int s = (from + stop) * w / IMP_SIZE / 2;
        if (e->x() > s) {
            stop = e->x() * IMP_SIZE / w;
        } else {
            from = e->x() * IMP_SIZE / w;
        }
        this->update();
    }
    emit clicked();

    e->accept();
}

