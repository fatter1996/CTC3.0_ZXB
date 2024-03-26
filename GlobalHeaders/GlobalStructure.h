#ifndef GLOBALSTRUCTURE_H
#define GLOBALSTRUCTURE_H
#include "Global.h"
typedef struct
{
    int stationID;
    QPoint offSetPt;
    QPoint staNamePt;
    int m_nCTCSoftID;
    int m_nLSSoftID;
    int m_nXLMNJSoftID;
    void init()
    {
        stationID=m_nLSSoftID=m_nCTCSoftID=m_nXLMNJSoftID=0;
        offSetPt=QPoint(0,0);
        staNamePt=QPoint(0,0);
    }
}StaInfo;

typedef struct
{
    QString str_Speech;
    int n_Count;
    void init()
    {
        str_Speech="";
        n_Count=0;
    }
}SpeechData;

typedef struct
{
    QString strName;
    int nStationID;
    QPoint pt;
    bool isRunFlag;
    int runTime;
    void init()
    {
        strName="";
        nStationID=runTime=0;
        pt=QPoint(0,0);
        isRunFlag=false;
    }
}DLB_Data;

typedef struct
{
    QString strType;//类型
    QString strTypeName;//车次类型名车
}TrainNumType;

typedef struct
{
    QVector<QString> vectBTJLChild;
}BTJL;//变通进路


//进路办理检查结果
struct CheckResult{
    int id = 0; //id(当前时间的毫秒数取后4个字节)
    int stationId = 0;
    bool bEnforced = false;//强制执行
    QString checkMsg;//检查结果信息
};

//用户登录信息
typedef struct
{
    int nstate;//用户登录状态
    QString strUserName;//用户名
    QString strPassWord;//密码
    int nindex;//登录顺序
    int nbc;//当前登录班次
    void Init()
    {
       nstate=-1;
       strUserName="";
       strPassWord="";
       nindex=-1;
    }
}UserInfo;
//QF信息
typedef struct
{
    int Type;//类型
    QString StrName;//名称
    int  Counter;//计数器
    QString JucDevName;//关联设备名称

    void Init()
    {
        Type = 0;
        StrName = "";
        Counter = 0;
        JucDevName ="";
    }
}QianFeng;
typedef struct
{
    QString strLevel;
    QString strType;
    QString strData;
    QString strDescribe;
    QString strSource;
    void Init()
    {
        strLevel="";
        strType="";
        strData="";
        strDescribe="";
        strSource="";
    }
}TableData;
//调度命令模板
typedef struct DisOrderTemplate
{
    QString strDisOrderHeader;	//调度命令模板标题
    QString strDisOrderContent;	//调度命令模板内容
    void Init()
    {
        strDisOrderHeader="";
        strDisOrderContent="";
    }
}DisOrderTemplate;

typedef struct TextFrame
{
    unsigned char captureState = 0; //录制状态,0xA5:开始录制，0X5A：结束录制，0xAA录制中状态，0x00其他状态
    unsigned char textDisplayState= 0; //文字显示状态,0xB5:显示文字，0X5B：不显示文字，0x00其他
    QPoint point = { 0,0 }; //坐标
    int duration = 0; //时长
    int textLenght = 0; //文字长度
    unsigned int textColor = 0; //文字颜色
    QByteArray text; //显示内容
}TextFrame;
//占线版界面显示配置信息
typedef struct ZXBWindowCon
{
    int nGDView=0;//股道视图车站数量
    QVector<int> nGDViewStationID;//股道视图车站ID
    bool bGDHV = true;//股道视图布局 true:垂直布局 false:TableWidget类型
    int nViewType = 1;//股道视图类型 1:股道视图模式;2:序列视图模式
    int nMaxRow = 1;//股道视图单股道最大显示条数

    int nJLView=0;//进路视图车站数量
    QVector<int> nRouteViewStationID;//进路序列视图车站ID
    bool bJLHV = true;//进路视图布局 true:水平布局 false:TableWidget类型

    int nFLView=0;//防溜视图车站数量
    QVector<int> nFLViewStationID;//防溜与站存车视图车站ID
    bool bFLHV = true;//防溜与站存车视图 true:水平布局 false：TableWidget类型

    int nDCView=0;//进路视图车站数量
    QVector<int> nShuntViewStationID;//进路序列视图车站ID
    bool bDCHV = true;//进路视图布局 true:水平布局 false:TableWidget类型

}ZXBWindowCon;
/*供电臂设置状态*/
typedef struct
{
    QString PowerName;//供电臂名称
    bool PowerFlag;//供电臂状态 ture:有电 false:停电
    QStringList PowerDEVName;//股道设备序列
    QStringList PowerDCName;//道岔设备序列
    QStandardItem *Item;
    QStandardItem *Item1;
    void Init()
    {
        PowerName = "";
        PowerFlag = true;
        PowerDEVName.clear();
        PowerDCName.clear();
        Item = nullptr;
        Item1 =  nullptr;
    }
}PowerStr;
typedef struct
{
    unsigned int qdnode;
    QString qdName;
    bool Blocked;//封锁状态
    QStringList FSDEVName;//封锁设备名称
    QList<int> FSDEVCode;//封锁设备编号
    QStandardItem *item;
    QStandardItem *item1;
    void Init()
    {
        qdnode = 0xffff;
        qdName = "";
        Blocked = false;
        FSDEVCode={};
        item = nullptr;
        item1 = nullptr;
    }
} qdData;
#define DEVTYPE_DC 0x11
#define DEVTYPE_QD 0x22
#define DEVTYPE_QJ 0x33
#define DEVTYPE_XH 0x44
#define DCDev 0x11
#define QDDev 0x22
#define QJDev 0x33
#define XHDev 0x44
struct DeviceInfo{
    QString name;
    int code = 0;
    int type = 0;

    DeviceInfo(int _type, int _code, QString _name)
    {
        type = _type;
        code = _code;
        name = _name;
    }
};
typedef struct
{
   //表格显示信息
    QStringList strlist;
    //股道占用信息
    QString strGDTrain;
    //类别
    QString strlb;
    //车种
    QString strcz;
    //
    //作业说明
    QString strzysm;
    //辆数
    QString strnum;
    //运用标记
    QString strbj;
    //去向
    QString strqx;
    void Init()
    {
        strlist.clear();
        strGDTrain="";
        strlb="";
        strcz="";
        strzysm="";
        strnum="";
        strbj="";
        strqx="";
    }
}TableShowData;
typedef  struct
{
    int m_nCode = -1;
    QString devName;

    int operType = 3;
    int state = 0;

    Qt::CheckState isSelectFlag = Qt::Checked;

    QString getOperStr()
    {
        switch (operType) {
        case 0 : return "";
        case 1 : return "单锁(定)";
        case 2 : return "单锁(反)";
        case 3 : return "单锁";
        case 4 : return "封";
        case 5 : return "封/单锁(定)";
        case 6 : return "封/单锁(反)";
        case 7 : return "封/单锁";
        default: return "";
        }
    }
    QString getStateStr()
    {
        switch (state & 0x0F) {
        case 0 : return "";
        case 1 : return "单锁 定位";
        case 2 : return "单锁 反位";
        case 3 : return "单锁";
        case 4 : return "封锁";
        case 5 : return "封锁 单锁 定位";
        case 6 : return "封锁 单锁 反位";
        case 7 : return "封锁 单锁";

        case 8 : return "解锁";
        case 9 : return "解锁";
        case 10 : return "解锁";
        case 11 : return "解锁";
        case 12 : return "解封";
        case 13 : return "解封 解锁";
        case 14 : return "解封 解锁";
        case 15 : return "解封 解锁";
        default: return "";
        }
    }
}ZoneDev;

struct ZoneData
{
    int id = -1;
    int stationId = -1;

    QString zoneName;

    QVector<ZoneDev> vecDcDev;
    QVector<ZoneDev> vecQdDev;
    QVector<ZoneDev> vecQjDev;
    QVector<ZoneDev> vecXhDev;
public:
    void SetAllCheck(Qt::CheckState state)
    {
        for(unsigned int i = 0;i<vecDcDev.size();i++)
        {
           vecDcDev[i].isSelectFlag = state;
        }
        for(unsigned int i = 0;i<vecQdDev.size();i++)
        {
           vecQdDev[i].isSelectFlag = state;
        }
        for(unsigned int i = 0;i<vecQjDev.size();i++)
        {
           vecQjDev[i].isSelectFlag = state;
        }
        for(unsigned int i = 0;i<vecXhDev.size();i++)
        {
           vecXhDev[i].isSelectFlag = state;
        }
    }

    void SetCheck(QString Name, Qt::CheckState state)
    {
        if(Name == zoneName)
        {
            SetAllCheck(state);
        }
        else if(Name == "道岔")
        {
            for(unsigned int i = 0; i < vecDcDev.size(); i++)
            {
                  vecDcDev[i].isSelectFlag = state;
            }
        }
        else if(Name == "股道/无岔")
        {
            for(unsigned int i = 0; i < vecQdDev.size(); i++)
            {
                  vecQdDev[i].isSelectFlag = state;
            }
        }
        else if(Name == "区间")
        {
            for(unsigned int i = 0; i < vecQjDev.size(); i++)
            {
                  vecQjDev[i].isSelectFlag = state;
            }
        }
        else if(Name == "信号机")
        {
            for(unsigned int i = 0; i < vecXhDev.size(); i++)
            {
                  vecXhDev[i].isSelectFlag = state;
            }
        }
        else
        {
            for(unsigned int i = 0; i < vecDcDev.size(); i++)
            {
                if(Name == vecDcDev[i].devName)
                {
                  vecDcDev[i].isSelectFlag = state;
                  return ;
                }
            }
            for(unsigned int i = 0; i < vecQdDev.size(); i++)
            {
                if(Name == vecQdDev[i].devName)
                {
                  vecQdDev[i].isSelectFlag = state;
                  return ;
                }
            }
            for(unsigned int i = 0; i < vecQjDev.size(); i++)
            {
                if(Name == vecQjDev[i].devName)
                {
                  vecQjDev[i].isSelectFlag = state;
                  return ;
                }
            }
            for(unsigned int i = 0; i < vecXhDev.size();i++)
            {
                if(Name == vecXhDev[i].devName)
                {
                  vecXhDev[i].isSelectFlag = state;
                  return ;
                }
            }
        }
    }

    void AddDevList(QVector<DeviceInfo> vecDevData)
    {
        ZoneDev zone;
        for(int i = 0; i < vecDevData.size(); i++)
        {
            if(vecDevData.at(i).name == "")
                continue;
            ZoneDev* zoneT = GetZoneDevByCode(vecDevData.at(i).type, vecDevData.at(i).code);
            if(zoneT)
            {
                zoneT->isSelectFlag = Qt::Checked;
                continue;
            }

            zone.devName = vecDevData.at(i).name;
            zone.m_nCode = vecDevData.at(i).code;
            if(vecDevData.at(i).type == DEVTYPE_DC)
                zone.operType = 3;
            else zone.operType = 4;

            switch (vecDevData.at(i).type) {
            case DEVTYPE_DC : {}vecDcDev.push_back(zone); break;
            case DEVTYPE_QD : vecQdDev.push_back(zone); break;
            case DEVTYPE_QJ : vecQjDev.push_back(zone); break;
            case DEVTYPE_XH : vecXhDev.push_back(zone); break;
            }
        }
    }

    ZoneDev* GetZoneDevByCode(int type, int code)
    {
        if(type == DEVTYPE_DC)
        {
            for(unsigned int i = 0; i < vecDcDev.size(); i++)
            {
                  if(vecDcDev.at(i).m_nCode == code)
                      return &vecDcDev[i];
            }
            return nullptr;
        }
        else if(type == DEVTYPE_QD)
        {
            for(unsigned int i = 0; i < vecQdDev.size(); i++)
            {
                  if(vecQdDev.at(i).m_nCode == code)
                      return &vecQdDev[i];
            }
            return nullptr;
        }
        else if(type == DEVTYPE_QJ)
        {
            for(unsigned int i = 0; i < vecQjDev.size(); i++)
            {
                  if(vecQjDev.at(i).m_nCode == code)
                      return &vecQjDev[i];
            }
            return nullptr;
        }
        else if(type == DEVTYPE_XH)
        {
            for(unsigned int i = 0; i < vecXhDev.size(); i++)
            {
                  if(vecXhDev.at(i).m_nCode == code)
                      return &vecXhDev[i];
            }
            return nullptr;
        }
        return nullptr;
    }

    void ResrtState()
    {
        for(unsigned int i = 0; i < vecDcDev.size(); i++)
        {
            vecDcDev[i].state |= 0x08;
            vecDcDev[i].state &= 0x1f;
            vecDcDev[i].operType = 3;
        }
        for(unsigned int i = 0; i < vecQdDev.size(); i++)
        {
            vecQdDev[i].state |= 0x08;
            vecQdDev[i].state &= 0x1f;
            vecQdDev[i].operType = 4;
        }
        for(unsigned int i = 0; i < vecQjDev.size(); i++)
        {
            vecQjDev[i].state |= 0x08;
            vecQjDev[i].state &= 0x1f;
            vecQjDev[i].operType = 4;
        }
        for(unsigned int i = 0; i < vecXhDev.size();i++)
        {
            vecXhDev[i].state |= 0x08;
            vecXhDev[i].state &= 0x1f;
            vecXhDev[i].operType = 4;
        }
    }

    void Clear()
    {
        vecDcDev.clear();
        vecQdDev.clear();
        vecQjDev.clear();
        vecXhDev.clear();
    }

}; //分区定义


//联锁表进路
struct InterlockRoute
{
    QString     Number;//序号
    QString     Type;//类型
    QStringList BtnArr;//按钮
    QString     strXHD;//信号机
    QStringList DCArr;//道岔
    QStringList QDArr;//区段
    QStringList DDXHDArr;//敌对信号
};
#endif // GLOBALSTRUCTURE_H
