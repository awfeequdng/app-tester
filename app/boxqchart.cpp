/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       环形显示
*******************************************************************************/
#include "boxqchart.h"

BoxQChart::BoxQChart(QWidget *parent) : QLabel(parent)
{
    m_lenth = 0;
    m_timer = 0;
    m_count = 24;
}

void BoxQChart::setNum(int num)
{
    m_count = num;
    this->update();
}

void BoxQChart::setStr(QString text)
{
    m_title = text;
    if (m_timer%500) {
        this->update();
    }
    m_timer++;
}

void BoxQChart::setNews(QVariantMap msg)
{
    QList<int> t;
    t << AddrHighAG << AddrHighAC << AddrHighAL << AddrHighLC << AddrSetIMP;

    int addr = t.indexOf(msg.value("text").toInt());
    m_lenth = (addr + 1) * m_count / t.size();
    this->update();

}

void BoxQChart::mouseReleaseEvent(QMouseEvent *e)
{
    emit sendClick();
    e->accept();
}

void BoxQChart::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QFont font = painter.font();
    font.setPixelSize(11);
    painter.setFont(font);
    painter.setRenderHint(QPainter::Antialiasing);
    int side = qMin(width(), height());
    painter.translate(width()/2, height()/2);
    painter.scale(side/SCALE, side /SCALE);
    drawPie(&painter);
    drawCrown(&painter);
    drawTitle(&painter);
    drawScaleNum(&painter);
    painter.end();
    e->accept();
}

void BoxQChart::drawPie(QPainter *painter)
{  // 外圈扇形
    painter->save();
    double angle = 360/m_count;
    double startAngle = 90-angle/2;
    int radius = SCALE*48/100;

    painter->setPen(QPen(Qt::transparent));
    for (int i=0; i < m_count; i++) {
        if (i < m_lenth) {
            painter->setBrush(QBrush(QColor("#2C8EE3")));
        } else {
            painter->setBrush(QBrush(QColor(Qt::darkYellow)));
        }
        int t = qMin(3, qMax(1, (5 - m_count/12)));
        painter->drawPie(-radius, -radius, radius << 1, radius << 1,
                         int(startAngle*16), int((angle-t)*16));
        startAngle -= angle;
    }
    painter->restore();
}

void BoxQChart::drawCrown(QPainter *painter)
{ // 内圈圆形
    painter->save();
    int radius = SCALE*32/100;
    if (m_count < 12) {
        radius = SCALE*28/100;
    }
    painter->setBrush(QBrush(QColor("#191919")));
    painter->setPen(Qt::NoPen);
    painter->drawEllipse(-radius, -radius, radius << 1, radius << 1);
    painter->restore();
}

void BoxQChart::drawScaleNum(QPainter *painter)
{  // 刻度
    painter->save();
    double startRad = PI/2;
    double deltaRad = 2 * PI / m_count;
    double sina, cosa;
    int radius = SCALE*28/100;
    if (m_count < 12) {
        radius = SCALE*24/100;
    }
    int x, y;
    QFontMetricsF fm(this->font());
    double w, h, tmpVal;
    QString str;

    int ttt = 1;
    if (m_count > 24) {
        ttt = 2;
    }
    if (m_count > 36) {
        ttt = 3;
    }

    for (int i = 0; i < m_count; i+=ttt) {
        sina = sin(startRad - i * deltaRad);
        cosa = cos(startRad - i * deltaRad);
        tmpVal = 1.0 * i;
        str = QString("%1").arg(int(tmpVal) + 1);
        w = fm.size(Qt::TextSingleLine, str).width();
        h = fm.size(Qt::TextSingleLine, str).height();
        x = radius * cosa - w / 3;
        y = -radius * sina + h / 4;
        painter->drawText(x, y, str);
    }
    painter->restore();
}

void BoxQChart::drawTitle(QPainter *painter)
{
    painter->save();
    painter->setPen(Qt::white);
    QString str(m_title); //显示仪表的功能
    QFont font = painter->font();
    font.setPixelSize(24);
    painter->setFont(font);
    QFontMetricsF fm(painter->font());
    double w = fm.size(Qt::TextSingleLine, str).width();
    double h = fm.size(Qt::TextSingleLine, str).height();
    painter->drawText(-w*46/100, h/3, str);
    painter->restore();
}


