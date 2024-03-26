#include "gd.h"
#pragma execution_character_set("utf-8")
CGD::CGD()
{
    p1.setX(1);
    p1.setY(1);
    p2.setX(1);
    p2.setY(1);
    p3.setX(1);
    p3.setY(1);
    p4.setX(1);
    p4.setY(1);
    p12.setX(1);
    p12.setY(1);
    p34.setX(1);
    p34.setY(1);
    pz12.setX(1);
    pz12.setY(1);
    pz34.setX(1);
    pz34.setY(1);
    m_nGDType = QD;            //轨道区段类型
    m_nZ = 0;                   //折点个数
    //    isLostTrainNum = false;     //是否列车丢失--暂时跟着车次走，先预留
    //    isLock = false;             //是否封锁
    isGDFLBL = false;           //是否分录不良
    m_nGDFLBLKX = false;
    //GLB_QDleft=0;
    //GLB_QDright=0;
    //isSXDC = false;
    isZXGD = false;
    isCXGD = false;
    isJJGD = false;
    //GD_LCode= 0xffff;
    //GD_RCode= 0xffff;
    //CarrierFrequency="2300-2";
    //Dir_DMH=LEFT;
    //isNoShow=false;
    m_strMark1="";
    m_strMark1="";

    m_strCheCiNum = "";//车次
    m_strCheCiNumCTC = "";
    m_nCheciLost = false;//车次是否丢失
    m_bElectric = true;//电力牵引
    m_nSXCheCi = 0;//1右行 0左行
    m_bLCTW = false;//列车挺稳标志
    m_nKHType = LCTYPE_KC;//客货类型：客车/货车
    m_nSpeed = 0;

    isLock = false;
    isPowerCut = false;
    isSpeedLimit = false;
    isFS=false;
    isMA=false;
    m_bHaveGrayBack = false/*true*/;//FALSE;
    bCCKIsUp = true;
    m_nVisibleCheci = false;
    nWbWidth=10;

    bsqdfmCode = 0;
    bsqdfmDirection = 0;
    speedLimitStatus = 0;
    m_RangeVisible = false;
    m_RangeVisible_XZ = false;
    bShow_Jyj=true;
    bTextShowGD=true;
    wordcolor = 3;//文字颜色//默认白色
    wordsize = 12;//文字大小

    isZDQJXHDDirFlag= false;
    isZDQJXHDRight= false;
    m_nBSQDFMCode = 0;
    m_bBSQDFMRight = false;
    setDevStateToSafe();
}

//成员功能函数实现
void CGD::GDInit(int type)
{
    p1Const.setX(p1.x());
    p1Const.setY(p1.y());
    p2Const.setX(p2.x());
    p2Const.setY(p2.y());
    p3Const.setX(p3.x());
    p3Const.setY(p3.y());
    p4Const.setX(p4.x());
    p4Const.setY(p4.y());
    p12Const.setX(p12.x());
    p12Const.setY(p12.y());
    p34Const.setX(p34.x());
    p34Const.setY(p34.y());
    pz12Const.setX(pz12.x());
    pz12Const.setY(pz12.y());
    pz34Const.setX(pz34.x());
    pz34Const.setY(pz34.y());
    pCenterConst.setX(pCenter.x());
    pCenterConst.setY(pCenter.y());
    m_textRectConst.setLeft(m_textRect.left());
    m_textRectConst.setTop(m_textRect.top());
    m_textRectConst.setRight(m_textRect.right());
    m_textRectConst.setBottom(m_textRect.bottom());
}
void CGD::Draw(QPainter *painter, long nElapsed, double nDiploid,QPoint offset,int type)
{
    if(type==0x55)
    {
        Draw_Th_kb(painter,nElapsed, nDiploid,offset);
    }
}
void CGD::Draw_Th_kb(QPainter *painter, long nElapsed, double nDiploid,QPoint offset)
{
    QFont font;
    p1.setX(p1Const.x() * nDiploid+offset.x()*nDiploid);
    p1.setY(p1Const.y() * nDiploid+offset.y()*nDiploid);
    p2.setX(p2Const.x() * nDiploid+offset.x()*nDiploid);
    p2.setY(p2Const.y() * nDiploid+offset.y()*nDiploid);
    p3.setX(p3Const.x() * nDiploid+offset.x()*nDiploid);
    p3.setY(p3Const.y() * nDiploid+offset.y()*nDiploid);
    p4.setX(p4Const.x() * nDiploid+offset.x()*nDiploid);
    p4.setY(p4Const.y() * nDiploid+offset.y()*nDiploid);
    p12.setX(p12Const.x() * nDiploid+offset.x()*nDiploid);
    p12.setY(p12Const.y() * nDiploid+offset.y()*nDiploid);
    p34.setX(p34Const.x() * nDiploid+offset.x()*nDiploid);
    p34.setY(p34Const.y() * nDiploid+offset.y()*nDiploid);
    pz12.setX(pz12Const.x() * nDiploid+offset.x()*nDiploid);
    pz12.setY(pz12Const.y() * nDiploid+offset.y()*nDiploid);
    pz34.setX(pz34Const.x() * nDiploid+offset.x()*nDiploid);
    pz34.setY(pz34Const.y() * nDiploid+offset.y()*nDiploid);
    pCenter.setX(pCenterConst.x() * nDiploid+offset.x()*nDiploid);
    pCenter.setY(pCenterConst.y() * nDiploid+offset.y()*nDiploid);
    nWbWidth = nWbWidth*nDiploid;
    m_textRect.setLeft(m_textRectConst.left() * nDiploid+offset.x()*nDiploid);
    m_textRect.setTop(m_textRectConst.top() * nDiploid+offset.y()*nDiploid);
    m_textRect.setRight(m_textRectConst.right() * nDiploid+offset.x()*nDiploid);
    m_textRect.setBottom(m_textRectConst.bottom() * nDiploid+offset.y()*nDiploid);
    m_devRect.setRect(p12.x()+10* nDiploid,p12.y()-2* nDiploid,p34.x()-p12.x()-20* nDiploid,4* nDiploid);
    //取消防锯齿//反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, false);


    //绘制股道底部的灰色区域
    if(m_bHaveGrayBack)
    {
        if(m_nGDType == GD_QD)
        {
            QPoint ptLeft, ptRight;
            ptLeft = QPoint(p12.x() + (p34.x()-p12.x())/4, p12.y() - (GDWIDTH2*2*nDiploid));
            ptRight = QPoint(p34.x() - (p34.x()-p12.x())/4, p34.y() + (GDWIDTH2*2*nDiploid));
            QRect BackRect(ptLeft.x(),ptLeft.y(),ptRight.x(),ptRight.y());
            QBrush newBrushBK;
            QPen pen; //画笔
            pen.setColor(GDBK_GRAY);
            newBrushBK.setStyle(Qt::SolidPattern);
            newBrushBK.setColor(GDBK_GRAY);
            painter->setPen(pen);
            painter->setBrush(newBrushBK);
            painter->drawRect(ptLeft.x(),ptLeft.y(),ptRight.x()-ptLeft.x(),ptRight.y()-ptLeft.y());
        }
    }
    setQDColor(BLUEKX);

    if (getState(QDSB) && WndType == 1)   //轨道锁闭
    {
        setQDColor(WHITE);
    }
    else if(isPLCZ && WndType == 2)
    {
        setQDColor(Qt::cyan);
    }
    else setQDColor(SkyBlue);

    painter->setPen(QPen(getQDColor(), GDWIDTH * nDiploid,Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    if (0 == getZ())
    {
        //绘制股道
        painter->drawLine(p12, p34);//绘制轨道电路光带
    }
    else if (1 == getZ())
    {
        painter->drawLine(p12, pCenter);//绘制轨道电路光带
        painter->drawLine(pCenter, p34);//绘制轨道电路光带
    }
    else
    {
        painter->drawLine(p12, pz12);//绘制轨道电路光带
        painter->drawLine(pz12, pz34);//绘制轨道电路光带
        painter->drawLine(pz34, p34);//绘制轨道电路光带
    }

    //绘制绝缘节 2021.1.11 BJT
    if(bShow_Jyj)
    {
        painter->setPen(QPen(GREY,2));
        painter->drawLine(p1, p2); //绘制轨道区段左侧绝缘节
        painter->drawLine(p3, p4); //绘制轨道区段右侧绝缘节
    }

    //绘制轨道区段名称 2021.1.11 BJT
    if(WndType == 1)
    {
        if((bTextShowWCQD&&getGDType()==QD)||(bTextShowJJQD&&getGDType()==JJ_QD))
            isDisPlayName = false;
    }
    if((bTextShowGD&&getGDType()==GD_QD)||(bTextShowWCQD&&getGDType()==QD)||(bTextShowJJQD&&getGDType()==JJ_QD))
    {
        if (getDisplayName())
        {
            font.setFamily("宋体");
            font.setPointSize(wordsize * nDiploid);//字号
            font.setItalic(false);//斜体
            painter->setFont(font);//设置字体

            painter->setPen(getLightState(wordcolor,nElapsed));//设置画笔颜色getLightState(int nType,int nElapsed)

            //绘制文本
            painter->drawText(m_textRect.left(), m_textRect.bottom()-5*nDiploid, getName());
        }
    }

    if(bsqdfmDirection == 1)
    {
        m_bBSQDFMRight = false;
    }
    else if(bsqdfmDirection == 2)
    {
        m_bBSQDFMRight = true;
    }
    else m_bBSQDFMRight = false;
    if(isVollover)
        m_bBSQDFMRight = !m_bBSQDFMRight;
    m_nBSQDFMCode = bsqdfmCode;
}
void CGD::Draw_FLBL_WBX(QPainter *painter,long nElapsed,QPointF pt1,QPointF pt2, double nDiploid)
{
//    painter->setPen(QPen(Qt::magenta,1*nDiploid,Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    if(m_nGDFLBLKX)
    {
        painter->setPen(QPen(Qt::magenta,1*nDiploid,Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    }
    else if(nElapsed % XHDBTN_FLASTIME == 0) //控制时间间隔
    {
        painter->setPen(QPen(Qt::transparent,1*nDiploid,Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));//Qt::transparent这个颜色为透明色
    }
    else
    {
        painter->setPen(QPen(Qt::magenta,1*nDiploid,Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    }
//    if(pt1.y()==pt2.y())
//    {
//        painter->drawLine(QPoint(pt1.x(),pt1.y()-2*nDiploid), QPoint(pt2.x(),pt2.y()-2*nDiploid));//绘制分路不良外包线
//        painter->drawLine(QPoint(pt1.x(),pt1.y()+2*nDiploid), QPoint(pt2.x(),pt2.y()+2*nDiploid));//绘制分路不良外包线
//    }
//    else
//    {
//        painter->drawLine(QPoint(pt1.x(),pt1.y()-4*nDiploid), QPoint(pt2.x(),pt2.y()-4*nDiploid));//绘制分路不良外包线
//        painter->drawLine(QPoint(pt1.x(),pt1.y()+4*nDiploid), QPoint(pt2.x(),pt2.y()+4*nDiploid));//绘制分路不良外包线
//    }
    double a = (pt1.x()-pt2.x());
    double b = (pt1.y()-pt2.y());
    double c = sqrt(a*a+b*b);
    QPointF firstP,secondP;
    firstP = QPointF(pt1.x()+(GDWIDTH-1)*nDiploid*b/c,pt1.y()-(GDWIDTH-1)*nDiploid*a/c);
    secondP = QPointF(pt2.x()+(GDWIDTH-1)*nDiploid*b/c,pt2.y()-(GDWIDTH-1)*nDiploid*a/c);
    painter->drawLine(secondP,firstP);

    firstP = QPointF(pt1.x()-(GDWIDTH-1)*nDiploid*b/c,pt1.y()+(GDWIDTH-1)*nDiploid*a/c);
    secondP = QPointF(pt2.x()-(GDWIDTH-1)*nDiploid*b/c,pt2.y()+(GDWIDTH-1)*nDiploid*a/c);
    painter->drawLine(firstP,secondP);
}
void CGD::Draw_FLBL_WBX_QJLJJC(QPainter *painter,long nElapsed,QPointF pt1,QPointF pt2, double nDiploid)
{
    if(nElapsed % XHDBTN_FLASTIME == 0) //控制时间间隔
    {
        painter->setPen(QPen(Qt::transparent,1*nDiploid,Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    }
    else
    {
        painter->setPen(QPen(Qt::magenta,1*nDiploid,Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    }
//    if(pt1.y()==pt2.y())
//    {
//        painter->drawLine(QPoint(pt1.x(),pt1.y()-2*nDiploid), QPoint(pt2.x(),pt2.y()-2*nDiploid));//绘制分路不良外包线
//        painter->drawLine(QPoint(pt1.x(),pt1.y()+2*nDiploid), QPoint(pt2.x(),pt2.y()+2*nDiploid));//绘制分路不良外包线
//    }
//    else
//    {
//        painter->drawLine(QPoint(pt1.x(),pt1.y()-4*nDiploid), QPoint(pt2.x(),pt2.y()-4*nDiploid));//绘制分路不良外包线
//        painter->drawLine(QPoint(pt1.x(),pt1.y()+4*nDiploid), QPoint(pt2.x(),pt2.y()+4*nDiploid));//绘制分路不良外包线
//    }
    double a = (pt1.x()-pt2.x());
    double b = (pt1.y()-pt2.y());
    double c = sqrt(a*a+b*b);
    QPointF firstP,secondP;
    firstP = QPointF(pt1.x()+(GDWIDTH-1)*nDiploid*b/c,pt1.y()-(GDWIDTH-1)*nDiploid*a/c);
    secondP = QPointF(pt2.x()+(GDWIDTH-1)*nDiploid*b/c,pt2.y()-(GDWIDTH-1)*nDiploid*a/c);
    painter->drawLine(secondP,firstP);

    firstP = QPointF(pt1.x()-(GDWIDTH-1)*nDiploid*b/c,pt1.y()+(GDWIDTH-1)*nDiploid*a/c);
    secondP = QPointF(pt2.x()-(GDWIDTH-1)*nDiploid*b/c,pt2.y()+(GDWIDTH-1)*nDiploid*a/c);
    painter->drawLine(firstP,secondP);
}

void CGD::Draw_WBX(QPainter *painter,QPointF pt1,QPointF pt2, double nDiploid,int nLineW,QColor color,int nSpace)//增加接触网无电,临时限速,移动授权外包线
{
    painter->setPen(QPen(color,nLineW*nDiploid,Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
//    if(pt1.y()==pt2.y())
//    {
//        painter->drawLine(QPoint(pt1.x(),pt1.y()-(2+nSpace)*nDiploid), QPoint(pt2.x(),pt2.y()-(2+nSpace)*nDiploid));
//        painter->drawLine(QPoint(pt1.x(),pt1.y()+(2+nSpace)*nDiploid), QPoint(pt2.x(),pt2.y()+(2+nSpace)*nDiploid));
//    }
//    else
//    {
//        painter->drawLine(QPoint(pt1.x(),pt1.y()-(4+nSpace)*nDiploid), QPoint(pt2.x(),pt2.y()-(4+nSpace)*nDiploid));
//        painter->drawLine(QPoint(pt1.x(),pt1.y()+(4+nSpace)*nDiploid), QPoint(pt2.x(),pt2.y()+(4+nSpace)*nDiploid));
//    }
    double a = (pt1.x()-pt2.x());
    double b = (pt1.y()-pt2.y());
    double c = sqrt(a*a+b*b);
    QPointF firstP,secondP;
    firstP = QPointF(pt1.x()+(GDWIDTH-1)*nDiploid*b/c,pt1.y()-(GDWIDTH-1)*nDiploid*a/c);
    secondP = QPointF(pt2.x()+(GDWIDTH-1)*nDiploid*b/c,pt2.y()-(GDWIDTH-1)*nDiploid*a/c);
    painter->drawLine(secondP,firstP);

    firstP = QPointF(pt1.x()-(GDWIDTH-1)*nDiploid*b/c,pt1.y()+(GDWIDTH-1)*nDiploid*a/c);
    secondP = QPointF(pt2.x()-(GDWIDTH-1)*nDiploid*b/c,pt2.y()+(GDWIDTH-1)*nDiploid*a/c);
    painter->drawLine(firstP,secondP);
}
void CGD::Draw_WBX_Light(QPainter *painter,QPointF pt1,QPointF pt2, double nDiploid,int nLineW,QColor color,int nSpace)//增加接触网无电,临时限速,移动授权外包线
{
    painter->setPen(QPen(color,nLineW*nDiploid,Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));
    double a = (pt1.x()-pt2.x());
    double b = (pt1.y()-pt2.y());
    double c = sqrt(a*a+b*b);
    QPointF firstP,secondP;
    firstP = QPointF(pt1.x()+(GDWIDTH)*nDiploid*b/c,pt1.y()-(GDWIDTH)*nDiploid*a/c);
    secondP = QPointF(pt2.x()+(GDWIDTH)*nDiploid*b/c,pt2.y()-(GDWIDTH)*nDiploid*a/c);
    painter->drawLine(secondP,firstP);

    firstP = QPointF(pt1.x()-(GDWIDTH+1)*nDiploid*b/c,pt1.y()+(GDWIDTH+1)*nDiploid*a/c);
    secondP = QPointF(pt2.x()-(GDWIDTH+1)*nDiploid*b/c,pt2.y()+(GDWIDTH+1)*nDiploid*a/c);
    painter->drawLine(firstP,secondP);

}
//绘制股道防溜标志
void CGD::Draw_GDAntiSlip(QPainter *painter, long nElapsed, double nDiploid, QPoint offset)
{
    //设置了防溜（绘制红色三角形）lwm.2023.5.24
    if ((m_nGDType == GD_QD) && (m_nLAntiSlipType > 0 || m_nRAntiSlipType > 0))
    {
        int dis = 10 * nDiploid;
        int a = 14 * nDiploid;
        int b = 22 * nDiploid;
        //三角形
        QPoint arrayTxLeft[3] = { QPoint(0, 0), QPoint(0, 0), QPoint(0, 0)};
        QPoint arrayDotLeft[3] = { QPoint(0, 0), QPoint(0, 0), QPoint(0, 0) };
        QPoint arrayTxRight[3] = { QPoint(0, 0), QPoint(0, 0), QPoint(0, 0) };
        QPoint arrayDotRight[3] = { QPoint(0, 0), QPoint(0, 0), QPoint(0, 0) };
        if(p12.x()<p34.x()) //正向
        {
            arrayTxLeft[0]=QPoint(p12.x() + dis, p12.y() - dis);
            arrayTxLeft[1]=QPoint(p12.x() + dis, p12.y() - dis - a);
            arrayTxLeft[2]=QPoint(p12.x() + dis + b, p12.y() - dis);

            arrayDotLeft[0]=QPoint(arrayTxLeft[0].x() + 3 * nDiploid, arrayTxLeft[0].y() - 3 * nDiploid);
            arrayDotLeft[1]=QPoint(arrayTxLeft[0].x() + 3 * nDiploid, arrayTxLeft[0].y() - 8 * nDiploid);
            arrayDotLeft[2]=QPoint(arrayTxLeft[0].x() + 10 * nDiploid, arrayTxLeft[0].y() - 3 * nDiploid);

            arrayTxRight[0]=QPoint(p34.x() - dis, p34.y() - dis);
            arrayTxRight[1]=QPoint(p34.x() - dis, p34.y() - dis - a);
            arrayTxRight[2]=QPoint(p34.x() - dis - b, p34.y() - dis);

            arrayDotRight[0]=QPoint(arrayTxRight[0].x() - 3 * nDiploid, arrayTxRight[0].y() - 3 * nDiploid);
            arrayDotRight[1]=QPoint(arrayTxRight[0].x() - 3 * nDiploid, arrayTxRight[0].y() - 8 * nDiploid);
            arrayDotRight[2]=QPoint(arrayTxRight[0].x() - 10 * nDiploid, arrayTxRight[0].y() - 3 * nDiploid);
        }
        else //反向
        {
            arrayTxLeft[0]=QPoint(p12.x() - dis, p12.y() - dis);
            arrayTxLeft[1]=QPoint(p12.x() - dis, p12.y() - dis - a);
            arrayTxLeft[2]=QPoint(p12.x() - dis - b, p12.y() - dis);

            arrayDotLeft[0]=QPoint(arrayTxLeft[0].x() - 3 * nDiploid, arrayTxLeft[0].y() - 3 * nDiploid);
            arrayDotLeft[1]=QPoint(arrayTxLeft[0].x() - 3 * nDiploid, arrayTxLeft[0].y() - 8 * nDiploid);
            arrayDotLeft[2]=QPoint(arrayTxLeft[0].x() - 10 * nDiploid, arrayTxLeft[0].y() - 3 * nDiploid);

            arrayTxRight[0]=QPoint(p34.x() + dis, p34.y() - dis);
            arrayTxRight[1]=QPoint(p34.x() + dis, p34.y() - dis - a);
            arrayTxRight[2]=QPoint(p34.x() + dis + b, p34.y() - dis);

            arrayDotRight[0]=QPoint(arrayTxRight[0].x() + 3 * nDiploid, arrayTxRight[0].y() - 3 * nDiploid);
            arrayDotRight[1]=QPoint(arrayTxRight[0].x() + 3 * nDiploid, arrayTxRight[0].y() - 8 * nDiploid);
            arrayDotRight[2]=QPoint(arrayTxRight[0].x() + 10 * nDiploid, arrayTxRight[0].y() - 3 * nDiploid);
        }
        //绘制红色三角形
        QPen pen1;
        pen1.setWidth(1);
        pen1.setColor(Qt::red);
        painter->setPen(pen1);
        QBrush brush1;
        brush1.setStyle(Qt::SolidPattern);
        brush1.setColor(Qt::red);
        painter->setBrush(brush1);
        //if(m_nLAntiSlipType > 0)
        {
            painter->drawPolygon(arrayTxLeft, 3);
        }
        //if(m_nRAntiSlipType > 0)
        {
            painter->drawPolygon(arrayTxRight, 3);
        }
        //绘制白点
        QPen pen2;
        pen2.setWidth(3 * nDiploid);
        pen2.setColor(Qt::white);
        painter->setPen(pen2);
        //if(m_nLAntiSlipType > 0)
        {
            painter->drawPoint(arrayDotLeft[0]);
            painter->drawPoint(arrayDotLeft[1]);
            painter->drawPoint(arrayDotLeft[2]);
        }
        //if(m_nRAntiSlipType > 0)
        {
            painter->drawPoint(arrayDotRight[0]);
            painter->drawPoint(arrayDotRight[1]);
            painter->drawPoint(arrayDotRight[2]);
        }
    }
}
void CGD::Draw_ToolTip(QPainter *painter, double nDiploid)
{
    //    if(getisNoShow()==true)
    //    {
    //        return;
    //    }
    QFont font;
    QPoint pt;
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, false);
    //绘制鼠标进入文字提示信息
    if((true == getToolTipFlag()) && (""!=getToolTipStr()))
    {
        pt.setX(m_textRect.left()+5*nDiploid);
        pt.setY(m_textRect.top()-10*nDiploid);
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
//绘制车次窗
//void CGD::DrawTrainWindown(QPainter *painter,double nDiploid,int nLineW,QColor color,double nCheciDiploid,int nShowState)
//{
//    //上下行判断
//    bool sxlorr = false;
//    if(nShowState & SXLORR)
//    {
//        sxlorr = true;
//    }
//    else
//    {
//        sxlorr = false;
//    }
//    if(m_nGDType == JJ_QD)
//    {
//        if(!bCCKIsUp)
//        {
//            //在下方
//            m_rectCheCi.setRect(pCenter.x() - 30*nDiploid*nCheciDiploid,
//                pCenter.y() + 10*nDiploid*nCheciDiploid,
//                60*nDiploid*nCheciDiploid,
//                20*nDiploid*nCheciDiploid);
//        }
//        else
//        {
//            //在上方
//            m_rectCheCi.setRect(pCenter.x() - 30*nDiploid*nCheciDiploid,
//                pCenter.y() - 30*nDiploid*nCheciDiploid,
//                60*nDiploid*nCheciDiploid,
//                20);
//        }
//    }
//    else if(m_nGDType == GD_QD)
//    {
//        //车次窗位于股道中央，接近区段不带车次窗！
//        m_rectCheCi.setRect(pCenter.x() - (GDWIDTH*nDiploid)*5*1.5*nCheciDiploid,
//            pCenter.y() - (GDWIDTH*nDiploid)*2.5*nCheciDiploid,
//            2*(GDWIDTH*nDiploid)*5*1.5*nCheciDiploid,
//            2*(GDWIDTH*nDiploid)*2.5*nCheciDiploid);
//    }
//    else
//    {
//        //接近区段不带车次窗！
//        m_rectCheCi.setRect(-101,-101,0,0);
//    }
//    //如果车次显示 或者 鼠标移动到车次窗上 则车次窗显示
//    if((nShowState&GD_CHECIWND_SHOW) || m_nVisibleCheci)
//    {
//        QBrush newBrushBK;
//        newBrushBK.setStyle(Qt::NoBrush);
//        painter->setBrush(newBrushBK);
//        painter->setPen(QPen(color, nLineW, Qt::DashLine));
//        painter->drawRect(m_rectCheCi);
//    }
//}
//绘制车次
//void CGD::DrawTrainNum(QPainter *painter,long nElapsed,double nDiploid,int nLineW,QColor color,double nCheciDiploid,int nShowState)
//{
//    //车次基本位置信息
//    //double Diplod = 1.0; //放大倍数 用于车次丢失时 放大缩小 有1.0 1.5 两种取值
//    QRect l_checiRect; //股道显示车次的位置
//    QRect CCWEIBURect; //车次停稳尾部矩形
//    CCWEIBURect.setRect(0,0,0,0);
//    //字体高度
//    int nHeightCC;
//    if((m_nCheciLost==false))
//    {
//        nHeightCC = 26*nDiploid*nCheciDiploid;
//    }
//    if((m_nCheciLost_xj==false))
//    {
//        nHeightCC = 26*nDiploid*nCheciDiploid;
//    }
//    //占用丢失放大2倍和1倍交替显示
//    if(m_nCheciLost==true)
//    {
//        //if(pDoc->bQJLJJCIsOnByQJ(m_strName))
//        {
//            if((nElapsed%2==1))
//            {
//                nHeightCC = 26*nDiploid*nCheciDiploid;
//            }
//            else
//            {
//                nHeightCC = 26*nDiploid*nCheciDiploid*2;
//            }
//        }
//    }
//    if(m_nCheciLost_xj==true)
//    {
//       // if(pDoc->bQJLJJCIsOnByQJ(m_strName))//检查该接发车口的区间逻辑检查功能是否开启
//        {
//            if((nElapsed%2==1))
//            {
//                nHeightCC = 26*nDiploid*nCheciDiploid;
//            }
//            else
//            {
//                nHeightCC = 26*nDiploid*nCheciDiploid*2;
//            }
//        }
//    }
//    m_strCheCiNum = "1234";
//    if(m_strCheCiNum != "" || m_strCheCiNumCTC != "")
//    {
////        QPen pen;
////        pen.setWidth(6);
////        pen.setColor(Qt::red);
////        painter->setPen(pen);
////        if(m_strCheCiNum != "")
////        {
////            painter->drawText(m_rectCheCi,m_strCheCiNum);
////        }
////        else if(m_strCheCiNumCTC != "")
////        {
////            painter->drawText(m_rectCheCi,m_strCheCiNumCTC);
////        }

//        //接近区段则将车次号放股道上方
//        if(m_nGDType == JJ_QD)
//        {
//            if(!bCCKIsUp)
//            {
//                //在下方
//                l_checiRect.setRect(pCenter.x() - 20,
//                    pCenter.y() + 5,
//                    40,
//                    20);
//            }
//            else
//            {
//                //在上方
//                l_checiRect.setRect(pCenter.x() - 20,
//                    pCenter.y() - 25,
//                    40,
//                    20);
//            }
//        }
//        else
//        {
//            l_checiRect.setRect(pCenter.x() - 20,
//                pCenter.y() - 10,
//                40,
//                20);
//        }

////        if((m_bCheCiFlash == TRUE) && (nElapsed%2 == 0))
////        {
////            ;//编辑状态下的闪烁效果
////        }
////        else
//        {
//            //如果实际有车次则显示连锁车次
//            if(m_strCheCiNum != "")
//            {
//                //如果实际有车则将人工添加的车次置删除
//                m_strCheCiNumCTC = "";

//                QPoint arrayArrow[4]; //方向箭头
//                //车次左行右行 0 左行 1右行
//                if(0 == m_nSXCheCi)
//                {
//                    arrayArrow[0] = QPoint(l_checiRect.left() - (l_checiRect.bottom()-l_checiRect.top())/2,
//                                           l_checiRect.top()+1 + (l_checiRect.bottom()-l_checiRect.top())/2);
//                    arrayArrow[1] = QPoint(l_checiRect.left(),
//                        l_checiRect.top()+1);
//                    arrayArrow[2] = QPoint(l_checiRect.left(),
//                        l_checiRect.bottom());
//                    arrayArrow[3] = arrayArrow[0];
//                    //CCWEIBURect.setRect(l_checiRect.right(),l_checiRect.top(),l_checiRect.right()+nWbWidth,l_checiRect.bottom());
//                    CCWEIBURect.setRect(l_checiRect.right(),l_checiRect.top(),nWbWidth,(l_checiRect.bottom()-l_checiRect.top()+1));
//                }
//                else
//                {
//                    arrayArrow[0] = QPoint(l_checiRect.right() + (l_checiRect.bottom()-l_checiRect.top())/2,
//                        l_checiRect.top()+1 + (l_checiRect.bottom()-l_checiRect.top())/2);
//                    arrayArrow[1] = QPoint(l_checiRect.right(),
//                        l_checiRect.top()+1);
//                    arrayArrow[2] = QPoint(l_checiRect.right(),
//                        l_checiRect.bottom());
//                    arrayArrow[3] = arrayArrow[0];
//                    CCWEIBURect.setRect(l_checiRect.left()-nWbWidth,l_checiRect.top(),nWbWidth,(l_checiRect.bottom()-l_checiRect.top()+1));
//                }

//                //绘制车次框
//                QPen pen_CHECI;
//                QBrush brush_CHECI;
//                pen_CHECI.setWidth(1);
//                if(m_bElectric)
//                {
//                    brush_CHECI.setColor(Qt::green);
//                    pen_CHECI.setColor(Qt::white);
//                }
//                else
//                {
//                    brush_CHECI.setColor(Qt::white);
//                    pen_CHECI.setColor(Qt::white);
//                }
//                painter->setPen(pen_CHECI);
//                painter->setBrush(brush_CHECI);
//                painter->drawRect(l_checiRect);
//                //绘制车次方向箭头
//                painter->drawPolygon(&arrayArrow[0], 4);
//                //绘制车次号
//                if(m_nKHType == LCTYPE_KC)
//                {
//                    pen_CHECI.setColor(Qt::red);//客车红
//                    painter->setPen(pen_CHECI);
//                }
//                else
//                {
//                    pen_CHECI.setColor(Qt::blue);//货车蓝
//                    painter->setPen(pen_CHECI);
//                }
//                if(m_strCheCiNum != "")
//                {
//                    painter->drawText(l_checiRect,m_strCheCiNum);
//                }


//                //列车停稳时，绘制车次号尾部黄色方框
//                //m_bLCTW = true;
//                if(m_nGDType != JJ_QD && true == m_bLCTW)
//                {
//                    QPen pen_TW;
//                    pen_TW.setWidth(1);
//                    pen_TW.setColor(Qt::yellow);
//                    painter->setPen(pen_TW);
//                    QBrush brush_TW;
//                    brush_TW.setColor(Qt::yellow);
//                    painter->setBrush(brush_TW);
//                    painter->drawRect(CCWEIBURect);
//                    //painter->setBrush(Qt::NoBrush);//设置背景为透明
//                }

//            }
//            //如果实际无车次则显示自己添加的车次
//            else if(m_strCheCiNumCTC != "")
//            {
//                QPen pen_CHECI;
//                pen_CHECI.setWidth(1);
//                pen_CHECI.setColor(Qt::white);
//                painter->setPen(pen_CHECI);
//                QBrush brush_CHECI;
//                if(m_bElectric)
//                {
//                    brush_CHECI.setColor(Qt::green);
//                }
//                else
//                {
//                    brush_CHECI.setColor(Qt::white);
//                }
//                painter->setBrush(brush_CHECI);
//                painter->drawRect(l_checiRect);
//                //painter->setBrush(Qt::NoBrush);//设置背景为透明
//                if(m_nKHType == LCTYPE_KC)
//                {
//                    pen_CHECI.setColor(Qt::red);//客车红
//                    painter->setPen(pen_CHECI);
//                }
//                else
//                {
//                    pen_CHECI.setColor(Qt::blue);//货车蓝
//                    painter->setPen(pen_CHECI);
//                }
//                //绘制车次号
//                if(m_strCheCiNumCTC != "")
//                {
//                    painter->drawText(l_checiRect,m_strCheCiNumCTC);
//                }
////                //绘制车次方向箭头
////                painter->drawPolygon(&arrayArrow[0], 4);
//            }
//        }
//    }
//}



void CGD::setDevStateToSafe()
{
    gdColor=Qt::red;          //轨道区段颜色
}
unsigned int CGD::getDevType()
{
    return Dev_GD;
}
int CGD::moveCursor(QPoint p)
{
    QPoint pt;
    //    if(getisNoShow()==true)
    //    {
    //        return 0;
    //    }
    pt.setX(p.x());
    pt.setY(p.y()-Off_Global);
    if (m_textRect.contains(pt))
    {
        return 1;
    }
    return 0;
}
void CGD::setVollover(QPoint pt_Base)//设置翻转上下行咽喉翻转
{
    double x1=0;
    double x2=0;
    double y1=0;
    double y2=0;
    double y3=0;
    setTextRect(QRectF(pt_Base.x() - (getTextRect().right()-pt_Base.x()),pt_Base.y() - (getTextRect().bottom()-pt_Base.y())/*-20*/,getTextRect().width(),getTextRect().height()));
    x1=getp1().x();
    x2=getp2().x();
    y1=getp1().y();
    y2=getp2().y();
    y3=getp12().y();
    setp1(QPointF(pt_Base.x() - (x2-pt_Base.x()),pt_Base.y() - (y3 - pt_Base.y())-(((y2-y1)+1)/2)));
    setp2(QPointF(pt_Base.x() - (x1-pt_Base.x()),pt_Base.y() - (y3 - pt_Base.y())+(((y2-y1)+1)/2)));
    x1=getp3().x();
    x2=getp4().x();
    y1=getp3().y();
    y2=getp4().y();
    y3=getp34().y();
    setp3(QPointF(pt_Base.x() - (x2-pt_Base.x()),pt_Base.y() - (y3 - pt_Base.y())-(((y2-y1)+1)/2)));
    setp4(QPointF(pt_Base.x() - (x1-pt_Base.x()),pt_Base.y() - (y3 - pt_Base.y())+(((y2-y1)+1)/2)));

    setp12(QPointF(pt_Base.x() - (getp12().x()-pt_Base.x()),pt_Base.y() - (getp12().y() - pt_Base.y())));
    setp34(QPointF(pt_Base.x() - (getp34().x()-pt_Base.x()),pt_Base.y() - (getp34().y() - pt_Base.y())));
    setpz12(QPointF(pt_Base.x() - (getpz12().x()-pt_Base.x()),pt_Base.y() - (getpz12().y() - pt_Base.y())));
    setpz34(QPointF(pt_Base.x() - (getpz34().x()-pt_Base.x()),pt_Base.y() - (getpz34().y() - pt_Base.y())));
    setCenterPt(QPointF(pt_Base.x() - (getCenterPt().x()-pt_Base.x()),pt_Base.y() - (getCenterPt().y() - pt_Base.y())));
    isVollover = true;
}
//绘制车次窗

//void CGD::gd_StatePro()
//{
//    if((true == getIsQDZY()) || true == getIsQDGZ())
//    {
//        setQDColor("RED");
//    }
//    else
//    {
//        setQDColor("BLUE");
//    }
//}
//成员变量封装函数实现
void CGD::setGDType(QString str_type)
{
    if(str_type == "GD_QD")
    {
        m_nGDType = GD_QD;
    }
    else if(str_type == "JJ_QD")
    {
        m_nGDType = JJ_QD;
    }
    else if(str_type == "QD")
    {
        m_nGDType = QD;
    }
}
unsigned int CGD::getGDType()
{
    return m_nGDType;
}
void CGD::setZ(unsigned int nZ)
{
    m_nZ = nZ;
}
unsigned int CGD::getZ()
{
    return m_nZ;
}
//void CGD::setLostTraNum(bool nFlag)
//{
//    isLostTrainNum = nFlag;
//}
//bool CGD::getLostTraNum()
//{
//    return isLostTrainNum;
//}
void CGD::setLock(bool nFlag)
{
    isLock = nFlag;
}
bool CGD::getLock()
{
    return isLock;
}
void CGD::setGDFLBL(bool nFlag)
{
    isGDFLBL = nFlag;
}
bool CGD::getFS()
{
    return isFS;
}
void CGD::setFS(bool nFlag)
{
    isFS = nFlag;
}
bool CGD::getGDFLBL()
{
    return isGDFLBL;
}
void CGD::setGDFLBL_QJLJJC(bool nFlag)
{
    isGDFLBL_xjFlag = nFlag;
}
bool CGD::getGDFLBL_QJLJJC()
{
    return isGDFLBL_xjFlag;
}
void CGD::setp1(QPointF pt)
{
    p1=pt;
    p1Const=pt;
}
QPointF CGD::getp1()
{
    return p1Const;
}
void CGD::setp2(QPointF pt)
{
    p2=pt;
    p2Const=pt;
}
QPointF CGD::getp2()
{
    return p2Const;
}
void CGD::setp3(QPointF pt)
{
    p3=pt;
    p3Const=pt;
}
QPointF CGD::getp3()
{
    return p3Const;
}
void CGD::setp4(QPointF pt)
{
    p4=pt;
    p4Const=pt;
}
QPointF CGD::getp4()
{
    return p4Const;
}
void CGD::setp12(QPointF pt)
{
    p12=pt;
    p12Const=pt;
}
QPointF CGD::getp12()
{
    return p12Const;
}
void CGD::setp34(QPointF pt)
{
    p34=pt;
    p34Const=pt;
}
QPointF CGD::getp34()
{
    return p34Const;
}
void CGD::setpz12(QPointF pt)
{
    pz12=pt;
    pz12Const=pt;
}
QPointF CGD::getpz12()
{
    return pz12Const;
}
void CGD::setpz34(QPointF pt)
{
    pz34=pt;
    pz34Const=pt;
}
QPointF CGD::getpz34()
{
    return pz34Const;
}
//void CGD::setGLB_QDleft(int glb)
//{
//    GLB_QDleft = glb;
//}
//int CGD::getGLB_QDleft()
//{
//    return GLB_QDleft;
//}
//void CGD::setGLB_QDright(int glb)
//{
//    GLB_QDright=glb;
//}
//int CGD::getGLB_QDright()
//{
//    return GLB_QDright;
//}
//void CGD::setIsSXDC(bool flag)
//{
//    isSXDC=flag;
//}
//bool CGD::getIsSXDC()
//{
//    return isSXDC;
//}
void CGD::setIsZXGD(bool flag)
{
    isZXGD=flag;
}
bool CGD::getIsZXGD()
{
    return isZXGD;
}
void CGD::setIsCXGD(bool flag)
{
    isCXGD=flag;
}
bool CGD::getIsCXGD()
{
    return isZXGD;
}
//void CGD::setIsJJGD(bool flag)
//{
//    isJJGD=flag;
//}
//bool CGD::getIsJJGD()
//{
//    return isJJGD;
//}
//void CGD::setGD_LCode(int code)
//{
//    GD_LCode=code;
//}
//int CGD::getGD_LCode()
//{
//    return GD_LCode;
//}
//void CGD::setGD_RCode(int code)
//{
//    GD_RCode=code;
//}
//int CGD::getGD_RCode()
//{
//    return GD_RCode;
//}
//void CGD::setCarrierFrequency(QString str)
//{
//    CarrierFrequency=str;
//}
//QString CGD::getCarrierFrequency()
//{
//    return CarrierFrequency;
//}
//void CGD::setDir_DMH(bool flag)
//{
//    if(flag == 1)
//    {
//        Dir_DMH=LEFT;
//    }
//    else
//    {
//        Dir_DMH=RIGHT;
//    }
//}
//int CGD::getDir_DMH()
//{
//    return Dir_DMH;
//}
//void CGD::setCodeFHXH(int fhxh)
//{
//    m_nCodeFHXH=fhxh;
//}
//int CGD::getCodeFHXH()
//{
//    return m_nCodeFHXH;
//}
//void CGD::setisNoShow(bool flag)
//{
//    isNoShow=flag;
//}
//bool CGD::getisNoShow()
//{
//    return isNoShow;
//}
void CGD::setMarkStr1(QString str)
{
    m_strMark1=str;
}
QString CGD::getMarkStr1()
{
    return m_strMark1;
}
void CGD::setMarkStr2(QString str)
{
    m_strMark2=str;
}
QString CGD::getMarkStr2()
{
    return m_strMark2;
}
void CGD::setSpeedLimit(bool nFlag)
{
    isSpeedLimit=nFlag;
}
bool CGD::getSpeedLimit()
{
    return isSpeedLimit;
}
void CGD::setPowerCut(bool nFlag)
{
    isPowerCut=nFlag;
}
bool CGD::getPowerCut()
{
    return isPowerCut;
}
void CGD::setMA(bool nFalg)
{
    isMA=nFalg;
}
bool CGD::getMA()
{
    return  isMA;
}
void CGD::SetJyj(bool b)
{
    bShow_Jyj=b;
}
bool CGD::GetMousePoint(QPoint p)
{
    if(m_devRect.contains(p)||m_textRect.contains(p))
    {
        return true;
    }
    else
    {
        return false;
    }
}
QColor CGD::getLightState(int nType,int nElapsed)//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
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


//绘制闭塞区间低频发码
void CGD::DrawBSQJCode(
                        QPainter *painter,
                        long nElapsed,
                        double nDiploid,
                        QPoint offset
                       )
{
#pragma region //坐标变换 2017.10.17 dxx
    //放大缩小
    QRectF textRectForDraw;
    QPoint p1ForDraw;
    QPoint p2ForDraw;
    QPoint p3ForDraw;
    QPoint p4ForDraw;
    QPoint p12ForDraw;
    QPoint p34ForDraw;
    QPoint pz12ForDraw;
    QPoint pz34ForDraw;
    QPoint centerForDraw;

    {
        textRectForDraw.setLeft(m_textRect.left());
        textRectForDraw.setTop(m_textRect.top());
        textRectForDraw.setRight(m_textRect.right());
        textRectForDraw.setBottom(m_textRect.bottom());
        p1ForDraw.setX(p1.x());
        p1ForDraw.setY(p1.y());
        p2ForDraw.setX(p2.x());
        p2ForDraw.setY(p2.y());
        p3ForDraw.setX(p3.x());
        p3ForDraw.setY(p3.y());
        p4ForDraw.setX(p4.x());
        p4ForDraw.setY(p4.y());
        p12ForDraw.setX(p12.x());
        p12ForDraw.setY(p12.y());
        p34ForDraw.setX(p34.x());
        p34ForDraw.setY(p34.y());
        pz12ForDraw.setX(pz12.x());
        pz12ForDraw.setY(pz12.y());
        pz34ForDraw.setX(pz34.x());
        pz34ForDraw.setY(pz34.y());
        centerForDraw.setX(pCenter.x());
        centerForDraw.setY(pCenter.y());
    }
#pragma endregion //坐标变换 2017.10.17 dxx
    bool sxlorr = false;
    if(bStaSXLORR & SXLORR)
    {
        sxlorr = true;
    }
    else
    {
        sxlorr = false;
    }

    int nSize = 4;

    if(m_nGDType == JJ_QD)
    {
        //普铁显示区间信号机，高速客专CTC中才显示区间低频码
        if((1 == bQJxhdShow)/*||(3 == bQJxhdShow)*/)//状态3的情况暂时未做
        {
#pragma region //区间信号机
            if(m_strName.right(3) == "1LQ" || m_strName == "SJG" || m_strName == "SJJG" || m_strName == "XJJG")
            {
                //return;
            }
            else
            {
                QPoint p_11,p_12;
                QPoint p_21,p_22;
                QRect rect1, rect2;
                p_11 = QPoint(0,0);
                p_12 = QPoint(0,0);
                p_21 = QPoint(0,0);
                p_22 = QPoint(0,0);
                rect1 = QRect(0,0,0,0);
                rect2 = QRect(0,0,0,0);
                QPoint basePointForDraw;//绘制基准点
                basePointForDraw = QPoint (0,0);

                //站场为正常方向 S接近离去绘制向左箭头 站场为反方向 X接近离去绘制向左箭头
                if(((isZDQJXHDDirFlag == false)&&((m_strName.left(1) == "S" && sxlorr) || (m_strName.left(1) == "X" && !sxlorr)))
                    || ((isZDQJXHDDirFlag == true)&&(isZDQJXHDRight == false)))
                {
                    //朝右
                    basePointForDraw = QPoint(p12ForDraw.x(),p12ForDraw.y());//centerForDraw.x,centerForDraw.y
                    //basePointForDraw = QPoint(p34ForDraw.x(),p34ForDraw.y());//centerForDraw.x,centerForDraw.y

                    p_11 = QPoint(basePointForDraw.x(),basePointForDraw.y()-(8*nDiploid));
                    p_12 = QPoint(basePointForDraw.x(),basePointForDraw.y()-3*(8*nDiploid));
                    p_21 = QPoint(basePointForDraw.x(),basePointForDraw.y()-2*(8*nDiploid));
                    p_22 = QPoint(basePointForDraw.x()+(8*nDiploid),basePointForDraw.y()-2*(8*nDiploid));

                    rect1 = QRect(basePointForDraw.x()+(8*nDiploid),
                        basePointForDraw.y()-3*(8*nDiploid),
                        16*nDiploid,
                        16*nDiploid);

                    rect2 = QRect(basePointForDraw.x()+3*(8*nDiploid),
                        basePointForDraw.y()-3*(8*nDiploid),
                        16*nDiploid,
                        16*nDiploid);
                }
                //站场为正常方向 X接近离去绘制向右箭头 站场为反方向 S接近离去绘制向右箭头
                //else if((m_strName.Left(1) == "X" && pFrame->StaConfigInfo.bStaSXLORR) || (m_strName.Left(1) == "S" && !pFrame->StaConfigInfo.bStaSXLORR))
                //else if((m_strName.Left(1) == "X" && sxlorr) || (m_strName.Left(1) == "S" && !sxlorr))
                else if(((isZDQJXHDDirFlag == false)&&((m_strName.left(1) == "X" && sxlorr) || (m_strName.left(1) == "S" && !sxlorr)))
                    || ((isZDQJXHDDirFlag == true)&&(isZDQJXHDRight == true)))
                {
                    //朝左
                    basePointForDraw = QPoint(p34ForDraw.x(),p34ForDraw.y());//centerForDraw.x,centerForDraw.y
                    //basePointForDraw = QPoint(p12ForDraw.x(),p12ForDraw.y());//centerForDraw.x,centerForDraw.y

                    p_11 = QPoint(basePointForDraw.x(),basePointForDraw.y()+(8*nDiploid));
                    p_12 = QPoint(basePointForDraw.x(),basePointForDraw.y()+3*(8*nDiploid));
                    p_21 = QPoint(basePointForDraw.x(),basePointForDraw.y()+2*(8*nDiploid));
                    p_22 = QPoint(basePointForDraw.x()-(8*nDiploid),basePointForDraw.y()+2*(8*nDiploid));

                    rect1 = QRect(basePointForDraw.x()-3*(8*nDiploid),
                        basePointForDraw.y()+1*(8*nDiploid),
                        16*nDiploid,
                        16*nDiploid);

                    rect2 = QRect(basePointForDraw.x()-5*(8*nDiploid),
                        basePointForDraw.y()+1*(8*nDiploid),
                        16*nDiploid,
                        16*nDiploid);
                }

                //绘制灯柱
                QBrush newBrushBK;
                QPen pen; //画笔
                pen.setColor(BLUEKX);
                newBrushBK.setStyle(Qt::SolidPattern);
                //newBrushBK.setStyle(Qt::NoBrush);
                newBrushBK.setColor(BLUEKX);
                painter->setPen(pen);
                painter->setPen(QPen(SkyBlue, 2));
                painter->setBrush(newBrushBK);

                painter->drawLine(p_11,p_12);
                painter->drawLine(p_21,p_22);

                //反走样,防止出现锯齿状线条
                painter->setRenderHint(QPainter::Antialiasing, true);
                painter->drawEllipse(rect1);
                painter->drawEllipse(rect2);
                //反走样,防止出现锯齿状线条


                //开始绘制
                if(gIsConnectInterlock)
                {
                    switch(m_nBSQDFMCode)
                    {
                    case BSQDFM_H:
                        {
                            newBrushBK.setColor(getLightState(0,nElapsed));//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawEllipse(rect1);

                            newBrushBK.setColor(getLightState(1,nElapsed));//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawEllipse(rect2);
                        }
                        break;

                    case BSQDFM_ZYJC:
                        {
                            newBrushBK.setColor(getLightState(10,nElapsed));//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawEllipse(rect1);

                            newBrushBK.setColor(getLightState(10,nElapsed));//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawEllipse(rect2);
                        }
                        break;
                    case BSQDFM_HU:
                        {
                            newBrushBK.setColor(getLightState(1,nElapsed));//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawEllipse(rect1);

                            newBrushBK.setColor(getLightState(4,nElapsed));//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawEllipse(rect2);
                        }
                        break;

                    case BSQDFM_ZPQH:
                        {
                            newBrushBK.setColor(getLightState(10,nElapsed));//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawEllipse(rect1);

                            newBrushBK.setColor(getLightState(10,nElapsed));//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawEllipse(rect2);
                        }
                        break;
                    case BSQDFM_HB:
                        {
                            newBrushBK.setColor(getLightState(1,nElapsed));//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawEllipse(rect1);

                            newBrushBK.setColor(getLightState(3,nElapsed));//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawEllipse(rect2);
                        }
                        break;
                    case BSQDFM_L4:
                        {
                            newBrushBK.setColor(getLightState(0,nElapsed));//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawEllipse(rect1);

                            newBrushBK.setColor(getLightState(2,nElapsed));//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawEllipse(rect2);
                        }
                        break;
                    case BSQDFM_L6:
                        {
                            newBrushBK.setColor(getLightState(0,nElapsed));//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawEllipse(rect1);

                            newBrushBK.setColor(getLightState(2,nElapsed));//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawEllipse(rect2);
                        }
                        break;
                    case BSQDFM_L5:
                        {
                            newBrushBK.setColor(getLightState(0,nElapsed));//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawEllipse(rect1);

                            newBrushBK.setColor(getLightState(2,nElapsed));//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawEllipse(rect2);
                        }
                        break;
                    case BSQDFM_U2S:
                        {
                            newBrushBK.setColor(getLightState(0,nElapsed));//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawEllipse(rect1);

                            newBrushBK.setColor(getLightState(8,nElapsed));//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawEllipse(rect2);
                        }
                        break;
                    case BSQDFM_USU:
                        {
                            newBrushBK.setColor(getLightState(8,nElapsed));//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawEllipse(rect1);

                            newBrushBK.setColor(getLightState(4,nElapsed));//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawEllipse(rect2);
                        }
                        break;
                    case BSQDFM_UU:
                        {
                            newBrushBK.setColor(getLightState(4,nElapsed));//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawEllipse(rect1);

                            newBrushBK.setColor(getLightState(4,nElapsed));//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawEllipse(rect2);
                        }
                        break;
                    case BSQDFM_U:
                        {
                            newBrushBK.setColor(getLightState(0,nElapsed));//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawEllipse(rect1);

                            newBrushBK.setColor(getLightState(4,nElapsed));//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawEllipse(rect2);
                        }
                        break;
                    case BSQDFM_LU2:
                        {
                            newBrushBK.setColor(getLightState(2,nElapsed));//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawEllipse(rect1);

                            newBrushBK.setColor(getLightState(4,nElapsed));//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawEllipse(rect2);
                        }
                        break;
                    case BSQDFM_U2:
                        {
                            newBrushBK.setColor(getLightState(4,nElapsed));//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawEllipse(rect1);

                            newBrushBK.setColor(getLightState(4,nElapsed));//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawEllipse(rect2);
                        }
                        break;
                    case BSQDFM_LU:
                        {
                            newBrushBK.setColor(getLightState(2,nElapsed));//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawEllipse(rect1);

                            newBrushBK.setColor(getLightState(4,nElapsed));//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawEllipse(rect2);
                        }
                        break;
                    case BSQDFM_L2:
                        {
                            newBrushBK.setColor(getLightState(0,nElapsed));//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawEllipse(rect1);

                            newBrushBK.setColor(getLightState(2,nElapsed));//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawEllipse(rect2);
                        }
                        break;
                    case BSQDFM_L:
                        {
                            newBrushBK.setColor(getLightState(0,nElapsed));//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawEllipse(rect1);

                            newBrushBK.setColor(getLightState(2,nElapsed));//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawEllipse(rect2);
                        }
                        break;
                    case BSQDFM_L3:
                        {
                            newBrushBK.setColor(getLightState(0,nElapsed));//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawEllipse(rect1);

                            newBrushBK.setColor(getLightState(2,nElapsed));//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawEllipse(rect2);
                        }
                        break;

                    default:
                        newBrushBK.setColor(getLightState(0,nElapsed));//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                        painter->setBrush(newBrushBK);
                        painter->drawEllipse(rect1);

                        newBrushBK.setColor(getLightState(0,nElapsed));//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                        painter->setBrush(newBrushBK);
                        painter->drawEllipse(rect2);
                        break;
                    }
                }
                else
                {
                    newBrushBK.setColor(getLightState(0,nElapsed));//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                    painter->setBrush(newBrushBK);
                    painter->drawEllipse(rect1);

                    newBrushBK.setColor(getLightState(0,nElapsed));//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                    painter->setBrush(newBrushBK);
                    painter->drawEllipse(rect2);
                }
                painter->setRenderHint(QPainter::Antialiasing, false);
            }
#pragma endregion //区间信号机
        }
        if((2 == bQJxhdShow)/*||(3 == bQJxhdShow)*/)//状态3的情况暂时未做
        {
#pragma region //低频码
            if((m_strName == "SJG" || m_strName == "SJJG" || m_strName == "XJJG"))
            {
                //return;
            }
            else
            {
                QBrush newBrushBK;
                QPen pen; //画笔
                pen.setColor(WHITE);
                newBrushBK.setStyle(Qt::SolidPattern);
                //newBrushBK.setStyle(Qt::NoBrush);
                newBrushBK.setColor(BLUEKX);
                painter->setPen(pen);
                painter->setBrush(newBrushBK);

                QPoint arrayArrowSmall[4] = {QPoint(0,0), QPoint(0,0), QPoint(0,0), QPoint(0,0)};
                QPoint arrayTXSmall[5] = {QPoint(0,0), QPoint(0,0), QPoint(0,0), QPoint(0,0), QPoint(0,0)};
                QPoint arrayArrowBig[4] = {QPoint(0,0), QPoint(0,0), QPoint(0,0), QPoint(0,0)};

                bool bSXLorrForDraw = bStaSXLORR;

                //站场为正常方向 S接近离去绘制向左箭头 站场为反方向 X接近离去绘制向左箭头
                if((m_strName.left(1) == "S" && bSXLorrForDraw) || (m_strName.left(1) == "X" && !bSXLorrForDraw))
                {
                    arrayArrowSmall[0] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2) - nSize*2*nDiploid, p12ForDraw.y());
                    arrayArrowSmall[1] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2), p12ForDraw.y() - nSize*nDiploid);
                    arrayArrowSmall[2] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2), p12ForDraw.y() + nSize*nDiploid);
                    arrayArrowSmall[3] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2) - nSize*2*nDiploid,p12ForDraw.y());

                    arrayTXSmall[0] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2),p12ForDraw.y() - nSize*nDiploid);
                    arrayTXSmall[1] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2) + nSize*2*nDiploid,p12ForDraw.y() - nSize*2*nDiploid);
                    arrayTXSmall[2] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2) + nSize*2*nDiploid,p12ForDraw.y() + nSize*2*nDiploid);
                    arrayTXSmall[3] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2),p12ForDraw.y() + nSize*nDiploid);
                    arrayTXSmall[4] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2),p12ForDraw.y() - nSize*nDiploid);

                    arrayArrowBig[0] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2) - nSize*2*nDiploid,p12ForDraw.y());
                    arrayArrowBig[1] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2) + nSize*2*nDiploid,p12ForDraw.y() - nSize*2*nDiploid);
                    arrayArrowBig[2] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2) + nSize*2*nDiploid,p12ForDraw.y() + nSize*2*nDiploid);
                    arrayArrowBig[3] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2) - nSize*2*nDiploid,p12ForDraw.y());
                }

                //站场为正常方向 X接近离去绘制向右箭头 站场为反方向 S接近离去绘制向右箭头
                else if((m_strName.left(1) == "X" && bSXLorrForDraw) || (m_strName.left(1) == "S" && !bSXLorrForDraw))
                {
                    arrayArrowSmall[0] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2) + nSize*2*nDiploid,p12ForDraw.y());
                    arrayArrowSmall[1] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2),p12ForDraw.y()-nSize*nDiploid);
                    arrayArrowSmall[2] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2),p12ForDraw.y()+nSize*nDiploid);
                    arrayArrowSmall[3] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2) + nSize*2*nDiploid,p12ForDraw.y());

                    arrayTXSmall[0] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2),p12ForDraw.y()-nSize*nDiploid);
                    arrayTXSmall[1] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2) - nSize*2*nDiploid,p12ForDraw.y() - nSize*2*nDiploid);
                    arrayTXSmall[2] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2) - nSize*2*nDiploid,p12ForDraw.y() + nSize*2*nDiploid);
                    arrayTXSmall[3] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2),p12ForDraw.y()+nSize*nDiploid);
                    arrayTXSmall[4] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2),p12ForDraw.y()-nSize*nDiploid);

                    arrayArrowBig[0] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2) + nSize*2*nDiploid,p12ForDraw.y());
                    arrayArrowBig[1] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2) - nSize*2*nDiploid,p12ForDraw.y() - nSize*2*nDiploid);
                    arrayArrowBig[2] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2) - nSize*2*nDiploid,p12ForDraw.y() + nSize*2*nDiploid);
                    arrayArrowBig[3] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2) + nSize*2*nDiploid,p12ForDraw.y());
                }

                //箭头方向（改方前后都由联锁传过来）
                if((gIsConnectInterlock == true) && (m_bBSQDFMRight == true))//向右箭头
                {
                    arrayArrowSmall[0] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2) + nSize*2*nDiploid,p12ForDraw.y());
                    arrayArrowSmall[1] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2),p12ForDraw.y()-nSize*nDiploid);
                    arrayArrowSmall[2] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2),p12ForDraw.y()+nSize*nDiploid);
                    arrayArrowSmall[3] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2) + nSize*2*nDiploid,p12ForDraw.y());

                    arrayTXSmall[0] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2),p12ForDraw.y()-nSize*nDiploid);
                    arrayTXSmall[1] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2) - nSize*2*nDiploid,p12ForDraw.y() - nSize*2*nDiploid);
                    arrayTXSmall[2] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2) - nSize*2*nDiploid,p12ForDraw.y() + nSize*2*nDiploid);
                    arrayTXSmall[3] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2),p12ForDraw.y()+nSize*nDiploid);
                    arrayTXSmall[4] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2),p12ForDraw.y()-nSize*nDiploid);

                    arrayArrowBig[0] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2) + nSize*2*nDiploid,p12ForDraw.y());
                    arrayArrowBig[1] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2) - nSize*2*nDiploid,p12ForDraw.y() - nSize*2*nDiploid);
                    arrayArrowBig[2] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2) - nSize*2*nDiploid,p12ForDraw.y() + nSize*2*nDiploid);
                    arrayArrowBig[3] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2) + nSize*2*nDiploid,p12ForDraw.y());
                }
                else if((gIsConnectInterlock == true) && (m_bBSQDFMRight == false)) //向左箭头
                {
                    arrayArrowSmall[0] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2) - nSize*2*nDiploid, p12ForDraw.y());
                    arrayArrowSmall[1] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2), p12ForDraw.y()-nSize*nDiploid);
                    arrayArrowSmall[2] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2),p12ForDraw.y()+nSize*nDiploid);
                    arrayArrowSmall[3] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2) - nSize*2*nDiploid,p12ForDraw.y());

                    arrayTXSmall[0] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2),p12ForDraw.y()-nSize*nDiploid);
                    arrayTXSmall[1] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2) + nSize*2*nDiploid,p12ForDraw.y() - nSize*2*nDiploid);
                    arrayTXSmall[2] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2) + nSize*2*nDiploid,p12ForDraw.y() + nSize*2*nDiploid);
                    arrayTXSmall[3] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2),p12ForDraw.y()+nSize*nDiploid);
                    arrayTXSmall[4] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2),p12ForDraw.y()-nSize*nDiploid);

                    arrayArrowBig[0] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2) - nSize*2*nDiploid,p12ForDraw.y());
                    arrayArrowBig[1] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2) + nSize*2*nDiploid,p12ForDraw.y() - nSize*2*nDiploid);
                    arrayArrowBig[2] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2) + nSize*2*nDiploid,p12ForDraw.y() + nSize*2*nDiploid);
                    arrayArrowBig[3] = QPoint((p12ForDraw.x() + p34ForDraw.x())/(2) - nSize*2*nDiploid,p12ForDraw.y());
                }

                //开始绘制
                if(gIsConnectInterlock)
                {
                    switch(m_nBSQDFMCode)
                    {

                    case BSQDFM_H:
                        {
                            newBrushBK.setColor(getLightState(1,nElapsed));//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawPolygon(arrayArrowBig,4);
                        }
                        break;

                    case BSQDFM_ZYJC:
                        {
                            newBrushBK.setColor(getLightState(10,nElapsed));//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawPolygon(arrayArrowBig,4);
                        }
                        break;
                    case BSQDFM_HU:
                        {
                            newBrushBK.setColor(getLightState(4,nElapsed));
                            painter->setBrush(newBrushBK);
                            painter->drawPolygon(arrayArrowSmall,4);
                            newBrushBK.setColor(getLightState(1,nElapsed));
                            painter->setBrush(newBrushBK);
                            painter->drawPolygon(arrayTXSmall,5);
                        }
                        break;

                    case BSQDFM_ZPQH:
                        {
                            newBrushBK.setColor(getLightState(10,nElapsed));//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawPolygon(arrayArrowBig,4);
                        }
                        break;
                    case BSQDFM_HB:
                        {
                            newBrushBK.setColor(getLightState(8,nElapsed));
                            painter->setBrush(newBrushBK);
                            painter->drawPolygon(arrayArrowSmall,4);
                            newBrushBK.setColor(getLightState(5,nElapsed));
                            painter->setBrush(newBrushBK);
                            painter->drawPolygon(arrayTXSmall,5);
                        }
                        break;
                    case BSQDFM_L4:
                        {
                            newBrushBK.setColor(getLightState(2,nElapsed));//（颜色：0黑,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawPolygon(arrayArrowBig,4);

                            QPen pen_m;
                            pen_m.setColor(getLightState(0,nElapsed));
                            QFont font_m;
                            font_m.setWeight(50);//字体宽度,0~99
                            font_m.setPointSize(8 * nDiploid);//字体高度,10表示高度为10个像素点
                            painter->setPen(pen_m);
                            painter->setFont(font_m);
                            QRectF rect_m;
                            if(m_bBSQDFMRight)
                            {
                                rect_m.setLeft(arrayArrowBig[0].x()-20 * nDiploid);
                                rect_m.setTop(arrayArrowBig[0].y()-8 * nDiploid);
                                rect_m.setRight(arrayArrowBig[0].x()+ 0 * nDiploid);
                                rect_m.setBottom(arrayArrowBig[0].y()+8 * nDiploid);
                            }
                            else
                            {
                                rect_m.setLeft(arrayArrowBig[0].x()+20 * nDiploid);
                                rect_m.setTop(arrayArrowBig[0].y()-5 * nDiploid);
                                rect_m.setRight(arrayArrowBig[0].x()+ 0 * nDiploid);
                                rect_m.setBottom(arrayArrowBig[0].y()+5 * nDiploid);
                            }
                            painter->drawText(rect_m,Qt::AlignHCenter|Qt::AlignVCenter,"4");
                        }
                        break;
                    case BSQDFM_L6:
                        {
                            newBrushBK.setColor(getLightState(2,nElapsed));//（颜色：0黑,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawPolygon(arrayArrowBig,4);

                            QPen pen_m;
                            pen_m.setColor(getLightState(0,nElapsed));
                            QFont font_m;
                            font_m.setWeight(50);//字体宽度,0~99
                            font_m.setPointSize(8 * nDiploid);//字体高度,10表示高度为10个像素点
                            painter->setPen(pen_m);
                            painter->setFont(font_m);
                            QRectF rect_m;
                            if(m_bBSQDFMRight)
                            {
                                rect_m.setLeft(arrayArrowBig[0].x()-20 * nDiploid);
                                rect_m.setTop(arrayArrowBig[0].y()-8 * nDiploid);
                                rect_m.setRight(arrayArrowBig[0].x()+ 0 * nDiploid);
                                rect_m.setBottom(arrayArrowBig[0].y()+8 * nDiploid);
                            }
                            else
                            {
                                rect_m.setLeft(arrayArrowBig[0].x()+20 * nDiploid);
                                rect_m.setTop(arrayArrowBig[0].y()-5 * nDiploid);
                                rect_m.setRight(arrayArrowBig[0].x()+ 0 * nDiploid);
                                rect_m.setBottom(arrayArrowBig[0].y()+5 * nDiploid);
                            }
                            painter->drawText(rect_m,Qt::AlignHCenter|Qt::AlignVCenter,"6");
                        }
                        break;
                    case BSQDFM_L5:
                        {
                            newBrushBK.setColor(getLightState(2,nElapsed));//（颜色：0黑,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawPolygon(arrayArrowBig,4);

                            QPen pen_m;
                            pen_m.setColor(getLightState(0,nElapsed));
                            QFont font_m;
                            font_m.setWeight(50);//字体宽度,0~99
                            font_m.setPointSize(8 * nDiploid);//字体高度,10表示高度为10个像素点
                            painter->setPen(pen_m);
                            painter->setFont(font_m);
                            QRectF rect_m;
                            if(m_bBSQDFMRight)
                            {
                                rect_m.setLeft(arrayArrowBig[0].x()-20 * nDiploid);
                                rect_m.setTop(arrayArrowBig[0].y()-8 * nDiploid);
                                rect_m.setRight(arrayArrowBig[0].x()+ 0 * nDiploid);
                                rect_m.setBottom(arrayArrowBig[0].y()+8 * nDiploid);
                            }
                            else
                            {
                                rect_m.setLeft(arrayArrowBig[0].x()+20 * nDiploid);
                                rect_m.setTop(arrayArrowBig[0].y()-5 * nDiploid);
                                rect_m.setRight(arrayArrowBig[0].x()+ 0 * nDiploid);
                                rect_m.setBottom(arrayArrowBig[0].y()+5 * nDiploid);
                            }
                            painter->drawText(rect_m,Qt::AlignHCenter|Qt::AlignVCenter,"5");
                        }
                        break;
                    case BSQDFM_U2S:
                        {
                            newBrushBK.setColor(getLightState(8,nElapsed));//（颜色：0黑,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawPolygon(arrayArrowBig,4);

                            QPen pen_m;
                            pen_m.setColor(getLightState(0,nElapsed));
                            QFont font_m;
                            font_m.setWeight(50);//字体宽度,0~99
                            font_m.setPointSize(8 * nDiploid);//字体高度,10表示高度为10个像素点
                            painter->setPen(pen_m);
                            painter->setFont(font_m);
                            QRectF rect_m;
                            if(m_bBSQDFMRight)
                            {
                                rect_m.setLeft(arrayArrowBig[0].x()-20 * nDiploid);
                                rect_m.setTop(arrayArrowBig[0].y()-8 * nDiploid);
                                rect_m.setRight(arrayArrowBig[0].x()+ 0 * nDiploid);
                                rect_m.setBottom(arrayArrowBig[0].y()+8 * nDiploid);
                            }
                            else
                            {
                                rect_m.setLeft(arrayArrowBig[0].x()+20 * nDiploid);
                                rect_m.setTop(arrayArrowBig[0].y()-5 * nDiploid);
                                rect_m.setRight(arrayArrowBig[0].x()+ 0 * nDiploid);
                                rect_m.setBottom(arrayArrowBig[0].y()+5 * nDiploid);
                            }
                            painter->drawText(rect_m,Qt::AlignHCenter|Qt::AlignVCenter,"2");
                        }
                        break;
                    case BSQDFM_USU:
                        {
                            newBrushBK.setColor(getLightState(4,nElapsed));//（颜色：0黑,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawPolygon(arrayArrowSmall,4);
                            newBrushBK.setColor(getLightState(8,nElapsed));//（颜色：0黑,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawPolygon(arrayTXSmall,5);
                        }
                        break;
                    case BSQDFM_UU:
                        {
                            newBrushBK.setColor(getLightState(4,nElapsed));//（颜色：0黑,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawPolygon(arrayArrowSmall,4);
                            newBrushBK.setColor(getLightState(4,nElapsed));//（颜色：0黑,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawPolygon(arrayTXSmall,5);
                        }
                        break;
                    case BSQDFM_U:
                        {
                            newBrushBK.setColor(getLightState(4,nElapsed));//（颜色：0黑,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawPolygon(arrayArrowBig,4);
                        }
                        break;
                    case BSQDFM_LU2:
                        {
                            newBrushBK.setColor(getLightState(4,nElapsed));//（颜色：0黑,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawPolygon(arrayArrowBig,4);
                        }
                        break;
                    case BSQDFM_U2:
                        {
                            newBrushBK.setColor(getLightState(4,nElapsed));//（颜色：0黑,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawPolygon(arrayArrowBig,4);
                        }
                        break;
                    case BSQDFM_LU:
                        {
                            newBrushBK.setColor(getLightState(2,nElapsed));//（颜色：0黑,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawPolygon(arrayArrowSmall,4);
                            newBrushBK.setColor(getLightState(4,nElapsed));//（颜色：0黑,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawPolygon(arrayTXSmall,5);
                        }
                        break;
                    case BSQDFM_L2:
                        {
                            newBrushBK.setColor(getLightState(2,nElapsed));//（颜色：0黑,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawPolygon(arrayArrowBig,4);

                            QPen pen_m;
                            pen_m.setColor(getLightState(0,nElapsed));
                            QFont font_m;
                            font_m.setWeight(50);//字体宽度,0~99
                            font_m.setPointSize(8 * nDiploid);//字体高度,10表示高度为10个像素点
                            painter->setPen(pen_m);
                            painter->setFont(font_m);
                            QRectF rect_m;
                            if(m_bBSQDFMRight)
                            {
                                rect_m.setLeft(arrayArrowBig[0].x()-20 * nDiploid);
                                rect_m.setTop(arrayArrowBig[0].y()-8 * nDiploid);
                                rect_m.setRight(arrayArrowBig[0].x()+ 0 * nDiploid);
                                rect_m.setBottom(arrayArrowBig[0].y()+8 * nDiploid);
                            }
                            else
                            {
                                rect_m.setLeft(arrayArrowBig[0].x()+20 * nDiploid);
                                rect_m.setTop(arrayArrowBig[0].y()-5 * nDiploid);
                                rect_m.setRight(arrayArrowBig[0].x()+ 0 * nDiploid);
                                rect_m.setBottom(arrayArrowBig[0].y()+5 * nDiploid);
                            }
                            painter->drawText(rect_m,Qt::AlignHCenter|Qt::AlignVCenter,"2");

    //                        newBrushBK.setColor(getLightState(4,nElapsed));//（颜色：0黑,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
    //                        painter->setBrush(newBrushBK);
    //                        painter->drawPolygon(arrayArrowSmall,4);
    //                        newBrushBK.setColor(getLightState(8,nElapsed));//（颜色：0黑,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
    //                        painter->setBrush(newBrushBK);
    //                        painter->drawPolygon(arrayTXSmall,5);
                        }
                        break;
                    case BSQDFM_L:
                        {
                            newBrushBK.setColor(getLightState(2,nElapsed));//（颜色：0黑,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawPolygon(arrayArrowBig,4);
                        }
                        break;
                    case BSQDFM_L3:
                        {
                            newBrushBK.setColor(getLightState(2,nElapsed));//（颜色：0黑,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝,10灰）
                            painter->setBrush(newBrushBK);
                            painter->drawPolygon(arrayArrowBig,4);

                            QPen pen_m;
                            pen_m.setColor(getLightState(0,nElapsed));
                            QFont font_m;
                            font_m.setWeight(50);//字体宽度,0~99
                            font_m.setPointSize(8 * nDiploid);//字体高度,10表示高度为10个像素点
                            painter->setPen(pen_m);
                            painter->setFont(font_m);
                            QRectF rect_m;
                            if(m_bBSQDFMRight)
                            {
                                rect_m.setLeft(arrayArrowBig[0].x()-20 * nDiploid);
                                rect_m.setTop(arrayArrowBig[0].y()-8 * nDiploid);
                                rect_m.setRight(arrayArrowBig[0].x()+ 0 * nDiploid);
                                rect_m.setBottom(arrayArrowBig[0].y()+8 * nDiploid);
                            }
                            else
                            {
                                rect_m.setLeft(arrayArrowBig[0].x()+20 * nDiploid);
                                rect_m.setTop(arrayArrowBig[0].y()-5 * nDiploid);
                                rect_m.setRight(arrayArrowBig[0].x()+ 0 * nDiploid);
                                rect_m.setBottom(arrayArrowBig[0].y()+5 * nDiploid);
                            }
                            painter->drawText(rect_m,Qt::AlignHCenter|Qt::AlignVCenter,"3");
                        }
                        break;

                    default:
                        newBrushBK.setColor(GRAY);
                        painter->setBrush(newBrushBK);
                        painter->drawPolygon(arrayArrowBig,4);
                        break;
                    }
                }
                else
                {
                    newBrushBK.setColor(GRAY);
                    painter->setBrush(newBrushBK);
                    painter->drawPolygon(arrayArrowBig,4);
                }
            }

#pragma endregion  //低频码
        }
    }
}

//赋值低频发码
void CGD::SetBSQDFMCode(int nQDFMCode)
{
    m_nBSQDFMCode = nQDFMCode;
}
//股道防溜属性初始化
void CGD::IntiSlip()
{
    bIronShoesSetS=false;//上行（东部）铁鞋设置
    bIronShoesSetX=false;//下行（西部）铁鞋设置
    nManualBrakeS=0;//上行（东部） 人力制动机 or 人力制动机紧固器 0:两者皆无 1：人力制动机 2：人力制动机紧固器
    nManualBrakeX=0;
    m_nLAntiSlipType = 0;//左侧防溜类型（接车口）
    m_nLTxNum=0;//左侧铁鞋号
    m_nLJgqNum = 0;//左侧紧固器号
    m_nLJnMeters = 0;//左侧警内米数
    m_nRAntiSlipType = 0;//右侧防溜类型（发车口）
    m_nRTxNum=0;//右侧铁鞋号
    m_nRJgqNum = 0;//右侧紧固器号
    m_nRJnMeters = 0;//右侧警内米数
    m_nTrainNums = 0;//存车信息
    m_sTrainInfoShow="";//存车信息 股道栏显示
    m_sTrainInfoFL="";
}
bool CGD::GetTrainInfoFLShow()
{
    bool bShow=false;
    bShow=bIronShoesSetS|bIronShoesSetX;
    bShow=bShow|(nManualBrakeS>0)|(nManualBrakeX>0);
    return bShow;
}
