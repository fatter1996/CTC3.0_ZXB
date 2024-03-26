#ifndef CBZDBS_H
#define CBZDBS_H

#include "BaseData.h"
class CBZDBS: public CBaseData
{
public:
    explicit CBZDBS();

//成员变量声明
private:
    int m_nGLXH;//关联信号设备编号
    QVector<int>vect_JJQD;//对应接发车口的接近区段编号
    bool isNX;//是否是逆向发车口
    QPoint ArrowPt;//箭头坐标
    QPoint ArrowPtConst;
    int m_nAnNum;//按钮个数

    QPoint p_BSQHAN;//闭塞切换按钮坐标
    QPoint p_BSQHANConst;
    QPoint p_BSAN;//闭塞按钮坐标
    QPoint p_BSANConst;
    QPoint p_SGAN;//事故按钮坐标
    QPoint p_SGANConst;
    QPoint p_FYAN;//复原按钮坐标
    QPoint p_FYANConst;
    QPoint p_MN;//邻站模拟的基准点坐标
    QPoint p_MNConst;

    QRect rect_BSQH;//闭塞切换按钮
    QRect rect_BS;//闭塞按钮
    QRect rect_SG;//事故按钮
    QRect rect_FY;//复原按钮

    bool isDown_BSQH;
    bool isDown_BS;
    bool isDown_SG;
    bool isDown_FY;

//    int m_nDownTime_FCFZ;   //0xFF时不显示
//    int m_nDownTime_JCFZ;

    QPoint p_BSD_ZY; //占用灯坐标
    QPoint p_BSD_ZYConst;
    QPoint p_BSD_KX;  //空闲灯坐标
    QPoint p_BSD_KXConst;
    QPoint p_BSD_RGBS;  //人工闭塞灯坐标
    QPoint p_BSD_RGBSConst;
    QPoint p_BSD_ZDBS;  //自动闭塞灯坐标
    QPoint p_BSD_ZDBSConst;
    QRect rect_FK;//方框区域
    QRect rect_FKConst;

    bool isBSDLight_ZY;//占用灯状态
    bool isBSDLight_KX;//空闲灯
    bool isBSDLight_RGBS;//人工闭塞灯状态
    bool isBSDLight_ZDBS;//自动闭塞灯状态

    int m_nArrowStateJC;//接车箭头状态
    int m_nArrowStateFC;//发车箭头状态

public:
    QString BSQHAN_Name;
    QString BSAN_Name;
    QString SGAN_Name;
    QString FYAN_Name;

    int BSQHAN_QF;
    int BSAN_QF;
    int SGAN_QF;
    int FYAN_QF;
    int BZDBS_QF;
    bool haveBSQHAN_QF;//闭塞切换铅封标志
    bool haveBSAN_QF;//闭塞按钮铅封标志
    bool haveSGAN_QF;//事故按钮铅封标志
    bool haveFYAN_QF;//复原按钮铅封标志
    bool haveBZDBS_QF;//

    bool haveBSQHAN;
    bool haveBSAN;
    bool haveSGAN;
    bool haveFYAN;

    QString ZYD_Name;
    QString KXD_Name;
    QString RGBSD_Name;
    QString ZDBSD_Name;

    bool haveZYD;
    bool haveKXD;
    bool haveRGBSD;
    bool haveZDBSD;

    QPoint arrayArrowLeft[8];	//左箭头
    QPoint arrayArrowRight[8];	//右箭头
    QString ArrowLeftName;
    QString ArrowRightName;

    //bool m_RangeVisible;//鼠标移动至设备上以后的高亮显示
    int ButtonForm;//按钮形状,0为圆,1为方;

    bool haveFK;
    QPoint ptFK;
    int FK_length;
    int FK_breath;

    bool haveWord;
    QPoint ptWord;
    QRect rect_Word;//文字区域
    QString WordName;
    bool bBtnNameShow=true;//按钮名称显示
    bool bShowQFJS=true;//铅封计数显示标志
    bool bVisleJSQ;
    int BSWordSize=12;//自动闭塞半自动闭塞功能按钮文字:文字大小;
    bool bStaSXLORR=false;//站场上下行方向标志

public:
    void setGLXH(int x);
    int getGLXH();
    void addVectJJQD(int x);
    void setVectJJQD(QVector<int> x);
    QVector<int> getVectJJQD();
    void setIsNX(bool flag);
    bool getIsNX();
    void setArrowPt(QPoint pt);
    QPoint getArrowPt();
    void setAnNum(int x);
    int getAnNum();

    void setBSQHANPt(QPoint pt);
    QPoint getBSQHANPt();
    void setBSANPt(QPoint pt);
    QPoint getBSANPt();
    void setSGANPt(QPoint pt);
    QPoint getSGANPt();
    void setFYANPt(QPoint pt);
    QPoint getFYANPt();
    void setMNPt(QPoint pt);
    QPoint getMNPt();
    void setIsDown_BSQH(bool flag);
    bool getIsDown_BSQH();
    void setIsDown_BS(bool flag);
    bool getIsDown_BS();
    void setIsDown_SG(bool flag);
    bool getIsDown_SG();
    void setIsDown_FY(bool flag);
    bool getIsDown_FY();
//    void setDownTime_FCFZ(int x);
//    int getDownTime_FCFZ();
//    void setDownTime_JCFZ(int x);
//    int getDownTime_JCFZ();
    void setBSD_ZYPt(QPoint pt);
    QPoint getBSD_ZYPt();
    void setBSD_KXPt(QPoint pt);
    QPoint getBSD_KXPt();
    void setBSD_RGBSPt(QPoint pt);
    QPoint getBSD_RGBSPt();
    void setBSD_ZDBSPt(QPoint pt);
    QPoint getBSD_ZDBSPt();
    void setRect_FK(QRect rect);
    QRect getRect_FK();
    void setIsBSDLight_ZY(bool flag);
    bool getIsBSDLight_ZY();
    void setIsBSDLight_KX(bool flag);
    bool getIsBSDLight_KX();
    void setIsBSDLight_RGBS(bool flag);
    bool getIsBSDLight_RGBS();
    void setIsBSDLight_ZDBS(bool flag);
    bool getIsBSDLight_ZDBS();
    void setArrowStateJC(int x);
    int getArrowStateJC();
    void setArrowStateFC(int x);
    int getArrowStateFC();


public:
    virtual void Draw(QPainter *painter,long nElapsed, double nDiploid,QPoint offset,int type);
    virtual void DrawM(QPainter *painter,long nElapsed, double nDiploid,QPoint offset,int type,int flag);
    virtual void Draw_ToolTip(QPainter *painter, double nDiploid);
    virtual unsigned int getDevType();
    virtual int moveCursor(QPoint p);
    virtual void setVollover(QPoint pt_Base);

    void Draw_Th_kb(QPainter *painter,long nElapsed, double nDiploid,QPoint offset,bool isMulti);
    void DrawZDBSArrow(QPainter *painter,long nElapsed, double nDiploid,QPoint offset);
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
    void Draw_Rect_Light(QPainter *painter,//画笔
                         QPointF LightPoint,//指示灯中心坐标
                         QString rectTextShow, //文字显示
                         bool Light_color,//指示灯状态
                         long nElapsed,//闪烁读秒
                         double nDiploid,//放大倍数
                         QPoint offset);//绘制KSK方形按钮
    QColor getLightState(int nType,int nElapsed);
    //设置按钮按下抬起状态
    void SetANIsDown(int nType);
    QString GetANIsDown(int nType);
    //根据按下按钮类型获取铅封状态
    bool GetQFByAnType(int nytpe);

    bool GetMousePoint_BSQH(QPoint p);
    bool GetMousePoint_BS(QPoint p);
    bool GetMousePoint_SG(QPoint p);
    bool GetMousePoint_FY(QPoint p);
};

#endif // CBZDBS_H

