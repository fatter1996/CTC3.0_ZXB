#ifndef CZDBS_H
#define CZDBS_H

#include "BaseData.h"
class CZDBS: public CBaseData
{
public:
    explicit CZDBS();

    //成员变量声明
private:
    int m_nGLXH;//关联信号设备编号
    QVector<int>vect_JJQD;//对应接发车口的接近区段编号
    bool isNX;//是否是逆向发车口//决定是否有改方按钮
    QPoint ArrowPt;//箭头坐标
    QPoint ArrowPtConst;
    int m_nAnNum;//按钮个数

    QPoint p_GFAN;//改方按钮坐标
    QPoint p_GFANConst;
    QPoint p_ZFZAN;//总辅助按钮坐标
    QPoint p_ZFZANConst;
    QPoint p_FCFZAN;//发车辅助按钮坐标
    QPoint p_FCFZANConst;
    QPoint p_JCFZAN;//接车辅助按钮坐标
    QPoint p_JCFZANConst;
    QPoint p_MN;//邻站模拟的基准点坐标
    QPoint p_MNConst;

    QRect rect_GF;//改方按钮
    QRect rect_ZFZ;//总辅助按钮
    QRect rect_FCFZ;//发车辅助按钮
    QRect rect_JCFZ;//接车辅助按钮

    int isDown_GF;
    int isDown_ZFZ;
    int isDown_FCFZ;
    int isDown_JCFZ;

    int m_nDownTime_FCFZ;   //0xFF时不显示
    int m_nDownTime_JCFZ;

    QPoint p_BSD_YXFC; //允许发车灯
    QPoint p_BSD_YXFCConst; //允许发车灯
    QPoint p_BSD_FZ;  //辅助表示灯
    QPoint p_BSD_FZConst;  //辅助表示灯
    QPoint p_BSD_SG;  //闪光表示灯或区轨灯
    QPoint p_BSD_SGConst;  //闪光表示灯或区轨灯
    QPoint p_BSD_QJ;  //区间表示灯
    QPoint p_BSD_QJConst;  //区间表示灯

    QRect rect_FK;//方框区域
    QRect rect_FKConst;


    int isBSDLight_YXFC;//允许发车灯状态
    int isBSDLight_SG;//闪光灯状态或区轨灯
    int isBSDLight_FZ;//辅助灯状态
    int isBSDLight_QJ;//区间灯状态

    int m_nArrowStateJC;//接车箭头状态
    int m_nArrowStateFC;//发车箭头状态

public:
    QString GFAN_Name;
    QString ZFZAN_Name;
    QString FCFZAN_Name;
    QString JCFZAN_Name;

    int GFAN_QF;
    int ZFZAN_QF;
    int FCFZAN_QF;
    int JCFZAN_QF;
    int ZDBS_QF;

    bool haveGFAN_QF;
    bool haveZFZAN_QF;
    bool haveFCFZAN_QF;
    bool haveJCFZAN_QF;
    bool haveZDBS_QF;

    bool haveZFZAN;
    bool haveFCFZAN;
    bool haveJCFZAN;

    QString YXFC_Name;
    QString FZ_Name;
    QString SG_Name;
    QString QJ_Name;

    bool haveYXFC;//允许发车灯绘制标志
    bool haveFZ;//辅助灯绘制标志
    bool haveSG;//事故灯绘制标志
    bool haveQJ;//区间灯绘制标志
    bool bShowhave=true;//灯文字显示标志

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

    bool haveWord;//方向文字显示标志
    QPoint ptWord;
    QRect rect_Word;//文字区域
    QString WordName;

    int QJDColorType;//区间灯点灯颜色类型
    int SGDColorType;//事故灯点灯颜色类型
    int FZDColorType;//辅助灯点灯颜色类型
    int YXFCDColorType;//允许点灯发车灯颜色类型

    int m_nDownTime_ZFZ_1;   //当按钮按下为1时持续时间
    int m_nDownTime_JCFZ_1;
    int m_nDownTime_FCFZ_1;
    int m_nDownTime_GF_1;
    bool bShowQFJS=true;//铅封计数显示标志
    bool bBtnNameShow=true;//按钮名称显示
    bool bVisleJSQ;

    bool bStaSXLORR=false;//站场上下行方向标志
    int BSWordSize=12;//自动闭塞半自动闭塞功能按钮文字:文字大小;
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
    void setGFANPt(QPoint pt);
    QPoint getGFANPt();
    void setZFZANPt(QPoint pt);
    QPoint getZFZANPt();
    void setFCFZANPt(QPoint pt);
    QPoint getFCFZANPt();
    void setJCFZANPt(QPoint pt);
    QPoint getJCFZANPt();
    void setMNPt(QPoint pt);
    QPoint getMNPt();
    void setIsDown_GF(int flag);
    int getIsDown_GF();
    void setIsDown_ZFZ(int flag);
    int getIsDown_ZFZ();
    void setIsDown_FCFZ(int flag);
    int getIsDown_FCFZ();
    void setIsDown_JCFZ(int flag);
    int getIsDown_JCFZ();
    void setDownTime_FCFZ(int x);
    int getDownTime_FCFZ();
    void setDownTime_JCFZ(int x);
    int getDownTime_JCFZ();
    void setBSD_YXFCPt(QPoint pt);
    QPoint getBSD_YXFCPt();
    void setBSD_SGPt(QPoint pt);
    QPoint getBSD_SGPt();
    void setBSD_FZPt(QPoint pt);
    QPoint getBSD_FZPt();
    void setBSD_QJPt(QPoint pt);
    QPoint getBSD_QJPt();
    void setRect_FK(QRect rect);
    QRect getRect_FK();
    void setIsBSDLight_YXFC(int flag);
    int getIsBSDLight_YXFC();
    void setIsBSDLight_SG(int flag);
    int getIsBSDLight_SG();
    void setIsBSDLight_FZ(int flag);
    int getIsBSDLight_FZ();
    void setIsBSDLight_QJ(int flag);
    int getIsBSDLight_QJ();
    void setArrowStateJC(int x);
    int getArrowStateJC();
    void setArrowStateFC(int x);
    int getArrowStateFC();

public:
    virtual void Draw(QPainter *painter,long nElapsed, double nDiploid,QPoint offset,int type);
    virtual void DrawM(QPainter *painter,long nElapsed, double nDiploid,QPoint offset,int type,int flag = 0);
    virtual void Draw_ToolTip(QPainter *painter, double nDiploid);
    virtual unsigned int getDevType();
    virtual int moveCursor(QPoint p);
    virtual void setVollover(QPoint pt_Base);

    void Draw_Th_kb(QPainter *painter,long nElapsed, double nDiploid,QPoint offset, bool isMulti = false);
    void DrawZDBSArrow(QPainter *painter,long nElapsed, double nDiploid,QPoint offset);
    void DrawQFRect(QPainter *painter,QPointF QFPoint,int m_nQFNumber,long nElapsed, double nDiploid,QPoint offset);
    void Draw_circle_Button(QPainter *painter,//画笔
                            QRectF m_ButtonRect,//按钮绘制区域
                            bool m_FCZKState,//非常站控状态
                            int m_ButtonIsDownState,//按钮是否按下
                            bool m_FSState,//封锁状态
                            bool m_bZDANFlash,//终端闪烁状态
                            int mTimeCount,//倒计时时间
                            bool HaveQF,//是否有铅封
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
                          bool HaveQF,//是否有铅封
                          QString rectTextShow,//显示按钮文字
                          bool m_RangeVisible,//按钮是否高亮
                          long nElapsed,//闪烁读秒
                          double nDiploid,//放大倍数
                          QPoint offset);//绘制KSK方形按钮
    void Draw_circle_Light(QPainter *painter,//画笔
                           QPointF LightPoint,//指示灯中心坐标
                           QString rectTextShow, //文字显示
                           bool Light_color,//指示灯状态
                           int colorType,//颜色类型
                           long nElapsed,//闪烁读秒
                           double nDiploid,//放大倍数
                           QPoint offset);//绘制指示灯
    QColor getLightState(int nType,int nElapsed);
    //设置高亮状态
    void  SetRangeVisible(bool b);
    //获取高亮状态
    bool GetRangeVisible();
    void SetANIsDown(int nType);
    QString GetANIsDown(int nType);
    bool GetMousePoint_ZFZ(QPoint p);
    bool GetMousePoint_JCFZ(QPoint p);
    bool GetMousePoint_FCFZ(QPoint p);
    bool GetMousePoint_GF(QPoint p);
};

#endif // CZDBS_H
