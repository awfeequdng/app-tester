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
    isTurn = 0;
    m_turn = 0;
    m_start = 0;
    m_lenth = 0;
    m_timer = 0;
    m_width = 34;
    m_count = 24;
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(setStr()));
    timer->start(10);
    setStr();
}

void BoxQChart::setNum(int num)
{
    m_count = num;
    if (num <= 36)
        m_width = 33;
    if (num <= 24)
        m_width = 32;
    if (num <= 12)
        m_width = 28;
    if (num <= 8)
        m_width = 24;
    this->update();
}

void BoxQChart::setRow(int num)
{
    rows.append(num);
    this->update();
}

void BoxQChart::setPie(int num)
{
    pies.append(num);
    this->update();
}

void BoxQChart::setRun(int num)
{
    if (num != 0)
        isTurn = 0;
    m_start = num;
    if (num > 0) {
        m_timer = 0;
        m_turn = 0;
        pies.clear();
        rows.clear();
    }
}

void BoxQChart::setTurn(int turn)
{
    m_turn = turn;
    isTurn = 1;
}

void BoxQChart::setTime(double t)
{
    m_title = QString::number(t) + "s";
    m_timer = 1;
    this->update();
}

void BoxQChart::setStr()
{
    if (m_timer % 200 == 0) {
        m_title = QTime::currentTime().toString("hh:mm");
    }
    if (m_timer % 10 == 0) {
        this->update();
    }
    m_timer++;
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
    drawRow(&painter);
    drawPie(&painter);
    drawTurn(&painter);
    drawCrown(&painter);
    drawTitle(&painter);
    drawScaleNum(&painter);
    painter.end();
    e->accept();
}

void BoxQChart::drawRow(QPainter *painter)
{
    painter->save();
    double angle = 360.0/m_count;
    //    double startAngle = 90-angle;
    double startAngle = 90;
    int radius = SCALE*49/100;
    int t = 1;
    if (m_count <= 36)
        t = 2;
    if (m_count <= 12)
        t = 3;
    if (m_count <= 8)
        t = 4;
    painter->setBrush(QBrush(QColor("#121912")));
    for (int i=0; i < m_count; i++) {
        if (rows.contains(i)) {
            painter->setPen(QPen(QColor("#FF0000"), 2, Qt::SolidLine));
        } else {
            painter->setPen(QPen(Qt::darkYellow, 2, Qt::SolidLine));
        }
        painter->drawPie(-radius, -radius, radius << 1, radius << 1,
                         int(startAngle*16), int((angle-t)*16));
        startAngle -= angle;
    }
    painter->restore();
}

void BoxQChart::drawPie(QPainter *painter)
{  // 外圈扇形
    painter->save();
    double angle = 360.0/m_count;
    double startAngle = 90-angle/2;
    int radius = SCALE*46/100;
    int t = 1;
    if (m_count <= 36)
        t = 2;
    if (m_count <= 12)
        t = 3;
    if (m_count <= 8)
        t = 4;
    painter->setPen(QPen(Qt::transparent));
    for (int i=0; i < m_count; i++) {
        if (pies.contains(i)) {
            painter->setBrush(QBrush(QColor("#FF0000")));
        } else {
            painter->setBrush(QBrush(QColor("#808000")));
        }
        if (m_start > 0 && ((m_timer / 10) % m_count == i)) {
            painter->setBrush(QBrush(QColor("#00FF00")));
        }
        painter->drawPie(-radius, -radius, radius << 1, radius << 1,
                         int(startAngle*16), int((angle-t)*16));
        startAngle -= angle;
    }
    painter->restore();
}

void BoxQChart::drawTurn(QPainter *painter)
{
    if (isTurn == 0)
        return;
    painter->save();
    int radius = SCALE*65/100;
    int t = 1;
    if (m_count <= 36)
        t = 2;
    if (m_count <= 12)
        t = 3;
    if (m_count <= 8)
        t = 4;
    if (m_turn >= 2)
        painter->setPen(QPen(QColor("#FF0000"), 2, Qt::SolidLine));
    else
        painter->setPen(QPen(QColor("#808000"), 2, Qt::SolidLine));
    if (m_turn%2 == 0) {
        painter->drawArc(-radius, -radius, radius << 1, radius << 1, int(-30*16), int((72-t)*16));
        painter->drawLine(radius-17, radius/2, radius - 4, radius/2 - 12);
        painter->drawLine(radius-18, radius/2, radius - 17, radius/2 - 17);
    } else {
        painter->drawArc(-radius, -radius, radius << 1, radius << 1, int(140*16), int((72-t)*16));
        painter->drawLine(-radius+17, radius/2, -radius + 4, radius/2 - 12);
        painter->drawLine(-radius+18, radius/2, -radius + 16, radius/2 - 17);
    }
    painter->restore();
}

void BoxQChart::drawCrown(QPainter *painter)
{ // 内圈圆形
    painter->save();
    int radius = SCALE*m_width/100;
    painter->setBrush(QBrush(QColor("#121922")));
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
    int radius = SCALE*(m_width-4)/100;
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


