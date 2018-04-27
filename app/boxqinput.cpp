#include "boxqinput.h"

BoxQInput *BoxQInput::_instance = 0;

BoxQInput::BoxQInput(QWidget *parent) : QWidget(parent)
{
    initUI();
    initFilter();
}

void BoxQInput::initUI()
{
    initSkin();
    initLayout();
    initButton();
}

void BoxQInput::initSkin()
{
    QString style;
    style += "QPushButton{border:1px solid #121922;border-radius:0px;padding:0px;}";
    this->setStyleSheet(style);
}

void BoxQInput::initFilter()
{
    //绑定全局改变焦点信号槽
    connect(qApp, SIGNAL(focusChanged(QWidget *, QWidget *)),
            this, SLOT(focusChanged(QWidget *, QWidget *)));
    //绑定按键事件过滤器
    qApp->installEventFilter(this);
    upper = true;
    currentWidget = NULL;
}

void BoxQInput::initLayout()
{
    btnLayout = new QVBoxLayout;
    btnLayout->setMargin(0);
    btnLayout->setSpacing(0);

    QFrame *frame = new QFrame(this);
    frame->setLayout(btnLayout);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(frame);

    this->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
}

void BoxQInput::initButton()
{
    btns = new QButtonGroup;

    QList<int> ids;
    ids << Qt::Key_1 << Qt::Key_2 << Qt::Key_3 << Qt::Key_4 << Qt::Key_5
        << Qt::Key_6 << Qt::Key_7 << Qt::Key_8 << Qt::Key_9 << Qt::Key_0 << Qt::Key_Backspace
        << Qt::Key_Q << Qt::Key_W << Qt::Key_E << Qt::Key_R << Qt::Key_T
        << Qt::Key_Y << Qt::Key_U << Qt::Key_I << Qt::Key_O << Qt::Key_P << Qt::Key_CapsLock
        << Qt::Key_A << Qt::Key_S << Qt::Key_D << Qt::Key_F << Qt::Key_G
        << Qt::Key_H << Qt::Key_J << Qt::Key_K << Qt::Key_L << Qt::Key_Period << Qt::Key_Enter
        << Qt::Key_Z << Qt::Key_X << Qt::Key_C << Qt::Key_V << Qt::Key_B
        << Qt::Key_N << Qt::Key_M << Qt::Key_Minus << Qt::Key_Space;

    QStringList keys;
    keys << "1" << "2" << "3" << "4" << "5"
         << "6" << "7" << "8" << "9" << "0" << "←"
         << "Q" << "W" << "E" << "R" << "T"
         << "Y" << "U" << "I" << "O" << "P" << "Caps"
         << "A" << "S" << "D" << "F" << "G"
         << "H" << "J" << "K" << "L" << "." << "Enter"
         << "Z" << "X" << "C" << "V" << "B"
         << "N" << "M" << "-" << "Space";

    QGridLayout *layout = new QGridLayout;
    for (int i=0; i < 4; i++) {
        for (int t=0; t < 11; t++) {
            if (i*11 + t >= keys.size())
                break;
            QPushButton *btn = new QPushButton(this);
            btn->setFixedSize(60, 60);
            btn->setFocusPolicy(Qt::NoFocus);
            btn->setText(keys.at(i*11 + t));
            btns->addButton(btn, ids.at(i*11 + t));
            int r = 1;
            int c = 1;
            if (i == 2 && t == 10) {
                r = 2;
                btn->setFixedSize(60, 120);
            }
            if (i == 3 && t == 8) {
                c = 2;
                btn->setFixedSize(120, 60);
            }
            layout->addWidget(btn, i, t, r, c);
        }
    }
    layout->setMargin(0);
    btnLayout->addLayout(layout);
    connect(btns, SIGNAL(buttonClicked(int)), this, SLOT(clickButton(int)));
}

void BoxQInput::clickButton(int id)
{
    switch (id) {
    case Qt::Key_Enter :
        this->setVisible(false);
        QApplication::focusWidget()->clearFocus();
        break;
    case Qt::Key_CapsLock:
        upper = !upper;
        for (int i=0; i < btns->buttons().size(); i++) {
            if (btns->buttons().at(i)->text().size() < 2) {
                if (upper)
                    btns->buttons().at(i)->setText(btns->buttons().at(i)->text().toUpper());
                else
                    btns->buttons().at(i)->setText(btns->buttons().at(i)->text().toLower());
            }
        }

        break;
    default:
#ifdef __arm__
        if (id >= Qt::Key_A && id <= Qt::Key_Z && !upper)
            id += 32;
        QWSServer::sendKeyEvent(id, id, Qt::NoModifier, true, false);
#endif
        break;
    }
}

void BoxQInput::focusChanged(QWidget *, QWidget *curr)
{
#ifdef __arm__
    if (curr != 0 && !this->isAncestorOf(curr)) {
        bool visible = false;
        if (curr->inherits("QSpinBox"))
            visible = true;
        if (curr->inherits("QLineEdit"))
            visible = true;
        if (curr->inherits("QComboBox") && qobject_cast<QComboBox*>(curr)->isEditable()) {
            visible = true;
        }
        if (curr->inherits("QDateTimeEdit"))
            visible = true;
        if (curr->inherits("QDoubleSpinBox"))
            visible = true;
        this->setVisible(visible);
    }
#endif
}

void BoxQInput::showEvent(QShowEvent *)
{
    int mouse_y = QCursor::pos().y();//鼠标点击处纵坐标
    int x = 70;
    this->resize(660, 240);

    if(mouse_y>300) {
        this->move(x, mouse_y-280);
    } else {
        this->move(x, mouse_y+50);
    }
}
/*********************************END OF FILE**********************************/
