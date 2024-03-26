#ifndef CDCBTN_H
#define CDCBTN_H

#include "BaseData.h"
class CDcBtn: public CBaseData
{
public:
    explicit CDcBtn();

//成员变量声明
public:

    int m_tType; //类型-尖轨、心轨
    QString m_strDcName;    //道岔名称
    int     m_nDcCode;      //道岔号

//    int m_nFuncBtn1IsDown;	//功能按钮是否按下 0吸起 1正在按下 2按下
//    int m_nFuncBtn2IsDown;	//功能按钮是否按下 0吸起 1正在按下 2按下
//    int m_nFuncBtn1Status;  //功能按钮的状态 0吸起 1按下 //尖轨故障
//    int m_nFuncBtn2Status;  //功能按钮的状态 0吸起 1按下 //心轨故障

    QPoint p_ButtonPt;//按钮坐标
    QPoint p_ButtonPtConst;
    QRect rect_Button;//按钮区域
    int isDown_Button;
    int m_nDownTime_1 = 0;   //当按钮按下为1时持续时间

    QString m_strType;		//按钮类型
    QString m_strJuncType;	//关联设备或类型
    QString Button_Name;
    int Button_QF;
    bool haveButton;

    int m_nStyle;//显示风格 0(尖轨-铅封按钮文字，心轨-文字按钮铅封) 1(尖轨-铅封按钮文字，心轨-铅封按钮文字)
    //bool m_RangeVisible; //股道范围显示（高亮显示）
    int ButtonForm;//按钮形状,0为圆,1为方;

    bool bShowQFJS=true;//铅封计数显示标志
    bool bBtnNameShow=true;//按钮名称显示
    bool bVisleJSQ=true;//显示铅封


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
                          QString rectTextShow,//显示按钮文字
                          bool m_RangeVisible,//按钮是否高亮
                          long nElapsed,//闪烁读秒
                          double nDiploid,//放大倍数
                          QPoint offset);//绘制KSK方形按钮
    QColor getLightState(int nType,int nElapsed);
    bool GetMousePoint(QPoint p);










};

#endif // CDCBTN_H
