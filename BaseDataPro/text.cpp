#include "text.h"
#pragma execution_character_set("utf-8")
CText::CText()
{
    m_nDCNode = 0xffff;         //关联道岔名称
    //m_nQDNode = 0xffff;
    m_nSize = 10;     //文本大小
    textColor = Qt::white;       //文本显示颜色
    m_nTextType = TEXT;
    bDCQDTextShow=true;
    bStationNameShow=true;
    bdirectionShow=true;
    bgeneralShow=true;
}

//成员功能函数实现
void CText::TextInit()
{
    m_textRectConst.setLeft(m_textRect.left());
    m_textRectConst.setTop(m_textRect.top());
    m_textRectConst.setRight(m_textRect.right());
    m_textRectConst.setBottom(m_textRect.bottom());
}
void CText::Draw(QPainter *painter, long nElapsed, double nDiploid,QPoint offset,int type)
{
    QFont font;
    if (STATIONNAMETEXT == getTextType())
    {
        m_textRect.setLeft(m_textRectConst.left() * nDiploid);
        m_textRect.setTop(m_textRectConst.top() * nDiploid);
        m_textRect.setRight(m_textRectConst.right() * nDiploid);
        m_textRect.setBottom(m_textRectConst.bottom() * nDiploid);
    }
    else
    {
        m_textRect.setLeft(m_textRectConst.left() * nDiploid+offset.x()*nDiploid);
        m_textRect.setTop(m_textRectConst.top() * nDiploid+offset.y()*nDiploid);
        m_textRect.setRight(m_textRectConst.right() * nDiploid+offset.x()*nDiploid);
        m_textRect.setBottom(m_textRectConst.bottom() * nDiploid+offset.y()*nDiploid);
    }
    if(false == getDisplayName())
    {
        return;
    }
    if (STATIONNAMETEXT == getTextType()&&bStationNameShow)
    {
        font.setFamily("宋体");
        font.setPointSize(getTextSize() * nDiploid);//字号
        font.setItalic(false);//斜体
        painter->setFont(font);//设置字体
        painter->setPen(Qt::white);//设置画笔颜色
        //绘制文本
        painter->drawText(m_textRect.left(), m_textRect.top() + 100 * nDiploid, getName());
    }
    else if (DCQDNAMETEXT == getTextType()&&bDCQDTextShow && WndType != 1)
    {
        font.setFamily("宋体");
        //font.setPointSize(getTextSize() * nDiploid);//字号
        font.setPointSize(DCQDWordSize * nDiploid);//字号
        font.setItalic(false);//斜体
        painter->setFont(font);//设置字体
        if(bDCQDWordColor)
        {
            painter->setPen(Qt::red);//设置画笔颜色
        }
        else
        {
            painter->setPen(Qt::white);//设置画笔颜色
        }
        //绘制文本
        painter->drawText(m_textRect.left(), m_textRect.top()+10* nDiploid, getName());

    }
    else if(TEXT == getTextType() && WndType != 1)
    {
        //判断字符串中是否有中文
        QString str=getName();
        if(((str.contains("安全线")||!str.contains("方面"))&&!bgeneralShow))
        {
            return;
        }
        else if(!bdirectionShow&&((!(str.contains("安全线")))&&str.contains("方面")))
        {
            return;
        }
        font.setFamily("宋体");
//        font.setPointSize(getTextSize() * nDiploid);//字号
        if(str.contains("方面"))
        {
            font.setPointSize(DirWordSize * nDiploid);//字号
        }
        else
        {
            //font.setPointSize(getTextSize() * nDiploid);//字号
            font.setPointSize(DirWordSize * nDiploid);//字号
        }

        font.setItalic(false);//斜体
        painter->setFont(font);//设置字体
        painter->setPen(Qt::white);//设置画笔颜色
        //绘制文本
        painter->drawText(m_textRect.left(), m_textRect.top()+10* nDiploid, getName());
    }
}
void CText::setVollover(QPoint pt_Base)
{
    if(getTextType() == STATIONNAMETEXT)
    {
        return;  //站名不需要翻转
    }
    else if(getTextType() == DCQDNAMETEXT)
    {
        setTextRect(QRectF(pt_Base.x() - (getTextRect().right()-pt_Base.x()),pt_Base.y() - (getTextRect().top()-pt_Base.y()),getTextRect().width(),getTextRect().height()));
        setCenterPt(QPointF(pt_Base.x() - (getCenterPt().x()-pt_Base.x()),pt_Base.y() - (getCenterPt().y() - pt_Base.y())));
    }
    else if(m_strName.length() == 1)
    {
        setTextRect(QRectF(pt_Base.x() - (getTextRect().right()-pt_Base.x()),/*pt_Base.y() - (*/getTextRect().bottom()-offsetWord.y()/*-pt_Base.y())*/,getTextRect().width(),getTextRect().height()));
        setCenterPt(QPointF(pt_Base.x() - (getCenterPt().x()-pt_Base.x()),/*pt_Base.y() - (*/getCenterPt().y()-offsetWord.y()/* - pt_Base.y())*/));
    }
    else
    {
        setTextRect(QRectF(pt_Base.x() - (getTextRect().right()-pt_Base.x()),pt_Base.y() - (getTextRect().bottom()-pt_Base.y()),getTextRect().width(),getTextRect().height()));
        setCenterPt(QPointF(pt_Base.x() - (getCenterPt().x()-pt_Base.x()),pt_Base.y() - (getCenterPt().y() - pt_Base.y())));
    }
}
unsigned int CText::getDevType()
{
    return Dev_TEXT;
}
int CText::moveCursor(QPoint p)
{
    QPoint pt;
    pt.setX(p.x());
    pt.setY(p.y()-Off_Global);
    QRect rect;
    rect.setRect(m_textRect.left(),m_textRect.top()-10,m_textRect.width(),m_textRect.height());
    if (rect.contains(pt))
    {
        if(getTextType() == DCQDNAMETEXT)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    return 0;
}
//成员变量封装函数实现
void CText::setGLDCNode(int node)
{
    m_nDCNode = node;
}
int CText::getGLDCNode()
{
    return m_nDCNode;
}
//void CText::setGLQDNode(unsigned int node)
//{
//    m_nQDNode = node;
//}
//unsigned int CText::getGLQDNode()
//{
//    return m_nQDNode;
//}
void CText::setTextSize(unsigned int size)
{
    m_nSize = size;
}
unsigned int CText::getTextSize()
{
    return m_nSize;
}
void CText::setTextColor(QString strColor)
{
    if ("WHITE" == strColor)
    {
        textColor = Qt::white;
    }
    else if ("GREEN" == strColor)
    {
        textColor = Qt::green;
    }
    else if ("BLUE" == strColor)
    {
        textColor = Qt::blue;
    }
    else if ("YELLOW" == strColor)
    {
        textColor = Qt::yellow;
    }
    else if ("BLACK" == strColor)
    {
        textColor = Qt::black;
    }
    else if ("RED" == strColor)
    {
        textColor = Qt::red;
    }
    else if ("GRAY" == strColor)
    {
        textColor = Qt::gray;
    }
    else
    {
        textColor = Qt::white;
    }
}
QColor CText::getTextColor()
{
    return textColor;
}
void CText::setTextType(QString type)
{
    if(type == "STATIONNAMETEXT")
    {
        m_nTextType = STATIONNAMETEXT;
    }
    else if(type == "DCQDNAMETEXT")
    {
        m_nTextType = DCQDNAMETEXT;
    }
    else if(type == "TEXT")
    {
        m_nTextType = TEXT;
    }
}
unsigned int CText::getTextType()
{
    return m_nTextType;
}

bool CText::GetMousePoint(QPoint p)
{
    if(m_nTextType == DCQDNAMETEXT)
    {
        if(m_textRect.contains(p))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}
