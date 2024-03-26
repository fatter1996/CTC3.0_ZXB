#ifndef CJZ_H
#define CJZ_H

#include "BaseData.h"
class CJZ: public CBaseData
{
public:
    explicit CJZ();

//成员变量声明
private:
    int m_nGLXH;//关联信号设备编号
    QVector<int>vect_JJQD;//对应接发车口的接近区段编号
    bool isNX;//是否是逆向发车口//决定是否有改方按钮
    QPoint ArrowPt;//箭头坐标
    QPoint ArrowPtConst;
    int m_nAnNum;//按钮个数

    QPoint p_JZSYAN;//计轴使用按钮坐标
    QPoint p_JZSYANConst;
    QPoint p_JZTYAN;//计轴停用按钮坐标
    QPoint p_JZTYANConst;
    QPoint p_JZFLAN;//计轴复零按钮坐标
    QPoint p_JZFLANConst;
    QPoint p_BSQHAN;//闭塞切换按钮坐标
    QPoint p_BSQHANConst;
    QPoint p_MN;//邻站模拟的基准点坐标
    QPoint p_MNConst;

    QRect rect_JZSY;//计轴使用按钮
    QRect rect_JZTY;//计轴停用按钮
    QRect rect_JZFL;//计轴复零按钮
    QRect rect_BSQH;//闭塞切换按钮

    int isDown_JZSY;
    int isDown_JZTY;
    int isDown_JZFL;
    int isDown_BSQH;

    QPoint p_BSD_JZSY; //计轴使用灯
    QPoint p_BSD_JZSYConst;
    QPoint p_BSD_JZTY;  //计轴停用灯
    QPoint p_BSD_JZTYConst;
    QPoint p_BSD_JZFL;  //计轴复零灯
    QPoint p_BSD_JZFLConst;
    QPoint p_BSD_JZBJ;  //计轴报警灯
    QPoint p_BSD_JZBJConst;
    QPoint p_BSD_QJ;  //区间灯
    QPoint p_BSD_QJConst;

    QRect rect_FK;//方框区域
    QRect rect_FKConst;


    bool isBSDLight_JZSY;//计轴使用灯状态
    bool isBSDLight_JZTY;//计轴停用灯状态
    bool isBSDLight_JZFL;//计轴复零灯状态
    bool isBSDLight_JZBJ;//计轴报警灯状态
    bool isBSDLight_QJ;//计轴报警灯状态

    int m_nArrowStateJC;//接车箭头状态
    int m_nArrowStateFC;//发车箭头状态

public:
    //按钮
    QString JZSYAN_Name;
    QString JZTYAN_Name;
    QString JZFLAN_Name;
    QString BSQHAN_Name;

    int JZSYAN_QF;
    int JZTYAN_QF;
    int JZFLAN_QF;
    int BSQHAN_QF;

    bool haveJZSYAN;
    bool haveJZTYAN;
    bool haveJZFLAN;
    bool haveBSQHAN;

    //表示灯
    QString JZSYD_Name;
    QString JZTYD_Name;
    QString JZFLD_Name;
    QString JZBJD_Name;
    QString QJD_Name;

    bool haveJZSYD;
    bool haveJZTYD;
    bool haveJZFLD;
    bool haveJZBJD;
    bool haveQJ;

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

    bool bVisleJSQ;

    int m_nDownTime_JZFL_1;   //当按钮按下为1时持续时间
    int m_nDownTime_JZSY_1;
    int m_nDownTime_JZTY_1;
    int m_nDownTime_JZBSQH_1;
    int BSWordSize=12;//自动闭塞半自动闭塞功能按钮文字:文字大小;
    bool bBtnNameShow=true;//按钮名称显示
    bool bShowQFJS=true;//铅封计数显示标志
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
    void setJZSYANPt(QPoint pt);
    QPoint getJZSYANPt();
    void setJZTYANPt(QPoint pt);
    QPoint getJZTYANPt();
    void setJZFLANPt(QPoint pt);
    QPoint getJZFLANPt();
    void setBSQHANPt(QPoint pt);
    QPoint getBSQHANPt();
    void setMNPt(QPoint pt);
    QPoint getMNPt();
    void setIsDown_JZSY(int flag);
    int getIsDown_JZSY();
    void setIsDown_JZTY(int flag);
    int getIsDown_JZTY();
    void setIsDown_JZFL(int flag);
    int getIsDown_JZFL();
    void setIsDown_BSQH(int flag);
    int getIsDown_BSQH();
//    void setDownTime_FCFZ(int x);
//    int getDownTime_FCFZ();
//    void setDownTime_JCFZ(int x);
//    int getDownTime_JCFZ();
    void setBSD_JZSYPt(QPoint pt);
    QPoint getBSD_JZSYPt();
    void setBSD_JZTYPt(QPoint pt);
    QPoint getBSD_JZTYPt();
    void setBSD_JZFLPt(QPoint pt);
    QPoint getBSD_JZFLPt();
    void setBSD_JZBJPt(QPoint pt);
    QPoint getBSD_JZBJPt();
    void setBSD_QJPt(QPoint pt);
    QPoint getBSD_QJPt();
    void setRect_FK(QRect rect);
    QRect getRect_FK();
    void setIsBSDLight_JZSY(bool flag);
    bool getIsBSDLight_JZSY();
    void setIsBSDLight_JZTY(bool flag);
    bool getIsBSDLight_JZTY();
    void setIsBSDLight_JZFL(bool flag);
    bool getIsBSDLight_JZFL();
    void setIsBSDLight_JZBJ(bool flag);
    bool getIsBSDLight_JZBJ();
    void setIsBSDLight_QJ(bool flag);
    bool getIsBSDLight_QJ();
    void setArrowStateJC(int x);
    int getArrowStateJC();
    void setArrowStateFC(int x);
    int getArrowStateFC();

public:
    virtual void Draw(QPainter *painter,long nElapsed, double nDiploid,QPoint offset,int type);
    virtual void Draw_ToolTip(QPainter *painter, double nDiploid);
    virtual unsigned int getDevType();
    virtual int moveCursor(QPoint p);
    virtual void setVollover(QPoint pt_Base);

    void Draw_Th_kb(QPainter *painter,long nElapsed, double nDiploid,QPoint offset);
    //void DrawZDBSArrow(QPainter *painter,long nElapsed, double nDiploid,QPoint offset);
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
    void Draw_Rect_Light(QPainter *painter,//画笔
                         QPointF LightPoint,//指示灯中心坐标
                         QString rectTextShow, //文字显示
                         bool Light_color,//指示灯状态
                         int light_type,//指示灯类型
                         long nElapsed,//闪烁读秒
                         double nDiploid,//放大倍数
                         QPoint offset);//绘制KSK方形按钮
    QColor getLightState(int nType,int nElapsed);

    bool GetMousePoint_BSQH(QPoint p);
    bool GetMousePoint_JZSY(QPoint p);
    bool GetMousePoint_JZTY(QPoint p);
    bool GetMousePoint_JZFL(QPoint p);
    QString GetANIsDown(int nType);
    void SetANIsDown(int nType);
};

#endif // CJZ_H

