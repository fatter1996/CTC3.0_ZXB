#ifndef MYSTATION_H
#define MYSTATION_H

#include <QObject>
#include "BaseDataPro/station.h"
#include <QMenu>
#include <QContextMenuEvent>
#include <QThread>
#include "train.h"
#include "trainnumwnd.h"
#include "routeprewnd.h"
#include "symbol6perthousand.h"
#include "trainrouteorder.h"
#include "stageplan.h"
#include "trafficlog.h"
#include "dispatchorderstation.h"
#include "dispatchorderdispatcher.h"
#include "dispatchorderlocomotive.h"
#include "textannotation.h"
#include "GDData/gddata.h"
#pragma execution_character_set("utf-8")

//自定义车站类，车站的业务逻辑类
class MyStation : public CStation
{
    Q_OBJECT
public:
    MyStation();
    ~MyStation();

public:
    //线程是否启动
    bool isThreadStart;
    //线程计时器
    int  ticksOfThread;
public slots:
    //开始工作
    void startWorkSlot();
signals:
    //结束工作
    void endWorkSignal();
    //信号-发送数据给CTC服务终端
    void sendDataToCTCServerSignal(MyStation* pMyStation, QByteArray dataArray,int len);
    //用户登录信息更新
    void UserUpDate(int n,QString name,int nstate);
public:
    //绘制站场
    void draw(QPainter *painter,long nElapsed, double factor = 1.0f, bool isMuti = false);
    void SetDiploid(int n, double &nDiploid);

    //根据道岔分路不良状态设置道岔区段名称显示颜色
    void SetDCQDTextColor();
private:
    //绘制CTC站场
    void drawMyStation(QPainter *painter,long nElapsed, double nDiploid=1.0f, double nDiploidCheCi=1.0f, bool bMulti=false);


public:
    double nDiploid;//单站缩放系数
    double nDiploidCheCi;//车次缩放系数
    double nDiploidMuti;//缩放系数-站间透明
    double nDiploidCheCiMuti;//车次缩放系数-站间透明
    bool isOverturn = false;//站场翻转

    QVector<DispatchOrderStation*> m_ArrayDisOrderRecv;//本站接收的调度命令
    QVector<DispatchOrderDispatcher*> m_ArrayDisOrderDisp;//调度台调度命令
    QVector<DispatchOrderLocomotive*> m_ArrayDisOrderLocomot;//机车调度命令
    QVector<StagePlan*> m_ArrayStagePlan;//阶段计划
//    QVector<CStagePlan*> m_ArrayStagePlanChg;//修改的阶段计划
    QVector<TrafficLog*> m_ArrayTrafficLog;//行车日志数组
    QVector<TrainRouteOrder*> m_ArrayRouteOrder;//列车进路序列数组
    QVector<CGD*> m_ArrayGDAntiSlip;//股道防溜
    QVector<GDData*> m_ArrayGDData;//占线版股道防溜
    QVector<Train*> m_ArrayTrain;//列车车次信息
    QVector<TrainNumWnd*> m_ArrayTrainNumWnd;//车次窗
    QVector<RoutePreWnd*> vectRoutePreWnd;//进路预告窗
    QVector<RoutePreWnd*> vectRoutePreWndTemp;//进路预告窗-临时
    QVector<Symbol6PerThousand*> vectSymbol6PerThousand;//千分之六标志
    //QVector<TextAnnotationWidget*> vectTextAnnotationWidget;//文字标注
    QVector<TextAnnotation*> vectTextAnnotation;//文字标注
    QVector<CGD*> vectGDAttr;//（防错办）股道属性
    QVector<CXHD*> vectGatewayAttr;//（防错办）出入口属性
    QVector<ZoneData> m_vecZoneData;//批量操作工作区
    QVector<InterlockRoute*> vectInterlockRoute;//联锁进路表
//    //******** 本站联锁地址端口 ********
//    QString lsAddr;//IP地址
//    int lsPort;//端口
//    //******** 本站CTC地址端口 ********
//    QString ctcAddr[10];//IP地址
//    int ctcPort[10];//端口
//    int  m_nTimeCTCBreak[10];//本站CTC终端通信中断计时器
//    //******** 本站占线板地址端口 ********
//    QString zxbAddr[10];//IP地址
//    int zxbPort[10];//端口
//    int m_nTimeBoardBreak[10];//本站占线板终端通信中断计时器
//    //******** 本站培训软件端口 ********
//    QString trainning_Addr;//IP地址
//    int trainningPort;//端口

    /*********ctc终端 yzr **************/
    int UdpMode;
    QString ServerIP;
    int ServerPort1;
    int ServerPort2;
    int LocalPort;
    int ABNUM=1;//岗位码
    int nShowView = 1;
    QString strZXBPath;

    QString strAntislip;//防溜铁鞋设置显示(东/西部铁鞋;上/下行铁鞋等)
    bool bSleepers = true;

public:
    typedef struct  //股道可到达的延续终端
    {
       QStringList strArrGD; //股道列表
       QStringList strArrYXEnd;//延续终d列表
       QString strDefaultEnd; //默认延续终端
    }GDYXJL_T;
    typedef struct            //延续进路定义
    {
       QString  strXHDbegin; //延续进路始端
       int      nGDYXJLCount;
       GDYXJL_T GD_YXJL[50];//股道们相应的延续终d
    }YXJLGroup_T;

    typedef struct           //接发车口定义
    {
       QString strJCKName;//接车口信号机名称//CString strName;
       int     nJCKCode;  //接车口设备号//int     nCode;
       QString strFCKName;//发车口信号机名称
       int     nFCKCode;  //发车口设备号
       QString strDirectSta;//CString strDirectSta; //接发车口方向车站名称（车站名称）
       QString strJCKjucJJQD;//接车口关联的接近区段（1个）
       QStringList strArrJckJJQD;//接车口关联的接近区段数组
       QString     strFCKjucLQQD;//发车口关联的接近区段（1个）
       QStringList strArrFckLQQD;//发车口关联的离去区段数组
       QStringList strArrJckJJQD_Temp;//接车口关联的接近区段数组
       int bJCKAdvance = 3;
       int bFCKAdvance = 3;
    }JFCK_T;

    typedef  struct
    {
         QString DCname;
         QPoint p;
         QPoint pConst;
         int m_ncode;
         bool isAQXSpeechAlert;
         int m_nShowType;
         int m_nShowNameType;
     }AQXBJ;

    typedef struct //ConfigInfo //车站配置信息定义结构体
    {
        bool isHavePXRJ;   //2020.7.9-BJT-记录是否有培训软件
        bool isHaveDDFZ;   //2020.7.17-BJT-记录是否有调度仿真
        QString strStaName;       //车站名
        int StationID;
        QSize initialSize;   //站场界面显示大小
        int     nCTCDevType;//CTC单机版0x00，值班员终端0x01,信号员终端0x02
        int     nBoardType;       //T3Term占线板单机版0x00，值班员终端0x01,信号员终端0x02
        bool    bGaoTie;          //高铁站标志
        bool    bStaSXLORR;       //上下行方向,站场下行方向对应左边1，否则为0
        bool    bChgModeNeedJSJ;  //控制模式转换是否需要教师机（中心）同意
        int     YXJLBeginNum;
        YXJLGroup_T YXJLGroup[40];//延续进路始端和默认终端信息
        int     XF_nCode;        //XF/XN...（下反/下逆等）十进制设备编号
        int     X_nCode;         //下行接车信号机十进制设备编号
        QString X_JuncXHD;       //下行接车信号机名称
        int     SF_nCode;        //SF/SN...（上反/上逆等）十进制设备编号
        int     S_nCode;         //上行接车信号机十进制设备编号
        QString S_JuncXHD;       //上行接车信号机名称

        int JCKCount;
        QVector<JFCK_T> JFCKInfo;//接发车口信息
        int BTJLCount;//有变通进路的进路数目
        QString BTJLArray[300];//(基本进路|变通进路|变通进路)

        int  RouteDeleteTime;//进路序列删除时间秒
        bool bBTJLNewStyle;//变通进路新格式
        QStringList XianLuSuoXHDArray;//线路所信号机 //BOOL bXianLuSuo;//线路所
        bool bQJAllLock;//区间封锁单个或全部
        bool bXHDDSBJStatus;//站场状态是否包含信号机灯丝断丝状态

        bool HaveCLPZFlag=false;//场联配置//机务段配置相关：0为无机务段，1为有机务段

        QString DCGZBtnNameJG;//道岔故障按钮尖轨名称
        QString DCGZBtnNameXG;//道岔故障按钮心轨名称
        QPoint  LeftDCGZBtnPt;//左侧道岔故障按钮基准坐标
        int     LeftLineFeed; //左侧道岔故障按钮换行个数
        int     LeftDCGZStyle;//左侧道岔故障按钮显示风格0或1
        bool    LeftDCGZnewlineBRight;//左侧道岔故障按钮换行后在右侧
        QStringList LeftDCArray;//左侧道岔列表
        QPoint  RightDCGZBtnPt; //右侧道岔故障按钮基准坐标
        int     RightLineFeed;  //右侧换行个数
        int     RightDCGZStyle;//左侧道岔故障按钮显示风格0或1
        bool    RightDCGZnewlineBRight;//左侧道岔故障按钮换行后在右侧
        QStringList RightDCArray;//右侧道岔列表
        bool bCanChgVisleJSQ;//可见设置中的计数器功能是否可用
        bool bVisleJSQ;//计数器初始是否显示
        int bQJxhdShow;//区间信号灯是否显示
        AQXBJ AQXBJDCarray[10];  //安全线道岔不在定位报警灯
        int AQXDCNUM;
        bool DlgDCTimeShow=false;//调车进路弹窗调车进路参数窗口显示：1/true-显示 0/false-不显示
        bool bSpeechFlag=false;//语音播报标志，默认不播报
    }StaConfig_T;
    StaConfig_T StaConfigInfo;

    struct StationLiansuo
    {
       QString strGDName;//股道名称
       QString strGDName1;//股道名称（可能为无岔或道岔区段，一般为有中岔的股道配置）
       int nCode;//股道设备号
       QString strLeftNode;//左侧信号机名称
       QStringList strArrLeftFCK;//左边的发车口
       QString strRightNode;//右侧信号机名称
       QStringList strArrRightFCK;//右边的发车口
    }StationGDNode[100];
    //股道数量
    int m_nStationGDNode;
    QStringList m_ArrGD;
    //拥有进路权限（值班员默认有，信号员默认无）
    bool m_bHaveRoutePermissions;
    //铅封数组
    QVector<QianFeng>mQianFengVect;
    //车站控制模式
    struct StationModal
    {
       QString strName;
       bool nAgrRequest;	//同意转换
       bool nPlanCtrl;		//计划控制
       int nModeState;		//0 中心控制 1 车站控制 2 分散自律
       int nRequestMode;	//0 中心控制 1 车站控制 2 分散自律(请求转换的模式)   //1 中心控制 2 车站控制 3 分散自律(请求转换的模式) （0为默认值，处理不便）
       int nStateSelect;	//0 手工排路 1 按图排路
    }StationModalSelect;
    int nOldModeState;//上一次的控制模式
    bool m_bModeChanged;
    bool m_nFCZKMode; //非常站控
    bool m_bSDJS;//上电解锁
    bool m_bGZTZ;//故障通知
    int m_nModeState;//模式转换//0为中心控制,1为车站控制,2为车站调车
    bool nStateSelect;//按图排路
    bool nPlanCtrl;//计划控制
    bool AutoSignStage;//阶段计划自动签收标志
    bool m_nComCTCToLS;//与联锁通信
    bool m_nComCTCCenter;//中心通信
    bool m_nComZLJState;//自律机通信
    bool m_bAllowZH;//允许回转
    bool m_nComLKState;//列控通信
    bool m_nX180;//延迟解锁3分钟 吸起计时3分钟后落下
    bool m_nX30s;//延迟解锁30钟 吸起计时30钟后落下
    bool m_nS180;//延迟解锁3分钟 吸起计时3分钟后落下
    bool m_nS30s;//延迟解锁30钟 吸起计时3分钟后落下
    bool m_nX240;//延迟解锁4分钟 吸起计时3分钟后落下
    bool m_nX60s;//延迟解锁60s 吸起计时30钟后落下
    bool m_nS240;//延迟解锁4分钟 吸起计时3分钟后落下
    bool m_nS60s;//延迟解锁60s 吸起计时3分钟后落下
    bool PD180s;
    bool m_nJWDState;//机务段状态灯
    bool m_nPDJSState;//坡道解锁
    bool m_nSXYDZS;//上行引导总锁
    bool m_nXXYDZS;//下行引导总锁
    bool m_nQCYDZS;//全场引导总锁
    int m_nRoutePermission;//进路权限

    typedef struct //ZDBSInfo  //自动闭塞定义
    {
        QString strZDJuncXHD;    //关联信号机名称
        int ButtonForm;       //按钮样式
        QPoint  ptZDBox;         //自动闭塞方框左上角坐标//箭头坐标

        bool haveWord_Flag;
        QPoint  ptWord;       //自动闭塞文字坐标
        QString WordName;

        bool ZDBS_FK_Flag;
        QPoint  ptZDBS_FK;       //自动闭塞方框左上角坐标
        int ZDBS_FK_length;       //自动闭塞方框长度
        int ZDBS_FK_breadth;       //自动闭塞方框宽度

        bool ZFZANFlag;//总辅助
        QPoint ZFZANPT;
        QString ZFZANName;

        bool JCFZANFlag;//接车辅助
        QPoint JCFZANPT;
        QString JCFZANName;

        bool FCFZANFlag;//发车辅助
        QPoint FCFZANPT;
        QString FCFZANName;

        bool YXGFANFlag;//允许改方
        QPoint YXGFANPT;
        QString YXGFANName;

        bool QJDFlag;//区间灯
        QPoint QJDPT;
        QString QJDName;

        bool FZDFlag;//辅助灯
        QPoint FZDPT;
        QString FZDName;

        bool SGDFlag;//闪光灯或者区轨灯
        QPoint SGDPT;
        QString SGDName;

        bool YXFCDFlag;//允许发车灯
        QPoint YXFCDPT;
        QString YXFCDName;

    }ZDBSInfo_T;
    int ZDBSConfigNum = 0;
    ZDBSInfo_T ZDBSInfo [50];   //自动闭塞数组

    typedef struct //BZDBSInfo  //半自动闭塞定义
    {
        QString strBZDJuncXHD;    //关联信号机名称
        int m_nCode;
        int ButtonForm;       //按钮样式
        QPoint  ptBZDBSArrow;         //箭头坐标

        bool haveBZDWord_Flag;
        QPoint  ptBZDWord;       //半自动闭塞文字坐标
        QString BZDWordName;

        bool BZDBS_FK_Flag;
        QPoint  ptBZDBS_FK;       //半自动闭塞方框左上角坐标
        int BZDBS_FK_length;       //半自动闭塞方框长度
        int BZDBS_FK_breadth;       //半自动闭塞方框宽度

        bool BSANFlag;//闭塞
        QPoint BSANPT;
        QString BSANName;

        bool SGANFlag;//事故
        QPoint SGANPT;
        QString SGANName;

        bool FYANFlag;//复原
        QPoint FYANPT;
        QString FYANName;

        bool QHBSANFlag;//切换闭塞
        QPoint QHBSANPT;
        QString QHBSANName;

        bool ZYDFlag;//占用灯
        QPoint ZYDPT;
        QString ZYDName;

        bool KXDFlag;//空闲灯
        QPoint KXDPT;
        QString KXDName;

        bool RGBSDFlag;//人工闭塞灯
        QPoint RGBSDPT;
        QString RGBSDName;

        bool ZDBSDFlag;//自动闭塞灯
        QPoint ZDBSDPT;
        QString ZDBSDName;

    }BZDBSInfo_T;
    int BZDBSConfigNum = 0;
    BZDBSInfo_T BZDBSInfo [50];   //半自动闭塞数组

    typedef struct //BZDBSInfo  //计轴定义
    {
        QString strJZJuncXHD;    //关联信号机名称
        int ButtonForm;       //按钮样式
        QPoint ptJZArrow;

        bool haveJZWord_Flag;
        QPoint  ptJZWord;       //计轴文字坐标
        QString JZWordName;

        bool JZ_FK_Flag;
        QPoint  ptJZ_FK;       //计轴方框左上角坐标
        int JZ_FK_length;       //计轴方框长度
        int JZ_FK_breadth;       //计轴方框宽度

        bool JZSYANFlag;//计轴使用
        QPoint JZSYANPT;
        QString JZSYANName;

        bool JZTYANFlag;//计轴停用
        QPoint JZTYANPT;
        QString JZTYANName;

        bool JZFLANFlag;//计轴复零
        QPoint JZFLANPT;
        QString JZFLANName;

        bool BSQHANFlag;//切换闭塞
        QPoint BSQHANPT;
        QString BSQHANName;

        bool JZSYDFlag;//计轴使用灯
        QPoint JZSYDPT;
        QString JZSYDName;

        bool JZTYDFlag;//计轴停用灯
        QPoint JZTYDPT;
        QString JZTYDName;

        bool JZFLDFlag;//计轴复零灯
        QPoint JZFLDPT;
        QString JZFLDName;

        bool JZBJDFlag;//计轴报警灯
        QPoint JZBJDPT;
        QString JZBJDName;

        bool QJFlag;//计轴报警灯
        QPoint QJPT;
        QString QJDName;

    }JZInfo_T;
    int JZConfigNum = 0;
    JZInfo_T JZInfo [50];   //半自动闭塞数组

    typedef struct              //表示灯定义
    {
        QString strCLJuncXHD;    //关联信号机名称

        bool HaveLeftArrow;
        bool HaveRightArrow;

        QPoint ptCLLEFTArrow;
        QPoint ptCLRIGHTArrow;

        QPoint ptCheCiWindown;//车次接近窗坐标

        bool haveCLWord_Flag;
        QPoint  ptCLWord;       //场联文字坐标
        QString CLWordName;

        bool CL_FK_Flag;
        QPoint  ptCL_FK;       //场联方框左上角坐标
        int CL_FK_length;       //场联方框长度
        int CL_FK_breadth;       //场联方框宽度

        bool AFJFlag;//场联使用灯
        QPoint AFJPT;
        QString AFJName;
        int AFJLightForm;
        int AFJTextPos;

        bool BFJFlag;//场联停用灯
        QPoint BFJPT;
        QString BFJName;
        int BFJLightForm;
        int BFJTextPos;

        bool CFJFlag;//场联复零灯
        QPoint CFJPT;
        QString CFJName;
        int CFJLightForm;
        int CFJTextPos;

        bool JGJFlag;//场联报警灯
        QPoint JGJPT;
        QString JGJName;
        int JGJLightForm;
        int JGJTextPos;

    }CLInfo_T;
    int CLConfigNum = 0;
    CLInfo_T CLInfo [50];   //场联数组

    typedef struct              //表示灯定义
    {
        QPoint  ptLamp;        //表示灯坐标
        QString strName;       //表示灯名称
        int     nStatus_XiQi;	   //表示灯状态//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝）
        int     nStatus_LuoXia;	   //表示灯状态//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝）
        //CStringArray DEVName;  //关联区段设备名称//区段占用为1亮灯，区段不占用为0灭灯，
        QString DEVName;
    }LAMP_T;
    QVector<LAMP_T> vectLamp; //表示灯信息

    typedef struct              //功能按钮
    {
        QPoint  ptFuncBtn;        //功能按钮坐标
        QString strName;       //功能按钮名称
        int     nStatus_XiQi;	   //功能按钮状态//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝）
        int     nStatus_LuoXia;	   //功能按钮状态//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝）
        //CStringArray DEVName;  //关联区段设备名称//区段占用为1亮灯，区段不占用为0灭灯，
        QString DEVName;//
        QString BtnType;//按钮类型
        QString DEVBtnType;//关联按钮类型
        bool haveQF;
        bool haveAN;//按钮显示标志
    }FuncBtn_T;
    QVector<FuncBtn_T> vectFuncBtn; //表示灯信息

    typedef struct              //站台定义
    {
        bool High;        //高站台1，低站台0
        QString Type;        //类型 - 小写u型、小写n型
        int     Length;      //站台长度
        QPoint  ptZT;        //左上角坐标
    }ZHANTAI_T;
    int  ZHANTAICount;    //站台个数
    ZHANTAI_T  ZHANTAIInfo[40]; //站台
    bool       bShowXinHaoLou;  //信号楼显示与否
    QPoint     XinHaoLouPt;     //信号楼坐标
    bool       XinHaoLouFaceNorth;//信号楼朝北/朝南

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
        int LineState;//虚线为0,实线为1
    }LINES_T;
    QVector<LINES_T> vectLine; //直线信息

    typedef struct  //矩形定义
    {
        int     nColorType;	//颜色类型（颜色：0白色,1绿）
        QPoint  pt1;//矩形点1
        QPoint  pt2;//矩形点2
        int LineWidth;//线宽
        int LineState;//虚线为0,实线为1
    }RECT_T;
    QVector<RECT_T> vectRect;//矩形信息

    int commToServer;
    int ANFlashFlag;
    bool ButtonFlashDown;

    int GDWordColor;//股道文字颜色
    int GDWordSize;//股道文字大小
    int GDDCWordColor_DW;//道岔文字颜色
    int GDDCWordColor_FW;//道岔文字颜色
    int GDDCWordColor_SK;//道岔文字颜色
    int GDDCWordSize;//道岔文字大小
    int XHDWordColor;//信号灯文字颜色
    int XHDWordSize;//信号灯文字大小
    int DCQDWordSize;//道岔区段文字
    int DirWordSize;//方向文字:文字大小;
    int BSWordSize; //自动闭塞、半自动闭塞、计轴功能按钮文字:文字大小;
    int CLWordSize; //场连文字:文字大小;
    int ViewBtnAndLmapWordSize=0xFF;//界面按钮名称及标识灯文字大小；//文字大小
    int ViewBtnAndLmapSize=0xFF;//界面按钮名称及标识灯文字大小；按钮（指示灯大小）
    bool bDDMDFalg=false;//点灯灭灯按钮显示标志
    bool nDDMDStateS=false;//上行点灯灭灯
    bool nDDMDStateX=false;//下行点灯灭灯
    QStringList strPDJSXHJ;
    typedef struct{
        QString startLine;
        QString endLine;
    }RailwayLine;
    typedef struct  //运行图界面定义
    {
        int width = 0;
        int height = 0;
        int miniteDistance = 0; //十分格距离
        int stationDistance = 0; //车站间距离
        int lintDistance = 0; //线路间距离
        QVector<RailwayLine> vectRailwayLine;
    }TrainDiagramInfo; //运行图
    TrainDiagramInfo TrainDiagram;

    typedef struct{
        QString strName; //进站口名称
        bool bHerald = true; //是否预告
    }EntranceInfo;
    QVector<EntranceInfo> vecEntranceInfo;
    /*供电臂设置状态*/
    QVector<PowerStr*> v_PowerArray;
    //封锁设备数据
    //设备封锁 股道设备序列
    QVector<qdData*> v_GDArray;
     //设备封锁 区间设备序列
    QVector<qdData*> v_QJArray;



public:
    //读取站场设备数据
    bool readStationDev(QString fileName, MyStation *pMyStation);
    //读取站场配置数据
    bool readStationConfig(QString fileName, MyStation *pMyStation);
    //读取联锁表
    bool readInterlockTable(QString fileName, MyStation *pStation);
    //读取闭塞配置数据
    bool readBSConfig(QString fileName, MyStation *pMyStation);
    //初始化自动闭塞
    void InitZDBSPos(MyStation *pMyStation);
    //初始化半自动闭塞
    void InitBZDBSPos(MyStation *pMyStation);
    //初始化计轴
    void InitJZPos(MyStation *pMyStation);
    //初始化场联
    void InitCLPos(MyStation *pMyStation);
    //读取表示灯配置数据
    bool readLampFuncBConfig(QString fileName, MyStation *pMyStation);
    //读取供电臂设置数据
    bool readPowerSupplyArmData(QString fileName, MyStation *pMyStation=nullptr);
    //读取封锁设备配置数据-股道
    void readBlockingDevDataGD(QString fileName, MyStation *pMyStation=nullptr);
    //读取封锁设备配置数据-区间
    void readBlockingDevDataQJ(QString fileName, MyStation *pMyStation=nullptr);
    //读取调度命令模板
    bool readDDOrderTemplate(QString fileName, MyStation *pMyStation);
    //初始化表示灯
    void InitLAMPPos(MyStation *pMyStation);
    //初始化功能按钮
    void InitFuncBtnPos(MyStation *pMyStation);
    //初始化站台位置
    void InitZTPos(MyStation *pMyStation);
    //初始化DC尖轨、心轨故障按钮
    void InitDcGZBtn();
    //初始化车次框
    void InitTrainWnd();
    //初始化站场文字
    void InitWord();
    //初始化通过按钮
    void InitXHDTGButton(CXHD *xhd);
    //初始化安全线报警灯
    void InitAQXBJD();

    //初始化数据
    void InitData(bool bMulti=false);
    //根据设备号获取设备名称
    QString GetStrNameByCode(int nCode);
    //根据无岔区段code获取股道指针
     CGD * GetGDByCode(int nCode);
    //根据道岔code获取道岔指针
     CGDDC * GetDCByCode(int nCode);
     CXHD * GetXHDByCode(int nCode);
    //根据设备名称获取设备号
    int GetCodeByStrName(QString devName);
    //股道在站场数组中的索引
    int GetGDPosInzcArray(int nCode);
    //信号机是否属于线路所
    bool IsXHDinXianLuSuo(QString _strXHD);
    //根据名称获取设备索引
    int GetIndexByStrName(QString devName);
    //设备是否是股道
    bool DevIsGDByCode(int nCode);
    //匹配信号机是否具有通过按钮
    void SetXHJTGBtnState();
    //匹配信号机是否具有坡道解锁
    void SetXHJPDJSState();
    //获取设备上下咽喉信息
    QString GetDevSX(int ntype,int ncode,bool dcqd=false);
    //获取按钮设备名称
    QString GetANNameByCode(int ntype,int nCode);
    //破封统计计数
    void SetQFTJAdd(QString str);
public:
    //信号机及其他数据初始化
    void SetXHDFlag();
    //获取（接车口）信号机关联的接近区段("XHD.h"循环调用修改)
    QString GetXHDjucJJQDName(QPoint xhdP1,QPoint xhdP2);
    QString GetXHDjucGDName(QPoint xhdP1,QPoint xhdP2);
    //根据第一个接近区段名称查找下一个接近区段
    QString FindNextJJQD(int index, QString strJJQD);
    QStringList getGDQDListByXHD(CXHD* pXHD);
    //在接车口接近区段能否找到该接近区段
    bool FindJJQDFromJCK(int index, QString strJJQD);
    //变通进路的进路信息
    QVector<BTJL> vectBTJL;
    //初始化本站变通进路基础信息
    void initBTJLinfo();
    //自动闭塞管理信号机是否是上行
    bool ZDBSisSX(QString xhdname);
    //设置站场状态非常站控状态
    void SetFCZKState(bool b);
public:
    //联锁状态解析
    void updateDevStateOfLS(unsigned char *array);
    void updateGDDC_StateOfLS(CGDDC *gddc, int state_0, int state_1, int state_2, int state_3);
    void updateXHD_StateOfLS(CXHD *xhd, int state_xhd_0,int state_xhd_1,int state_xhd_2,int state_xhd_3);
    void updateGD_StateOfLS(CGD *gd, int state_gd_0,int state_gd_1,int state_gd_2);
    void updateQD_StateOfLS(CQD *qd,int state);
    //解析自动闭塞状态
    void updateZDBS_StateOfLS(int state_ZDBS_0,int state_ZDBS_1,int state_ZDBS_2,int state_ZDBS_3,int state_ZDBS_4,int state_ZDBS_5);
    //解析半自动闭塞状态
    void updateBZDBS_StateOfLS(int state_BZDBS_0,int state_BZDBS_1,int state_BZDBS_2,int state_BZDBS_3);
    //解析计轴状态
    void updateJZ_StateOfLS(int state_JZ_0,int state_JZ_1,int state_JZ_2,int state_JZ_3);
    //解析场联状态
    void updateCL_StateOfLS(int state_JZ_0,int state_JZ_1,int state_JZ_2,int state_JZ_3);
    void updatelamp_StateOfLS(lamp *lamp_Temp);
    void updateFUNCButton_StateOfLS(FUNCBUTTON *FUNC_Temp);
    void setQD_Color(CQD *qd,QColor color);
    //通过信号机名称获取信号机编号
    int GetXHCodeByName(QString strName);
    //通过股道code获取股道名称
    QString GetGDNameByCode(int ncode);
    //获取非常站控状态
    int GetFCZKMode();
public:
    //接收服务端数据-列车位置信息
    void recvServerData_TrainPos(unsigned char *recvArray, int recvlen);
    //设置车次信息
    void SetCheCiInfo(Train* pTrainTemp);
    void SetQDCheCi();//设置区段车次
    void UpdateCheCiInfo();//更新车次信息
    void DeleteCheCi();//删除没有的车次
    //更新激活车次信息
    void UpdateStationCheCiInfo(int nElaps);
    //根据Code查找相应车次框的位置
    QPoint GetPointInTrainNumWndArray(int code);

    //车次操作接口方法
    bool OnMenuCheCiClicked(QString menuName, int gdCode, Train* pTrainTemp);
    //更新车次框的显示
    void UpdateTrainNumWnd();
    //根据车次获取列车信息
    Train* GetTrainByCheCi(QString _strCheci);
    //设置车次窗显示隐藏
    void SetTrainNumWndShow(bool b);
    //设置进路预告窗显示隐藏
    void SetRoutePreWndShow(bool b);
    //设置进路预告窗显示隐藏
    void SetRoutePreWndShow_m(bool b);
    //设置信号机按钮不闪烁//重置状态
    void ResetState();
    //设置信号机按钮显示
    void SetXHJANShow(int ntype,bool bshow);
    //站场设备鼠标移动响应
    void SetDevMouseMoveShow(QPoint mcoursePoint,CBaseData *mCBaseDataOld);
public:
    //制作数据帧头帧尾和关键信息
    void packHeadAndTail(QByteArray *pDataArray, int length);
    //站场重置
    void ResetStationInfo();
    //获取最新调度命令 用于调度命令签收界面显示
    DispatchOrderStation* GetDisOrderRecvByShow();
    //获取当前调度命令序列

private:
    //更新进路预告窗信息
    void UpdateRoutePreWndInfo();

public:
    QVector<TrainNumType*> v_TrainNumType;
    QVector<QString> v_TrainRunType;
    //根据索引索取列车类型（管内动车组、通勤列车等几十种）
    QString GetTrainType(int index);
    //根据名称获取列车类型索引
    int GetTrainTypeIndex(QString _strType);
    //根据索引索取列车运行类型（动车组、快速旅客列车等几种）
    QString GetTrainRunType(int index);
    //根据名称获取列车运行类型索引
    int GetTrainRunTypeIndex(QString _strRunType);
    //根据索引索取超限级别
    QString GetChaoXianLevel(int index);
    //根据名称获取超限级别索引
    int GetChaoXianLevelIndex(QString _strChaoXian);

    //获取铅封计数名称
    void GetQJNameList();
public:
    QTextToSpeech *tts;
    QVector<UserInfo>mUserInfoVectSTA;
    //语音播报
    void SpeechText(QString str);
    //接收服务端数据-语音播报信息
    void recvServerData_Speach(QByteArray recvArray, int recvlen);
    //系统报警
    void SetServerAlarm(QByteArray recvArray, int recvlen);
    //设置该站所有的股道防溜
    void SetAllGDAntiSlip();
    //清除该站所有的股道防溜
    void ClearAllGDAntiSlip();
    //初始化占线版所有股道数据
    void InitZXBGDData();
    //初始化时道岔获取对应的道岔区段名称的方框
    void getDGNameFromText();
    //重置站场设备状态
    void ResetStationDevStatus();
    //绘制CTC站场安全状态
    void drawMyStationSafe();
    //设置道岔故障按钮状态
    void setDCGZButtonState();   

public:
    //接收服务端数据-防错办信息
    CheckResult* recvServerData_RouteCheck(QByteArray recvArray);
    int WorkFlowControl(int type, TrafficLog* pTrafficLog);

public:
   //组合进路
   typedef struct
   {
       int ZHJL_SonRouteNum;//子进路个数
       QString ZHJL_SDXH;
       QString ZHJL_ZDXH;
       QString ZHJL_RouteDescrip;//整体的始端与终端（两个）
       QString ZHJL_SDXH_JL[10];
       QString ZHJL_ZDXH_JL[10];
       QString ZHJL_RouteDescrip_JL[10];//进路0和进路1的信号灯组合
       QString ZHJL_RouteDescrip_ZTJL;//整体进路的所有信号灯（四个）
   }ZHJL;
   QVector<ZHJL> vectZHJL;

   bool ZHJL_UNFLOD;//组合进路是否默认展开

   int nFunBtnState = 0xffff;
   //判断进路是否有组合进路
   bool HaveZHJL(QString ZHJL);
   //判断进路是否有组合进路
   QString GetZHJL(QString ZHJL);
   ZHJL* GetZHJLByDescrip(QString ZHJL);
   bool HaveZHJL_JL(QString ZHJL);//是否有组合进路的分支进路存在
   //获取进路序列的联锁表进路索引并组织进路命令//存在组合进路的情况下
   //int GetTrainRouteOrder_ZHJL(CTrainRouteOrder* _pTrainRouteOrder);
   QString GetZHJL_JL1(QString ZHJL,int j);
   //组合进路增加两条进路序列
   void StagePlanToRouteOrder_ZHJLAdd(TrainRouteOrder* _pTrainRouteOrder);//接收0，导入1
   void StagePlanToRouteOrder_ZHJLRemove(TrainRouteOrder* _pTrainRouteOrder);//接收0，导入1
   int GetNumberZHJL_ZTJL(QString ZHJL);//获取进路序列的排列位置
   void DealZHJLstate();
   void DealZHJLstate_JL0();
   void DealZHJLstate_JL1();
   //处理组合进路和分支进路的状态存储
   void DealZHJLstate_JL0_SAVE();
   void DealZHJLstate_JL1_SAVE();

   //处理组合进路和分支进路的自触状态
   void DealZHJLZCstate();
   void DealZHJLZCstate_JL0();
   void DealZHJLZCstate_JL1();
   void DealZHJLZCstate_JL0_SAVE();
   void DealZHJLZCstate_JL1_SAVE();
   //处理组合进路和分支进路的自触状态
   void DealZHJLZC_JL0(TrainRouteOrder* _pTrainRouteOrder);
   //处理组合进路和分支进路的点开时的进路状态显示
   void DealZHJLstateTo_JL0(TrainRouteOrder* _pTrainRouteOrder);
   //通过组合进路子进路的序号获取子进路的进路序列
   QString GetJLXLFromIndex(int l);

   void ZHJLUnfoldState();//组合进路展开
   // nCode接车、发车设备号, nFlg出发1 到达0 //阶段计划传来该设备号是否是上行
   bool GetSXByCode(int nCode, int nFlg);

/************ mystation2.cpp函数定义 数据同步 ************/
signals:
    //信号-发送数据给CTC服务终端-TCP通道
    void sendDataToCTCServerSignal2(MyStation* pMyStation, QByteArray dataArray,int len);
    //获取用户信息
    void GetUserInfo(QString name,QString password);
    void updateDisOrderSignal();
    //收到服务端调度命令已签收信号
    void RecSeverDDMLSign();
    //收到服务端调度命令已签收信号
    void RecSeverStagePlanSign();
    //信号-行车日志更新
    void TrafficLogUpData(int type, int logId = 0);
    //信号-进路序列更新
    void TrainRouteOrderUpData(int type, int route_id = 0);
    //信号-刷新股道防溜界面显示
    void StationStorageUpData();
    void SendTextAnalysisSignal(int index, QPoint pt, QString dev = "");
public:
    //向服务端请求数据（请求类型）
    void RequestDataFromServer(int syncType);
    //根据id在数组中查找该阶段计划的索引
    int GetIndexOfStagePlanInArrayById(int id);
    //根据id在数组中查找该列车进路序列的索引
    int GetIndexOfTrainRouteOrderInArrayById(int id);
    //根据id在数组中查找该行车日志的索引
    int GetIndexOfTrafficLogInArrayById(int id);
    //根据id在数组中查找该调度命令的索引
    int GetIndexOfDisOrderInArrayById(int id);
    //根据id在数组中查找该调度台调度调度命令的索引
    int GetIndexOfDisOrderDDTInArrayById(int id);
    //根据id在数组中查找该机车调度命令的索引
    int GetIndexOfDisOrderJCInArrayById(int id);
    //更新收到的阶段计划信息（功能码-同步标志）
    void updateStagePlan(QByteArray recvArray,int syncFlag);
    //更新收到的列车进路序列信息（功能码-同步标志）
    void updateTrainRouteOrder(QByteArray recvArray,int syncFlag);
    //更新收到的行车日志信息（功能码-同步标志）
    void updateTrafficLog(QByteArray recvArray,int syncFlag);
    //更新收到的调度命令信息（功能码-同步标志）
    void updateDisOrder(QByteArray recvArray,int syncFlag);
    //更新收到的车站调度命令信息（功能码-同步标志）
    void updateDisOrderStation(QByteArray recvArray,int syncFlag);
    //更新收到的调度台调度命令信息（功能码-同步标志）
    void updateDisOrderDispatcher(QByteArray recvArray,int syncFlag);
    //更新收到的机车调度命令信息（功能码-同步标志）
    void updateDisOrderLocomotive(QByteArray recvArray,int syncFlag);
    //更新收到的股道防溜信息（功能码-同步标志）
    void updateGDAntiSlip(QByteArray recvArray,int syncFlag);
    //更新收到的防错办信息（功能码-同步标志）
    void updateFCBAttr(QByteArray recvArray,int syncFlag);
    //更新收到的防错办-股道属性信息（功能码-同步标志）
    void updateGDAttr(QByteArray recvArray,int syncFlag);
    //更新收到的防错办-出入口属性信息（功能码-同步标志）
    void updateGatewayAttr(QByteArray recvArray,int syncFlag);
    //初始化组合进路
    void initCombinedTrainRouteOrder();

    //根据区间口名称获取占线版供电臂设置区段序列
    QList<int> GetPowerSupplyGDList(QString strPowerName);
    //根据区间口名称获取占线版供电臂设置道岔序列
    QList<int> GetPowerSupplyGDDCList(QString strPowerName);
    //获取股道设备有电无电状态
    bool GetGDPowerState(QString devName);
    //设置供电臂对应区间是否有电
    void SetPowerSupplyQJState();
    //设置封锁设备是否有电
    void SetBlockingDevState(/*QString strdevName*/);
    //获取设备封锁状态
    bool GetBlockingDevState(QString strdevName);

    //根据id查找列车进路序列
    TrainRouteOrder* GetTrainRouteOrderById(int routeId);
    //根据车次和方向查找列车进路序列(组合进路只找主进路)
    TrainRouteOrder* getRouteOrderByTrainNum(QString trainNum, int btRecvOrDepart);
    QVector<TrainRouteOrder> getRouteOrdersByTrainNum(QString trainNum, int type);
    //获取进路序列方向
    bool getRouteOrderDirection(TrainRouteOrder* pRouteOrder); //true-上行.false-下行
    //根据id查找行车日志
    TrafficLog* GetTrafficLogById(int logId);
    //根据车次号查找行车日志
    TrafficLog* GetTrafficLogByTrainNum(QString trainNum);
    InterlockRoute* GetInterlockRouteByDescrip(QString descrip);

    bool IsReadOnly();
    void InitZoneData();
    void ResetZoneData();
    void UpdataZoneDataState();

    QVector<CGDDC*> getDCListByInterlock(InterlockRoute* pInterlock);
    QVector<CGDDC*> getCorrelationGDDC(CGDDC* pCGDDC, CGDDC* pCGDDC2);
};

#endif // MYSTATION_H
