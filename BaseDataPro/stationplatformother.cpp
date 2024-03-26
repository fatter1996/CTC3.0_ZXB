#include "stationplatformother.h"

StationPlatformOther::StationPlatformOther()
{
    ZHANTAICount = 0;
    XinHaoLouFaceNorth = true;
    ptXinHaoLou = QPoint(0,0);

    for(int i=0;i<50;i++)
    {
        StaZTInfo[i].m_bVisibleZT = true;
        StaZTInfo[i].StationIsHigh = true;
        StaZTInfo[i].StationType = "";
        StaZTInfo[i].ptStation = QPoint(0,0);
        StaZTInfo[i].StationZTLenth = 0;
    }
}
void StationPlatformOther::Draw_ToolTip(QPainter *painter, double nDiploid)
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
void StationPlatformOther::Draw(QPainter *painter,long nElapsed, double nDiploid,QPoint offset,int type)
{
    if(type==0x55)
    {
        Draw_Th_kb(painter,nElapsed, nDiploid,offset);
    }
}
void StationPlatformOther::Draw_Th_kb(QPainter *painter,long nElapsed, double nDiploid,QPoint offset)
{
#pragma region //绘制站台
    QPen pen;
    QBrush newBrushBK;
    for(int i = 0; i < ZHANTAICount; i++)
    {
        if(StaZTInfo[i].m_bVisibleZT)
        {
            newBrushBK.setStyle(Qt::SolidPattern);
            newBrushBK.setColor(Qt::NoBrush);
            if(StaZTInfo[i].StationIsHigh)
            {
                pen.setStyle(Qt::SolidLine);
                pen.setWidthF(ZHANTAIWIDTH * nDiploid);
                pen.setColor(ZHANTAICOLOR1);
                painter->setPen(pen);
            }
            else
            {
                pen.setStyle(Qt::SolidLine);
                pen.setWidthF(ZHANTAIWIDTH * nDiploid);
                pen.setColor(ZHANTAICOLOR2);
                painter->setPen(pen);
            }

            QPoint p11, p12, p13, p14;
            int length = StaZTInfo[i].StationZTLenth;
            int X = StaZTInfo[i].ptStation.x();
            int Y = StaZTInfo[i].ptStation.y();

            p11 = QPoint(X * nDiploid, Y * nDiploid);
            p12 = QPoint(p11.x(), p11.y() + nDiploid * ZHANTAIHEIGHT);
            p13 = QPoint(p12.x() + nDiploid * length, p12.y());
            p14 = QPoint(p13.x(), p11.y());

            if(StaZTInfo[i].StationType == "u")
            {
                painter->drawLine(p11,p12);
                painter->drawLine(p12,p13);
                painter->drawLine(p13,p14);
            }
            else if(StaZTInfo[i].StationType == "n")
            {
                painter->drawLine(p12,p11);
                painter->drawLine(p11,p14);
                painter->drawLine(p14,p13);
            }
        }
    }

    //绘制信号楼
    if(ShowXinHaoLou)
    {
        pen.setStyle(Qt::SolidLine);
        pen.setWidthF(ZHANTAIWIDTH * nDiploid);
        pen.setColor(WHITE);
        painter->setPen(pen);
        newBrushBK.setStyle(Qt::SolidPattern);
        newBrushBK.setColor(Qt::NoBrush);
        painter->setBrush(newBrushBK);

        QRect l_rect;
        l_rect.setRect(ptXinHaoLou.x() * nDiploid + offset.x() * nDiploid,
                       ptXinHaoLou.y() * nDiploid + offset.y() * nDiploid,
                       80 * nDiploid, 50 * nDiploid);

        QPoint p11, p12, p13, p14;
        if(XinHaoLouFaceNorth)
        {
            p11 = QPoint(l_rect.left() + 10 * nDiploid, l_rect.top() + 15 * nDiploid);
            p12 = QPoint(l_rect.right() - 10 * nDiploid, p11.y());
            p13 = QPoint(l_rect.left() + 23 * nDiploid, l_rect.top() + 25 * nDiploid);
            p14 = QPoint(l_rect.right() - 23 * nDiploid, p13.y());
        }
        else
        {
            p11 = QPoint(l_rect.left() + 10 * nDiploid, l_rect.bottom() - 15 * nDiploid);
            p12 = QPoint(l_rect.right() - 10 * nDiploid, p11.y());
            p13 = QPoint(l_rect.left() + 23 * nDiploid, l_rect.bottom() - 25 * nDiploid);
            p14 = QPoint(l_rect.right() - 23 * nDiploid, p13.y());
        }
        painter->drawRect(l_rect);
        painter->drawLine(p11,p12);
        painter->drawLine(p13,p14);
    }

    //文本
    for (int i = 0; i < vectText.size(); i++)
    {
        pen.setColor(YELLOW);
        QFont font;
        font.setWeight(50);//字体宽度,0~99
        font.setFamily("宋体");
        font.setPointSize(12 * nDiploid);//字体高度,10表示高度为10个像素点
        font.setItalic(false);//斜体
        painter->setPen(pen);
        painter->setFont(font);
        painter->setPen(getLightState(vectText[i].nColorType,nElapsed));//设置画笔颜色

        QPoint temp = vectText[i].ptLT;
        temp.setX(vectText[i].ptLT.x()*nDiploid /*+ offset.x()*nDiploid*/);
        temp.setY(vectText[i].ptLT.y()*nDiploid /*+ offset.y()*nDiploid*/);

        QRectF rectBtnTG_X;
        rectBtnTG_X.setLeft(temp.x()-20 * nDiploid);
        rectBtnTG_X.setTop(temp.y()-25 * nDiploid);
        rectBtnTG_X.setRight(temp.x()+20 * nDiploid);
        rectBtnTG_X.setBottom(temp.y()+15 * nDiploid);

        //painter->drawText(rectBtnTG_X,Qt::AlignHCenter|Qt::AlignVCenter,vectText[i].strText);
        painter->drawText(rectBtnTG_X.left(),rectBtnTG_X.top(), vectText[i].strText);
    }

    //线条
    for (int i = 0; i < vectLine.size(); i++)
    {
        int LineWidth = 2;
        QColor color = getLightState(vectLine[i].nColorType,nElapsed);
        if(vectLine[i].LineWidth>0)
        {
            LineWidth = vectLine[i].LineWidth;
        }
        if(vectLine[i].LineState==0)
        {
            pen.setStyle(Qt::DashLine);//DashLine
        }
        else
        {
            pen.setStyle(Qt::SolidLine);
        }
        pen.setColor(color);
        pen.setWidthF(LineWidth * nDiploid);
        QPoint p1 = QPoint(vectLine[i].pt1.x()*nDiploid, vectLine[i].pt1.y()*nDiploid);
        QPoint p2 = QPoint(vectLine[i].pt2.x()*nDiploid, vectLine[i].pt2.y()*nDiploid);
        painter->setPen(pen);
        painter->drawLine(p1,p2);
    }

    //自定义矩形
    for(int i = 0; i < vectRect.size(); i++)
    {
        int LineWidth = 2;
        QColor color = getLightState(vectRect[i].nColorType,nElapsed);
        if(vectRect[i].LineWidth>0)
        {
            LineWidth = vectRect[i].LineWidth;
        }
        if(vectRect[i].LineState==0)
        {
            pen.setStyle(Qt::DashLine);//DashLine
        }
        else
        {
            pen.setStyle(Qt::SolidLine);
        }
        pen.setWidthF(LineWidth * nDiploid);
        pen.setColor(color);
        painter->setPen(pen);
        newBrushBK.setStyle(Qt::NoBrush);
        painter->setBrush(newBrushBK);
        QPoint p1 = QPoint(vectRect[i].pt1.x()*nDiploid, vectRect[i].pt1.y()*nDiploid);
        QPoint p2 = QPoint(vectRect[i].pt2.x()*nDiploid, vectRect[i].pt2.y()*nDiploid);
        QRect rect_Temp;
        rect_Temp = QRect(p1,p2);
        painter->drawRect(rect_Temp);
    }

#pragma endregion
}
void StationPlatformOther::setVollover(QPoint pt_Base)
{

}
unsigned int StationPlatformOther::getDevType()
{
    return Dev_ZT;
}
QColor StationPlatformOther::getLightState(int nType,int nElapsed)//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝）
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


