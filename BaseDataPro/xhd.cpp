#include <QtDebug>
#include "xhd.h"
#include "qnamespace.h"
#pragma execution_character_set("utf-8")
CXHD::CXHD()
{
    m_nXHDType=JZ_XHJ;    //信号机类型
    isSignalType = false;   //是否是虚拟信号
    m_nSafeLamp=XHD_HD;   //安全灯光
    isHigh = false;       //是否为高柱
    m_nXHDState = m_nSafeLamp;
    isLCBt_Down = false;
    isDCBt_Down = false;
    isYDBt_Down = false;

    m_nDcIsDown=false;
    m_nLcIsDown=false;
    m_nTgIsDown=false;
    m_nYdIsDown=false;

    nXHANSize = 15;
    nXHANSizeConst = nXHANSize;
    m_rectLC.setRect(0, 0, 0, 0);       //列车按钮区域
    m_rectDC.setRect(0, 0, 0, 0);       //调车按钮区域
    m_rectYD.setRect(0, 0, 0, 0);       //引导按钮区域
    m_rectLCConst.setRect(0, 0, 0, 0);
    m_rectDCConst.setRect(0, 0, 0, 0);
    m_rectYDConst.setRect(0, 0, 0, 0);
    isYDSD=false;
    signalDCAN=false;
    xhd_Rect.setRect(0,0,0,0);
    isMD=false;
    isLCANFB=false;
    isDCANFB=false;
    isYDANFB=false;
    m_nTimeType=0x11;
    m_nTimeCount=0;

    m_nFuncLockState = 0;
    p1 = QPoint(0,0);
    p2 = QPoint(0,0);
    p12 = QPoint(0,0);
    p34 = QPoint(0,0);
    p56 = QPoint(0,0);
    center = QPoint(0,0);
    m_nRadius = 7;//XHD_RADIUS;
    m_nHeight = 10;

    times=0;
    m_bHaveTGbtn = false;
    m_bCanDC     = false;
    m_bCanYD     = false;
    m_bYDActive  = true;
    m_bFirstCount = true;
    m_bCanPDJS   = false;
    m_RangeVisible = false;
    m_RangeVisible_XHD = false;
    m_RangeVisible_DCAN = false;
    m_RangeVisible_LCAN = false;
    m_RangeVisible_TGAN = false;
    m_RangeVisible_Name = false;
    m_RangeVisible_XHD_DDMD = false;
    m_RangeVisible_XHD_YDAN = false;

    rectBtnLC.setRect(0, 0, 0, 0);
    rectBtnDC.setRect(0, 0, 0, 0);
    rectBtnTG.setRect(0, 0, 0, 0);
    rectBtnYD.setRect(0, 0, 0, 0);

    rectTxtLC.setRect(0, 0, 0, 0);
    rectTxtTG.setRect(0, 0, 0, 0);

    XHD_ds_HD=false;
    XHD_ds_LD=false;
    XHD_ds_YD=false;
    XHD_ds_BD=false;
    XHD_ds_AD=false;
    XHD_ds_YBD=false;
    isLCANFlash=false;
    isDCANFlash=false;
    isBTANFlash=false;

    m_DCXHDButtonShowFlag = false;
    m_DCXHDButtonShowPt = QPoint(0,0);
    m_LCXHDButtonShowFlag = false;
    m_LCXHDButtonShowPt = QPoint(0,0);
    m_YDXHDButtonShowFlag = false;
    m_YDXHDButtonShowPt = QPoint(0,0);
    m_TGXHDButtonShowFlag = false;
    m_TGXHDButtonShowPt = QPoint(0,0);
    m_XHDShowFlag = true;
    m_XHDShowPt = QPoint(0,0);

//    m_DCXHDButtonShowFlag_BTN = false;//调车按钮是否显示-界面按钮功能
//    m_LCXHDButtonShowFlag_BTN = false;//列车按钮是否显示-界面按钮功能
//    m_YDXHDButtonShowFlag_BTN = false;//引导按钮是否显示-界面按钮功能
//    m_TGXHDButtonShowFlag_BTN = false;//通过信号灯按钮显示标志-界面按钮功能

    TxtDCButton="";
    TxtLCButton="";
    TxtYDButton="";
    TxtTGButton="";
    TxtBTButton="";
    bXHJTextShow=true;
    bZDFlash=true;
    nTxtTGButtonCode=-1;

    wordcolor = 3;//文字颜色//默认白色
    wordsize = 12;//文字大小

    setDevStateToSafe();
}

//成员功能函数实现
void CXHD::XHDInit(int type)
{
    pCenterConst.setX(pCenter.x());
    pCenterConst.setY(pCenter.y());
    m_textRectConst.setLeft(m_textRect.left());
    m_textRectConst.setTop(m_textRect.top());
    m_textRectConst.setRight(m_textRect.right());
    m_textRectConst.setBottom(m_textRect.bottom());

    m_nXHDState = XHD_DS;
    if(type == 0x55)
    {
        XHDInit_Th_kb();
    }

    center.setX(pCenter.x());
    center.setY(pCenter.y());
    if(m_nType==31||m_nType==32){
        center.setX(center.x() - m_nHeight);
    }
    else
    {
        center.setX(center.x() + m_nHeight);
    }

    if(m_nType==31||m_nType==32){
        if(isHigh)
        {
            p1 = QPoint(center.x(),center.y()-m_nRadius);
            p2 = QPoint(center.x(),center.y()+m_nRadius);
            p12 = QPoint(center.x(),center.y());
            p34 = QPoint(center.x()+m_nRadius+2,center.y());
            p56 = QPoint(center.x()+m_nRadius*2+2,center.y());
        }
        else
        {
            p1 = QPoint(center.x(),center.y()-m_nRadius);
            p2 = QPoint(center.x(),center.y()+m_nRadius);
            p12 = QPoint(center.x(),center.y());
            p34 = QPoint(center.x()+2,center.y());
            p56 = QPoint(center.x()+2+m_nRadius,center.y());
        }
    }
    else{
        if(isHigh)
        {
            p1 = QPoint(center.x(),center.y()-m_nRadius);
            p2 = QPoint(center.x(),center.y()+m_nRadius);
            p12 = QPoint(center.x(),center.y());
            p34 = QPoint(center.x()-m_nRadius,center.y());
            p56 = QPoint(center.x()-m_nRadius*2,center.y());
        }
        else
        {
            p1 = QPoint(center.x(),center.y()-m_nRadius);
            p2 = QPoint(center.x(),center.y()+m_nRadius);
            p12 = QPoint(center.x(),center.y());
            p34 = QPoint(center.x(),center.y());
            p56 = QPoint(center.x()-m_nRadius,center.y());
        }
    }
}
void CXHD::XHDInit_Th_kb()
{
    nXHANSize = 10;
    nXHANSizeConst = nXHANSize;
    if(getSignalType() == true)
    {

        //m_TGXHDButtonShowFlag = false;
        m_YDXHDButtonShowFlag = getIsYDSD();
//       m_YDXHDButtonShowFlag_BTN = getIsYDSD();
        if(m_nXHDType == DC_XHJ)
        {
            m_DCXHDButtonShowFlag = true;
//            m_DCXHDButtonShowFlag_BTN = true;
        }
        else
        {
            m_DCXHDButtonShowFlag = false;
//            m_DCXHDButtonShowFlag_BTN = false;
        }
        if((m_nXHDType == JZ_XHJ)||(m_nXHDType == CZ_XHJ))
        {
            m_LCXHDButtonShowFlag = true;
//            m_LCXHDButtonShowFlag_BTN = true;
        }
        else
        {
            m_LCXHDButtonShowFlag = false;
//            m_LCXHDButtonShowFlag_BTN = false;
        }
//        m_DCXHDButtonShowFlag = true;
//        m_LCXHDButtonShowFlag = true;

    }
    else if((m_nXHDType == DC_XHJ) || m_bCanDC)
    {
        m_DCXHDButtonShowFlag = true;
        m_YDXHDButtonShowFlag = getIsYDSD();
//        m_DCXHDButtonShowFlag_BTN = true;
//        m_YDXHDButtonShowFlag_BTN = getIsYDSD();
    }
    else if(m_nXHDType == JZ_XHJ)
    {
        m_LCXHDButtonShowFlag = true;
        //m_TGXHDButtonShowFlag = true;
        m_YDXHDButtonShowFlag = getIsYDSD();
//        m_LCXHDButtonShowFlag_BTN = true;
//        //m_TGXHDButtonShowFlag = true;
//        m_YDXHDButtonShowFlag_BTN = getIsYDSD();
    }
    else if(m_nXHDType == CZ_XHJ)
    {
        m_LCXHDButtonShowFlag = true;
        m_DCXHDButtonShowFlag = true;
        m_YDXHDButtonShowFlag = getIsYDSD();
//        m_LCXHDButtonShowFlag_BTN = true;
//        m_DCXHDButtonShowFlag_BTN = true;
//        m_YDXHDButtonShowFlag_BTN = getIsYDSD();
    }
    else
    {
        m_LCXHDButtonShowFlag = false;
        //m_TGXHDButtonShowFlag = false;
        m_YDXHDButtonShowFlag = getIsYDSD();
        m_DCXHDButtonShowFlag = false;
//        m_LCXHDButtonShowFlag_BTN = false;
//        //m_TGXHDButtonShowFlag = false;
//        m_YDXHDButtonShowFlag_BTN = getIsYDSD();
//        m_DCXHDButtonShowFlag_BTN = false;
    }


    if(getType() == 31)
    {
        int interval=10;
        if(m_DCXHDButtonShowFlag/* && m_DCXHDButtonShowFlag_BTN*/)
        {
            interval+=16;
            nXHANSize = 14;
            m_rectDC.setRect(pCenter.x()-interval,pCenter.y()-7,nXHANSize,nXHANSize);
        }
        if(m_LCXHDButtonShowFlag /*&& m_LCXHDButtonShowFlag_BTN*/)
        {
            interval+=16;
            nXHANSize = 10;
            m_rectLC.setRect(pCenter.x()-interval,pCenter.y()-5,nXHANSize,nXHANSize);
            interval+=2;
        }
        if(m_YDXHDButtonShowFlag /*&& m_YDXHDButtonShowFlag_BTN*/)
        {
            interval+=16;
            nXHANSize = 10;
            m_rectYD.setRect(pCenter.x()-interval,pCenter.y()-5,nXHANSize,nXHANSize);
            interval+=2;
        }
        if(m_TGXHDButtonShowFlag /*&& m_TGXHDButtonShowFlag_BTN*/)
        {
            interval+=16;
            nXHANSize = 10;
            rectBtnTG.setRect(pCenter.x()-interval,pCenter.y()-5,nXHANSize,nXHANSize);
            interval+=2;
        }
    }
    else if(getType() == 33)
    {
        int interval=10;
        if(m_DCXHDButtonShowFlag /*&& m_DCXHDButtonShowFlag_BTN*/)
        {
            interval+=3;
            nXHANSize = 14;
            m_rectDC.setRect(pCenter.x()+interval,pCenter.y()-6,nXHANSize,nXHANSize);
            interval+=14;
        }
        if(m_LCXHDButtonShowFlag /*&& m_LCXHDButtonShowFlag_BTN*/)
        {
            interval+=6;
            nXHANSize = 10;
            m_rectLC.setRect(pCenter.x()+interval,pCenter.y()-5,nXHANSize,nXHANSize);
            interval+=12;
        }
        if(m_YDXHDButtonShowFlag /*&& m_YDXHDButtonShowFlag_BTN*/)
        {
            interval+=6;
            nXHANSize = 10;
            m_rectYD.setRect(pCenter.x()+interval,pCenter.y()-5,nXHANSize,nXHANSize);
            interval+=12;
        }
        if(m_TGXHDButtonShowFlag /*&& m_TGXHDButtonShowFlag_BTN*/)
        {
            interval+=6;
            nXHANSize = 10;
            rectBtnTG.setRect(pCenter.x()+interval,pCenter.y()-5,nXHANSize,nXHANSize);
            interval+=12;
        }
    }

    if(getType() == 32)
    {
        int interval=10;
        if(m_DCXHDButtonShowFlag /*&& m_DCXHDButtonShowFlag_BTN*/)
        {
            interval+=16;
            nXHANSize = 14;
            m_rectDC.setRect(pCenter.x()-interval,pCenter.y()-7,nXHANSize,nXHANSize);
        }
        if(m_LCXHDButtonShowFlag /*&& m_LCXHDButtonShowFlag_BTN*/)
        {
            interval+=16;
            nXHANSize = 10;
            m_rectLC.setRect(pCenter.x()-interval,pCenter.y()-5,nXHANSize,nXHANSize);
            interval+=2;
        }
        if(m_YDXHDButtonShowFlag /*&& m_YDXHDButtonShowFlag_BTN*/)
        {
            interval+=16;
            nXHANSize = 10;
            m_rectYD.setRect(pCenter.x()-interval,pCenter.y()-5,nXHANSize,nXHANSize);
            interval+=2;
        }
        if(m_TGXHDButtonShowFlag /*&& m_TGXHDButtonShowFlag_BTN*/)
        {
            interval+=16;
            nXHANSize = 10;
            rectBtnTG.setRect(pCenter.x()-interval,pCenter.y()-5,nXHANSize,nXHANSize);
            interval+=2;
        }
    }
    else if(getType() == 34)
    {
        int interval=10;
        if(m_DCXHDButtonShowFlag /*&& m_DCXHDButtonShowFlag_BTN*/)
        {
            interval+=3;
            nXHANSize = 14;
            m_rectDC.setRect(pCenter.x()+interval,pCenter.y()-7,nXHANSize,nXHANSize);
            interval+=14;
        }
        if(m_LCXHDButtonShowFlag /*&& m_LCXHDButtonShowFlag_BTN*/)
        {
            interval+=6;
            nXHANSize = 10;
            m_rectLC.setRect(pCenter.x()+interval,pCenter.y()-5,nXHANSize,nXHANSize);
            interval+=12;
        }
        if(m_YDXHDButtonShowFlag /*&& m_YDXHDButtonShowFlag_BTN*/)
        {
            interval+=6;
            nXHANSize = 10;
            m_rectYD.setRect(pCenter.x()+interval,pCenter.y()-5,nXHANSize,nXHANSize);
            interval+=12;
        }
        if(m_TGXHDButtonShowFlag /*&& m_TGXHDButtonShowFlag_BTN*/)
        {
            interval+=6;
            nXHANSize = 10;
            rectBtnTG.setRect(pCenter.x()+interval,pCenter.y()-5,nXHANSize,nXHANSize);
            interval+=12;
        }
    }

    m_rectLCConst=m_rectLC;
    m_rectDCConst=m_rectDC;
    m_rectYDConst=m_rectYD;
    m_rectTGConst=rectBtnTG;
}

void CXHD::Draw(QPainter *painter, long nElapsed, double nDiploid,QPoint offset,int type)
{
    if(type==0x55)
    {
        Draw_Th_kb(painter,nElapsed, nDiploid,offset, true/*false*/);
    }
}

void CXHD::DrawM(QPainter *painter,long nElapsed, double nDiploid,QPoint offset,int type, int flag)
{
    if(type==0x55)
    {
        Draw_Th_kb(painter,nElapsed, nDiploid,offset, true);
    }
}

void CXHD::Draw_Th_kb(QPainter *painter,long nElapsed, double nDiploid,QPoint offset, bool isMulti)
{
    QPointF p_Line1;  //信号机竖线绘制坐标
    QPointF p_Line2;  //信号机竖线绘制坐标
    QPointF p_Line3;  //信号机高柱横线绘制坐标
    QPointF p_Line4;  //信号机高柱横线绘制坐标
    QPointF p_Xhd;    //信号机靠近信号柱灯位坐标
    QPointF p_Xhd2;   //信号机远离信号柱灯位坐标
    QColor xhd1_Color = Qt::red;  //信号机近端信号灯位显示颜色
    if(m_nXHDType == DC_XHJ)
        xhd1_Color = Qt::blue;
    QColor xhd2_Color = Qt::black;  //信号机远端信号灯位显示颜色
    QFont font;

    //坐标变换 2021.1.11 BJT
    pCenter.setX(pCenterConst.x() * nDiploid+offset.x()*nDiploid);
    pCenter.setY(pCenterConst.y() * nDiploid+offset.y()*nDiploid);
    m_textRect.setLeft(m_textRectConst.left()* nDiploid+offset.x()*nDiploid);
    m_textRect.setTop(m_textRectConst.top()* nDiploid+offset.y()*nDiploid);
    m_textRect.setRight(m_textRectConst.right()* nDiploid+offset.x()*nDiploid);
    m_textRect.setBottom(m_textRectConst.bottom()* nDiploid+offset.y()*nDiploid);
    m_rectLC.setLeft(m_rectLCConst.left()*nDiploid+offset.x()*nDiploid);
    m_rectLC.setTop(m_rectLCConst.top()*nDiploid+offset.y()*nDiploid);
    m_rectLC.setRight(m_rectLCConst.right()*nDiploid+offset.x()*nDiploid);
    m_rectLC.setBottom(m_rectLCConst.bottom()*nDiploid+offset.y()*nDiploid);
    m_rectDC.setLeft(m_rectDCConst.left()*nDiploid+offset.x()*nDiploid);
    m_rectDC.setTop(m_rectDCConst.top()*nDiploid+offset.y()*nDiploid);
    m_rectDC.setRight(m_rectDCConst.right()*nDiploid+offset.x()*nDiploid);
    m_rectDC.setBottom(m_rectDCConst.bottom()*nDiploid+offset.y()*nDiploid);
    m_rectYD.setLeft(m_rectYDConst.left()*nDiploid+offset.x()*nDiploid);
    m_rectYD.setTop(m_rectYDConst.top()*nDiploid+offset.y()*nDiploid);
    m_rectYD.setRight(m_rectYDConst.right()*nDiploid+offset.x()*nDiploid);
    m_rectYD.setBottom(m_rectYDConst.bottom()*nDiploid+offset.y()*nDiploid);

    rectBtnTG.setLeft(m_rectTGConst.left()*nDiploid+offset.x()*nDiploid);
    rectBtnTG.setTop(m_rectTGConst.top()*nDiploid+offset.y()*nDiploid);
    rectBtnTG.setRight(m_rectTGConst.right()*nDiploid+offset.x()*nDiploid);
    rectBtnTG.setBottom(m_rectTGConst.bottom()*nDiploid+offset.y()*nDiploid);

    nXHANSize = nXHANSizeConst * nDiploid;

    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, false);
    if(getXHDType() == DC_XHJ && WndType == 1)//绘制信号牌 2021.7.29 BJT
    {
        return;
    }
    if(getXHDType() == XHP_XHJ)//绘制信号牌 2021.7.29 BJT
    {
        painter->setPen(QPen(Qt::white, 2));
        painter->setBrush(Qt::black);
        if ((31 == getType()) || (32 == getType()))
        {
            p_Line1.setX(pCenter.x() - 10 * nDiploid);
            p_Line1.setY(pCenter.y() - 16 * nDiploid);
            p_Line2.setX(pCenter.x() - 10 * nDiploid);
            p_Line2.setY(pCenter.y());
            painter->drawLine(p_Line1, p_Line2); //绘制信号牌竖线

            p_Xhd.setX(pCenter.x() - 10 * nDiploid);
            p_Xhd.setY(pCenter.y() - 8* nDiploid);
            p_Xhd2.setX(p_Xhd.x() + 20 * nDiploid);
            p_Xhd2.setY(p_Xhd.y());
            painter->drawRect(p_Xhd2.x()-12 * nDiploid,p_Xhd2.y(), 12 * nDiploid, 12 * nDiploid);
            QPointF points[3] = {QPointF(p_Xhd2.x()-11 * nDiploid, p_Xhd2.y()+2* nDiploid), QPointF(p_Xhd2.x()-1* nDiploid, p_Xhd2.y()+2* nDiploid), QPointF(p_Xhd2.x()-6* nDiploid, p_Xhd2.y()+11* nDiploid)};
            painter->setPen(QPen(Qt::black, 1));
            painter->setBrush(Qt::yellow);
            painter->drawPolygon(points, 3);
            painter->setPen(QPen(Qt::white, 1));
            painter->drawLine(p_Xhd, p_Xhd2);
        }
        else
        {
            p_Xhd.setX(pCenter.x() + 10 * nDiploid);
            p_Xhd.setY(pCenter.y() + 8* nDiploid);
            p_Xhd2.setX(p_Xhd.x() -20 * nDiploid);
            p_Xhd2.setY(p_Xhd.y());
            painter->setPen(QPen(Qt::white, 1));
            painter->setBrush(Qt::black);
            painter->drawRect(p_Xhd2.x(),p_Xhd2.y()-12* nDiploid, 12 * nDiploid, 12 * nDiploid);
            QPointF points[3] = {QPointF(p_Xhd2.x()+1 * nDiploid, p_Xhd2.y()-2* nDiploid), QPointF(p_Xhd2.x()+11* nDiploid, p_Xhd2.y()-2* nDiploid), QPointF(p_Xhd2.x()+6* nDiploid, p_Xhd2.y()-11* nDiploid)};
            painter->setPen(QPen(Qt::black, 1));
            painter->setBrush(Qt::yellow);
            painter->drawPolygon(points, 3);
            painter->setPen(QPen(Qt::white, 1));
            painter->drawLine(p_Xhd, p_Xhd2);

            p_Line1.setX(pCenter.x() + 10 * nDiploid);
            p_Line1.setY(pCenter.y());
            p_Line2.setX(pCenter.x() + 10 * nDiploid);
            p_Line2.setY(pCenter.y() + 16 * nDiploid);
            painter->drawLine(p_Line1, p_Line2); //绘制信号牌竖线
        }
        xhd_Rect.setRect(-100,-100,-80,-80);
    }
    else
    {
        if (false == getSignalType())   //虚拟按钮不绘制信号灯位
        {
            //绘制信号机柱 2021.1.12 BJT
            if ((31 == getType()) || (32 == getType()))
            {
                p_Line1.setX(pCenter.x() - 10 * nDiploid);
                p_Line1.setY(pCenter.y() - 8 * nDiploid);
                p_Line2.setX(pCenter.x() - 10 * nDiploid);
                p_Line2.setY(pCenter.y() + 8 * nDiploid);
            }
            else
            {
                p_Line1.setX(pCenter.x() + 10 * nDiploid);
                p_Line1.setY(pCenter.y() - 8 * nDiploid);
                p_Line2.setX(pCenter.x() + 10 * nDiploid);
                p_Line2.setY(pCenter.y() + 8 * nDiploid);
            }
            painter->setPen(QPen(SkyBlue, 2));
            //if(!bFCZKState)
            {
                if(m_RangeVisible)
                {
                    painter->setPen(QPen(THIN_BLUE_DOWN, 2));
                }
                if(m_RangeVisible_XHD)
                {
                    painter->setPen(QPen(THIN_BLUE_DOWN, 2));
                }
            }
            painter->drawLine(p_Line1, p_Line2); //绘制信号机柱竖线

            if (true == getIsHigh())
            {
                p_Line3.setX(p_Line1.x());
                p_Line3.setY((p_Line1.y() + p_Line2.y()) / 2);
                if ((31 == getType()) || (32 == getType()))
                {
                    p_Line4.setX(p_Line1.x() + 6 * nDiploid);
                    p_Line4.setY(p_Line3.y());
                }
                else
                {
                    p_Line4.setX(p_Line1.x() - 6 * nDiploid);
                    p_Line4.setY(p_Line3.y());
                }
                painter->setPen(QPen(SkyBlue, 2));
                //if(!bFCZKState)
                {
                    if(m_RangeVisible)
                    {
                        painter->setPen(QPen(THIN_BLUE_DOWN, 2));
                    }
                    if(m_RangeVisible_XHD)
                    {
                        painter->setPen(QPen(THIN_BLUE_DOWN, 2));
                    }
                }
                painter->drawLine(p_Line3, p_Line4); //绘制信号机柱高柱短横线
            }
            //反走样,防止出现锯齿状线条
            painter->setRenderHint(QPainter::Antialiasing, true);
            //绘制信号机灯位 2021.7.29 BJT
            if ((31 == getType()) || (32 == getType()))
            {
                if (true == getIsHigh())
                {
                    p_Xhd.setX(pCenter.x() + 4 * nDiploid);
                    p_Xhd.setY(pCenter.y());
                }
                else
                {
                    p_Xhd.setX(pCenter.x() - 1 * nDiploid);
                    p_Xhd.setY(pCenter.y());
                }
                p_Xhd2.setX(p_Xhd.x() + 16 * nDiploid);
                p_Xhd2.setY(p_Xhd.y());
            }
            else
            {
                if (true == getIsHigh())
                {
                    p_Xhd.setX(pCenter.x() - 4 * nDiploid);
                    p_Xhd.setY(pCenter.y());
                }
                else
                {
                    p_Xhd.setX(pCenter.x() + 1 * nDiploid);
                    p_Xhd.setY(pCenter.y());
                }
                p_Xhd2.setX(p_Xhd.x() - 16 * nDiploid);
                p_Xhd2.setY(p_Xhd.y());
            }
            setXHD_Color(&xhd1_Color, &xhd2_Color, nElapsed);   //根据信号机状态实时设置信号机灯位颜色
            //if(isPLCZ)
            //{
            //    painter->setPen(QPen(Qt::cyan,2));
            //}
            //else
            {
                painter->setPen(QPen(SkyBlue, 2));
            }
            painter->setBrush(xhd1_Color);
            painter->drawEllipse(p_Xhd, 8 * nDiploid, 8 * nDiploid);
            m_devRect_lamp1 = QRect(p_Xhd.x()-8 * nDiploid,p_Xhd.y()-8 * nDiploid, 16 * nDiploid, 16 * nDiploid);

            //if(getIsMD() == true)
            if((getIsMD() == true)&&(getXHDType()!=DC_XHJ))
            {
                //painter->drawLine(QPoint(p_Xhd.x()-5 * nDiploid,p_Xhd.y()-5 * nDiploid), QPoint(p_Xhd.x()+5 * nDiploid,p_Xhd.y()+5 * nDiploid)); //绘制灭灯状态
                //painter->drawLine(QPoint(p_Xhd.x()+5 * nDiploid,p_Xhd.y()-5 * nDiploid), QPoint(p_Xhd.x()-5 * nDiploid,p_Xhd.y()+5 * nDiploid)); //绘制灭灯状态
            }
            {
                if((m_RangeVisible_XHD)||(m_RangeVisible_Name))
                {
                    //反走样,防止出现锯齿状线条
                    painter->setRenderHint(QPainter::Antialiasing, false);
                    QRectF p_XhdRect = QRect(p_Xhd.x()-8 * nDiploid,p_Xhd.y()-8 * nDiploid, 16 * nDiploid, 16 * nDiploid);
                    painter->setPen(QPen(Qt::white, 1,Qt::DashLine));
                    painter->setBrush(Qt::transparent);
                    painter->drawRect(p_XhdRect);
                    //painter->setPen(QPen(THIN_BLUE_DOWN, 2,Qt::DashLine));
                    //反走样,防止出现锯齿状线条
                    painter->setRenderHint(QPainter::Antialiasing, true);
                }
            }
            if(!bFCZKState)
            {
                if(m_RangeVisible)
                {
                    //反走样,防止出现锯齿状线条
                    painter->setRenderHint(QPainter::Antialiasing, false);
                    QRectF p_XhdRect = QRect(p_Xhd.x()-8 * nDiploid,p_Xhd.y()-8 * nDiploid, 16 * nDiploid, 16 * nDiploid);
                    painter->setPen(QPen(Qt::white, 1,Qt::DashLine));
                    painter->setBrush(Qt::transparent);
                    painter->drawRect(p_XhdRect);
                    //painter->setPen(QPen(THIN_BLUE_DOWN, 2,Qt::DashLine));
                    //反走样,防止出现锯齿状线条
                    painter->setRenderHint(QPainter::Antialiasing, true);
                }
                if((m_RangeVisible_DCAN)||(m_RangeVisible_LCAN)||(m_RangeVisible_TGAN)
                        ||(m_RangeVisible_XHD_DDMD)||(m_RangeVisible_XHD_YDAN))
                {
                    //反走样,防止出现锯齿状线条
                    painter->setRenderHint(QPainter::Antialiasing, false);
                    QRectF p_XhdRect = QRect(p_Xhd.x()-8 * nDiploid,p_Xhd.y()-8 * nDiploid, 16 * nDiploid, 16 * nDiploid);
                    painter->setPen(QPen(Qt::white, 1,Qt::DashLine));
                    painter->setBrush(Qt::transparent);
                    painter->drawRect(p_XhdRect);
                    //painter->setPen(QPen(THIN_BLUE_DOWN, 2,Qt::DashLine));
                    //反走样,防止出现锯齿状线条
                    painter->setRenderHint(QPainter::Antialiasing, true);
                }
            }
            if ((34 == getType()) || (32 == getType()))
            {
                //painter->setPen(QPen(SkyBlue, 2));
                painter->setPen(QPen(SkyBlue, 2));
                painter->setBrush(xhd2_Color);
                painter->drawEllipse(p_Xhd2, 8* nDiploid, 8* nDiploid);
                m_devRect=QRect(p_Xhd2.x()-8 * nDiploid,p_Xhd.y()-8 * nDiploid, 16 * nDiploid, 16 * nDiploid);
                if(getIsMD() == true)
                {
                    //painter->drawLine(QPoint(p_Xhd2.x()-5 * nDiploid,p_Xhd2.y()-5 * nDiploid), QPoint(p_Xhd2.x()+5 * nDiploid,p_Xhd2.y()+5 * nDiploid)); //绘制灭灯状态
                    //painter->drawLine(QPoint(p_Xhd2.x()+5 * nDiploid,p_Xhd2.y()-5 * nDiploid), QPoint(p_Xhd2.x()-5 * nDiploid,p_Xhd2.y()+5 * nDiploid)); //绘制灭灯状态
                }
                {
//                    }
                    if((m_RangeVisible_XHD)||(m_RangeVisible_Name))
                    {
                        //反走样,防止出现锯齿状线条
                        painter->setRenderHint(QPainter::Antialiasing, false);
                        QRectF p_XhdRect = QRect(p_Xhd2.x()-8 * nDiploid,p_Xhd.y()-8 * nDiploid, 16 * nDiploid, 16 * nDiploid);
                        painter->setPen(QPen(Qt::white, 1,Qt::DashLine));
                        painter->setBrush(Qt::transparent);
                        painter->drawRect(p_XhdRect);
                        //painter->setPen(QPen(THIN_BLUE_DOWN, 2,Qt::DashLine));
                        //反走样,防止出现锯齿状线条
                        painter->setRenderHint(QPainter::Antialiasing, true);
                    }
                }
                if(!bFCZKState)
                {
                    if(m_RangeVisible)
                    {
                        //反走样,防止出现锯齿状线条
                        painter->setRenderHint(QPainter::Antialiasing, false);
                        QRectF p_XhdRect = QRect(p_Xhd2.x()-8 * nDiploid,p_Xhd.y()-8 * nDiploid, 16 * nDiploid, 16 * nDiploid);
                        painter->setPen(QPen(Qt::white, 1,Qt::DashLine));
                        painter->setBrush(Qt::transparent);
                        painter->drawRect(p_XhdRect);
                        //painter->setPen(QPen(THIN_BLUE_DOWN, 2,Qt::DashLine));
                        //反走样,防止出现锯齿状线条
                        painter->setRenderHint(QPainter::Antialiasing, true);
                    }
                    if((m_RangeVisible_DCAN)||(m_RangeVisible_LCAN)||(m_RangeVisible_TGAN)
                            ||(m_RangeVisible_XHD_DDMD)||(m_RangeVisible_XHD_YDAN))
                    {
                        //反走样,防止出现锯齿状线条
                        painter->setRenderHint(QPainter::Antialiasing, false);
                        QRectF p_XhdRect = QRect(p_Xhd2.x()-8 * nDiploid,p_Xhd.y()-8 * nDiploid, 16 * nDiploid, 16 * nDiploid);
                        painter->setPen(QPen(Qt::white, 1,Qt::DashLine));
                        painter->setBrush(Qt::transparent);
                        painter->drawRect(p_XhdRect);
                        //painter->setPen(QPen(THIN_BLUE_DOWN, 2,Qt::DashLine));
                        //反走样,防止出现锯齿状线条
                        painter->setRenderHint(QPainter::Antialiasing, true);
                    }
                }
            }
            else if (((XHD_UU == getXHDState())) || (XHD_LU == getXHDState()) || (XHD_LL == getXHDState()) || (XHD_YD == getXHDState()) || (XHD_USU == getXHDState()))
            {
                painter->setPen(QPen(SkyBlue, 2));
                {
                    if((m_RangeVisible_XHD)||(m_RangeVisible_Name))
                    {
                        //反走样,防止出现锯齿状线条
                        painter->setRenderHint(QPainter::Antialiasing, false);
                        QRectF p_XhdRect = QRect(p_Xhd2.x()-8 * nDiploid,p_Xhd.y()-8 * nDiploid, 16 * nDiploid, 16 * nDiploid);
                        painter->setPen(QPen(Qt::white, 1,Qt::DashLine));
                        painter->setBrush(Qt::transparent);
                        painter->drawRect(p_XhdRect);
                        painter->setPen(QPen(THIN_BLUE_DOWN, 2,Qt::DashLine));
                        //反走样,防止出现锯齿状线条
                        painter->setRenderHint(QPainter::Antialiasing, true);
                    }
                }
                if(!bFCZKState)
                {
                    if(m_RangeVisible)
                    {
                        //反走样,防止出现锯齿状线条
                        painter->setRenderHint(QPainter::Antialiasing, false);
                        QRectF p_XhdRect = QRect(p_Xhd2.x()-8 * nDiploid,p_Xhd.y()-8 * nDiploid, 16 * nDiploid, 16 * nDiploid);
                        painter->setPen(QPen(Qt::white, 1,Qt::DashLine));
                        painter->setBrush(Qt::transparent);
                        painter->drawRect(p_XhdRect);
                        painter->setPen(QPen(THIN_BLUE_DOWN, 2,Qt::DashLine));
                        //反走样,防止出现锯齿状线条
                        painter->setRenderHint(QPainter::Antialiasing, true);
                    }
                    if((m_RangeVisible_DCAN)||(m_RangeVisible_LCAN)||(m_RangeVisible_TGAN)
                            ||(m_RangeVisible_XHD_DDMD)||(m_RangeVisible_XHD_YDAN))
                    {
                        //反走样,防止出现锯齿状线条
                        painter->setRenderHint(QPainter::Antialiasing, false);
                        QRectF p_XhdRect = QRect(p_Xhd2.x()-8 * nDiploid,p_Xhd.y()-8 * nDiploid, 16 * nDiploid, 16 * nDiploid);
                        painter->setPen(QPen(Qt::white, 1,Qt::DashLine));
                        painter->setBrush(Qt::transparent);
                        painter->drawRect(p_XhdRect);
                        painter->setPen(QPen(THIN_BLUE_DOWN, 2,Qt::DashLine));
                        //反走样,防止出现锯齿状线条
                        painter->setRenderHint(QPainter::Antialiasing, true);
                    }
                }
                painter->setBrush(xhd2_Color);
                painter->drawEllipse(p_Xhd2, 8* nDiploid, 8* nDiploid);
                if(getIsMD() == true)
                {
                    painter->drawLine(QPoint(p_Xhd2.x()-5 * nDiploid,p_Xhd2.y()-5 * nDiploid), QPoint(p_Xhd2.x()+5 * nDiploid,p_Xhd2.y()+5 * nDiploid)); //绘制灭灯状态
                    painter->drawLine(QPoint(p_Xhd2.x()+5 * nDiploid,p_Xhd2.y()-5 * nDiploid), QPoint(p_Xhd2.x()-5 * nDiploid,p_Xhd2.y()+5 * nDiploid)); //绘制灭灯状态
                }
            }
            //反走样,防止出现锯齿状线条
            painter->setRenderHint(QPainter::Antialiasing, false);
        }
        //绘制信号机按钮
        if(!isMulti)
        {
            Draw_Button(painter,nElapsed, nDiploid, offset);
        }

        //倒计时
        if(m_nTimeCount > 0 && getXHDState(XHD_YD))
        {
            //if(m_rectLC==m_ButtonRect)
            {
                if(m_textRect.top()< pCenter.y())
                {
                    if((0<m_nTimeCount)&&(255>m_nTimeCount))
                    {
                        QString strTimeCount = "";
                        strTimeCount=QString::number(m_nTimeCount);
                        strTimeCount = "Y"+strTimeCount;

                        QPen penX;
                        penX.setColor(WHITE);
                        penX.setWidth(1);
                        painter->setPen(penX);
                        QRectF m_rectDC_X;

                        m_rectDC_X.setLeft(m_textRect.left()-38 * nDiploid);
                        m_rectDC_X.setTop(m_textRect.top()-4 * nDiploid);
                        m_rectDC_X.setRight(m_textRect.right()-2 * nDiploid);
                        m_rectDC_X.setBottom(m_textRect.bottom()-4 * nDiploid);
                        painter->drawText(m_rectDC_X,Qt::AlignHCenter|Qt::AlignVCenter,strTimeCount);
                    }
                }
                else
                {
                    if((0<m_nTimeCount)&&(255>m_nTimeCount))
                    {
                        QString strTimeCount = "";
                        strTimeCount=QString::number(m_nTimeCount);
                        strTimeCount = "Y"+strTimeCount;

                        QPen penX;
                        penX.setColor(WHITE);
                        penX.setWidth(1);
                        painter->setPen(penX);
                        QRectF m_rectDC_X;

                        m_rectDC_X.setLeft(m_textRect.left()+12 * nDiploid);
                        m_rectDC_X.setTop(m_textRect.top()-4 * nDiploid);
                        m_rectDC_X.setRight(m_textRect.right()+36 * nDiploid);
                        m_rectDC_X.setBottom(m_textRect.bottom()-4 * nDiploid);
                        painter->drawText(m_rectDC_X,Qt::AlignHCenter|Qt::AlignVCenter,strTimeCount);
                    }
                }
            }
        }
        else
        {
            //if(m_rectLC==m_ButtonRect)
            {
                if((0<m_nTimeCount)&&(255>m_nTimeCount))
                {
                    QString strTimeCount = "";
                    strTimeCount=QString::number(m_nTimeCount);
                    strTimeCount = "Y"+strTimeCount;

                    QPen penX;
                    penX.setColor(WHITE);
                    penX.setWidth(1);
                    painter->setPen(penX);
                    QRectF m_rectDC_X;
                    m_rectDC_X.setLeft(m_textRect.left()+18 * nDiploid);
                    m_rectDC_X.setTop(m_textRect.top()-4 * nDiploid);
                    m_rectDC_X.setRight(m_textRect.right()+42 * nDiploid);
                    m_rectDC_X.setBottom(m_textRect.bottom()-4 * nDiploid);
                    painter->drawText(m_rectDC_X,Qt::AlignHCenter|Qt::AlignVCenter,strTimeCount);
                }
            }
        }
    }
    //绘制信号机名称
    if ((true == getDisplayName()) && (getTimeType() == 0x11)&&bXHJTextShow)
    {
        font.setFamily("宋体");
        font.setPointSize(wordsize * nDiploid);//字号
        font.setItalic(false);//斜体
        painter->setFont(font);//设置字体
        painter->setPen(getLightState(wordcolor,nElapsed));//设置画笔颜色
        //if(!bFCZKState)
        {
            if(m_RangeVisible)
            {
                QRectF textRect;
                textRect.setLeft(m_textRect.left()-1 * nDiploid);
                textRect.setTop(m_textRect.top()-4 * nDiploid);
                textRect.setRight(m_textRect.right());
                textRect.setBottom(m_textRect.bottom()-4 * nDiploid);
                painter->setPen(QPen(Qt::white, 1,Qt::DashLine));
                painter->setBrush(THIN_BLUE_DOWN);
                painter->drawRect(textRect);
                painter->setPen(BLACK);
            }
            //if(m_RangeVisible_XHD)
            if((m_RangeVisible_XHD)||(m_RangeVisible_Name))
            {
                QRectF textRect;
                textRect.setLeft(m_textRect.left()-1 * nDiploid);
                textRect.setTop(m_textRect.top()-4 * nDiploid);
                textRect.setRight(m_textRect.right());
                textRect.setBottom(m_textRect.bottom()-4 * nDiploid);
                painter->setPen(QPen(Qt::white, 1,Qt::DashLine));
                painter->setBrush(THIN_BLUE_DOWN);
                painter->drawRect(textRect);
                painter->setPen(BLACK);
            }
        }
        //绘制文
        if(!isSignalType)
            painter->drawText(m_textRect.left(), m_textRect.top()+10* nDiploid, getName());
    }

}
void CXHD::Draw_ToolTip(QPainter *painter, double nDiploid)
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
void CXHD::setDevStateToSafe()
{
    m_nXHDState = XHD_DS;
    isLCBt_Down = false;
    isDCBt_Down = false;
    isYDBt_Down = false;
    isMD=false;
    m_nTimeType=0x11;
    m_nTimeCount=0xFF;
}
void CXHD::setVollover(QPoint pt_Base)
{
    double x1=0;
    double x2=0;
    double y1=0;
    double y2=0;
    double y3=0;
    if(getType() == 32)
    {
        setType(34);
    }
    else if(getType() == 34)
    {
        setType(32);
    }
    else if(getType() == 31)
    {
        setType(33);
    }
    else if(getType() == 33)
    {
        setType(31);
    }
    setTextRect(QRectF(pt_Base.x() - (getTextRect().right()-pt_Base.x()),pt_Base.y() - (getTextRect().bottom()-pt_Base.y()),getTextRect().width(),getTextRect().height()));
    setCenterPt(QPointF(pt_Base.x() - (getCenterPt().x()-pt_Base.x()),pt_Base.y() - (getCenterPt().y() - pt_Base.y())));
    setDCAN_Rect(QRectF(pt_Base.x() - (m_rectDC.right()-pt_Base.x()),pt_Base.y() - (m_rectDC.bottom()-pt_Base.y()),m_rectDC.width(),m_rectDC.height()));
    setLCAN_Rect(QRectF(pt_Base.x() - (m_rectLC.right()-pt_Base.x()),pt_Base.y() - (m_rectLC.bottom()-pt_Base.y()),m_rectLC.width(),m_rectLC.height()));
    setYDAN_Rect(QRectF(pt_Base.x() - (m_rectYD.right()-pt_Base.x()),pt_Base.y() - (m_rectYD.bottom()-pt_Base.y()),m_rectYD.width(),m_rectYD.height()));
}
unsigned int CXHD::getDevType()
{
    return Dev_XH;
}
void CXHD::setXHD_Color(QColor *xhd1_Color, QColor *xhd2_Color, int nElapsed)
{
    *xhd1_Color = Qt::black;
    *xhd2_Color = Qt::black;
    if (XHD_HD == getXHDState())
    {
        *xhd1_Color = Qt::red;
        *xhd2_Color = Qt::black;
    }
    else if (XHD_DS == getXHDState())
    {
        if (0 == nElapsed % 2)
        {
            if(XHD_AD == getSafeLamp())
            {
                *xhd1_Color = Qt::blue;
            }
            else
            {
                *xhd1_Color = Qt::red;
            }
        }
        else
        {
            *xhd1_Color = Qt::black;
        }
        *xhd2_Color = Qt::black;
    }
    else if (XHD_AD == getXHDState())
    {
        *xhd1_Color = Qt::blue;
        *xhd2_Color = Qt::black;
    }
    else if (XHD_BD == getXHDState())
    {
        *xhd1_Color = Qt::white;
        *xhd2_Color = Qt::black;
    }
    else if (XHD_LD == getXHDState())
    {
        *xhd1_Color = Qt::black;
        *xhd2_Color = Qt::green;
    }
    else if (XHD_UD == getXHDState())
    {
        *xhd1_Color = Qt::black;
        *xhd2_Color = Qt::yellow;
    }
    else if (XHD_UU == getXHDState())
    {
        *xhd1_Color = Qt::yellow;
        *xhd2_Color = Qt::yellow;
    }
    else if (XHD_LL == getXHDState())
    {
        *xhd1_Color = Qt::green;
        *xhd2_Color = Qt::green;
    }
    else if (XHD_YD == getXHDState())
    {
        *xhd1_Color = Qt::white;
        *xhd2_Color = Qt::red;
    }
    else if (XHD_LU == getXHDState())
    {
        *xhd1_Color = Qt::yellow;
        *xhd2_Color = Qt::green;
    }
    else if (XHD_2U == getXHDState())
    {
        *xhd1_Color = Qt::yellow;
        *xhd2_Color = Qt::black;
    }
    else if (XHD_BS == getXHDState())
    {
        if (0 == nElapsed % 2)
        {
            *xhd1_Color = Qt::white;
        }
        else
        {
            *xhd1_Color = Qt::black;
        }
        *xhd2_Color = Qt::black;
    }
    else if (XHD_US == getXHDState())
    {
        *xhd1_Color = Qt::black;
        if (0 == nElapsed % 2)
        {
            *xhd2_Color = Qt::yellow;
        }
        else
        {
            *xhd2_Color = Qt::black;
        }
    }
    else if (XHD_LS == getXHDState())
    {
        *xhd1_Color = Qt::black;
        if (0 == nElapsed % 2)
        {
            *xhd2_Color = Qt::green;
        }
        else
        {
            *xhd2_Color = Qt::black;
        }
    }
    else if (XHD_USU == getXHDState())
    {
        *xhd1_Color = Qt::yellow;
        if (0 == nElapsed % 2)
        {
            *xhd2_Color = Qt::yellow;
        }
        else
        {
            *xhd2_Color = Qt::black;
        }
    }
    else if (XHD_MD == getXHDState())
    {
        *xhd1_Color = Qt::black;
        *xhd2_Color = Qt::black;
    }
}
int CXHD::moveCursor(QPoint p)
{
    if (m_rectDC.contains(p))
    {
        return 1;
    }
    else if(m_rectLC.contains(p))
    {
        return 2;
    }
    else if(m_rectYD.contains(p))
    {
        return 3;
    }
    return 0;
}
void CXHD::xhd_StatePro()
{
    //    if (XHD_HD == getXHDState())
    //    {
    //        if(true == getDSDS_H())
    //        {
    //            setXHDState(XHD_DS);
    //        }
    //    }
    //    else if (XHD_DS == getXHDState())
    //    {
    //        if((true == getDSDS_H()) && (XHD_HD == getSafeLamp()))
    //        {
    //            setXHDState(XHD_DS);
    //        }
    //        else if((false == getDSDS_H()) && (XHD_HD == getSafeLamp()))
    //        {
    //            setXHDState(XHD_HD);
    //        }
    //        else if((true == getDSDS_A()) && (XHD_AD == getSafeLamp()))
    //        {
    //            setXHDState(XHD_DS);
    //        }
    //        else if((false == getDSDS_A()) && (XHD_AD == getSafeLamp()))
    //        {
    //            setXHDState(XHD_AD);
    //        }
    //    }
    //    else if (XHD_AD == getXHDState())
    //    {
    //        if(true == getDSDS_A())
    //        {
    //            setXHDState(XHD_DS);
    //        }
    //    }
    //    else if (XHD_BD == getXHDState())
    //    {
    //    }
    //    else if (XHD_LD == getXHDState())
    //    {
    //    }
    //    else if (XHD_UD == getXHDState())
    //    {
    //    }
    //    else if (XHD_UU == getXHDState())
    //    {
    //    }
    //    else if (XHD_LL == getXHDState())
    //    {
    //    }
    //    else if (XHD_YD == getXHDState())
    //    {
    //    }
    //    else if (XHD_LU == getXHDState())
    //    {
    //    }
    //    else if (XHD_2U == getXHDState())
    //    {
    //    }
    //    else if (XHD_BS == getXHDState())
    //    {
    //    }
    //    else if (XHD_US == getXHDState())
    //    {
    //    }
    //    else if (XHD_LS == getXHDState())
    //    {
    //    }
    //    else if (XHD_USU == getXHDState())
    //    {
    //    }
    //    else if (XHD_MD == getXHDState())
    //    {
    //    }
}
//成员变量封装函数
void CXHD::setXHDType(QString strType)
{
    unsigned int nType=JZ_XHJ;
    if (strType == "JZ_XHJ")
    {
        nType = JZ_XHJ;//进站
    }
    else if (strType == "DCJL_XHJ")
    {
        nType = DCJL_XHJ;
    }
    else if (strType == "JZFS_XHJ")
    {
        nType = JZFS_XHJ;
    }
    else if (strType == "SXCZ_XHJ")
    {
        nType = SXCZ_XHJ;
    }
    else if (strType == "YG_XHJ")
    {
        nType = YG_XHJ;
    }
    else if (strType == "CZ_XHJ")
    {
        nType = CZ_XHJ;//出站
    }
    else if (strType == "DC_XHJ")
    {
        nType = DC_XHJ;//调车
    }
    else if (strType == "DCFS_XHJ")
    {
        nType = DCFS_XHJ;
    }
    else if (strType == "FCJL_XHJ")
    {
        nType = FCJL_XHJ;
    }
    else if (strType == "JLFS_XHJ")
    {
        nType = JLFS_XHJ;
    }
    else if (strType == "XHP_XHJ")
    {
        nType = XHP_XHJ;
    }
    m_nXHDType = nType;
}
unsigned int CXHD::getXHDType()
{
    return m_nXHDType;
}
void CXHD::setSignalType(bool flag)
{
    isSignalType = flag;
}
bool CXHD::getSignalType()
{
    return isSignalType;
}
void CXHD::setSafeLamp(QString strSafelamp)
{
    unsigned int safelamp=XHD_HD;
    if (strSafelamp == "XHD_HD")
    {
        safelamp = XHD_HD;
    }
    else
    {
        safelamp = XHD_AD;
    }
    m_nSafeLamp = safelamp;
}
unsigned int CXHD::getSafeLamp()
{
    return m_nSafeLamp;
}
void CXHD::setIsHigh(bool ishigh)
{
    isHigh = ishigh;
}
bool CXHD::getIsHigh()
{
    return isHigh;
}
void CXHD::setXHDState(unsigned int state)
{
    m_nXHDState = state;
}
unsigned int CXHD::getXHDState()
{
    return m_nXHDState;
}
//*取得信号机状态   * @param nState 所给信号机状态   * @return 如果信号机状态包含所给状态返回真，否则返回假*/
bool CXHD::getXHDState(int nState)
{
    if ((int)m_nXHDState == nState)
        return true;
    else
        return false;
}
//bool CXHD::getXHDState(int nState)
//{
//    if ((int)m_nState == nState)
//        return true;
//    else
//        return false;
//}
/*void CXHD::setXHD_lsState(unsigned int state)
{
    m_nXHD_lsState = state;
}
unsigned int CXHD::getXHD_lsState()
{
    return m_nXHD_lsState;
}*/
void CXHD::setIsLCDown(int nFlag)
{
    //isLCBt_Down = nFlag;
    m_nLcIsDown = nFlag;
}
int CXHD::getIsLCDown()
{
    //return isLCBt_Down;
   return m_nLcIsDown;
}
void CXHD::setIsDCDown(int nFlag)
{
    //isDCBt_Down = nFlag;
    m_nDcIsDown = nFlag;
}
int CXHD::getIsDCDown()
{
    //return isDCBt_Down;
    return m_nDcIsDown;
}
void CXHD::setIsYDDown(int nFlag)
{
    //isYDBt_Down = nFlag;
    m_nYdIsDown = nFlag;
}
int CXHD::getIsYDDown()
{
    //return isYDBt_Down;
    return m_nYdIsDown;
}
void CXHD::setIsYDSD(bool flag)
{
    isYDSD=flag;
}
bool CXHD::getIsYDSD()
{
    return isYDSD;
}
void CXHD::setLCAN_Rect(QRectF rect)
{
    m_rectLC=rect;
    m_rectLCConst=rect;
}
void CXHD::setTGAN_Rect(QRectF rect)
{
    rectBtnTG=rect;
    m_rectTGConst=rect;
}
void CXHD::setDCAN_Rect(QRectF rect)
{
    m_rectDC=rect;
    m_rectDCConst=rect;
}
void CXHD::setYDAN_Rect(QRectF rect)
{
    m_rectYD=rect;
    m_rectYDConst=rect;
}
QRectF CXHD::getLCAN_Rect()
{
    return m_rectLC;
}
QRectF CXHD::getTGAN_Rect()
{
    return rectBtnTG;
}
QRectF CXHD::getDCAN_Rect()
{
    return m_rectDC;
}
QRectF CXHD::getYDAN_Rect()
{
    return m_rectYD;
}
void CXHD::setSignalDCAN(bool flag)
{
    signalDCAN=flag;
}
bool CXHD::getSignalDCAN()
{
    return signalDCAN;
}
void CXHD::setIsMD(bool flag)
{
    isMD=flag;
}
bool CXHD::getIsMD()
{
    return isMD;
}
void CXHD::setIsLCANFB(bool flag)
{
    isLCANFB=flag;
}
bool CXHD::getIsLCANFB()
{
    return isLCANFB;
}
void CXHD::setIsDCANFB(bool flag)
{
    isDCANFB=flag;
}
bool CXHD::getIsDCANFB()
{
    return isDCANFB;
}
void CXHD::setIsYDANFB(bool flag)
{
    isYDANFB=flag;
}
bool CXHD::getIsYDANFB()
{
    return isYDANFB;
}
bool CXHD::getIsFB()
{
    if(isLCANFB||isDCANFB||isYDANFB)
        return true;
    else
        return false;
}
void CXHD::setTimeType(int x)
{
    m_nTimeType=x;
}
int CXHD::getTimeType()
{
    return m_nTimeType;
}
void CXHD::setTimeCount(int x)
{
    m_nTimeCount=x;
}
int CXHD::getTimeCount()
{
    return m_nTimeCount;
}

void CXHD::Draw_Button(QPainter *painter,long nElapsed, double nDiploid, QPoint offset)
{
    //绘制信号机按钮
#pragma region//>..调车按钮绘制
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, true);
    if(m_DCXHDButtonShowFlag && m_DCXHDButtonShowFlag_BTN)
    {
        //if(nShowState&XHD_DCBTN_SHOW)
        {
            //if((m_nXHDType == DC_XHJ) || m_bCanDC)//调车信号机或者可调车的列车信号机
            {
                if(m_rectDC.left() > 20) //调车按钮绘制
                {
                    Draw_circle_Button(painter,//画笔
                                       m_rectDC,//按钮绘制区域
                                       bFCZKState,//非常站控状态
                                       m_nDcIsDown,//按钮是否按下
                                       isDCANFB,//封锁状态
                                       isDCANFlash,//终端闪烁状态
                                       m_nTimeCount,//倒计时时间
                                       nElapsed,//闪烁读秒
                                       nDiploid,//放大倍数
                                       offset);//绘制圆形按钮
                }
            }
        }
    }
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, false);
#pragma endregion

#pragma region //>..列车按钮绘制
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, true);
    if(m_LCXHDButtonShowFlag && m_LCXHDButtonShowFlag_BTN)
    {
        if(getSignalType() == false)
        {
            //if((m_nXHDType == JZ_XHJ) || (m_nXHDType == CZ_XHJ))
            {
                if(m_rectLC.left() > 20)
                {
                    Draw_Rect_Button(painter,//画笔
                                     m_rectLC,//按钮绘制区域
                                     bFCZKState,//非常站控状态
                                     m_nLcIsDown,//按钮是否按下
                                     isLCANFB,//封锁状态
                                     isLCANFlash,//终端闪烁状态
                                     m_nTimeCount,//倒计时时间
                                     DEEP_GREEN,//按钮颜色
                                     TxtLCButton,//显示按钮文字
                                     nElapsed,//闪烁读秒
                                     nDiploid,//放大倍数
                                     offset);//绘制KSK方形按钮
                }
            }
        }
    }
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, false);
#pragma endregion

#pragma region //>..通过按钮绘制
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, true);
    if(m_TGXHDButtonShowFlag && m_TGXHDButtonShowFlag_BTN)
    {
//		if(nShowState&XHD_LCTGBTN_SHOW)
        {
//            if(m_bHaveTGbtn && rectBtnTG.left() !=0) //通过按钮绘制
            if(rectBtnTG.left() > 20) //通过按钮绘制
            {
                //TxtTGButton = m_strName + "TA";
                Draw_Rect_Button(painter,//画笔
                                 rectBtnTG,//按钮绘制区域
                                 bFCZKState,//非常站控状态
                                 m_nTgIsDown,//按钮是否按下
                                 isLCANFB,//封锁状态
                                 false,//终端闪烁状态
                                 0,//倒计时时间
                                 LIGHT_GREEN,//按钮颜色
                                 TxtTGButton,//显示按钮文字
                                 nElapsed,//闪烁读秒
                                 nDiploid,//放大倍数
                                 offset);//绘制KSK方形按钮
            }
        }
    }
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, false);
#pragma endregion
#pragma region //>..引导按钮绘制
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, true);
    if(m_YDXHDButtonShowFlag && m_YDXHDButtonShowFlag_BTN)
    {
//        if(nShowState&XHD_LCTGBTN_SHOW)
        {
//            if(m_bCanYD && m_rectYD.left() !=0) //引导按钮绘制
            if(m_rectYD.left() > 20) //引导按钮绘制
            {
                Draw_Rect_Button(painter,//画笔
                                 m_rectYD,//按钮绘制区域
                                 bFCZKState,//非常站控状态
                                 m_nYdIsDown,//按钮是否按下
                                 isYDANFB,//封锁状态
                                 false,//终端闪烁状态
                                 m_nTimeCount,//倒计时时间
                                 LIGHT_BLUEX,//按钮颜色
                                 TxtYDButton,//显示按钮文字
                                 nElapsed,//闪烁读秒
                                 nDiploid,//放大倍数
                                 offset);//绘制KSK方形按钮
            }
        }
    }
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, false);
#pragma endregion
#pragma region //>..虚信号按钮绘制
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, true);
    if(getSignalType() == true)
    {
        isLCANFlash = isDCANFlash;
        m_nLcIsDown = m_nDcIsDown;

        if(m_LCXHDButtonShowFlag && m_LCXHDButtonShowFlag_BTN)
        {
            if(m_rectLC.left() > 20)
            {
                Draw_Rect_Button(painter,//画笔
                                 m_rectLC,//按钮绘制区域
                                 bFCZKState,//非常站控状态
                                 m_nLcIsDown,//按钮是否按下
                                 isLCANFB,//封锁状态
                                 isLCANFlash,//终端闪烁状态
                                 m_nTimeCount,//倒计时时间
                                 THIN_GRAY,//按钮颜色
                                 TxtLCButton,//显示按钮文字
                                 nElapsed,//闪烁读秒
                                 nDiploid,//放大倍数
                                 offset);//绘制KSK方形按钮
            }
        }
    }
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, false);
#pragma endregion
}

void CXHD::Draw_circle_Button(QPainter *painter,//画笔
                              QRectF m_ButtonRect,//按钮绘制区域
                              bool m_FCZKState,//非常站控状态
                              int m_ButtonIsDownState,//按钮是否按下
                              bool m_FSState,//封锁状态
                              bool m_bZDANFlash,//终端闪烁状态
                              int mTimeCount,//倒计时时间
                              long nElapsed, //闪烁读秒
                              double nDiploid, //放大倍数
                              QPoint offset)//绘制圆形按钮
{
#pragma region//>..调车按钮绘制
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, true);

    //按钮边框默认颜色
    QColor btnClrTopLeft = WHITE;//按钮边框-TL
    QColor btnClrBottomRight = THIN_GRAY;//按钮边框-BR
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
    if(!m_FCZKState)
        {
        if (m_ButtonIsDownState==1)
        {
            if (nElapsed % 2 == 0)
            {
                btnClrTopLeft = WHITE;//按钮边框-TL
                btnClrBottomRight = THIN_GRAY;//按钮边框-BR
            }
            else
            {
                btnClrTopLeft = THIN_GRAY;//按钮边框-TL
                btnClrBottomRight = WHITE;//按钮边框-BR
            }
        }
        if (m_ButtonIsDownState==2)
        {
            btnClrTopLeft = THIN_GRAY;//按钮边框-TL
            btnClrBottomRight = WHITE;//按钮边框-BR
        }
        if((m_RangeVisible_XHD)||(m_RangeVisible_DCAN))
        {
            //反走样,防止出现锯齿状线条
            painter->setRenderHint(QPainter::Antialiasing, false);

            painter->setPen(QPen(Qt::white, 1,Qt::DashLine));
            painter->setBrush(Qt::transparent);
            painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.bottom()+2,m_ButtonRect.right()+2,m_ButtonRect.bottom()+2);
            painter->drawLine(m_ButtonRect.right()+2,m_ButtonRect.bottom()+2,m_ButtonRect.right()+2,m_ButtonRect.top()-2);

            painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.bottom()+2,m_ButtonRect.left()-2,m_ButtonRect.top()-2);
            painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.top()-2,m_ButtonRect.right()+2,m_ButtonRect.top()-2);

            //反走样,防止出现锯齿状线条
            painter->setRenderHint(QPainter::Antialiasing, true);
        }
    }

    //绘制下半圆
    QPen pen_BK;
    pen_BK.setColor(btnClrBottomRight);
    pen_BK.setWidth(2);
    painter->setPen(pen_BK);
    painter->drawArc(m_ButtonRect, 180*16, 180*16);
    //绘制上半圆
    pen_BK.setColor(btnClrTopLeft);
    painter->setPen(pen_BK);
    painter->drawArc(m_ButtonRect, 0, 180*16);

    QBrush newBrushBK;
    newBrushBK.setStyle(Qt::SolidPattern);
    QPen pen;
    pen.setWidth(1);
    //绘制调车按钮圆体
    if(m_FCZKState)//非常站控模式下
    {
        newBrushBK.setColor(THIN_THIN_GRAY);
        painter->setBrush(newBrushBK);

        pen.setColor(THIN_THIN_GRAY);
        painter->setPen(pen);
        if(m_RangeVisible_XHD)
        {
            newBrushBK.setColor(THIN_BLUE_DOWN);
            painter->setBrush(newBrushBK);

            pen.setColor(THIN_BLUE_DOWN);
            painter->setPen(pen);
        }
    }
    else if(0/*m_FSState*/)//按钮封锁-粉闪
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
                newBrushBK.setColor(GRAY);
                painter->setBrush(newBrushBK);

                pen.setColor(GRAY);
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
    else if(m_RangeVisible)
    {
        newBrushBK.setColor(THIN_BLUE_DOWN);
        painter->setBrush(newBrushBK);

        pen.setColor(THIN_BLUE_DOWN);
        painter->setPen(pen);
    }
    else if((m_RangeVisible_XHD)||(m_RangeVisible_DCAN))
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
                //newBrushBK.setColor(THIN_GRAY);//GRAY
                newBrushBK.setColor(GRAY);
                painter->setBrush(newBrushBK);

                //pen.setColor(THIN_GRAY);
                pen.setColor(GRAY);
                painter->setPen(pen);
                break;
            }
        case 1:
            {
                if(nElapsed % XHDBTN_FLASTIME == 0) //控制时间间隔
                {
//                    newBrushBK.setColor(THIN_GRAY);
//                    painter->setBrush(newBrushBK);

//                    pen.setColor(THIN_GRAY);
//                    painter->setPen(pen);
                    newBrushBK.setColor(WHITE);
                    painter->setBrush(newBrushBK);

                    pen.setColor(WHITE);
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
                newBrushBK.setColor(RED);
                painter->setBrush(newBrushBK);

                pen.setColor(RED);
                painter->setPen(pen);
                break;
            }
        case 4://可按下，终端闪烁
            {
                if(m_bZDANFlash&&bZDFlash)
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
                        newBrushBK.setColor(GRAY);
                        painter->setBrush(newBrushBK);

                        pen.setColor(GRAY);
                        painter->setPen(pen);
                    }
                }
                else
                {
                    newBrushBK.setColor(GRAY);
                    painter->setBrush(newBrushBK);

                    pen.setColor(GRAY);
                    painter->setPen(pen);
                }
                break;
            }
        }
    }
    QPointF point = QPointF(m_ButtonRect.left()+7*nDiploid,m_ButtonRect.top()+7*nDiploid);
    painter->drawEllipse(point,6 * nDiploid,6 * nDiploid);

    //封锁X
    if(m_FSState)
    {
        QPen penX;
        penX.setColor(RED);
        penX.setWidth(2);
        painter->setPen(penX);

        QRectF m_rectDC_X;       //调车按钮区域
        m_rectDC_X.setLeft(m_ButtonRect.left()+2*nDiploid);
        m_rectDC_X.setTop(m_ButtonRect.top()+2*nDiploid);
        m_rectDC_X.setRight(m_ButtonRect.right()-2*nDiploid);
        m_rectDC_X.setBottom(m_ButtonRect.bottom()-2*nDiploid);

        painter->drawLine(m_rectDC_X.left(), m_rectDC_X.top(),m_rectDC_X.right(), m_rectDC_X.bottom());
        painter->drawLine(m_rectDC_X.right(), m_rectDC_X.top(),m_rectDC_X.left(), m_rectDC_X.bottom());
    }

//    //倒计时
//    if(((0<mTimeCount)&&(255>mTimeCount))&&(!((m_DCXHDButtonShowFlag == true)&&(m_LCXHDButtonShowFlag == true))))
//    {
//        QString strTimeCount = "";
//        strTimeCount=QString::number(mTimeCount);
//        strTimeCount = "Y"+strTimeCount;

//        QPen penX;
//        penX.setColor(WHITE);
//        penX.setWidth(1);
//        painter->setPen(penX);
//        QRectF m_rectDC_X;
////        m_rectDC_X.setLeft(m_ButtonRect.left()-20 * nDiploid);
////        m_rectDC_X.setTop(m_ButtonRect.top()-20 * nDiploid);
////        m_rectDC_X.setRight(m_ButtonRect.right()+20 * nDiploid);
////        m_rectDC_X.setBottom(m_ButtonRect.bottom()-20 * nDiploid);
//        m_rectDC_X.setLeft(m_textRect.left()+18 * nDiploid);
//        m_rectDC_X.setTop(m_textRect.top()-4 * nDiploid);
//        m_rectDC_X.setRight(m_textRect.right()+42 * nDiploid);
//        m_rectDC_X.setBottom(m_textRect.bottom()-4 * nDiploid);

//        painter->drawText(m_rectDC_X,Qt::AlignHCenter|Qt::AlignVCenter,strTimeCount);
//    }
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, false);
#pragma endregion
}

void CXHD::Draw_Rect_Button(QPainter *painter,//画笔
                              QRectF m_ButtonRect,//按钮绘制区域
                              bool m_FCZKState,//非常站控状态
                              int m_ButtonIsDownState,//按钮是否按下
                              bool m_FSState,//封锁状态
                              bool m_bZDANFlash,//终端闪烁状态
                              int mTimeCount,//倒计时时间
                              QColor color,//按钮颜色
                              QString rectTextShow, //按钮文字显示
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
    QPen pen_BK;
    pen_BK.setColor(btnClrBottomRight);
    pen_BK.setWidth(2);
    painter->setPen(pen_BK);
    painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.bottom()+2,m_ButtonRect.right()+2,m_ButtonRect.bottom()+2);
    painter->drawLine(m_ButtonRect.right()+2,m_ButtonRect.bottom()+2,m_ButtonRect.right()+2,m_ButtonRect.top()-2);

    pen_BK.setColor(btnClrTopLeft);
    pen_BK.setWidth(2);
    painter->setPen(pen_BK);
    painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.bottom()+2,m_ButtonRect.left()-2,m_ButtonRect.top()-2);
    painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.top()-2,m_ButtonRect.right()+2,m_ButtonRect.top()-2);

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
    if(!m_FCZKState)
    {
        if (m_ButtonIsDownState==1)
        {
            if (nElapsed % 2 == 0)
            {
                btnClrTopLeft = WHITE;//按钮边框-TL
                btnClrBottomRight = GRAY;//按钮边框-BR

                pen_BK.setColor(btnClrBottomRight);
                pen_BK.setWidth(2);
                painter->setPen(pen_BK);
                painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.bottom()+2,m_ButtonRect.right()+2,m_ButtonRect.bottom()+2);
                painter->drawLine(m_ButtonRect.right()+2,m_ButtonRect.bottom()+2,m_ButtonRect.right()+2,m_ButtonRect.top()-2);

                pen_BK.setColor(btnClrTopLeft);
                pen_BK.setWidth(2);
                painter->setPen(pen_BK);
                painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.bottom()+2,m_ButtonRect.left()-2,m_ButtonRect.top()-2);
                painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.top()-2,m_ButtonRect.right()+2,m_ButtonRect.top()-2);

            }
            else
            {
                btnClrTopLeft = GRAY;//按钮边框-TL
                btnClrBottomRight = WHITE;//按钮边框-BR

                pen_BK.setColor(btnClrTopLeft);
                pen_BK.setWidth(2);
                painter->setPen(pen_BK);
                painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.bottom()+2,m_ButtonRect.left()-2,m_ButtonRect.top()-2);
                painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.top()-2,m_ButtonRect.right()+2,m_ButtonRect.top()-2);

                pen_BK.setColor(btnClrBottomRight);
                pen_BK.setWidth(2);
                painter->setPen(pen_BK);
                painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.bottom()+2,m_ButtonRect.right()+2,m_ButtonRect.bottom()+2);
                painter->drawLine(m_ButtonRect.right()+2,m_ButtonRect.bottom()+2,m_ButtonRect.right()+2,m_ButtonRect.top()-2);
            }
        }
        if (m_ButtonIsDownState==2)
        {
            btnClrTopLeft = GRAY;//按钮边框-TL
            btnClrBottomRight = WHITE;//按钮边框-BR

            pen_BK.setColor(btnClrTopLeft);
            pen_BK.setWidth(2);
            painter->setPen(pen_BK);
            painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.bottom()+2,m_ButtonRect.left()-2,m_ButtonRect.top()-2);
            painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.top()-2,m_ButtonRect.right()+2,m_ButtonRect.top()-2);

            pen_BK.setColor(btnClrBottomRight);
            pen_BK.setWidth(2);
            painter->setPen(pen_BK);
            painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.bottom()+2,m_ButtonRect.right()+2,m_ButtonRect.bottom()+2);
            painter->drawLine(m_ButtonRect.right()+2,m_ButtonRect.bottom()+2,m_ButtonRect.right()+2,m_ButtonRect.top()-2);
        }
        if((m_RangeVisible_XHD)||((rectBtnTG==m_ButtonRect)&&(m_RangeVisible_TGAN))||((m_rectLC==m_ButtonRect)&&(m_RangeVisible_LCAN))||((m_rectYD==m_ButtonRect)&&(m_RangeVisible_XHD_YDAN)))
        {
            //反走样,防止出现锯齿状线条
            painter->setRenderHint(QPainter::Antialiasing, false);

            painter->setPen(QPen(Qt::white, 1,Qt::DashLine));
            painter->setBrush(Qt::transparent);
            painter->drawLine(m_ButtonRect.left()-4,m_ButtonRect.bottom()+4,m_ButtonRect.right()+4,m_ButtonRect.bottom()+4);
            painter->drawLine(m_ButtonRect.right()+4,m_ButtonRect.bottom()+4,m_ButtonRect.right()+4,m_ButtonRect.top()-4);

            painter->drawLine(m_ButtonRect.left()-4,m_ButtonRect.bottom()+4,m_ButtonRect.left()-4,m_ButtonRect.top()-4);
            painter->drawLine(m_ButtonRect.left()-4,m_ButtonRect.top()-4,m_ButtonRect.right()+4,m_ButtonRect.top()-4);

            //反走样,防止出现锯齿状线条
            painter->setRenderHint(QPainter::Antialiasing, true);
        }
    }



    //绘制按钮主体
    QPen pen;
    pen.setJoinStyle(Qt::PenJoinStyle ::MiterJoin);
    pen.setWidth(2);
    QBrush newBrushBK;
    newBrushBK.setStyle(Qt::SolidPattern);
    if(m_FCZKState)//非常站控模式下
    {
        newBrushBK.setColor(THIN_GRAY);
        painter->setBrush(newBrushBK);

        pen.setColor(THIN_GRAY);
        painter->setPen(pen);
        if(m_RangeVisible_XHD)
        {
            newBrushBK.setColor(THIN_BLUE_DOWN);
            painter->setBrush(newBrushBK);

            pen.setColor(THIN_BLUE_DOWN);
            painter->setPen(pen);
        }
    }
    else if(0/*m_FSState*/)//按钮封锁-粉闪
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
    else if(m_RangeVisible)
    {
        newBrushBK.setColor(THIN_BLUE_DOWN);
        painter->setBrush(newBrushBK);

        pen.setColor(THIN_BLUE_DOWN);
        painter->setPen(pen);
    }
    else if(m_RangeVisible_XHD)
    {
        newBrushBK.setColor(THIN_BLUE_DOWN);
        painter->setBrush(newBrushBK);

        pen.setColor(THIN_BLUE_DOWN);
        painter->setPen(pen);
    }
    else if((m_rectLC==m_ButtonRect)&&(m_RangeVisible_LCAN))
    {
        newBrushBK.setColor(THIN_BLUE_DOWN);
        painter->setBrush(newBrushBK);

        pen.setColor(THIN_BLUE_DOWN);
        painter->setPen(pen);
    }
    else if((rectBtnTG==m_ButtonRect)&&(m_RangeVisible_TGAN))
    {
        newBrushBK.setColor(THIN_BLUE_DOWN);
        painter->setBrush(newBrushBK);

        pen.setColor(THIN_BLUE_DOWN);
        painter->setPen(pen);
    }
    else if((m_rectYD==m_ButtonRect)&&(m_RangeVisible_XHD_YDAN))
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
//                    newBrushBK.setColor(color);
//                    painter->setBrush(newBrushBK);

//                    pen.setColor(color);
//                    painter->setPen(pen);

                    newBrushBK.setColor(WHITE);
                    painter->setBrush(newBrushBK);

                    pen.setColor(WHITE);
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
                newBrushBK.setColor(RED);
                painter->setBrush(newBrushBK);

                pen.setColor(RED);
                painter->setPen(pen);
                break;
            }
        case 4://可按下，终端闪烁
            {
                if(m_bZDANFlash&&bZDFlash)
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
                        newBrushBK.setColor(color);
                        painter->setBrush(newBrushBK);

                        pen.setColor(color);
                        painter->setPen(pen);
                    }
                }
                else
                {
                    newBrushBK.setColor(color);
                    painter->setBrush(newBrushBK);

                    pen.setColor(color);
                    painter->setPen(pen);
                }
                break;
            }
        }
    }
    painter->drawRect(m_ButtonRect);

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

//    //倒计时
//    if(mTimeCount > 0 && getXHDState(XHD_YD))
//    {
//        if(m_rectLC==m_ButtonRect)
//        {
//            if(m_textRect.top()< pCenter.y())
//            {
//                if((0<mTimeCount)&&(255>mTimeCount))
//                {
//                    QString strTimeCount = "";
//                    strTimeCount=QString::number(mTimeCount);
//                    strTimeCount = "Y"+strTimeCount;

//                    QPen penX;
//                    penX.setColor(WHITE);
//                    penX.setWidth(1);
//                    painter->setPen(penX);
//                    QRectF m_rectDC_X;

//                    m_rectDC_X.setLeft(m_textRect.left()-38 * nDiploid);
//                    m_rectDC_X.setTop(m_textRect.top()-4 * nDiploid);
//                    m_rectDC_X.setRight(m_textRect.right()-2 * nDiploid);
//                    m_rectDC_X.setBottom(m_textRect.bottom()-4 * nDiploid);
//                    painter->drawText(m_rectDC_X,Qt::AlignHCenter|Qt::AlignVCenter,strTimeCount);
//                }
//            }
//            else
//            {
//                if((0<mTimeCount)&&(255>mTimeCount))
//                {
//                    QString strTimeCount = "";
//                    strTimeCount=QString::number(mTimeCount);
//                    strTimeCount = "Y"+strTimeCount;

//                    QPen penX;
//                    penX.setColor(WHITE);
//                    penX.setWidth(1);
//                    painter->setPen(penX);
//                    QRectF m_rectDC_X;

//                    m_rectDC_X.setLeft(m_textRect.left()+12 * nDiploid);
//                    m_rectDC_X.setTop(m_textRect.top()-4 * nDiploid);
//                    m_rectDC_X.setRight(m_textRect.right()+36 * nDiploid);
//                    m_rectDC_X.setBottom(m_textRect.bottom()-4 * nDiploid);
//                    painter->drawText(m_rectDC_X,Qt::AlignHCenter|Qt::AlignVCenter,strTimeCount);
//                }
//            }
//        }
//    }
//    else
//    {
//        if(m_rectLC==m_ButtonRect)
//        {
//            if((0<mTimeCount)&&(255>mTimeCount))
//            {
//                QString strTimeCount = "";
//                strTimeCount=QString::number(mTimeCount);
//                strTimeCount = "Y"+strTimeCount;

//                QPen penX;
//                penX.setColor(WHITE);
//                penX.setWidth(1);
//                painter->setPen(penX);
//                QRectF m_rectDC_X;
//        //        m_rectDC_X.setLeft(m_ButtonRect.left()-20 * nDiploid);
//        //        m_rectDC_X.setTop(m_ButtonRect.top()-20 * nDiploid);
//        //        m_rectDC_X.setRight(m_ButtonRect.right()+20 * nDiploid);
//        //        m_rectDC_X.setBottom(m_ButtonRect.bottom()-20 * nDiploid);
//                m_rectDC_X.setLeft(m_textRect.left()+18 * nDiploid);
//                m_rectDC_X.setTop(m_textRect.top()-4 * nDiploid);
//                m_rectDC_X.setRight(m_textRect.right()+42 * nDiploid);
//                m_rectDC_X.setBottom(m_textRect.bottom()-4 * nDiploid);
//                painter->drawText(m_rectDC_X,Qt::AlignHCenter|Qt::AlignVCenter,strTimeCount);
//            }
//        }
//    }

//    //绘制按钮的文字
//    if(rectTextShow != ""&&bTxtTGButtonshow)
//    {
//        pen.setColor(getLightState(wordcolor,nElapsed));
//        QFont font;
//        font.setWeight(50);//字体宽度,0~99
//        font.setPointSize((wordsize-2) * nDiploid);//字体高度,10表示高度为10个像素点
//        painter->setPen(pen);
//        painter->setFont(font);
//        QRectF rectBtnTG_X;
//        rectBtnTG_X.setLeft(m_ButtonRect.left()-20 * nDiploid);
//        rectBtnTG_X.setTop(m_ButtonRect.top()-25 * nDiploid);
//        rectBtnTG_X.setRight(m_ButtonRect.right()+20 * nDiploid);
//        rectBtnTG_X.setBottom(m_ButtonRect.bottom()-15 * nDiploid);
////        painter->drawText(rectBtnTG_X,Qt::AlignHCenter|Qt::AlignVCenter,m_strName + "TA");
//        painter->drawText(rectBtnTG_X,Qt::AlignHCenter|Qt::AlignVCenter,rectTextShow);
//    }
    //绘制按钮的文字
    if(rectTextShow != ""&&bTxtTGButtonshow)
    {
        pen.setColor(getLightState(wordcolor,nElapsed));
        QFont font;
        font.setWeight(50);//字体宽度,0~99
        font.setPointSize((wordsize) * nDiploid);//字体高度,10表示高度为10个像素点
        painter->setPen(pen);
        painter->setFont(font);



        if(rectBtnTG==m_ButtonRect)
        {
             //painter->drawText(rectBtnTG_word.left() * nDiploid,rectBtnTG_word.top() * nDiploid,TxtTGButton);

            //if(getSX()==1)//上行
            if(m_textRect.bottom()<rectBtnTG.bottom())
             {
                 rectTxtTG.setRect(rectBtnTG.left()-8, rectBtnTG.bottom()-15,
                                   rectBtnTG.right()+8, rectBtnTG.bottom()-2*8);
                 painter->drawText(rectTxtTG.left() * nDiploid,rectTxtTG.top() * nDiploid,TxtTGButton);
             }
             else /*if(getSX()==0)*///下行
             {
                 rectTxtTG.setRect(rectBtnTG.left()-8, rectBtnTG.bottom()+15,
                                   rectBtnTG.right()+8, rectBtnTG.bottom()+2*8);
                 painter->drawText(rectTxtTG.left() * nDiploid,rectTxtTG.top() * nDiploid,TxtTGButton);
             }
        }
        else
        {
            rectBtnTG_word.setLeft(m_ButtonRect.left()-20 * nDiploid);
            rectBtnTG_word.setTop(m_ButtonRect.top()-25 * nDiploid);
            rectBtnTG_word.setRight(m_ButtonRect.right()+20 * nDiploid);
            rectBtnTG_word.setBottom(m_ButtonRect.bottom()-15 * nDiploid);
            painter->drawText(rectBtnTG_word.left(),rectBtnTG_word.top(),rectTextShow);
        }
//        painter->drawText(rectBtnTG_X,Qt::AlignHCenter|Qt::AlignVCenter,m_strName + "TA");

    }
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, false);
#pragma endregion
}
bool CXHD::GetMousePoint_XHD(QPoint p)
{
    if ((m_devRect.contains(p))||(m_devRect_lamp1.contains(p)))
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool CXHD::GetMousePoint_DCAN(QPoint p)
{
    return getDCAN_Rect().contains(p);
}
bool CXHD::GetMousePoint_LCAN(QPoint p)
{
    if (getLCAN_Rect().contains(p))
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool CXHD::GetMousePoint_TGAN(QPoint p)
{
    if (rectBtnTG.contains(p))
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool CXHD::GetMousePoint_Name(QPoint p)
{
    if (m_textRect.contains(p))
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool CXHD::GetMousePoint_YDAN(QPoint p)
{
    if (getYDAN_Rect().contains(p))
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool CXHD::GetMousePoint(QPoint p)
{
    if ((m_devRect.contains(p))
            ||(m_devRect_lamp1.contains(p))
            ||(m_textRect.contains(p))
            ||(getLCAN_Rect().contains(p))
            ||(getDCAN_Rect().contains(p))
            ||(rectBtnTG.contains(p))
            ||(getYDAN_Rect().contains(p)))
    {
        return true;
    }
    else
    {
        return false;
    }
}

QColor CXHD::getLightState(int nType,int nElapsed)//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝）
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































