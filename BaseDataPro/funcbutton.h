#ifndef FUNCBUTTON_H
#define FUNCBUTTON_H

#include "BaseData.h"
class FUNCBUTTON: public CBaseData
{
public:
    explicit FUNCBUTTON();

//成员变量声明
private:

    QPoint p_ButtonPt;//按钮坐标
    QPoint p_ButtonPtConst;
    QRect rect_Button;//按钮区域
    int isDown_Button;

public:
    QString m_strType;		//按钮类型
    QString m_strJuncType;	//关联设备
    QString DEVName;
    QString DEVBtnType;     //关联按钮类型
    QString Button_Name;
    int Button_QF;
    bool haveButton;
    bool haveQF;//是否有铅封
    bool bVisleJSQ;//是否显示铅封
    bool bShowQFJS=true;//铅封计数显示标志
    bool bBtnNameShow=true;//按钮名称显示
    bool haveAN=true;//按钮显示标志
    //bool m_RangeVisible;//鼠标移动至设备上以后的高亮显示
    int ButtonForm;//按钮形状,0为圆,1为方;
    int m_nDownTime_1 = 0;   //当按钮按下为1时持续时间

    int wordcolor;//文字颜色
    int wordsize;//文字大小
    int ViewBtnAndLmapSize=12;//界面按钮名称及标识灯文字大小；按钮（指示灯大小）
public:
    void setButtonPt(QPoint pt);
    QPoint getButtonPt();

    void setIsDown_Button(int flag);
    int getIsDown_Button();

public:
    virtual void Draw(QPainter *painter,long nElapsed, double nDiploid,QPoint offset,int type);
    virtual void Draw_ToolTip(QPainter *painter, double nDiploid);
    virtual unsigned int getDevType();
    virtual void setVollover(QPoint pt_Base);

    void Draw_Th_kb(QPainter *painter,long nElapsed, double nDiploid,QPoint offset);
    void DrawQFRect(QPainter *painter,QPointF QFPoint,int m_nQFNumber,long nElapsed, double nDiploid,QPoint offset);
    void Draw_circle_Button(QPainter *painter,//画笔
                            QRectF m_ButtonRect,//按钮绘制区域
                            bool m_FCZKState,//非常站控状态
                            int m_ButtonIsDownState,//按钮是否按下
                            bool m_FSState,//封锁状态
                            bool m_bZDANFlash,//终端闪烁状态
                            int mTimeCount,//倒计时时间
                            bool HaveQF,//是否有铅封计数
                            QString rectTextShow, //按钮文字显示
                            bool RangeVisible,//按钮是否高亮
                            long nElapsed,//闪烁读秒
                            double nDiploid,//放大倍数
                            QPoint offset);//绘制圆形按钮
    void Draw_Rect_Button(QPainter *painter,//画笔
                          QRectF m_ButtonRect,//按钮绘制区域
                          bool m_FCZKState,//非常站控状态
                          int m_ButtonIsDownState,//按钮是否按下
                          bool m_FSState,//封锁状态
                          bool m_bZDANFlash,//终端闪烁状态
                          int mTimeCount,//倒计时时间
                          QColor color,//按钮颜色
                          bool HaveQF,//是否有铅封计数
                          QString rectTextShow,//显示按钮文字
                          bool m_RangeVisible,//按钮是否高亮
                          long nElapsed,//闪烁读秒
                          double nDiploid,//放大倍数
                          QPoint offset);//绘制KSK方形按钮
    QColor getLightState(int nType,int nElapsed);
    int GetMousePoint(QPoint p);
    //设置按钮按下抬起状态
    void SetANIsDown();
};

#endif // FUNCBUTTON_H
