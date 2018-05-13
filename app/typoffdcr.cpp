/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       电阻后台配置界面
*******************************************************************************/
#include "typoffdcr.h"

TypOffDcr::TypOffDcr(QWidget *parent) : QWidget(parent)
{
}

void TypOffDcr::recvAppMsg(QTmpMap msg)
{
    int c = msg.value(AddrEnum).toInt();
    switch (c) {
    case Qt::Key_Copy:
        tmpSet = msg;
        break;
    default:
        break;
    }
}

