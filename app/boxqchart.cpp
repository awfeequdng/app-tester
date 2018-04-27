#include "boxqchart.h"

BoxQChart::BoxQChart(QWidget *parent) : QLabel(parent)
{
    m_scaleMajor = NUM;
    m_title = "测试中";
    m_units = "s";
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(setTimeOut()));
//    timer->start(10);

}

void BoxQChart::mouseReleaseEvent(QMouseEvent *e)
{
    emit sendClick();
    e->accept();
}

void BoxQChart::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);//一个类中的this表示一个指向该类自己的指针
    QFont font = painter.font();
    font.setPixelSize(11);
    painter.setFont(font);
    painter.setRenderHint(QPainter::Antialiasing);
    int side = qMin(width(), height());
    painter.translate(width()/2, height()/2);
    painter.scale(side/SCALE, side /SCALE);
    drawPie(&painter);
    drawCrown(&painter);
    drawScaleNum(&painter);
    drawTitle(&painter);
    painter.end();
    e->accept();
}

void BoxQChart::drawPie(QPainter *painter)
{  // 外圈扇形
    painter->save();
    double angle = 360/m_scaleMajor;
    double startAngle = 90-angle/2;
    int radius = SCALE*48/100;

    painter->setPen(QPen(Qt::transparent));
//    painter->setBrush(QBrush(QColor(Qt::darkYellow)));
    for (int i=0; i < m_scaleMajor; i++) {
        if (t/10%m_scaleMajor == m_scaleMajor-i-1) {
            painter->setBrush(QBrush(QColor(Qt::green)));
        } else {
            painter->setBrush(QBrush(QColor(Qt::darkYellow)));
        }
        if (i == NUM-12) {
            painter->setBrush(QBrush(QColor(Qt::red)));
        }
        int ttt = 3;
        if (NUM > 24)
            ttt = 2;
        if (NUM > 36)
            ttt = 1;
        painter->drawPie(-radius, -radius, radius << 1, radius << 1, int(startAngle*16), int((angle-ttt)*16));
        startAngle += angle;
    }
    painter->restore();
}

void BoxQChart::drawCrown(QPainter *painter)
{ // 内圈圆形
    painter->save();
    int radius = SCALE*32/100;
    painter->setBrush(QBrush(QColor(19,19,19,255)));
    painter->setPen(Qt::NoPen);
    painter->drawEllipse(-radius, -radius, radius << 1, radius << 1);
    painter->restore();
}

void BoxQChart::drawScaleNum(QPainter *painter)
{  // 刻度
    painter->save();
    double startRad = PI/2;
    double deltaRad = 2 * PI / m_scaleMajor;
    double sina,cosa;
    int radius = SCALE*28/100;
    int x, y;
    QFontMetricsF fm(this->font());
    double w, h, tmpVal;
    QString str;

    int ttt = 1;
    if (NUM > 24) {
        ttt = 2;
    }
    if (NUM > 36) {
        ttt = 3;
    }

    for (int i = 0; i < m_scaleMajor; i+=ttt) {
        sina = sin(startRad - i * deltaRad);
        cosa = cos(startRad - i * deltaRad);
        tmpVal = 1.0 * i;
        str = QString( "%1" ).arg(int(tmpVal)+1);
        w = fm.size(Qt::TextSingleLine,str).width();
        h = fm.size(Qt::TextSingleLine,str).height();
        x = radius * cosa - w / 3;
        y = -radius * sina + h / 4;
        painter->drawText(x, y, str);

    }
    painter->restore();
}

void BoxQChart::drawParamNum(QPainter *painter)
{
    painter->save();
    painter->setPen(Qt::white);
    // m_scaleMajor在一个量程中分成的刻度数
    double startRad = PI/2;
    double deltaRad = 2 * PI / m_scaleMajor;
    double sina,cosa;
    int radius = SCALE*48/100;
    int x, y;
    QFontMetricsF fm(this->font());
    double w, h, tmpVal;
    QString str;

    for (int i = 0; i < m_scaleMajor; i++) {
        sina = sin(startRad - i * deltaRad);
        cosa = cos(startRad - i * deltaRad);
        tmpVal = 3000+i;
        str = QString( "%1" ).arg(int(tmpVal)+1);  //%1作为占位符   arg()函数比起 sprintf()来是类型安全的
        w = fm.size(Qt::TextSingleLine,str).width();
        h = fm.size(Qt::TextSingleLine,str).height();
        x = radius * cosa - w / 4;
        y = -radius * sina + h / 4;
        painter->drawText(x, y, str); //函数的前两个参数是显示的坐标位置，后一个是显示的内容，是字符类型""

    }
    painter->restore();
}

void BoxQChart::drawParamNum2(QPainter *painter)
{
    painter->save();
    painter->setPen(Qt::white);
    // m_scaleMajor在一个量程中分成的刻度数
    double startRad = PI/2;
    double deltaRad = 2 * PI / m_scaleMajor;
    double sina,cosa;
    int radius = SCALE*28/100;
    int x, y;
    QFontMetricsF fm(this->font());
    double w, h, tmpVal;
    QString str;

    for (int i = 0; i < m_scaleMajor; i++) {
        sina = sin(startRad - i * deltaRad);
        cosa = cos(startRad - i * deltaRad);
        tmpVal = 30+i;
        str = QString( "%1" ).arg(int(tmpVal)+1);  //%1作为占位符   arg()函数比起 sprintf()来是类型安全的
        w = fm.size(Qt::TextSingleLine,str).width();
        h = fm.size(Qt::TextSingleLine,str).height();
        x = radius * cosa - w / 4;
        y = -radius * sina + h / 4;
        painter->drawText(x, y, str); //函数的前两个参数是显示的坐标位置，后一个是显示的内容，是字符类型""

    }
    painter->restore();
}

void BoxQChart::drawClock(QPainter *painter)
{
    painter->save();
    int radius = SCALE*12/100;
    double startAngle = 90;
    painter->setPen(QPen(Qt::transparent));
    painter->setBrush(QBrush(QColor(Qt::darkGreen)));
    painter->drawPie(-radius, -radius, radius << 1, radius << 1, int(startAngle*16), int(-(30*t/100)*16));
    painter->restore();
}

void BoxQChart::drawTitle(QPainter *painter)
{
    painter->save();
    painter->setPen(Qt::white);
    QString str(m_title); //显示仪表的功能
    QFont font = painter->font();
    font.setPixelSize(28);
    painter->setFont(font);
    QFontMetricsF fm(painter->font());
    double w = fm.size(Qt::TextSingleLine,str).width();
    double h = fm.size(Qt::TextSingleLine,str).height();
    painter->drawText(-w*46/100, h/3, str);
    painter->restore();
}

void BoxQChart::drawNumericValue(QPainter *painter)
{
    QString str = QString("%1 %2").arg(t%1200/100).arg(m_units);
    QFontMetricsF fm(font());
    double w = fm.size(Qt::TextSingleLine,str).width();
    painter->setPen(Qt::white);
    painter->drawText(-w / 4, 42, str);
}

void BoxQChart::setTimeOut()
{
    t++;
    if (t == 100) {
        m_title = "测试中";
    }
    if (t%10 == 0) {
        this->update();
    }
//    t %= 1200;
}


