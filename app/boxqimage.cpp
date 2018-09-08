/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       QLabel显示波形
*******************************************************************************/
#include "boxqimage.h"

BoxQImage::BoxQImage(QWidget *parent) : QLabel(parent)
{
    Colors.append(QColor(Qt::yellow));
    Colors.append(QColor(Qt::green));
    Colors.append(QColor(Qt::blue));
    Colors.append(QColor(Qt::cyan));
    Colors.append(QColor(Qt::magenta));
    Colors.append(QColor(Qt::darkGreen));
    Colors.append(QColor(Qt::darkBlue));
    Colors.append(QColor(Qt::darkCyan));
    Colors.append(QColor(Qt::darkMagenta));
    Colors.append(QColor(Qt::darkYellow));
}

void BoxQImage::setClear()
{
    texts.clear();
    this->update();
}

QList<QVariantMap> BoxQImage::getLines()
{
    return lines;
}

QList<QVariantMap> BoxQImage::getTexts()
{
    return texts;
}

void BoxQImage::setLines(QVariantMap msg)
{
    bool n = true;
    for (int i=0; i < lines.size(); i++) {
        QVariantMap tmp = lines.at(i);
        if (tmp.value("index").toInt() == msg.value("index").toInt()) {
            lines[i] = msg;
            n = false;
        }
    }
    if (n)
        lines.append(msg);
}

void BoxQImage::setTexts(QVariantMap msg)
{
    bool n = true;
    for (int i=0; i < texts.size(); i++) {
        QVariantMap tmp = texts.at(i);
        if (tmp.value("index").toInt() == msg.value("index").toInt()) {
            texts[i] = msg;
            n = false;
        }
    }
    if (n)
        texts.append(msg);
}

void BoxQImage::setGrids(QVariantMap msg)
{
    bool n = true;
    for (int i=0; i < grids.size(); i++) {
        QVariantMap tmp = grids.at(i);
        if (tmp.value("index").toInt() == msg.value("index").toInt()) {
            grids[i] = msg;
            n = false;
        }
    }
    if (n)
        grids.append(msg);
}

void BoxQImage::setBodys(QVariantMap msg)
{
    bool n = true;
    for (int i=0; i < bodys.size(); i++) {
        QVariantMap tmp = bodys.at(i);
        if (tmp.value("index").toInt() == msg.value("index").toInt()) {
            bodys[i] = msg;
            n = false;
        }
    }
    if (n)
        bodys.append(msg);
}

void BoxQImage::drawLines(QPainter *painter)
{
    int w = this->width();
    int h = this->height();

    for (int i=0; i < lines.size(); i++) {
        QVariantMap tmp = lines.at(i);
        int lineColor = tmp.value("color").toInt();
        int lineWidth = tmp.value("width").toInt();
        QStringList p = tmp.value("point").toStringList();
        painter->setPen(QPen(Qt::GlobalColor(lineColor), lineWidth, Qt::SolidLine));
        int s = p.size();
        QPoint p1, p2;
        for (int t=0; t < s - 1; t++) {
            p1.setX((t + 0) * w / (s - 1));
            p1.setY(h - p.at(t + 0).toDouble() * h / 100);
            p2.setX((t + 1) * w / (s - 1));
            p2.setY(h - p.at(t + 1).toDouble() * h / 100);
            painter->drawLine(p1, p2);
        }
        if (tmp.value("frame").toInt() == 1) {  // 边框
            painter->setPen(QPen(Qt::white));
            painter->drawRect(0, 0, w, h);
        }
        if (tmp.value("shade").toInt() == 1) {  // 底纹
            int wt = (h > 350) ? 80 : 30;
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
        }
    }
}

void BoxQImage::drawTexts(QPainter *painter)
{
    int w = this->width();
    int h = this->height();

    for (int i=0; i < texts.size(); i++) {
        QVariantMap tmp = texts.at(i);
        int x = tmp.value("width").toInt();
        int y = tmp.value("lenth").toInt();
        QFont font = painter->font();
        font.setPixelSize(this->height() > 200 ? 24 : 11);
        painter->setFont(font);

        int lineColor = tmp.value("color").toInt();
        painter->setPen(QPen(Qt::GlobalColor(lineColor)));
        QString str = tmp.value("title").toString();
        painter->drawText(x * w / 100, h - y * h / 100, str);
    }
}

void BoxQImage::drawGrids(QPainter *painter)
{
    int w = this->width();
    int h = this->height();

    for (int i=0; i < grids.size(); i++) {
        QVariantMap tmp = grids.at(i);
        int x = tmp.value("point").toDouble() * w / 100;
        painter->setPen(QPen(Qt::gray, 1, Qt::DotLine));
        painter->drawLine(QPoint(x, 0), QPoint(x, h));
    }
}

void BoxQImage::drawBodys(QPainter *painter)
{
    int w = this->width();
    int h = this->height();
    int a = bodys.at(0).value("point").toInt();
    a = (a == 0) ? 1 : a;
    for (int i=0; i < bodys.size(); i++) {
        QVariantMap tmp = bodys.at(i);
        QString names = tmp.value("names").toString();
        QString point = tmp.value("point").toString();
        int x = i * w / bodys.size() + w / bodys.size() / 5;
        int y = (a - point.toInt()) * h * 80 / a / 100;
        int ww = w / bodys.size() * 70 / 100;
        int hh = point.toInt() * h * 80 / a / 100;
        painter->setPen(QPen(Colors.at(i % Colors.size())));
        painter->setBrush(QBrush(Colors.at(i % Colors.size())));
        painter->drawRect(x, y, ww, hh);
        painter->setPen(QPen(Qt::white));
        QFontMetricsF fm(painter->font());
        double f1 = fm.size(Qt::TextSingleLine, names).width();
        double f2 = fm.size(Qt::TextSingleLine, point).width();

        painter->drawText(x + (w / bodys.size() * 70 / 100 - f1) / 2, 88 * h / 100, names);
        painter->drawText(x + (w / bodys.size() * 70 / 100 - f2) / 2, 96 * h / 100, point);
    }
}

void BoxQImage::paintEvent(QPaintEvent *e)
{
    QPainter *painter = new QPainter(this);
    painter->setRenderHint(QPainter::Antialiasing, true);
    if (!lines.isEmpty())
        drawLines(painter);
    if (!texts.isEmpty())
        drawTexts(painter);
    if (!grids.isEmpty())
        drawGrids(painter);
    if (!bodys.isEmpty())
        drawBodys(painter);
    painter->end();
    e->accept();
}

void BoxQImage::mousePressEvent(QMouseEvent *e)
{
    emit clicked();
    e->accept();
}

