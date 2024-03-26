#ifndef CL_H
#define CL_H

#include "BaseData.h"

//场联+机务段
class cl: public CBaseData
{
public:
    explicit cl();

    //成员变量声明
public:
    int m_nGLXH;//关联信号设备编号
    QVector<int>vect_JJQD;//对应接发车口的接近区段编号
    bool isNX;//是否是逆向发车口//决定是否有改方按钮
    QPoint LEFTArrowPt;//箭头坐标
    QPoint LEFTArrowPtConst;
    QPoint RIGHTArrowPt;//箭头坐标
    QPoint RIGHTArrowPtConst;

    QPoint p_BSD_AFJ;
    QPoint p_BSD_AFJConst;
    QPoint p_BSD_BFJ;
    QPoint p_BSD_BFJConst;
    QPoint p_BSD_CFJ;
    QPoint p_BSD_CFJConst;
    QPoint p_BSD_JGJ;
    QPoint p_BSD_JGJConst;

    QRect rect_FK;//方框区域
    QRect rect_FKConst;


    int isBSDLight_AFJ;
    int isBSDLight_BFJ;
    int isBSDLight_CFJ;
    int isBSDLight_JGJ;

    int m_nArrowStateJC;//接车箭头状态
    int m_nArrowStateFC;//发车箭头状态

public:

    QString AFJ_Name;
    QString BFJ_Name;
    QString CFJ_Name;
    QString JGJ_Name;

    bool haveAFJ;//允许发车灯绘制标志
    bool haveBFJ;//辅助灯绘制标志
    bool haveCFJ;//事故灯绘制标志
    bool haveJGJ;//区间灯绘制标志
    bool bShowhave=true;//灯文字显示标志

    bool HaveLeftArrow;
    bool HaveRightArrow;
    QPoint arrayArrowLeft[8];	//左箭头
    QPoint arrayArrowRight[8];	//右箭头
    QString ArrowLeftName;
    QString ArrowRightName;

    bool haveFK;
    QPoint ptFK;
    int FK_length;
    int FK_breath;

    bool haveWord;//方向文字显示标志
    QPoint ptWord;
    QRect rect_Word;//文字区域
    QString WordName;

    int AFJColorType;//点灯颜色类型
    int BFJColorType;//点灯颜色类型
    int CFJColorType;//点灯颜色类型
    int JGJColorType;//颜色类型

    bool bStaSXLORR=false;//站场上下行方向标志
    int CLWordSize=12;//自动闭塞半自动闭塞功能按钮文字:文字大小;

    int AFJLightForm;//灯的形状:0为普通圆灯位,1为朝左单灯,2为朝右单灯,3为朝左双灯,4为朝右双灯
    int BFJLightForm;//灯的形状:0为普通圆灯位,1为朝左单灯,2为朝右单灯,3为朝左双灯,4为朝右双灯
    int CFJLightForm;//灯的形状:0为普通圆灯位,1为朝左单灯,2为朝右单灯,3为朝左双灯,4为朝右双灯
    int JGJLightForm;//灯的形状:0为普通圆灯位,1为朝左单灯,2为朝右单灯,3为朝左双灯,4为朝右双灯

    int AFJTextPos;//文字的位置:0为灯下,1为灯上
    int BFJTextPos;//文字的位置:0为灯下,1为灯上
    int CFJTextPos;//文字的位置:0为灯下,1为灯上
    int JGJTextPos;//文字的位置:0为灯下,1为灯上

public:
    virtual void Draw(QPainter *painter,long nElapsed, double nDiploid,QPoint offset,int type);
    virtual void DrawM(QPainter *painter,long nElapsed, double nDiploid,QPoint offset,int type,int flag = 0);
    virtual void Draw_ToolTip(QPainter *painter, double nDiploid);
    virtual unsigned int getDevType();

    void Draw_Th_kb(QPainter *painter,long nElapsed, double nDiploid,QPoint offset, bool isMulti = false);
    void DrawCLArrow(QPainter *painter,long nElapsed, double nDiploid,QPoint offset);
    void Draw_circle_Light(QPainter *painter,//画笔
                           QPointF LightPoint,//指示灯中心坐标
                           QString rectTextShow, //文字显示
                           bool Light_color,//指示灯状态
                           int colorType,//颜色类型
                           int LightForm,//灯的形状:0为普通圆灯位,1为朝左单灯,2为朝右单灯,3为朝左双灯,4为朝右双灯
                           int TextPos,
                           long nElapsed,//闪烁读秒
                           double nDiploid,//放大倍数
                           QPoint offset);//绘制指示灯
    QColor getLightState(int nType,int nElapsed);

};

#endif // CL_H
