#ifndef STATIONPLATFORMOTHER_H
#define STATIONPLATFORMOTHER_H

#include "BaseData.h"
class StationPlatformOther: public CBaseData
{
public:
    explicit StationPlatformOther();

//成员变量声明
private:


public:

    int ZHANTAICount;//站台数量
    bool ShowXinHaoLou;//是否有信号楼
    QPoint ptXinHaoLou;
    bool XinHaoLouFaceNorth;
    typedef struct //车站站台信息定义结构体
    {
        bool m_bVisibleZT;//站台是否隐藏
        bool StationIsHigh;
        QString StationType;
        QPoint ptStation;
        int StationZTLenth;//站台长度
    }StaZT_T;
    StaZT_T StaZTInfo[50];

    typedef struct  //文本定义
    {
        int     nColorType;	//颜色类型（颜色：0白色,1绿）
        QPoint  ptLT;   //左上角坐标
        QString strText;//文本
    }TEXT_T;
    QVector<TEXT_T> vectText; //文本信息

    typedef struct  //线条定义
    {
        int     nColorType;	//颜色类型（颜色：0白色,1绿）
        QPoint  pt1;//线条点1
        QPoint  pt2;//线条点2
        int LineWidth;//线宽
        int LineState;//0为虚线,1为实线
    }LINES_T;
    QVector<LINES_T> vectLine; //直线信息

    typedef struct  //矩形定义
    {
        int     nColorType;	//颜色类型（颜色：0白色,1绿）
        QPoint  pt1;//矩形点1
        QPoint  pt2;//矩形点2
        int LineWidth;//线宽
        int LineState;//0为虚线,1为实线
    }RECT_T;
    QVector<RECT_T> vectRect;//矩形信息


public:
    virtual void Draw(QPainter *painter,long nElapsed, double nDiploid,QPoint offset,int type);
    virtual void Draw_ToolTip(QPainter *painter, double nDiploid);
    virtual unsigned int getDevType();
    virtual void setVollover(QPoint pt_Base);
    void Draw_Th_kb(QPainter *painter,long nElapsed, double nDiploid,QPoint offset);
    QColor getLightState(int nType,int nElapsed);

};

#endif // STATIONPLATFORMOTHER_H
