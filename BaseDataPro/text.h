#ifndef CTEXT_H
#define CTEXT_H

#include <QPaintEvent>
#include <QPainter>
#include <QtMath>
#include "BaseData.h"
class CText : public CBaseData
{
public:
    explicit CText();

    //成员变量声明
private:
    int m_nDCNode;         //关联道岔名称
    //unsigned int m_nQDNode;      //关联道岔设备编号
    unsigned int m_nSize;     //文本大小
    QColor textColor;       //文本显示颜色
    unsigned int m_nTextType;


    //成员变量封装函数声明
public:
    void setGLDCNode(int node);
    int getGLDCNode();
    //void setGLQDNode(unsigned int node);
    //unsigned int getGLQDNode();
    void setTextSize(unsigned int size);
    unsigned int getTextSize();
    void setTextColor(QString strColor);
    QColor getTextColor();
    void setTextType(QString type);
    unsigned int getTextType();
    bool GetMousePoint(QPoint p);
    bool bStationNameShow;//车站名称显示
    bool bDCQDTextShow;//道岔区段名称显示
    bool bdirectionShow;//方向文字显示
    bool bgeneralShow;//一般文字
    QPointF offsetWord;
    int DCQDWordSize = 12;//道岔区段文字:文字大小
    int DirWordSize = 12;//方向文字:文字大小
    bool bDCQDWordColor=false;
    //成员功能函数声明
public:
    void TextInit(); //初始化
    virtual void Draw(QPainter *painter, long nElapsed, double nDiploid,QPoint offset,int type);
    virtual void setVollover(QPoint pt_Base);
    virtual unsigned int getDevType();
    virtual int moveCursor(QPoint p);
};

#endif // CTEXT_H
