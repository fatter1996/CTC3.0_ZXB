#ifndef CGD_H
#define CGD_H

#include <QPaintEvent>
#include <QPainter>
#include <QtMath>
#include "qd.h"
class CGD : public CQD
{
public:
    explicit CGD();

    //成员变量声明
private:
public:
    QPointF p1, p2, p3, p4, p12, p34, pz12, pz34;
    QPointF p1Const, p2Const, p3Const, p4Const, p12Const, p34Const, pz12Const, pz34Const;   //缩放及偏移使用
    unsigned int m_nGDType;             //轨道区段类型
    unsigned int m_nZ;                  //折点个数

    bool isGDFLBL;    //是否分路不良
    bool m_nGDFLBLKX; //是否分路不良空闲
    bool isGDFLBL_xjFlag=false;//区间逻辑检查室外分路不良
    bool bJSJSetFLBLFlag=false;//教师机设置分路不良
    //int GLB_QDleft;
    //int GLB_QDright;
    //bool isSXDC;
    bool isZXGD;
    bool isCXGD;
    bool isJJGD;
    //bool GD_LCode;
    //bool GD_RCode;
    //QString CarrierFrequency;
    //int Dir_DMH;
    //int m_nCodeFHXH;   //区间闭塞分区小轨道电路关联防护信号机
    //bool isNoShow;
    QString m_strMark1;
    QString m_strMark2;

public:
    //防溜信息
    bool bIronShoesSetS=false;//上行（东部）铁鞋设置
    bool bIronShoesSetX=false;//下行（西部）铁鞋设置
    int nManualBrakeS=0;//上行（东部） 人力制动机 or 人力制动机紧固器 0:两者皆无 1：人力制动机 2：人力制动机紧固器 3:防溜枕木
    int nManualBrakeX=0;//下行（西部） 人力制动机 or 人力制动机紧固器 0:两者皆无 1：人力制动机 2：人力制动机紧固器 3:防溜枕木
    QString m_sTrainInfoFL="";//存车信息 防溜

    QString m_sTrainInfoShow="";//存车信息 股道栏显示

    int m_nLAntiSlipType = 0;//左侧防溜类型（接车口）
    int m_nLTxNum=0;//左侧铁鞋号
    int m_nLJgqNum = 0;//左侧紧固器号
    int m_nLJnMeters = 0;//左侧警内米数
    int m_nRAntiSlipType = 0;//右侧防溜类型（发车口）
    int m_nRTxNum=0;//右侧铁鞋号
    int m_nRJgqNum = 0;//右侧紧固器号
    int m_nRJnMeters = 0;//右侧警内米数
    int m_nTrainNums = 0;//存车信息
    //站存车信息
    QString m_sTrainInfoZCC="";//存车信息 站存车
public:
    QString m_strCheCiNum;//车次
    QString m_strCheCiNumCTC;//人工加载的车次
    bool m_nCheciLost;//车次是否丢失
    bool m_nCheciLost_xj;//区间逻辑检查时车次是否丢失
    bool m_bElectric;//电力牵引
    int m_nSXCheCi;//1右行 0左行
    bool m_bLCTW;//列车挺稳标志
    int m_nKHType;//客货类型：客车/货车
    int m_nSpeed;

    bool isLock;          //是否锁闭
    bool isPowerCut;      //是否停电
    bool isSpeedLimit;    //是否限速（临时限速）
    bool isFS;     //是否封锁
    bool isMA;    //移动授权
public:
    bool m_bHaveGrayBack;//有无灰色背景（车站中间的股道才会有）
    //车次窗相关:
    bool bCCKIsUp;//车次框在上方
    bool m_nVisibleCheci; //车次窗显示状态
    QRect m_rectCheCi; //车次窗位置
    int nWbWidth;//尾部停稳标志宽度
    int nWbWidthForDraw;//尾部停稳标志宽度

//    int flblStatus;//分路不良,0无，1分路，2确认空闲
    int bsqdfmCode;//闭塞区间发送码
    int bsqdfmDirection;//闭塞区间发码方向，0未知，1向左，2向右
    int speedLimitStatus;//限速状态,0无限速,1定位限速表示稳定,2反位限速表示稳定
    //bool m_RangeVisible; //股道范围显示（或高亮显示）

    bool bShow_Jyj;//绝缘节显示
    bool bTextShowGD;//股道名称显示
    bool bTextShowWCQD=true;//无岔区段名称显示
    bool bTextShowJJQD=true;//接近区段名称显示
    bool isVollover = false; //站场是否翻转
    int wordcolor;//文字颜色
    int wordsize;//文字大小

    int bQJxhdShow;//区间信号灯是否显示
    bool isZDQJXHDDirFlag;  //是否指定区间信号机方向标识
    bool isZDQJXHDRight;    //是否指定区间信号机方向为向右----FLASE-向左  TRUE-向右
    bool m_bBSQDFMRight;//区间发码方向是否向右（0左  1右）
    int m_nBSQDFMCode;
    bool bStaSXLORR;
    bool isPLCZ=false;//批量操作
    //防错办基础属性
public:
    int gdId = NULL;//股道id
    int gdAttr;//线路性质（0 正线 1到发线）
    int jfcDir;//接发车方向（0 上行 1 下行 2 上下行）
    int jfcAttr;//接发车类型（0 客车 1 货车 2客货车）
    int overLimit;//超限（0 不能接发超限列车 1 一级超限 2 二级超限 3 三级超限 4超级超限）
    int platform;//站台（站台 1 高站台 2 低站台 0 无）
    int isCRH;//允许动车组（1 允许动车组 0不允许动车组）
    int isWater;//上水设备（1 上水设备 0无上水设备）
    int isBlowdown;//排污设备（1 排污设备  0无排污设备）
    int army;//军运（1可军用列车  0不能接军用列车）

//成员变量封装函数声明
public:
    void setGDType(QString str_type);
    unsigned int getGDType();
    void setZ(unsigned int nZ);
    unsigned int getZ();
    void setGDFLBL(bool nFlag);
    bool getGDFLBL();//分路不良
    void setGDFLBL_QJLJJC(bool nFlag);
    bool getGDFLBL_QJLJJC();
    void setp1(QPointF pt);
    QPointF getp1();
    void setp2(QPointF pt);
    QPointF getp2();
    void setp3(QPointF pt);
    QPointF getp3();
    void setp4(QPointF pt);
    QPointF getp4();
    void setp12(QPointF pt);
    QPointF getp12();
    void setp34(QPointF pt);
    QPointF getp34();
    void setpz12(QPointF pt);
    QPointF getpz12();
    void setpz34(QPointF pt);
    QPointF getpz34();
    //void setGLB_QDleft(int glb);
    //int getGLB_QDleft();
    //void setGLB_QDright(int glb);
    //int getGLB_QDright();
    //void setIsSXDC(bool flag);
    //bool getIsSXDC();
    void setIsZXGD(bool flag);
    bool getIsZXGD();//正线股道
    void setIsCXGD(bool flag);
    bool getIsCXGD();//侧线股道
//    void setIsJJGD(bool flag);
//    bool getIsJJGD();//接近区段 区间股道
    //void setGD_LCode(int code);
    //int getGD_LCode();
    //void setGD_RCode(int code);
    //int getGD_RCode();
    //void setCarrierFrequency(QString str);
    //QString getCarrierFrequency();
    //void setDir_DMH(bool flag);
    //int getDir_DMH();

    //void setCodeFHXH(int fhxh);
    //int getCodeFHXH();
    //void setisNoShow(bool flag);
    //bool getisNoShow();
    void setLock(bool nFlag);
    bool getLock();//锁闭
    void setFS(bool nFlag);
    bool getFS();//封锁/解封
    void setSpeedLimit(bool nFlag);
    bool getSpeedLimit();//临时限速
    void setMarkStr1(QString str);
    QString getMarkStr1();
    void setMarkStr2(QString str);
    QString getMarkStr2();
    void setPowerCut(bool nFlag);
    bool getPowerCut();//股道无电
    void setMA(bool nFalg);
    bool getMA();//MA显示
    void SetJyj(bool b);//设置绝缘节
    //成员功能函数声明
public:
    void GDInit(int type); //股道初始化
    virtual void Draw(QPainter *painter, long nElapsed, double nDiploid,QPoint offset,int type);
    void Draw_Th_kb(QPainter *painter, long nElapsed, double nDiploid,QPoint offset);
    void Draw_FLBL_WBX(QPainter *painter,long nElapsed,QPointF pt1,QPointF pt2, double nDiploid);//绘制分录不良外包线
    void Draw_FLBL_WBX_QJLJJC(QPainter *painter,long nElapsed,QPointF pt1,QPointF pt2, double nDiploid);//绘制分录不良外包线
    void Draw_WBX(QPainter *painter,QPointF pt1,QPointF pt2, double nDiploid,int nLineW,QColor color,int nSpace);//绘制设备外包线
    void Draw_WBX_Light(QPainter *painter,QPointF pt1,QPointF pt2, double nDiploid,int nLineW,QColor color,int nSpace);//绘制高亮外包线
    void Draw_GDAntiSlip(QPainter *painter, long nElapsed, double nDiploid,QPoint offset);//绘制股道防溜标志
    virtual void Draw_ToolTip(QPainter *painter, double nDiploid);
    virtual unsigned int getDevType();
    virtual int moveCursor(QPoint p);
    virtual void setVollover(QPoint pt_Base);
    virtual void setDevStateToSafe();
    bool GetMousePoint(QPoint p);
    QColor getLightState(int nType,int nElapsed);
    //void gd_StatePro();
//    void DrawTrainWindown(QPainter *painter,double nDiploid,int nLineW,QColor color,double nCheciDiploid,int nShowState);//绘制车次窗
//    void DrawTrainNum(QPainter *painter,long nElapsed,double nDiploid,int nLineW,QColor color,double nCheciDiploid,int nShowState);//绘制车次
    //绘制闭塞区间低频发码
    void DrawBSQJCode(
                     QPainter *painter,
                     long nElapsed,
                     double nDiploid,
                     QPoint offset
                     );
    //赋值低频发码
    void SetBSQDFMCode(int nQDFMCode);
    //股道防溜属性初始化
    void IntiSlip();
    //股道站存车信息
    void IntiStationStorage();
    //获取存车信息显示
    bool GetTrainInfoFLShow();
};

#endif // CGD_H
