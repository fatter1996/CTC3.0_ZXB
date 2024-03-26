#ifndef MYDOC_H
#define MYDOC_H

#include <QObject>
#include <QVector>
#include <QMutex>
#include <QThread>
#include <QTime>
#include <QMainWindow>
#include <QWidget>
#include "mainwindow.h"
#include "MyStation/mystation.h"
#include "SocketPro/socketudp.h"
#include "SocketPro/servertcp.h"
#include "SocketPro/sockettcp.h"
//#include "StationInterface/stationinterface.h"
//#include "AgainstWrong/againstwrongwidget.h"
#include "MyStation/stageplan.h"
#include "Log/log.h"
#include "RouteOrder/routeorderwidget.h"
#include "ZXBWindow/zxbwindow.h"

#include "Dialog/powersupplyarmdialog.h"
#include "Dialog/blockingdevdialog.h"

#include "Dialog/againstwrongwidget.h"
#include "TextAnalysis/TextAnalysis.h"


/*
*主文档类
*/
class MyDoc : public QObject
{
    Q_OBJECT
public:
    explicit MyDoc(QObject *parent = nullptr);

public:
    ~MyDoc();
    static MyDoc& GetInstance()
    {
        static MyDoc s_instance;
        return s_instance;
    }
    MyDoc(MyDoc const&) = delete ;
    MyDoc& operator= (MyDoc const&) = delete ;

public:
    //资源互斥锁，用来保护数据防止被篡改
    QMutex Mutex;
    QTimer *pTimer;
    QDateTime SysStartedDataTime;//系统启动时的时间
    long SysLifeSeconds;//系统自启动以来已运行时间（秒）
    bool bRequstData = false;//请求数据标志
    bool bRouteOrderShow = true; //列车进路序列窗口是否默认显示
    long SysInitSeconds;
public:
    //车站数组
    QVector<MyStation*> vectMyStation;
    //车站数组
    QVector<QThread*> vectThread;

    //当前车站的索引值
    int currStaIndex;
    //主通信是否UDP模式（兼容旧版模式）
    bool bUdpMode;
    //udp通信服务端
    SocketUDP* socketUDP;
    //tcp通信服务端
    //ServerTCP* serverTcp;
    //tcp通信客户端
    SocketTCP* tcpClient;
    //本机Server端口
    //int localServerPort;

    //数据库地址账号信息
    QString databaseIP;
    QString databaseName;
    QString databaseUser;
    QString databasePassWord;
    int databasePort;


    //本站<=>服务
    struct AddrCTCToJSJ
    {
        QString addrCTC;//本机地址
        int nPortCTC;//本机端口
        QString addrService;//Server地址
        int nPortService1;//Server端口1(UDP)
        int nPortService2;//Server端口2(TCP)
    }m_AddrCTCToService;

    bool bFlagCommToServer=false;//与服务端通信标志 true:通信正常 false:通信中断 注：当占线版配置多个站时，有一个站与服务端通信正常此标志设置为true,反之为false
private:
    //初始化所有数据
    void initAllData();
    //读取全局数据
    void readGlobalData();
    //初始化通信
    void initNetCom();
    //初始化全局逻辑
    void initGlobalLogic();
    //初始化全局窗口初始化
    void InitAllWindow();
    //供电臂设置
    PowerSupplyArmDialog* pPowerSupplyArmDialog=nullptr;
    /*封锁设备操作*/
    BlockingDevDialog* pBlockingDevDialog=nullptr;



public:
    //根据站名在车站数组中获取索引
    int getIndexByStaNameInStaArray(QString strStation);
    //根据站名在车站数组中获取车站指针
    MyStation* getMyStationByStaNameInStaArray(QString strStation);
    //根据id在车站数组中获取车站指针
    MyStation* getMyStationByStaIDInStaArray(int id);
    //根据索引在车站数组中获取车站指针
    MyStation* getMyStationByIndexInStaArray(int idx);

    //在车站数组中获取当前车站指针
    MyStation* getCurrMyStationInStaArray();
    //根据站名设置当前车站索引
    int setCurrIndexByStaName(QString strStation);

    //发送心跳信息给后台服务
    void SendHeartBeatToTCCService();
    //发送功能按钮数据给后台服务
    void SendFunButtonDataToCTCService(int nFunType,int ndevcode,int nDevCode2);
    void SendFunButtonDataToCTCService(int nFunType,int ndevcode[],QString str);
    //获取本站ID
    int GetStationID();
    //向服务端发送当前按下的信号机按钮
    void SendXHJPressType(int nFunType,int ndevcode);
    //向服务端发送设备按下的消息
    void SendDevPressInfo();
    //向服务端发送清除按钮按下信息
    void SendClearBtnInfo();

    QString getNameOfDevNode(MyStation* pStation, unsigned int devnode);//培训软件-根据设备编号获取设备名称
    unsigned int getDevNodeOfName(MyStation* pStation, QString strName);//培训软件-根据设备名称获取设备编号
    //设置占线版与服务端通信状态
    bool SetServiceCommState();

signals:
    //信号-发送数据给主通道
    void sendDataToMainSignal(QByteArray dataArray,QString OppAdress,int OppProt,int len);
    //信号-发送数据给教师机通道
    void sendDataToTeacherSignal(QByteArray dataArray,QString OppAdress,int OppProt,int len);
    //信号-发送数据给培训软件通道
    void sendDataToTrainningSignal(QByteArray dataArray,QString OppAdress,int OppProt,int len);

    //信号-清除功能按钮选中状态
    void ClearFunButtonStateSignal();
    //信号-收到系统报警信息
    void RecvServerAlarm(QByteArray recvArray, int recvlen);
    //信号-收到控制模式转换信息
    void RecvServerModeChange(QByteArray recvArray, int recvlen);
    //信号-收到阶段计划
    void StagePlanUpData(bool isSync = true);

    //信号-收到调度命令
    void DDOrderManageUpData();
    //信号-用户登录报警
    void UserAlarmSignal(int n);
    //信号-收到站场重置
    void ResetStationSignal();

public slots:
    //槽-发送数据给CTC后台服务逻辑
    void sendDataToCTCServerSlot(MyStation *pMyStation, QByteArray dataArray, int nLength);
    //解析CTCServer通道数据
    void receiveCTCServerDataSlot(QByteArray dataArray,QString clientAdd, int clientPort);
    void timerTime();
    //向服务端发送控制模式转换信息
    void SendStationCtrModeChangeSlot(int mode,int type1,int type2);
    //向服务端发送阶段计划签收的消息
    void SendSignFor();
    //向服务端发送防错办
    void SendAgainstWrong(int stationId, int id, int type);
    //向服务端发送进路序列的消息
    void SendRouteOrder(MyStation* pStation, int type, int routeId, int nData, QString strData);
    //向服务端发送进路序列的消息
    void SendTrafficLog(int stationId, int logId, int type, QDateTime date);
    void SendTrafficLogUpdata(int stationId, int type, TrafficLog pTrafficLog);
    void SendTrafficLogWorkFlow(int stationId, int logId, int type, int flag, QDateTime date);
    //向服务断发送用户登录 注销信息
    void SendUserLongInExitSlot(int ntype,QString strUserName,QString strPassWord="");
    //调度命令签收槽
    void DispatchOrderStationSignSolt(int norder,bool b);
    //调度命令 转发至机车
    void ForWardDDOrderToTrainSolt(DispatchOrderStation mDispatchOrderStation);
    //调度命令 发送至机车
    void SendOrderLocomotiveSolt(DispatchOrderLocomotive* mDispatchOrderLocomotive);
    //调度命令 发送至调度台
    void SendOrderDispatcherSolt(DispatchOrderDispatcher *mDispatchOrderDispatcher);
    //发送自动签收
    void SendAutoSignStateSolt(bool bsign);
    //槽-供电臂设置
    void PowerSupplyArmDialogShowSolt();
    //槽-供电臂设置向服务端发送设置数据
    void SendPowerSupplyArmDataToSeverSolt(int nStaIndex,QString strPowerName,bool bset);
    //槽-封锁设备操作
    void BlockingDevDialogShowSolt();
    //槽-封锁设备操作向服务端发送设置数据
    void SendBlockingDevDataToSeverSolt(int nStaIndex,QList<int> nDevCodelist,bool bset);
    //槽-区域批量设备操作
    void SendZoneBatchDevOperateToSeverSolt(int nStationID,unsigned char FUNType,unsigned int FUNSet_DeviceNum);
    //发送文字信息
    void SendTextAnalysis(TextFrame frame);

public:
    //发送更新数据消息（给所有连接终端-CTC、占线板）
    void sendUpdateDataMsg(MyStation *pStation, int _type=0x00, int currCtcIndex = -1, int currZxbIndex = -1);
    //向后台CTC逻辑发送数据
    void sendDataToCTCLogic(QByteArray pSendDate, int nLength);
public:
    //接收服务终端数据处理
    void recvServerData(QByteArray recvArray, int recvlen, QString clientAdd, int clientPort);
    //接收服务端转发教师机分路不良处理
    void recvServerDataJSJFLBL(QByteArray recvArray, int recvlen, QString clientAdd, int clientPort);
    //设置教师机发送分路不良标识
    void SetJSJFLBL( MyStation* pStation,QByteArray recvArray);
public:
    //系统窗口类型-1卡斯柯，2卡斯柯集控，3铁科，4通号...
    int wndType;
    //用于记录主窗体
    MainWindow *pMainWindow;
    //占线版主界面窗口
    ZXBWindow *pZXBMainWindow=nullptr;

    //放错办提示窗口
    AgainstWrongWidget* pAgainstWrongWidget = nullptr;


public:
    //初始化主窗口信息
    //void InitMainWnd(QMainWindow *widget);
    void InitMainWnd(MainWindow *widget);
    void InitAgainstWrongWnd();
    //...
    //初始化占线版主界面窗口
    void InitZXBMainWnd(ZXBWindow *pZXBWindow);
    //股道视图初始化
    void InitTrackView(ZXBTrackWidget* pZXBTrackWidget);
    //初始化占线版进路序列窗口信息
    void InitZXBRoutWnd(ZXBRouteWidget *pZXBRouteWidget);
    //初始化占线版防溜视图
    void InitZXBFLWnd(ZXBFLWidget* pZXBFLWidget);
    //初始化供电臂设置窗口视图
    void InitPowerSupplyArmDialog();
    //初始化封锁设备窗口视图
    void InitBlockingDevDialog();

public:
    QVector<TrainNumType*> v_TrainNumType;
    QVector<QString> v_TrainRunType;
    QVector<DisOrderTemplate> mVectDisOrderTemplate;
    ZXBWindowCon mZXBWindowCon;//占线版界面显示配置
    // 读取列车运行类型配置文件
    void ReadTrainNumTypeTXT();
    //读取调度命令类型模板
    void ReadDDOrderTemplate();
    // 各站列车运行类型数据同步初始化
    void InitStationTrainNumType();
    //读取占线版界面显示配置文件
    void ReadZXBWindowShowTxt();

public:
    //将接收到的数据根据帧头帧位进行分割并返回分割后的二维数组
    QVector<QByteArray> SplitReceiveData_SameHeadTail(QByteArray recvArray);

/************ mydoccomdata2.cpp函数定义 ************/
signals:
    //信号-发送数据给主通道-TCP
    void sendDataToMainSignal2(QByteArray dataArray,int len);

public:
    //发送数据给主通道-TCP
    void sendDataToCTCLogic2(QByteArray pSendDate, int nLength);
public slots:
    //槽-发送数据给CTC后台服务逻辑-TCP通道
    void sendDataToCTCServerSlot2(MyStation *pMyStation, QByteArray dataArray, int nLength);
    //TCP连接槽
    void tcpClientConnectedSlot(QString clientAdd, int clientPort);
    //TCP断开槽
    void tcpClientDisconnectedSlot(QString clientAdd, int clientPort);
    //解析服务端socket通道数据-TCP
    void receiveCTCServerDataSlot2(QByteArray dataArray,QString clientAdd, int clientPort);
    //防溜设置槽
    void SendAntiSlipSolt(GDData* pGDData,bool bSet,int nstationid);
public:
    //占线版向服务端发送防溜设置信息
    void sendAntiSlipDataToServer(GDData* pGDData,bool bSet,bool bSetLR,int nstationid);
    //占线版向服务端发送供电臂设置信息
    void SendPowerSupplyArmDataToSeverData(int nStaIndex, int nID, int bset);
    //占线版向服务端发送封锁设备设置信息
    void SendBlockingDevDataToSeverData(int nStaIndex,int nID,bool bset);

    TextAnalysis* pTextAnalysis = nullptr;
};

#endif // MYDOC_H
