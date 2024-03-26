#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ZXBWindow/zxbwindow.h"

#include <QPoint>
#include <QCloseEvent>//头文件记得添加
#include <QComboBox>
#include <QPushButton>
#include <QUdpSocket>
#include <QHostAddress>
#include <QTextToSpeech>
#include <QTime>
#include "Dialog/memorandumdialog.h"

//CTC终端类型
#define CTCDEVTYPE_SINGLE 0x00 //单机版
#define CTCDEVTYPE_ZBY    0x01 //值班员终端
#define CTCDEVTYPE_XHY    0x02 //信号员终端


#pragma execution_character_set("utf-8")


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void Init();
    ZXBWindow* InitZXBWindow(ZXBWindowCon* pZXBWindowCon, QVector<MyStation*>mMyStationArray);
    //void GetSplitWindowConfig();
    void SplitWindow();
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    void KeyDialogbox(QString str);
    void AlarmSpeech(QString str);//语音告警

    //UDP通信
    QString IP_Name;//IP地址名称
    QHostAddress IP_LocalAdress;//本机IP地址
    QHostAddress IP_OppAdress;//链接IP地址
    int IP_LocalPort;//本机IP端口
//    int IP_OppPort;//链接IP端口
    unsigned int IP_OppPort;//链接IP端口
    QUdpSocket *udpSocket;
    bool RegSuccFlag;//注册成功标志位

    QHostAddress IP_LocalAdress_T3;//本机IP地址
    QHostAddress IP_OppAdress_T3;//链接IP地址
    int IP_LocalPort_T3;//本机IP端口
    int IP_OppPort_T3;//链接IP端口

    //读取车站参数
    QString STATIONNAME;
    unsigned int STATIONID;
    int BOARDNum;
    int CTCDevType;
    int TargetAddr;
    //阶段计划签收状态
    bool JDJHQSState;
//    bool JDJHQSState_1;
    bool JDJHQSState_Flag;
    QPushButton *btn9;
    //调度命令签收
    QPushButton *btn10;
    bool DDMLQSState;
    bool DDMLQSState_Flag;
    //void ReadSQLData(unsigned char *recvArray, unsigned long recvlen);
    //MainWindowForStationTrack *mainWindowForStationTrack = NULL;
    bool commState = false;

    //设置按钮
    void Menu_SetButton();
    QMenu *m_menu_Set;//设置按钮下拉菜单
    QAction *m_ZXBST_Set;//占线板视图设置
    void ZXBViewSetFun();//占线板视图设置功能槽函数
    QAction *m_GDSTXS_Set;//股道视图显示设置
    void GDSTViewSetFun();//股道视图显示设置功能
    QAction *m_FXXS_Set;//方向显示设置
    void StationDirectionFun();//方向显示设置：
    QAction *m_DCCS_Set;//调车参数配置
    QAction *m_DCYXJ_Set;//调车优先级配置
    QAction *m_DCGZ_Set;//调车规则配置
    QAction *m_GNAN_Set;//功能按钮设置
    void FunctionPushButtonFun();//功能按钮设置
    QAction *m_ZCJMXS_Set;//站场界面显示设置
    QAction *m_BJTS_Set;//报警提示配置
    void BJTSSetFun();//报警提示配置：
    QAction *m_Version;//版本信息

    //T3按钮
    void Menu_T3Button();
    QMenu *m_menu_T3;//T3按钮下拉菜单
    QAction *m_GDBState_Set;//设置供电臂状态
    QAction *m_FSSBCZ;//封锁设备操作
    QAction *m_QYPLSBCZ;//区域批量设备操作
    QAction *m_ZCCGL;//站存车管理
    QAction *m_SBYXFX;//设备影响分析
    QAction *m_CZRZCX;//操作日志查询
    QAction *m_FLSZCX;//防溜设置查询
    QAction *m_XXTS;//信息提示


    //登录按钮
//    void LoginButtonPro();
//    void Dialog_LoginButton();
    QComboBox *comboBox_ID;
    QComboBox *comboBox_Name;
    QString LoginID;
    QString LoginName;
    int LoginState;//0为默认，1为值班员，2为信号员
    QPushButton *btn_Enter;
    QPushButton *btn_Net;//网络连接状态按钮
    QPushButton *btn_Memorandum;//备忘录按钮
//
    //Monitor *monitor=NULL;

    /**股道进路对照表数据结构**/
    typedef struct
    {
        int JLIndex;
        QString JLType;//进路类型
        QString JLPushButton;//进路按钮
        QString JLBMPPath;//进路图片路径

    }TrackBMP;
    TrackBMP BMPArray[300];
    int TrackBMP_JLNum;    //进路数量
    //void ReadRouteBmp();    //读取进路图片路径数据

    struct StationLiansuo
    {
        int nCode;
        QString strGDName;
        QString strLeftNode;
        QString strRightNode;
    }StationGDNode[50];
    int m_nStationGDNode;	//股道数量

    //void ReadGDConfig();    //读取区段设备结点关联关系数据

    typedef struct
    {
        QString strType;//类型
        QString strTypeName;//车次类型名车
    }TrainNumType;
    QVector<TrainNumType> v_TrainNumType;
    QVector<QString> v_TrainRunType;
    //void ReadTrainNumTypeTXT();

    typedef struct            //股道可到达的延续终端
    {
        QStringList strArrGD; //股道列表
        QStringList strArrYXEnd;//延续终d列表
        QString strDefaultEnd; //默认延续终端
    }GDYXJL_T;

    typedef struct            //延续进路定义
    {
        QString strXHDbegin; //延续进路始端
        int nGDYXJLCount;
        GDYXJL_T GD_YXJL[50];//股道们相应的延续终d
    }YXJLGroup_T;

    typedef struct //S_TGANInfo //上行通过按钮定义
    {
        int nS_nCode;      //上行通过按钮十进制设备编号
        QString strS_JuncXHD;  //上行通过按钮关联信号机名称
    }S_TGANInfos_T;

    typedef struct //X_TGANInfo //下行通过按钮定义
    {
        int nX_nCode;      //下行通过按钮十进制设备编号
        QString strX_JuncXHD;  //下行通过按钮关联信号机名称
    }X_TGANInfos_T;

    typedef struct              //接发车口定义
    {
        QString strJCKName;//接车口信号机名称//CString strName;
        int     nJCKCode;  //接车口设备号//int     nCode;
        QString strFCKName;//发车口信号机名称
        int     nFCKCode;  //发车口设备号
        QString strDirectSta;//CString strDirectSta; //接发车口方向车站名称（车站名称）
        QString strJCKjucJJQD;//接车口关联的接近区段（1个）
        QStringList strArrJckJJQD;//接车口关联的接近区段数组
        QString strFCKjucLQQD;//发车口关联的接近区段（1个）
        QStringList strArrFckLQQD;//发车口关联的离去区段数组
    }JFCK_T;

    typedef struct //ConfigInfo //车站配置信息定义结构体
    {
        QString strStaName;       //车站名
        int nCTCDevType;      //单机版0x00，值班员终端0x01,信号员终端0x02
        bool bWindows7;        //操作系统是否是Win7
        bool bGaoTie;          //高铁站标志
        bool bStaSXLORR;       //上下行方向,站场下行方向对应左边1，否则为0
        bool bChgModeNeedJSJ;  //控制模式转换是否需要教师机（中心）同意
        bool bNeedInterlockTable;//联锁表功能是否需要(暂时为进路终端可按下判断)
        int YXJLBeginNum;
        YXJLGroup_T YXJLGroup[40];//延续进路始端和默认终端信息

        int STGANnum;        //上行通过按钮数目
        S_TGANInfos_T STGANInfos[10];//上行通过按钮结构体数组-最多10个  *pstSTGANInfos[8];
        int XTGANnum;        //下行通过按钮数目
        X_TGANInfos_T XTGANInfos[10];//下行通过按钮结构体数组-最多10个 *pstXTGANInfos[8];

        int XF_nCode;        //XF/XN...（下反/下逆等）十进制设备编号
        int X_nCode;         //下行接车信号机十进制设备编号
        QString X_JuncXHD;       //下行接车信号机名称
        int SF_nCode;        //SF/SN...（上反/上逆等）十进制设备编号
        int S_nCode;         //上行接车信号机十进制设备编号
        QString S_JuncXHD;       //上行接车信号机名称


        bool SpeechFlag;//可语音播报标志
        int JCKCount;
        JFCK_T JFCKInfo[40];//接发车口信息
        int BTJLCount;//有变通进路的进路数目
        QString BTJLArray[300];//(基本进路|变通进路|变通进路)

        int  RouteDeleteTime;//进路序列删除时间秒
        bool bHaveDataBase;//有无数据库
        bool JL_FL_Lenth90;
        QStringList XianLuSuoXHDArray;//线路所信号机 //BOOL bXianLuSuo;//线路所
    }StaConfig_T;
    StaConfig_T StaConfigInfo;

    typedef struct
    {
        QVector<QString> vectBTJLChild;
        void Init()
        {
            vectBTJLChild.clear();
        }
    }BTJL;
    QVector<BTJL> vectBTJL;//有变通进路的进路信息
public:
    void InitStationList(QVector<MyStation*> vectMyStation) { m_vectMyStation = vectMyStation; }
    //获取占线版与服务端通信状态
    void SetServiceCommFlag(bool b);
    bool GetServiceCommFlag();
    //设置网络状态按钮图片
    void SetNetBtnPNG(bool b);
    //初始化区域批量设备操作菜单
    void InitZoneBatchDevOperate();

private:
    virtual void timerEvent(QTimerEvent *event) override;//定时器

private:
    Ui::MainWindow *ui;
    QPoint z;
    //备忘录
    MemorandumDialog* pMemorandumDialog = nullptr;
    QVector<MyStation*> m_vectMyStation;

    QStringList strmStationNameList={};
    int  timerIdMemorandum = -1;
    int nTimeCount=-1;
    int  timerId_500 = -1;

    ZXBWindow* pZXBWindow = nullptr;
signals:
    //信号-供电臂设置
    void PowerSupplyArmDialogShowSignal();
    //信号-封锁设备操作
    void BlockingDevDialogShowSignal();
    //信号-封锁设备操作
    void ZoneBatchDevOperateDialogShowSignal(QString strStationName);
    //信号-批量操作
    void SendZoneBatchDevOperateToSeverSignal(int nStationID,unsigned char FUNType,unsigned int FUNSet_DeviceNum);
signals:
    void SendTextAnalysisSignal(int index, QPoint pt, QString dev = "");
private slots:
    //T3
    void clicked_T3();
    void clicked_btn4_openMointer();//打开站场界面
    void clicked_btn5_openmainWindowForStationTrack();//打开占线板界面
    void Dialog_NetStatButton();//通信连接按钮
    void clicked_btn7_openBWL();//打开备忘录
    void Dialog_LoginButton();//登陆按钮
    void clicked_btn9_openJSJDJH();//签收阶段计划
    void clicked_btn10_openDDMLCL();//签收调度命令
    void clicked_btn12_Close();//关闭
    void clicked_btn12_KeyDialogbox();
    void clicked_LoginButton();//登陆
    void clicked_LogoutButton();//登出

    //槽-供电臂设置
    void PowerSupplyArmDialogSolt();
    //槽-封锁设备操作
    void BlockingDevDialogSolt();
    //槽-区域批量设备操作
    void ZoneBatchDevOperateDialogSolt(QAction *action);
    //槽-备忘录按钮图标变化
    void SendBtnIncoChangeSolt(bool b);
    //站场重置备忘录
    void ResetMemorandumDialog();

};
#endif // MAINWINDOW_H



















