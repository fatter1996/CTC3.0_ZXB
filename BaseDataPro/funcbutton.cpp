#include "funcbutton.h"

FUNCBUTTON::FUNCBUTTON()
{
    p_ButtonPt=QPoint(0,0);//按钮坐标
    p_ButtonPtConst=QPoint(0,0);
    rect_Button=QRect(0,0,0,0);//按钮区域
    isDown_Button=false;

    m_strType="";		//按钮类型
    m_strJuncType="";	//关联设备或类型
    DEVName="";
    DEVBtnType="";     //关联按钮类型
    Button_Name="";
    Button_QF=0;
    haveButton=true;
    haveQF = true;

    m_RangeVisible=false;//鼠标移动至设备上以后的高亮显示
    ButtonForm=1;//按钮形状,0为圆,1为方;
    m_nDownTime_1 = 0;   //当按钮按下为1时持续时间

    wordcolor = 3;//文字颜色//默认白色
    wordsize = 10;//文字大小12
}
void FUNCBUTTON::Draw_ToolTip(QPainter *painter, double nDiploid)
{
    QFont font;
    QPoint pt;
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, true);
    //绘制鼠标进入文字提示信息
    if((true == getToolTipFlag()) && (""!=getToolTipStr()))
    {
        pt.setX(pCenter.x()+5*nDiploid);
        pt.setY(pCenter.y()-10*nDiploid);
        font.setFamily("宋体");
        font.setPointSize(9 * nDiploid);//字号
        font.setItalic(false);//斜体

        QFontMetrics fm(font);
        QRect rec = fm.boundingRect(getToolTipStr());

        painter->setPen(QPen(QColor(38, 38, 38), 1));
        painter->setBrush(QColor(252, 245, 221));
        painter->drawRect(pt.x(), pt.y(), rec.width()+10*nDiploid, rec.height()+4*nDiploid);

        painter->setFont(font);//设置字体
        painter->setPen(QColor(38, 38, 38));//设置画笔颜色
        //绘制文本
        painter->drawText(pt.x()+5*nDiploid, pt.y()+12* nDiploid, getToolTipStr());
    }
}
void FUNCBUTTON::Draw(QPainter *painter,long nElapsed, double nDiploid,QPoint offset,int type)
{
    if(type==0x55)
    {
        Draw_Th_kb(painter,nElapsed, nDiploid,offset);
    }
}
void FUNCBUTTON::Draw_Th_kb(QPainter *painter,long nElapsed, double nDiploid,QPoint offset)
{
    QFont font;
    if(ButtonForm == 1)
    {
        rect_Button.setRect(p_ButtonPtConst.x() * nDiploid + offset.x() * nDiploid,p_ButtonPtConst.y() * nDiploid + offset.y() * nDiploid,10*nDiploid,10*nDiploid);
    }
    else
    {
        rect_Button.setRect(p_ButtonPtConst.x() * nDiploid + offset.x() * nDiploid,p_ButtonPtConst.y() * nDiploid + offset.y() * nDiploid,14*nDiploid,14*nDiploid);
    }

    //CALLON引导//CALLON_BTN引导按钮
    //CALLONLOCK引导总锁//CALLON_LOCK引导总锁按钮
    //ALLMAN_RELEASE总人解//ALLMAN_RELEASE总人解按钮
    //ALL_CLEAR总取消//FUNC_BTN功能按钮
    //CHANGEDIREC允许改方
    //CLEAR清除
    //SDJS上电解锁
    //SPEECHPAUSE语音暂停
    if(m_strType == "CALLONLOCK"
      || m_strType == "GZTZ"
      || m_strType == "CLEAR"
      || m_strType == "SDJS"
      || m_strType == "ALL_CLEAR"
      || m_strType == "DCANFW"
      || m_strType == "SPEECHPAUSE"
      || m_strType == "FIELDLOCK"
      || m_strType == "CHANGEDIREC"
      )//全站封锁
    {
//        haveQF = false;
    }
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, true);

    font.setFamily("宋体");
    font.setPointSize(10 * nDiploid);//字号
    //font.setPointSize(wordsize * nDiploid);//字号

    font.setItalic(false);//斜体
    painter->setFont(font);//设置字体
    painter->setPen(Qt::white);//设置画笔颜色

    if(haveButton)
    {
        if(ButtonForm == 1)
        {
            Draw_Rect_Button(painter,//画笔
                             rect_Button,//按钮绘制区域//需要修改按钮区域大小为10
                             bFCZKState,//非常站控状态
                             getIsDown_Button(),//按钮是否按下
                             false,//封锁状态
                             false,//终端闪烁状态
                             0,//倒计时时间
                             THIN_GRAY,//按钮颜色
                             haveQF,
                             Button_Name, //按钮文字显示
                             m_RangeVisible,//按钮是否高亮
                             nElapsed,//闪烁读秒
                             nDiploid,//放大倍数
                             offset);//绘制KSK方形按钮
        }
        else
        {
            Draw_circle_Button(painter,//画笔
                               rect_Button,//按钮绘制区域//需要修改按钮区域大小为14
                               bFCZKState,//非常站控状态
                               getIsDown_Button(),//按钮是否按下
                               false,//封锁状态
                               false,//终端闪烁状态
                               0,//倒计时时间
                               haveQF,
                               Button_Name, //按钮文字显示
                               m_RangeVisible,//按钮是否高亮
                               nElapsed, //闪烁读秒
                               nDiploid, //放大倍数
                               offset);//绘制圆形按钮
        }
        //if(VISIBLE_JSQ)
        if(haveQF&&bShowQFJS)
        {//绘制总辅助铅封 haveAN
            if(haveAN)
            {
                if(bVisleJSQ)
                {
                    QPoint ZFZQF;
                    ZFZQF.setX(rect_Button.x()-8*nDiploid);
                    ZFZQF.setY(rect_Button.y()-20*nDiploid);
                    DrawQFRect(painter,ZFZQF, Button_QF, nElapsed,nDiploid,offset);
                }
            }
            else if(Button_QF>0)
            {
                QPoint ZFZQF;
                ZFZQF.setX(rect_Button.x()-8*nDiploid);
                ZFZQF.setY(rect_Button.y()/*-20*nDiploid*/);
                DrawQFRect(painter,ZFZQF, Button_QF, nElapsed,nDiploid,offset);
            }
        }
    }
}
void FUNCBUTTON::setVollover(QPoint pt_Base)
{

}
unsigned int FUNCBUTTON::getDevType()
{
    return Dev_FUNCB;
}

void FUNCBUTTON::setButtonPt(QPoint pt)
{
    p_ButtonPt=pt;
    p_ButtonPtConst=pt;
}
QPoint FUNCBUTTON::getButtonPt()
{
    return p_ButtonPt;
}

void FUNCBUTTON::setIsDown_Button(int flag)
{
    isDown_Button=flag;
}
int FUNCBUTTON::getIsDown_Button()
{
    return isDown_Button;
}

void FUNCBUTTON::DrawQFRect(QPainter *painter,QPointF QFPoint, int m_nQFNumber, long nElapsed, double nDiploid,QPoint offset)
{
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, false);

    QRect m_rectQF;

    m_rectQF = QRect(0,0,0,0);

    //初始化铅封框
    m_rectQF = QRect(QFPoint.x(),
                     QFPoint.y(),
                     25*nDiploid,
                     14*nDiploid);

    QPen pen;
    pen.setColor(YELLOW);
    pen.setWidth(1);
    painter->setPen(pen);
    QBrush newBrushBK;
    newBrushBK.setStyle(Qt::NoBrush);
    painter->setBrush(newBrushBK);
    painter->drawRect(m_rectQF);

    pen.setColor(WHITE);
    painter->setPen(pen);
    if(m_nQFNumber == 0)
    {
        painter->drawText(m_rectQF,"");
    }
    else if(m_nQFNumber > 0 && m_nQFNumber < 10)
    {
        QString str = QString("%1").arg(m_nQFNumber);
        painter->drawText(m_rectQF,Qt::AlignHCenter|Qt::AlignVCenter,"00" + str);
    }
    else if(m_nQFNumber >9 && m_nQFNumber < 100)
    {
        QString str = QString("%1").arg(m_nQFNumber);
        painter->drawText(m_rectQF,Qt::AlignHCenter|Qt::AlignVCenter,"0" + str);
    }
    else
    {
        QString str = QString("%1").arg(m_nQFNumber);
        painter->drawText(m_rectQF,Qt::AlignHCenter|Qt::AlignVCenter,str);
    }

}

void FUNCBUTTON::Draw_circle_Button(QPainter *painter,//画笔
                              QRectF m_ButtonRect,//按钮绘制区域
                              bool m_FCZKState,//非常站控状态
                              int m_ButtonIsDownState,//按钮是否按下
                              bool m_FSState,//封锁状态
                              bool m_bZDANFlash,//终端闪烁状态
                              int mTimeCount,//倒计时时间
                              bool HaveQF,//是否有铅封计数
                              QString rectTextShow, //按钮文字显示
                              bool RangeVisible,//按钮是否高亮
                              long nElapsed, //闪烁读秒
                              double nDiploid, //放大倍数
                              QPoint offset)//绘制圆形按钮
{
#pragma region//>..调车按钮绘制
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, true);

    //按钮边框默认颜色
    QColor btnClrTopLeft = WHITE;//按钮边框-TL
    QColor btnClrBottomRight = GRAY;//按钮边框-BR
//    if(m_RangeVisible)
//    {
//        btnClrTopLeft = THIN_BLUE_DOWN;//按钮边框-TL
//        btnClrBottomRight = THIN_BLUE_DOWN;//按钮边框-BR
//    }
    //按钮动作正在按下
//    if (0<m_ButtonIsDownState && m_ButtonIsDownState<4)
//    {
//        if (nElapsed % 2 == 0)
//        {
//            btnClrTopLeft = WHITE;//按钮边框-TL
//            btnClrBottomRight = GRAY;//按钮边框-BR
//        }
//        else
//        {
//            btnClrTopLeft = GRAY;//按钮边框-TL
//            btnClrBottomRight = WHITE;//按钮边框-BR
//        }
//    }
    if (m_ButtonIsDownState==1)
    {
        if (nElapsed % 2 == 0)
        {
            btnClrTopLeft = WHITE;//按钮边框-TL
            btnClrBottomRight = GRAY;//按钮边框-BR
        }
        else
        {
            btnClrTopLeft = GRAY;//按钮边框-TL
            btnClrBottomRight = WHITE;//按钮边框-BR
        }
    }
    if (m_ButtonIsDownState==2)
    {
        btnClrTopLeft = GRAY;//按钮边框-TL
        btnClrBottomRight = WHITE;//按钮边框-BR
    }
    //绘制下半圆
    QPen pen;
    pen.setColor(btnClrBottomRight);
    pen.setWidth(2);
    painter->setPen(pen);
    painter->drawArc(m_ButtonRect, 180*16, 180*16);
    //绘制上半圆
    pen.setColor(btnClrTopLeft);
    painter->setPen(pen);
    painter->drawArc(m_ButtonRect, 0, 180*16);

    //绘制调车按钮圆体
    if(m_FCZKState)//非常站控模式下
    {
        QBrush newBrushBK;
        newBrushBK.setStyle(Qt::SolidPattern);
        if(m_ButtonIsDownState == 2)
        {
            newBrushBK.setColor(RED);
            painter->setBrush(newBrushBK);

            pen.setColor(RED);
            painter->setPen(pen);
        }
        else
        {
            newBrushBK.setColor(THIN_GRAY);
            painter->setBrush(newBrushBK);

            pen.setColor(THIN_GRAY);
            painter->setPen(pen);
        }
    }
    else if(m_FSState)//按钮封锁-粉闪
    {
        if(nElapsed % XHDBTN_FLASTIME == 0) //控制时间间隔
        {
            if(m_ButtonIsDownState == 1)
            {
                QBrush newBrushBK;
                newBrushBK.setStyle(Qt::SolidPattern);
                newBrushBK.setColor(BLUE);
                painter->setBrush(newBrushBK);

                pen.setColor(BLUE);
                painter->setPen(pen);
            }
            else
            {
                QBrush newBrushBK;
                newBrushBK.setStyle(Qt::SolidPattern);
                newBrushBK.setColor(THIN_GRAY);
                painter->setBrush(newBrushBK);

                pen.setColor(THIN_GRAY);
                painter->setPen(pen);
            }
        }
        else
        {
            QBrush newBrushBK;
            newBrushBK.setStyle(Qt::SolidPattern);
            newBrushBK.setColor(BROWN);
            painter->setBrush(newBrushBK);

            pen.setColor(BROWN);
            painter->setPen(pen);
        }
    }
    else if(RangeVisible)
    {
        QBrush newBrushBK;
        newBrushBK.setStyle(Qt::SolidPattern);
        newBrushBK.setColor(THIN_BLUE_DOWN);
        painter->setBrush(newBrushBK);

        pen.setColor(THIN_BLUE_DOWN);
        painter->setPen(pen);
    }
    else
    {
        switch (m_ButtonIsDownState)
        {
        case 0:
            {
                QBrush newBrushBK;
                newBrushBK.setStyle(Qt::SolidPattern);
                newBrushBK.setColor(THIN_GRAY);
                painter->setBrush(newBrushBK);

                pen.setColor(THIN_GRAY);
                painter->setPen(pen);
                break;
            }
        case 1:
            {
                if(nElapsed % XHDBTN_FLASTIME == 0) //控制时间间隔
                {
                    QBrush newBrushBK;
                    newBrushBK.setStyle(Qt::SolidPattern);
                    newBrushBK.setColor(WHITEX);
                    painter->setBrush(newBrushBK);

                    pen.setColor(WHITEX);
                    painter->setPen(pen);
                }
                else
                {
                    QBrush newBrushBK;
                    newBrushBK.setStyle(Qt::SolidPattern);
                    newBrushBK.setColor(BLUE);
                    painter->setBrush(newBrushBK);

                    pen.setColor(BLUE);
                    painter->setPen(pen);
                }
                break;
            }
        case 2:
            {
                QBrush newBrushBK;
                newBrushBK.setStyle(Qt::SolidPattern);
                newBrushBK.setColor(RED);
                painter->setBrush(newBrushBK);

                pen.setColor(RED);
                painter->setPen(pen);
                break;
            }
        case 4://可按下，终端闪烁
            {
                if(m_bZDANFlash)
                {
                    if(nElapsed % XHDBTN_FLASTIME == 0) //控制时间间隔
                    {
                        QBrush newBrushBK;
                        newBrushBK.setStyle(Qt::SolidPattern);
                        newBrushBK.setColor(YELLOW);
                        painter->setBrush(newBrushBK);

                        pen.setColor(YELLOW);
                        painter->setPen(pen);
                    }
                    else
                    {
                        QBrush newBrushBK;
                        newBrushBK.setStyle(Qt::SolidPattern);
                        newBrushBK.setColor(GREY);
                        painter->setBrush(newBrushBK);

                        pen.setColor(GREY);
                        painter->setPen(pen);
                    }
                }
                else
                {
                    QBrush newBrushBK;
                    newBrushBK.setStyle(Qt::SolidPattern);
                    newBrushBK.setColor(GREY);
                    painter->setBrush(newBrushBK);

                    pen.setColor(GREY);
                    painter->setPen(pen);
                }
                break;
            }
        }
    }
    QPointF point = QPointF(m_ButtonRect.left()+7*nDiploid,m_ButtonRect.top()+7*nDiploid);
    painter->drawEllipse(point,5 * nDiploid,5 * nDiploid);

    //封锁X
    if(m_FSState)
    {
        QPen penX;
        penX.setColor(RED);
        penX.setWidth(2);
        painter->setPen(penX);

        QRectF m_rectDC_X;       //调车按钮区域
        m_rectDC_X.setLeft(m_ButtonRect.left());
        m_rectDC_X.setTop(m_ButtonRect.top());
        m_rectDC_X.setRight(m_ButtonRect.right());
        m_rectDC_X.setBottom(m_ButtonRect.bottom());

        painter->drawLine(m_rectDC_X.left(), m_rectDC_X.top(),m_rectDC_X.right(), m_rectDC_X.bottom());
        painter->drawLine(m_rectDC_X.right(), m_rectDC_X.top(),m_rectDC_X.left(), m_rectDC_X.bottom());
    }
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, false);

    //倒计时
    if((0<mTimeCount)&&(255>mTimeCount))
    {
        QString strTimeCount;
        strTimeCount=QString::number(mTimeCount);

        QPen penX;
        penX.setColor(RED);
        penX.setWidth(1);
        painter->setPen(penX);
        QRectF m_rectDC_X;
        m_rectDC_X.setLeft(m_ButtonRect.left()-20 * nDiploid);
        m_rectDC_X.setTop(m_ButtonRect.top()-20 * nDiploid);
        m_rectDC_X.setRight(m_ButtonRect.right()+20 * nDiploid);
        m_rectDC_X.setBottom(m_ButtonRect.bottom()-20 * nDiploid);

        painter->drawText(m_rectDC_X,Qt::AlignHCenter|Qt::AlignVCenter,strTimeCount);
    }
    //绘制按钮的文字
    if(rectTextShow != ""&&bBtnNameShow)
    {
        if(HaveQF)
        {
            pen.setColor(YELLOW);
        }
        else
        {
            pen.setColor(WHITE);
        }
        QFont font;
        font.setWeight(50);//字体宽度,0~99
//        if(wordsize<=12)
//        {
//            font.setWeight(50);//字体宽度,0~99
//        }
//        else
//        {
//            font.setWeight(wordsize*5);//字体宽度,0~99
//        }

//        font.setPointSize(12 * nDiploid);//字体高度,10表示高度为10个像素点
        font.setPointSize(wordsize * nDiploid);//字体高度,10表示高度为10个像素点
        painter->setPen(pen);
        painter->setFont(font);
        QRectF rectBtnWord_X;
        rectBtnWord_X.setLeft(m_ButtonRect.left()-30 * nDiploid);
        rectBtnWord_X.setTop(m_ButtonRect.top()+15 * nDiploid);
        rectBtnWord_X.setRight(m_ButtonRect.right()+30 * nDiploid);
        rectBtnWord_X.setBottom(m_ButtonRect.bottom()+25 * nDiploid);
//        painter->drawText(rectBtnTG_X,Qt::AlignHCenter|Qt::AlignVCenter,m_strName + "TA");
        painter->drawText(rectBtnWord_X,Qt::AlignHCenter|Qt::AlignVCenter,rectTextShow);
    }
    if(RangeVisible)
    {
        QRectF textRect;
        textRect.setLeft(m_ButtonRect.left()-4 * nDiploid);
        textRect.setTop(m_ButtonRect.top()-4 * nDiploid);
        textRect.setRight(m_ButtonRect.right()+4);
        textRect.setBottom(m_ButtonRect.bottom()+4 * nDiploid);
        painter->setPen(QPen(Qt::white, 1,Qt::DashLine));
        painter->setBrush(THIN_BLUE_DOWN);
        painter->setBrush(Qt::transparent);
        painter->drawRect(textRect);
        painter->setPen(BLACK);
    }
#pragma endregion
}

void FUNCBUTTON::Draw_Rect_Button(QPainter *painter,//画笔
                              QRectF m_ButtonRect,//按钮绘制区域
                              bool m_FCZKState,//非常站控状态
                              int m_ButtonIsDownState,//按钮是否按下
                              bool m_FSState,//封锁状态
                              bool m_bZDANFlash,//终端闪烁状态
                              int mTimeCount,//倒计时时间
                              QColor color,//按钮颜色
                              bool HaveQF,//是否有铅封计数
                              QString rectTextShow, //按钮文字显示
                              bool RangeVisible,//按钮是否高亮
                              long nElapsed,//闪烁读秒
                              double nDiploid,//放大倍数
                              QPoint offset)//绘制KSK方形按钮
{
#pragma region//>..列车按钮绘制
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, false);

    //按钮边框默认颜色
    QColor btnClrTopLeft = WHITE;//按钮边框-TL
    QColor btnClrBottomRight = GRAY ;//按钮边框-BR

    //绘制按钮边框
     QPen pen_bk;
    if(/*1*/haveAN)
    {

        pen_bk.setColor(btnClrBottomRight);
        pen_bk.setWidth(2);
        painter->setPen(pen_bk);
        painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.bottom()+2,m_ButtonRect.right()+2,m_ButtonRect.bottom()+2);
        painter->drawLine(m_ButtonRect.right()+2,m_ButtonRect.bottom()+2,m_ButtonRect.right()+2,m_ButtonRect.top()-2);

        pen_bk.setColor(btnClrTopLeft);
        pen_bk.setWidth(2);
        painter->setPen(pen_bk);
        painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.bottom()+2,m_ButtonRect.left()-2,m_ButtonRect.top()-2);
        painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.top()-2,m_ButtonRect.right()+2,m_ButtonRect.top()-2);
    }
//    if(m_RangeVisible)
//    {
//        btnClrTopLeft = THIN_BLUE_DOWN;//按钮边框-TL
//        btnClrBottomRight = THIN_BLUE_DOWN;//按钮边框-BR
//    }
    //按钮动作正在按下
//    if (0<m_ButtonIsDownState && m_ButtonIsDownState<4)
//    {
//        if (nElapsed % 2 == 0)
//        {
//            btnClrTopLeft = WHITE;//按钮边框-TL
//            btnClrBottomRight = GRAY;//按钮边框-BR
//        }
//        else
//        {
//            btnClrTopLeft = GRAY;//按钮边框-TL
//            btnClrBottomRight = WHITE;//按钮边框-BR
//        }
//    }
//    if(!m_FCZKState)
    if(1)
    {
        if (m_ButtonIsDownState==1)
        {
            if (nElapsed % 2 == 0)
            {
                btnClrTopLeft = WHITE;//按钮边框-TL
                btnClrBottomRight = GRAY;//按钮边框-BR

                pen_bk.setColor(btnClrBottomRight);
                pen_bk.setWidth(2);
                painter->setPen(pen_bk);
                painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.bottom()+2,m_ButtonRect.right()+2,m_ButtonRect.bottom()+2);
                painter->drawLine(m_ButtonRect.right()+2,m_ButtonRect.bottom()+2,m_ButtonRect.right()+2,m_ButtonRect.top()-2);

                pen_bk.setColor(btnClrTopLeft);
                pen_bk.setWidth(2);
                painter->setPen(pen_bk);
                painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.bottom()+2,m_ButtonRect.left()-2,m_ButtonRect.top()-2);
                painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.top()-2,m_ButtonRect.right()+2,m_ButtonRect.top()-2);

            }
            else
            {
                btnClrTopLeft = GRAY;//按钮边框-TL
                btnClrBottomRight = WHITE;//按钮边框-BR

                pen_bk.setColor(btnClrTopLeft);
                pen_bk.setWidth(2);
                painter->setPen(pen_bk);
                painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.bottom()+2,m_ButtonRect.left()-2,m_ButtonRect.top()-2);
                painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.top()-2,m_ButtonRect.right()+2,m_ButtonRect.top()-2);

                pen_bk.setColor(btnClrBottomRight);
                pen_bk.setWidth(2);
                painter->setPen(pen_bk);
                painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.bottom()+2,m_ButtonRect.right()+2,m_ButtonRect.bottom()+2);
                painter->drawLine(m_ButtonRect.right()+2,m_ButtonRect.bottom()+2,m_ButtonRect.right()+2,m_ButtonRect.top()-2);
            }
        }
        if (m_ButtonIsDownState==2)
        {
            if((m_strType == "SDJS")||(m_strType == "CLEAR"))
            {
                btnClrTopLeft = WHITE;//按钮边框-TL
                btnClrBottomRight = GRAY;//按钮边框-BR

                pen_bk.setColor(btnClrBottomRight);
                pen_bk.setWidth(2);
                painter->setPen(pen_bk);
                painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.bottom()+2,m_ButtonRect.right()+2,m_ButtonRect.bottom()+2);
                painter->drawLine(m_ButtonRect.right()+2,m_ButtonRect.bottom()+2,m_ButtonRect.right()+2,m_ButtonRect.top()-2);

                pen_bk.setColor(btnClrTopLeft);
                pen_bk.setWidth(2);
                painter->setPen(pen_bk);
                painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.bottom()+2,m_ButtonRect.left()-2,m_ButtonRect.top()-2);
                painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.top()-2,m_ButtonRect.right()+2,m_ButtonRect.top()-2);

            }
            else
            {
                btnClrTopLeft = GRAY;//按钮边框-TL
                btnClrBottomRight = WHITE;//按钮边框-BR

                pen_bk.setColor(btnClrTopLeft);
                pen_bk.setWidth(2);
                painter->setPen(pen_bk);
                painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.bottom()+2,m_ButtonRect.left()-2,m_ButtonRect.top()-2);
                painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.top()-2,m_ButtonRect.right()+2,m_ButtonRect.top()-2);

                pen_bk.setColor(btnClrBottomRight);
                pen_bk.setWidth(2);
                painter->setPen(pen_bk);
                painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.bottom()+2,m_ButtonRect.right()+2,m_ButtonRect.bottom()+2);
                painter->drawLine(m_ButtonRect.right()+2,m_ButtonRect.bottom()+2,m_ButtonRect.right()+2,m_ButtonRect.top()-2);
            }
        }
        if(RangeVisible)
        {
            btnClrTopLeft = WHITE;//按钮边框-TL
            btnClrBottomRight = GRAY ;//按钮边框-BR

            pen_bk.setColor(btnClrBottomRight);
            pen_bk.setWidth(2);
            painter->setPen(pen_bk);
            painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.bottom()+2,m_ButtonRect.right()+2,m_ButtonRect.bottom()+2);
            painter->drawLine(m_ButtonRect.right()+2,m_ButtonRect.bottom()+2,m_ButtonRect.right()+2,m_ButtonRect.top()-2);

            pen_bk.setColor(btnClrTopLeft);
            pen_bk.setWidth(2);
            painter->setPen(pen_bk);
            painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.bottom()+2,m_ButtonRect.left()-2,m_ButtonRect.top()-2);
            painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.top()-2,m_ButtonRect.right()+2,m_ButtonRect.top()-2);
        }
    }

    QBrush newBrushBK;
    newBrushBK.setStyle(Qt::SolidPattern);
    QPen pen;
    pen.setWidth(2);
    pen.setJoinStyle(Qt::PenJoinStyle ::MiterJoin);
    //绘制按钮主体
//    if(m_FCZKState)//非常站控模式下
    if(0)//非常站控模式下
    {
        if(m_ButtonIsDownState == 2)
        {
            newBrushBK.setColor(RED);
            painter->setBrush(newBrushBK);

            pen.setColor(RED);
            painter->setPen(pen);
        }
        else
        {
            newBrushBK.setColor(THIN_GRAY);
            painter->setBrush(newBrushBK);

            pen.setColor(THIN_GRAY);
            painter->setPen(pen);
        }
    }
    else if(m_FSState)//按钮封锁-粉闪
    {
        if(nElapsed % XHDBTN_FLASTIME == 0) //控制时间间隔
        {
            if(m_ButtonIsDownState == 1)
            {
                newBrushBK.setColor(BLUE);
                painter->setBrush(newBrushBK);

                pen.setColor(BLUE);
                painter->setPen(pen);
            }
            else
            {
                newBrushBK.setColor(THIN_GRAY);
                painter->setBrush(newBrushBK);

                pen.setColor(THIN_GRAY);
                painter->setPen(pen);
            }
        }
        else
        {
            newBrushBK.setColor(BROWN);
            painter->setBrush(newBrushBK);

            pen.setColor(BROWN);
            painter->setPen(pen);
        }
    }
    else if(RangeVisible)
    {
        newBrushBK.setColor(THIN_BLUE_DOWN);
        painter->setBrush(newBrushBK);

        pen.setColor(THIN_BLUE_DOWN);
        painter->setPen(pen);
    }
    else
    {
        switch (m_ButtonIsDownState)
        {
        case 0:
            {
                newBrushBK.setColor(color);
                painter->setBrush(newBrushBK);

                pen.setColor(color);
                painter->setPen(pen);
                break;
            }
        case 1:
            {
                if(nElapsed % XHDBTN_FLASTIME == 0) //控制时间间隔
                {
                    newBrushBK.setColor(color);
                    painter->setBrush(newBrushBK);

                    pen.setColor(color);
                    painter->setPen(pen);
                }
                else
                {
                    newBrushBK.setColor(BLUE);
                    painter->setBrush(newBrushBK);

                    pen.setColor(BLUE);
                    painter->setPen(pen);
                }
                break;
            }
        case 2:
            {
                if(m_strType == "SDJS")
                {
                    newBrushBK.setColor(color);
                    painter->setBrush(newBrushBK);

                    pen.setColor(color);
                    painter->setPen(pen);
                }
                else
                {
                    newBrushBK.setColor(RED);
                    painter->setBrush(newBrushBK);

                    pen.setColor(RED);
                    painter->setPen(pen);
                }
                break;
            }
        case 4://可按下，终端闪烁
            {
                if(m_bZDANFlash)
                {
                    if(nElapsed % XHDBTN_FLASTIME == 0) //控制时间间隔
                    {
                        newBrushBK.setColor(YELLOW);
                        painter->setBrush(newBrushBK);

                        pen.setColor(YELLOW);
                        painter->setPen(pen);
                    }
                    else
                    {
                        newBrushBK.setColor(GREY);
                        painter->setBrush(newBrushBK);

                        pen.setColor(GREY);
                        painter->setPen(pen);
                    }
                }
                else
                {
                    newBrushBK.setColor(GREY);
                    painter->setBrush(newBrushBK);

                    pen.setColor(GREY);
                    painter->setPen(pen);
                }
                break;
            }
        }
    }
//     painter->drawRect(m_ButtonRect);
    if(haveAN)
    {
         painter->drawRect(m_ButtonRect);
    }


    //封锁X
    if(m_FSState)
    {
        QPen penX;
        penX.setColor(RED);
        penX.setWidth(2);
        painter->setPen(penX);

        QRectF m_rectDC_X;       //调车按钮区域
        m_rectDC_X.setLeft(m_ButtonRect.left());
        m_rectDC_X.setTop(m_ButtonRect.top());
        m_rectDC_X.setRight(m_ButtonRect.right());
        m_rectDC_X.setBottom(m_ButtonRect.bottom());

        painter->drawLine(m_rectDC_X.left(), m_rectDC_X.top(),m_rectDC_X.right(), m_rectDC_X.bottom());
        painter->drawLine(m_rectDC_X.right(), m_rectDC_X.top(),m_rectDC_X.left(), m_rectDC_X.bottom());
    }
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, false);
    //倒计时
    if((0<mTimeCount)&&(255>mTimeCount))
    {
        QString strTimeCount;
        strTimeCount=QString::number(mTimeCount);

        QPen penX;
        penX.setColor(RED);
        penX.setWidth(1);
        painter->setPen(penX);
        QRectF m_rectDC_X;
        m_rectDC_X.setLeft(m_ButtonRect.left()-20 * nDiploid);
        m_rectDC_X.setTop(m_ButtonRect.top()-20 * nDiploid);
        m_rectDC_X.setRight(m_ButtonRect.right()+20 * nDiploid);
        m_rectDC_X.setBottom(m_ButtonRect.bottom()-20 * nDiploid);

        painter->drawText(m_rectDC_X,Qt::AlignHCenter|Qt::AlignVCenter,strTimeCount);
    }
    //绘制按钮的文字
    if(rectTextShow != ""&&bBtnNameShow)
    {
        if(HaveQF)
        {
            pen.setColor(YELLOW);
        }
        else
        {
            pen.setColor(WHITE);
        }
        QFont font;
        font.setWeight(50);//字体宽度,0~99
        //font.setPointSize(12 * nDiploid);//字体高度,10表示高度为10个像素点
        font.setPointSize(wordsize * nDiploid);//字体高度,10表示高度为10个像素点
        painter->setPen(pen);
        painter->setFont(font);
        QRectF rectBtnWord_X;
        rectBtnWord_X.setLeft(m_ButtonRect.left()-30 * nDiploid);
        rectBtnWord_X.setTop(m_ButtonRect.top()+15 * nDiploid);
        rectBtnWord_X.setRight(m_ButtonRect.right()+30 * nDiploid);
        rectBtnWord_X.setBottom(m_ButtonRect.bottom()+25 * nDiploid);
//        painter->drawText(rectBtnTG_X,Qt::AlignHCenter|Qt::AlignVCenter,m_strName + "TA");
        painter->drawText(rectBtnWord_X,Qt::AlignHCenter|Qt::AlignVCenter,rectTextShow);
    }
    if(RangeVisible)
    {
        QRectF textRect;
        textRect.setLeft(m_ButtonRect.left()-4 * nDiploid);
        textRect.setTop(m_ButtonRect.top()-4 * nDiploid);
        textRect.setRight(m_ButtonRect.right()+4);
        textRect.setBottom(m_ButtonRect.bottom()+4 * nDiploid);
        painter->setPen(QPen(Qt::white, 1,Qt::DashLine));
        painter->setBrush(THIN_BLUE_DOWN);
        painter->setBrush(Qt::transparent);
        painter->drawRect(textRect);
        painter->setPen(BLACK);
    }

#pragma endregion
}

QColor FUNCBUTTON::getLightState(int nType,int nElapsed)//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝）
{
    QColor color;
    color = BLACK;
    switch(nType)
    {
    case 0:
        {
            color = BLACK;
            return color;
            break;
        }
    case 1:
        {
            color = RED;
            return color;
            break;
        }
    case 2:
        {
            color = GREEN;
            return color;
            break;
        }
    case 3:
        {
            color = WHITE;
            return color;
            break;
        }
    case 4:
        {
            color = YELLOW;
            return color;
            break;
        }
    case 5:
        {
            if(nElapsed%2 == 0)
                color = RED;
            else
                color = BLACK;
            return color;
            break;
        }
    case 6:
        {
            if(nElapsed%2 == 0)
                color = GREEN;
            else
                color = BLACK;
            return color;
            break;
        }
    case 7:
        {
            if(nElapsed%2 == 0)
                color = WHITE;
            else
                color = BLACK;
            return color;
            break;
        }
    case 8:
        {
            if(nElapsed%2 == 0)
                color = YELLOW;
            else
                color = BLACK;
            return color;
            break;
        }
    case 9:
        {
            color = BLUE;
            return color;
            break;
        }
    case 10:
        {
            color = THIN_GRAY;
            return color;
            break;
        }

    }
    return color;
}
int FUNCBUTTON::GetMousePoint(QPoint p)
{
    if ((rect_Button.contains(p)))
    {
        //语音暂停
        if(m_strType == "SPEECHPAUSE")
        {
            return 1;
        }
        //上电解锁
        else if(m_strType == "SDJS")
        {
            return 2;
        }
        //引导总锁
        else if(m_strType == "CALLONLOCK")
        {
            if(Button_Name.left(1)=="S")
            {
                return 3;
            }
            else if(Button_Name.left(1)=="X")
            {
                return 4;
            }
            else if(Button_Name.left(1)=="全")
            {
               return 5;
            }
            return 0;
        }
        //故障通知
        else if(m_strType == "GZTZ")
        {
            return 6;
        }
        //清除
        else if(m_strType == "CLEAR")
        {
            return 7;
        }
        //允许改方
        else if(m_strType == "CHANGEDIREC")
        {
            return 8;
        }
        else
        {
          return 0;
        }
    }
    else
    {
        return 0;
    }
}
void FUNCBUTTON::SetANIsDown()
{
    if(Button_Name.left(1)=="S")
    {
        setIsDown_Button(1);
    }
    else if(Button_Name.left(1)=="X")
    {
        setIsDown_Button(1);
    }
    else if(Button_Name.left(1)=="全")
    {
        setIsDown_Button(1);
    }
}
