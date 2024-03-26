#include "cl.h"

cl::cl()
{
    m_nGLXH=0xffff;
    isNX=0;
    LEFTArrowPt=QPoint(0,0);
    LEFTArrowPtConst=QPoint(0,0);
    RIGHTArrowPt=QPoint(0,0);
    RIGHTArrowPtConst=QPoint(0,0);
    p_BSD_AFJ=QPoint(0,0); //允许发车灯
    p_BSD_AFJConst=QPoint(0,0); //允许发车灯
    p_BSD_BFJ=QPoint(0,0);  //辅助表示灯
    p_BSD_BFJConst=QPoint(0,0);  //辅助表示灯
    p_BSD_CFJ=QPoint(0,0);  //闪光表示灯
    p_BSD_CFJConst=QPoint(0,0);  //闪光表示灯
    p_BSD_JGJ=QPoint(0,0);  //区间表示灯
    p_BSD_JGJConst=QPoint(0,0);  //区间表示灯
    rect_FK=QRect(0,0,0,0);
    rect_FKConst=QRect(0,0,0,0);
    isBSDLight_AFJ=true;//false;
    isBSDLight_BFJ=true;//false;
    isBSDLight_CFJ=true;//false;
    isBSDLight_JGJ=true;//false;
    m_nArrowStateJC=Arrow_Black;
    m_nArrowStateFC=Arrow_Black;

    AFJ_Name = "";
    BFJ_Name = "";
    CFJ_Name = "";
    JGJ_Name = "";
    ArrowLeftName = "";
    ArrowRightName = "";

    haveAFJ = true;
    haveBFJ = true;
    haveCFJ = true;
    haveJGJ = true;

    haveFK = false;
    ptFK=QPoint(0,0);
    FK_length = 0;
    FK_breath = 0;

    haveWord = false;
    ptWord = QPoint(0,0);
    WordName = "";

    m_RangeVisible = false;

    AFJColorType = 1;
    BFJColorType = 1;
    CFJColorType = 1;
    JGJColorType = 1;

    AFJLightForm=0;//灯的形状:0为普通圆灯位,1为朝左单灯,2为朝右单灯,3为朝左双灯,4为朝右双灯
    BFJLightForm=0;//灯的形状:0为普通圆灯位,1为朝左单灯,2为朝右单灯,3为朝左双灯,4为朝右双灯
    CFJLightForm=0;//灯的形状:0为普通圆灯位,1为朝左单灯,2为朝右单灯,3为朝左双灯,4为朝右双灯
    JGJLightForm=0;//灯的形状:0为普通圆灯位,1为朝左单灯,2为朝右单灯,3为朝左双灯,4为朝右双灯

    AFJTextPos=0;//文字的位置:0为灯下,1为灯上
    BFJTextPos=0;//文字的位置:0为灯下,1为灯上
    CFJTextPos=0;//文字的位置:0为灯下,1为灯上
    JGJTextPos=0;//文字的位置:0为灯下,1为灯上
}
void cl::Draw_ToolTip(QPainter *painter, double nDiploid)
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

void cl::Draw(QPainter *painter,long nElapsed, double nDiploid,QPoint offset,int type)
{
    if(type==0x55)
    {
        Draw_Th_kb(painter,nElapsed, nDiploid,offset, false);
    }
}
void cl::DrawM(QPainter *painter,long nElapsed, double nDiploid,QPoint offset,int type,int flag)
{
    if(type==0x55)
    {
        Draw_Th_kb(painter,nElapsed, nDiploid,offset, true);
    }
}

void cl::Draw_Th_kb(QPainter *painter,long nElapsed, double nDiploid,QPoint offset, bool isMulti)
{
    QFont font;
    LEFTArrowPt.setX(LEFTArrowPtConst.x() * nDiploid+offset.x()*nDiploid);
    LEFTArrowPt.setY(LEFTArrowPtConst.y() * nDiploid+offset.y()*nDiploid);
    RIGHTArrowPt.setX(RIGHTArrowPtConst.x() * nDiploid+offset.x()*nDiploid);
    RIGHTArrowPt.setY(RIGHTArrowPtConst.y() * nDiploid+offset.y()*nDiploid);

    p_BSD_AFJ.setX(p_BSD_AFJConst.x()  * nDiploid+offset.x()*nDiploid);
    p_BSD_AFJ.setY(p_BSD_AFJConst.y()  * nDiploid+offset.y()*nDiploid);
    p_BSD_BFJ.setX(p_BSD_BFJConst.x()  * nDiploid+offset.x()*nDiploid);
    p_BSD_BFJ.setY(p_BSD_BFJConst.y()  * nDiploid+offset.y()*nDiploid);
    p_BSD_CFJ.setX(p_BSD_CFJConst.x()  * nDiploid+offset.x()*nDiploid);
    p_BSD_CFJ.setY(p_BSD_CFJConst.y()  * nDiploid+offset.y()*nDiploid);
    p_BSD_JGJ.setX(p_BSD_JGJConst.x()  * nDiploid+offset.x()*nDiploid);
    p_BSD_JGJ.setY(p_BSD_JGJConst.y()  * nDiploid+offset.y()*nDiploid);

//    rect_FK.setRect(rect_FKConst.topLeft().x() * nDiploid + offset.x() * nDiploid,rect_FKConst.topLeft().y() * nDiploid + offset.y() * nDiploid,rect_FKConst.width()*nDiploid,rect_FKConst.height()*nDiploid);
    rect_FK.setRect(ptFK.x() * nDiploid + offset.x() * nDiploid,ptFK.y() * nDiploid + offset.y() * nDiploid,FK_length*nDiploid,FK_breath*nDiploid);
    int WordNameLenth;
    WordNameLenth = WordName.length();
    rect_Word.setRect(ptWord.x() * nDiploid + offset.x() * nDiploid,ptWord.y() * nDiploid + offset.y() * nDiploid,WordNameLenth*12*nDiploid,20*nDiploid);
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, true);

    font.setFamily("宋体");
    font.setPointSize(10 * nDiploid);//字号
    font.setItalic(false);//斜体
    painter->setFont(font);//设置字体
    painter->setPen(Qt::white);//设置画笔颜色

    DrawCLArrow(painter,nElapsed,nDiploid,offset);//绘制箭头
    if(isMulti)
        return;

    if(haveAFJ)
    {
        Draw_circle_Light(painter,//画笔
                        p_BSD_AFJ,//指示灯中心坐标
                        AFJ_Name, //文字显示
                        isBSDLight_AFJ,
                        AFJColorType,
                        AFJLightForm,
                        AFJTextPos,
                        nElapsed,//闪烁读秒
                        nDiploid,//放大倍数
                        offset);//绘制指示灯
    }
    if(haveBFJ)
    {
        Draw_circle_Light(painter,//画笔
                        p_BSD_BFJ,//指示灯中心坐标
                        BFJ_Name, //文字显示
                        isBSDLight_BFJ,
                        BFJColorType,
                        BFJLightForm,
                        BFJTextPos,
                        nElapsed,//闪烁读秒
                        nDiploid,//放大倍数
                        offset);//绘制指示灯
    }
    if(haveCFJ)
    {
        Draw_circle_Light(painter,//画笔
                        p_BSD_CFJ,//指示灯中心坐标
                        CFJ_Name, //文字显示
                        isBSDLight_CFJ,
                        CFJColorType,
                        CFJLightForm,
                        CFJTextPos,
                        nElapsed,//闪烁读秒
                        nDiploid,//放大倍数
                        offset);//绘制指示灯
    }
    if(haveJGJ)
    {
        Draw_circle_Light(painter,//画笔
                        p_BSD_JGJ,//指示灯中心坐标
                        JGJ_Name, //文字显示
                        isBSDLight_JGJ,
                        JGJColorType,
                        JGJLightForm,
                        JGJTextPos,
                        nElapsed,//闪烁读秒
                        nDiploid,//放大倍数
                        offset);//绘制指示灯
    }

    //绘制方框
    if(haveFK)
    {
        painter->setPen(QPen(Qt::white, 1));
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(rect_FK);
    }
    //绘制文字 //方向文字显示标志
    if(haveWord)
    {
        QFontMetrics fm(font);
        QRect rec = fm.boundingRect(getName());

        painter->setPen(QPen(Qt::black, 1));
        painter->setBrush(Qt::black);
        painter->drawRect(rect_Word);

        painter->setPen(Qt::white);//设置画笔颜色
        painter->drawText(rect_Word,Qt::AlignHCenter|Qt::AlignVCenter,WordName);
    }
}

unsigned int cl::getDevType()
{
    return Dev_CL;
}

void cl::DrawCLArrow(QPainter *painter,long nElapsed, double nDiploid,QPoint offset)
{
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, false);
    /*初始化箭头数据*/
    arrayArrowLeft[0] = QPoint((LEFTArrowPt.x()+5*nDiploid), (LEFTArrowPt.y()+32*nDiploid));
    arrayArrowLeft[1] = QPoint((LEFTArrowPt.x()+30*nDiploid), (LEFTArrowPt.y()+32*nDiploid));
    arrayArrowLeft[2] = QPoint((LEFTArrowPt.x()+30*nDiploid), (LEFTArrowPt.y()+24*nDiploid));
    arrayArrowLeft[3] = QPoint((LEFTArrowPt.x()+50*nDiploid), (LEFTArrowPt.y()+36*nDiploid));
    arrayArrowLeft[4] = QPoint((LEFTArrowPt.x()+30*nDiploid), (LEFTArrowPt.y()+48*nDiploid));
    arrayArrowLeft[5] = QPoint((LEFTArrowPt.x()+30*nDiploid), (LEFTArrowPt.y()+40*nDiploid));
    arrayArrowLeft[6] = QPoint((LEFTArrowPt.x()+5*nDiploid), (LEFTArrowPt.y()+40*nDiploid));
    arrayArrowLeft[7] = QPoint((LEFTArrowPt.x()+5*nDiploid), (LEFTArrowPt.y()+32*nDiploid));

    arrayArrowRight[0] = QPoint((RIGHTArrowPt.x()+125*nDiploid), (RIGHTArrowPt.y()+32*nDiploid));
    arrayArrowRight[1] = QPoint((RIGHTArrowPt.x()+100*nDiploid), (RIGHTArrowPt.y()+32*nDiploid));
    arrayArrowRight[2] = QPoint((RIGHTArrowPt.x()+100*nDiploid), (RIGHTArrowPt.y()+24*nDiploid));
    arrayArrowRight[3] = QPoint((RIGHTArrowPt.x()+80*nDiploid), (RIGHTArrowPt.y()+36*nDiploid));
    arrayArrowRight[4] = QPoint((RIGHTArrowPt.x()+100*nDiploid), (RIGHTArrowPt.y()+48*nDiploid));
    arrayArrowRight[5] = QPoint((RIGHTArrowPt.x()+100*nDiploid), (RIGHTArrowPt.y()+40*nDiploid));
    arrayArrowRight[6] = QPoint((RIGHTArrowPt.x()+125*nDiploid), (RIGHTArrowPt.y()+40*nDiploid));
    arrayArrowRight[7] = QPoint((RIGHTArrowPt.x()+125*nDiploid), (RIGHTArrowPt.y()+32*nDiploid));

    QPointF arrayArrowLeftForDraw[8];//左箭头
    QPointF arrayArrowRightForDraw[8];//右箭头
    for(int j = 0; j < 8; j++)
    {
        arrayArrowLeftForDraw[j] = QPoint(arrayArrowLeft[j].x(),
            arrayArrowLeft[j].y());
        arrayArrowRightForDraw[j] = QPoint(arrayArrowRight[j].x(),
            arrayArrowRight[j].y());
    }
    QPen pen;
    pen.setColor(GRAY);
    pen.setWidth(1);

    QBrush brush_Left;
    brush_Left.setStyle(Qt::SolidPattern);
    QColor color_Left;
    if(m_nArrowStateJC == Arrow_Red)
    {
        color_Left = RED;
    }
    else if(m_nArrowStateJC == Arrow_Yellow)
    {
        color_Left = YELLOW;
    }
    else if(m_nArrowStateJC == Arrow_Green)
    {
        color_Left = GREEN;
    }
    else if(m_nArrowStateJC == Arrow_Black)
    {
        color_Left = BLACK;
    }
    else if(m_nArrowStateJC == Arrow_White)
    {
        color_Left = WHITE;
    }
    else if(m_nArrowStateJC == Arrow_Gray)
    {
        color_Left = GREY;
    }

    QBrush brush_Right;
    brush_Right.setStyle(Qt::SolidPattern);
    QColor color_Right;
    if(m_nArrowStateFC == Arrow_Red)
    {
        color_Right = RED;
    }
    else if(m_nArrowStateFC == Arrow_Yellow)
    {
        color_Right = YELLOW;
    }
    else if(m_nArrowStateFC == Arrow_Green)
    {
        color_Right = GREEN;
    }
    else if(m_nArrowStateFC == Arrow_Black)
    {
        color_Right = BLACK;
    }
    else if(m_nArrowStateFC == Arrow_White)
    {
        color_Right = WHITE;
    }
    else if(m_nArrowStateFC == Arrow_Gray)
    {
        color_Right = GREY;
    }

//    if(m_nSX)//0是下行,1是上行
//    {
//        if(bStaSXLORR)
//        {
//            if(HaveLeftArrow)
//            {
//                brush_Left.setColor(color_Left);
//                painter->setPen(pen);
//                painter->setBrush(brush_Left);
//                painter->drawConvexPolygon(arrayArrowLeftForDraw, 8);
//            }

//            if(HaveRightArrow)
//            {
//                brush_Left.setColor(color_Left);
//                painter->setPen(pen);
//                painter->setBrush(brush_Left);
//                painter->drawConvexPolygon(arrayArrowRightForDraw, 8);
//            }
//        }
//        else
//        {
//            if(HaveLeftArrow)
//            {
//                brush_Right.setColor(color_Left);
//                painter->setPen(pen);
//                painter->setBrush(brush_Right);
//                painter->drawConvexPolygon(arrayArrowLeftForDraw, 8);
//            }

//            if(HaveRightArrow)
//            {
//                brush_Right.setColor(color_Left);
//                painter->setPen(pen);
//                painter->setBrush(brush_Right);
//                painter->drawConvexPolygon(arrayArrowRightForDraw, 8);
//            }
//        }
//    }
//    else
//    {
//        if(bStaSXLORR)
//        {
//            if(HaveLeftArrow)
//            {
//                brush_Left.setColor(color_Left);
//                painter->setPen(pen);
//                painter->setBrush(brush_Left);
//                painter->drawConvexPolygon(arrayArrowLeftForDraw, 8);
//            }

//            if(HaveRightArrow)
//            {
//                brush_Right.setColor(color_Left);
//                painter->setPen(pen);
//                painter->setBrush(brush_Left);
//                painter->drawConvexPolygon(arrayArrowRightForDraw, 8);
//            }
//        }
//        else
//        {
//            if(HaveLeftArrow)
//            {
//                brush_Left.setColor(color_Left);
//                painter->setPen(pen);
//                painter->setBrush(brush_Left);
//                painter->drawConvexPolygon(arrayArrowLeftForDraw, 8);
//            }

//            if(HaveRightArrow)
//            {
//                brush_Right.setColor(color_Left);
//                painter->setPen(pen);
//                painter->setBrush(brush_Left);
//                painter->drawConvexPolygon(arrayArrowRightForDraw, 8);
//            }
//        }

//    }
    if(m_nSX)//0是下行,1是上行
    {
        if(bStaSXLORR)
        {
            if(HaveLeftArrow)
            {
                brush_Left.setColor(color_Left);
                painter->setPen(pen);
                painter->setBrush(brush_Left);
                painter->drawConvexPolygon(arrayArrowLeftForDraw, 8);
            }

            if(HaveRightArrow)
            {
                brush_Left.setColor(color_Left);
                painter->setPen(pen);
                painter->setBrush(brush_Left);
                painter->drawConvexPolygon(arrayArrowRightForDraw, 8);
            }
        }
        else
        {
            if(HaveLeftArrow)
            {
                brush_Left.setColor(color_Left);
                painter->setPen(pen);
                painter->setBrush(brush_Left);
                painter->drawConvexPolygon(arrayArrowLeftForDraw, 8);
            }

            if(HaveRightArrow)
            {
                brush_Left.setColor(color_Left);
                painter->setPen(pen);
                painter->setBrush(brush_Left);
                painter->drawConvexPolygon(arrayArrowRightForDraw, 8);
            }
        }
    }
    else
    {
        if(bStaSXLORR)
        {
            if(HaveLeftArrow)
            {
                brush_Left.setColor(color_Left);
                painter->setPen(pen);
                painter->setBrush(brush_Left);
                painter->drawConvexPolygon(arrayArrowLeftForDraw, 8);
            }

            if(HaveRightArrow)
            {
                brush_Left.setColor(color_Left);
                painter->setPen(pen);
                painter->setBrush(brush_Left);
                painter->drawConvexPolygon(arrayArrowRightForDraw, 8);
            }
        }
        else
        {
            if(HaveLeftArrow)
            {
                brush_Left.setColor(color_Left);
                painter->setPen(pen);
                painter->setBrush(brush_Left);
                painter->drawConvexPolygon(arrayArrowLeftForDraw, 8);
            }

            if(HaveRightArrow)
            {
                brush_Left.setColor(color_Left);
                painter->setPen(pen);
                painter->setBrush(brush_Left);
                painter->drawConvexPolygon(arrayArrowRightForDraw, 8);
            }
        }

    }

}

void cl::Draw_circle_Light(QPainter *painter,//画笔
                              QPointF LightPoint,//指示灯中心坐标
                              QString rectTextShow, //文字显示
                              bool Light_color,//指示灯状态
                              int colorType,//颜色类型
                              int LightForm,
                              int TextPos,
                              long nElapsed,//闪烁读秒
                              double nDiploid,//放大倍数
                              QPoint offset)//绘制指示灯
{
#pragma region//>..绘制场联灯
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, true);

    QPointF CenterPoint;
    QBrush newBrushBK;
    newBrushBK.setStyle(Qt::SolidPattern);
    QColor color_Light;
    if(Light_color)//点灯状态
    {
        color_Light = getLightState(colorType,nElapsed);
    }
    else//灭灯状态
    {
        color_Light = BLACK;
    }
    newBrushBK.setColor(color_Light);
    painter->setBrush(newBrushBK);

    QPen pen;
    pen.setWidth(2);
    pen.setColor(THIN_GRAY);
    painter->setPen(pen);

    CenterPoint = QPointF(LightPoint.x(),LightPoint.y());
    if(LightForm == 0)
    {
        painter->drawEllipse(CenterPoint, 8 * nDiploid, 8 * nDiploid);
    }
    else if(LightForm == 1)
    {
        painter->drawEllipse(CenterPoint, 8 * nDiploid, 8 * nDiploid);
        painter->drawLine(CenterPoint.x()+10 * nDiploid,CenterPoint.y()-8 * nDiploid, CenterPoint.x()+10 * nDiploid,CenterPoint.y()+8 * nDiploid);
    }
    else if(LightForm == 2)
    {
        painter->drawEllipse(CenterPoint, 8 * nDiploid, 8 * nDiploid);
        painter->drawLine(CenterPoint.x()-10 * nDiploid,CenterPoint.y()-8 * nDiploid, CenterPoint.x()-10 * nDiploid,CenterPoint.y()+8 * nDiploid);
    }
    else if(LightForm == 3)
    {
        painter->drawEllipse(CenterPoint, 8 * nDiploid, 8 * nDiploid);
        painter->drawLine(CenterPoint.x()+10 * nDiploid,CenterPoint.y()-8 * nDiploid, CenterPoint.x()+10 * nDiploid,CenterPoint.y()+8 * nDiploid);

        newBrushBK.setColor(BLACK);
        painter->setBrush(newBrushBK);
        painter->drawEllipse(CenterPoint.x()-24 * nDiploid,CenterPoint.y()-8 * nDiploid, 16 * nDiploid, 16 * nDiploid);
    }
    else if(LightForm == 4)
    {
        painter->drawEllipse(CenterPoint, 8 * nDiploid, 8 * nDiploid);
        painter->drawLine(CenterPoint.x()-10 * nDiploid,CenterPoint.y()-8 * nDiploid, CenterPoint.x()-10 * nDiploid,CenterPoint.y()+8 * nDiploid);

        newBrushBK.setColor(BLACK);
        painter->setBrush(newBrushBK);
        painter->drawEllipse(CenterPoint.x()+8 * nDiploid,CenterPoint.y()-8 * nDiploid, 16 * nDiploid, 16 * nDiploid);
    }


    //绘制指示灯的文字
    if(rectTextShow != ""&&bShowhave)
    {
        pen.setColor(WHITE);
        QFont font;
        font.setWeight(50);//字体宽度,0~99
        font.setPointSize(CLWordSize * nDiploid);//字体高度,10表示高度为10个像素点
        painter->setPen(pen);
        painter->setFont(font);
        QRectF rectBtnWord_X;
        QFontMetrics  fontMetrics(font);
        int w = fontMetrics.horizontalAdvance(rectTextShow) + 8;//增加文字显示宽度
        rectBtnWord_X.setLeft(LightPoint.x()-w/2 * nDiploid);
        rectBtnWord_X.setTop(LightPoint.y()+10 * nDiploid);
        rectBtnWord_X.setRight(LightPoint.x()+w/2 * nDiploid);
        rectBtnWord_X.setBottom(LightPoint.y()+30 * nDiploid);

        //painter->drawText(rectBtnWord_X,Qt::AlignHCenter|Qt::AlignVCenter,rectTextShow);
        if(TextPos == 0)
        {

            painter->drawText(rectBtnWord_X,Qt::AlignHCenter|Qt::AlignVCenter,rectTextShow);
            //painter->drawText(rectBtnWord_X.left(), rectBtnWord_X.bottom(), rectTextShow);
        }
        else if(TextPos == 1)
        {
            QRectF temp = rectBtnWord_X;
            rectBtnWord_X.setY(temp.y() - 72* nDiploid);
            //painter->drawText(rectBtnWord_X.left(), rectBtnWord_X.top(), rectTextShow);
            painter->drawText(rectBtnWord_X,Qt::AlignHCenter|Qt::AlignVCenter,rectTextShow);
        }
    }
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, false);
#pragma endregion
}

QColor cl::getLightState(int nType,int nElapsed)//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝）
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








