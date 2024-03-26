#ifndef CXHD_H
#define CXHD_H

#include <QPaintEvent>
#include <QPainter>
#include <QtMath>
#include "BaseData.h"
class CXHD : public CBaseData
{
public:
    explicit CXHD();

    //成员变量声明
private:
    unsigned int m_nXHDType;    //信号机类型
    bool isSignalType;   //是否是虚拟信号
    unsigned int m_nSafeLamp;   //安全灯光
    bool isHigh;       //是否为高柱
    unsigned int m_nXHDState;  //信号机状态
    //unsigned int m_nXHD_lsState;  //信号机状态
    bool isLCBt_Down;
    bool isDCBt_Down;
    bool isYDBt_Down;
    double nXHANSize;   //信号按钮大小
    double nXHANSizeConst;   //信号按钮大小
    QRectF m_rectLC;       //列车按钮区域
    QRectF m_rectDC;       //调车按钮区域
    QRectF m_rectYD;       //引导按钮区域
    QRectF m_rectLCConst;
    QRectF m_rectDCConst;
    QRectF m_rectYDConst;
    QRectF m_rectTGConst;
    bool isYDSD;
    bool signalDCAN;    //是否有单独的调车按钮
    QRect xhd_Rect;
    bool isMD;
    bool isLock; //封锁
    bool isLCANFB;
    bool isDCANFB;
    bool isYDANFB;
    int m_nTimeType;   //0x11-无倒计时  0x22-人解30s  0x33-人解180s   0x44-坡道解锁180s  0x55-引导首区段故障保持15s
    int m_nTimeCount;  //0xFF-无倒计时  其他-实时值



    void DrawXHD(
        int nElapsed ,
        int offset_x = 0,	//偏移量
        int offset_y = 0,	//偏移量
        double nDiploid = 1.0,	//放大缩小系数 默认参数为1.0
        int nShowState = 0x00);	//文字显示:GD_TEXT_SHOW  非常站控:FCZK_MODE 通过 列车按钮显示：XHD_LCTGBTN_SHOW 调车按钮显示：XHD_DCBTN_SHOW

    void DrawRect(
                  QPoint x1,
                  QPoint x2,
                  QPoint x3);
    void DrawJcCode();
    void DrawXHDButton(
        int nElapsed,
        int offset_x = 0,	//偏移量
        int offset_y = 0,	//偏移量
        double nDiploid = 1.0,	//放大缩小系数 默认参数为1.0
        int nShowState = 0x00);	//文字显示:GD_TEXT_SHOW  非常站控:FCZK_MODE 通过 列车按钮显示：XHD_LCTGBTN_SHOW 调车按钮显示：XHD_DCBTN_SHOW

    void DrawJudgeButton(
        int nElapsed,
        int offset_x = 0,	//偏移量
        int offset_y = 0,	//偏移量
        double nDiploid = 1.0,	//放大缩小系数 默认参数为1.0
        int nShowState = 0x00);	//文字显示:GD_TEXT_SHOW  非常站控:FCZK_MODE 通过 列车按钮显示：XHD_LCTGBTN_SHOW 调车按钮显示：XHD_DCBTN_SHOW

    void DrawJudgeButtonRange(
        int nElapsed,
        int offset_x = 0,	//偏移量
        int offset_y = 0,	//偏移量
        double nDiploid = 1.0,	//放大缩小系数 默认参数为1.0
        int nShowState = 0x00);	//文字显示:GD_TEXT_SHOW  非常站控:FCZK_MODE 通过 列车按钮显示：XHD_LCTGBTN_SHOW 调车按钮显示：XHD_DCBTN_SHOW

public:
    int times;          //闪烁计时
    int m_nLcIsDown;   //是否按下列车
    int m_nDcIsDown;   //是否按下调车
    int m_nTgIsDown;   //是否按下通过
    int m_nYdIsDown;   //是否按下引导

    //bool m_RangeVisible;				//范围显示
    bool m_bHaveTGbtn;       //是否有通过按钮
    bool m_bCanDC;           //信号机可调车（指列车信号机，调车信号机除外）
    bool m_bCanYD;           //信号机可引导（指出站信号机，进站信号机默认可以引导）
    bool m_bYDActive;        //引导按钮可操作
    bool m_bFirstCount; //引导第一次计时
    bool m_bCanPDJS; //信号机可坡道解锁
public:
    bool XHD_ds_HD;
    bool XHD_ds_LD;
    bool XHD_ds_YD;
    bool XHD_ds_BD;
    bool XHD_ds_AD;
    bool XHD_ds_YBD;

    bool isLCANFlash;//终端闪烁 列车
    bool isDCANFlash;//终端闪烁 调车
    bool isBTANFlash;//终端闪烁 变通
    bool bXHJTextShow;//信号机文字显示
    bool bZDFlash;//终端闪烁显示标志

public:
    bool m_DCXHDButtonShowFlag;//调车按钮是否显示
    QPoint m_DCXHDButtonShowPt;
    bool m_LCXHDButtonShowFlag;//列车按钮是否显示
    QPoint m_LCXHDButtonShowPt;
    bool m_YDXHDButtonShowFlag;//引导按钮是否显示
    QPoint m_YDXHDButtonShowPt;
    bool m_TGXHDButtonShowFlag;//通过信号灯按钮显示标志
    QPoint m_TGXHDButtonShowPt;
    bool m_XHDShowFlag;//信号灯是否显示
    QPoint m_XHDShowPt;
    bool m_PDJSFalg=false;//是否具有坡道解锁

    bool m_DCXHDButtonShowFlag_BTN = true;//调车按钮是否显示-界面按钮功能
    bool m_LCXHDButtonShowFlag_BTN = true;//列车按钮是否显示-界面按钮功能
    bool m_YDXHDButtonShowFlag_BTN = true;//引导按钮是否显示-界面按钮功能
    bool m_TGXHDButtonShowFlag_BTN = true;//通过信号灯按钮显示标志-界面按钮功能

public:
    int m_nFuncLockState;//封锁状态
    QPoint p1, p2, p12, p34, p56, center;
    int m_nRadius;
    int m_nHeight;
    int nTxtTGButtonCode;//通过按钮ID
    QRectF rectBtnLC, rectBtnTG, rectBtnDC, rectBtnYD, rectBtnBT; //列车接车进路按钮，列车通过按钮
    QRectF rectTxtLC, rectTxtTG;
    QString TxtDCButton,TxtLCButton,TxtYDButton;
    QString TxtTGButton;//通过按钮显示名称
    QString TxtBTButton;
    bool bTxtTGButtonshow=true;//文字显示标志

    int wordcolor;//文字颜色
    int wordsize;//文字大小
    //成员变量封装函数声明
    QRectF m_devRect_lamp1;//灯1的范围
//    bool m_RangeVisible_XHD; //设置设备范围显示（或高亮显示）
//    bool m_RangeVisible_LCAN; //设置设备范围显示（或高亮显示）
//    bool m_RangeVisible_TGAN; //设置设备范围显示（或高亮显示）
//    bool m_RangeVisible_DCAN; //设置设备范围显示（或高亮显示）
//    bool m_RangeVisible_Name; //设置设备范围显示（或高亮显示）

    QRectF rectBtnTG_word;//通过按钮文字
    bool isPLCZ=false;//批量操作

    //防错办基础属性
public:
    QString enexName;//出入口名称
    int direct;//方向 0双向/1下行进站/2上行出站/3下行出站/4上行进站。双线自动闭塞区段，按进站信号机的行别、属性 选择“下行进站/上行出站/下行出站/上行进站”。 单线自动闭塞、半自动闭塞区段，选择“双向”
    int allowOverLimit;//允许超限等级1允许一级超限 2允许二级超限 3允许三级超限 4-允许超级超限 0不允许超限列车
    int allowPassenger;//允许旅客列车1 允许旅客列车 0不允许旅客列车
    int allowFreight;//允许货物列车1 允许货物列车  0不允许货物列车

public:
    void setXHDType(QString strType);
    unsigned int getXHDType();//获取信号灯类型 进站 出站 调车
    void setSignalType(bool flag);
    bool getSignalType();//获取信号类型
    void setSafeLamp(QString strSafelamp);
    unsigned int getSafeLamp();//安全侧
    void setIsHigh(bool ishigh);
    bool getIsHigh();//高柱 矮柱
    void setXHDState(unsigned int state);
    unsigned int getXHDState();//获取信号灯状态 红灯 绿灯 黄灯 。。。
    bool getXHDState(int nState);//获取信号灯状态 红灯 绿灯 黄灯 。。。
    void setIsLCDown(int nFlag);
    int getIsLCDown();//列车按钮按下
    void setIsDCDown(int nFlag);
    int getIsDCDown();//调车按钮按下
    void setIsYDDown(int nFlag);
    int getIsYDDown();//引导按钮按下
    void setIsYDSD(bool flag);
    bool getIsYDSD();
    void setLCAN_Rect(QRectF rect);
    void setDCAN_Rect(QRectF rect);
    void setYDAN_Rect(QRectF rect);
    void setTGAN_Rect(QRectF rect);
    QRectF getLCAN_Rect();
    QRectF getTGAN_Rect();
    QRectF getDCAN_Rect();
    QRectF getYDAN_Rect();
    void setSignalDCAN(bool flag);
    bool getSignalDCAN();
    void setIsMD(bool flag);
    bool getIsMD();//点灯 灭灯
    void setIsLCANFB(bool flag);
    bool getIsLCANFB();//列车按钮封闭
    void setIsDCANFB(bool flag);
    bool getIsDCANFB();//调车按钮封闭
    void setIsYDANFB(bool flag);
    bool getIsYDANFB();//引导按钮封闭
    void setTimeType(int x);
    int getTimeType();
    void setTimeCount(int x);
    int getTimeCount();//倒计时
    bool getIsFB();//获取信号机封锁状态

    //功能函数声明
public:
    void XHDInit(int type); //信号灯初始化
    void XHDInit_Th_kb(); //标准界面信号灯初始化
    virtual void Draw(QPainter *painter,long nElapsed, double nDiploid,QPoint offset,int type);
    virtual void DrawM(QPainter *painter,long nElapsed, double nDiploid,QPoint offset,int type,int flag = 0);
    void Draw_Th_kb(QPainter *painter,long nElapsed, double nDiploid,QPoint offset, bool isMulti);
    virtual void Draw_ToolTip(QPainter *painter, double nDiploid);
    virtual unsigned int getDevType();
    virtual int moveCursor(QPoint p);
    virtual void setVollover(QPoint pt_Base);
    void setXHD_Color(QColor *xhd1_Color, QColor *xhd2_Color, int nElapsed);
    void xhd_StatePro();
    virtual void setDevStateToSafe();
    void Draw_Button(QPainter *painter,long nElapsed, double nDiploid, QPoint offset);
    void Draw_circle_Button(QPainter *painter,//画笔
                            QRectF m_ButtonRect,//按钮绘制区域
                            bool m_FCZKState,//非常站控状态
                            int m_ButtonIsDownState,//按钮是否按下
                            bool m_FSState,//封锁状态
                            bool m_bZDANFlash,//终端闪烁状态
                            int mTimeCount,//倒计时时间
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
                          long nElapsed,//闪烁读秒
                          double nDiploid,//放大倍数
                          QPoint offset);//绘制KSK方形按钮
     bool GetMousePoint(QPoint p);
     bool GetMousePoint_XHD(QPoint p);
     bool GetMousePoint_LCAN(QPoint p);
     bool GetMousePoint_DCAN(QPoint p);
     bool GetMousePoint_TGAN(QPoint p);
     bool GetMousePoint_Name(QPoint p);
     bool GetMousePoint_YDAN(QPoint p);

     QColor getLightState(int nType,int nElapsed);
};

#endif // CXHD_H
