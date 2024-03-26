#ifndef TRAINROUTEORDER_H
#define TRAINROUTEORDER_H

#include <QString>
#include <QDateTime>


#define MSGTYPE_MANTRIGGER 1 //人工触发
#define MSGTYPE_SETTRIGGERTYPE 2 //触发方式改变
#define MSGTYPE_TRACKINFO 3 //修改股道信息
#define MSGTYPE_ROUTEDESCRIP 4 //修改进路描述
#define MSGTYPE_PERMISSION 5 //权限修改
#define MSGTYPE_DELETE 6 //删除
#define MSGTYPE_DJKX 9 //单机开行
#define MSGTYPE_CANCEL 255 //取消进路

//列车进路序列
class TrainRouteOrder
{
public:
    TrainRouteOrder();

public:
    int station_id = 0;//车站id
    int route_id = 0;//进路序列id
    int father_id = 0;//父进路序列id（组合进路使用）
    int m_nPlanNumber = 0;//计划号
    int m_btBeginOrEndFlg = 0;//（本条进路所属的阶段计划类型）始发（终到）标志位--0xAA正常(接发) 0xBB始发 0xCC终到 0xDD通过
    int m_btRecvOrDepart = 0;//0x00 表示到达 0x01 表示出发
    QString m_strTrainNum;//车次
    QString m_strTrainNumOld;//车次
    QString m_strTrack;//进路股道名称
    int m_nTrackCode = 0;//进路股道设备号
    int m_nGDPos = 0;//股道在站场数组中的索引
    QDateTime m_timPlanned;//计划时间
    QDateTime m_timBegin;//开始时间
    QDateTime m_timClean;//出清时间

    QString m_strXHD_JZk;//进站口信号机
    QString m_strXHD_CZk;//出站口信号机
    int m_nCodeReachStaEquip = 0;//进站口信号设备号
    int m_nCodeDepartStaEquip = 0;//出站口信号设备号
    bool m_nAutoTouch = false;//自触
    bool m_nManTouch = false;//人工触发
    bool m_bElectric = true;//电力
    int m_nLevelCX = 0;//超限等级
    int m_nLHFlg = 0;//1 列车 0 货车
    int bXianLuSuo = 0;//线路所标志
    QString m_strRouteDescrip;//进路描述（"-"分割）
    QString m_strRouteDescripReal;//进路描述（实际触发","分割）
    int m_nOldRouteState = 0;//进路当前状态//0"等待"、1"正在触发"、2"触发完成"、3"占用"、4"出清"、5"取消"
    QString m_strRouteState;//进路当前状态
    int m_bSuccessed = 0;//办理成功失败表示,1成功,2失败,0无效
    QString m_strDirection;//进路方向
    int m_bYXJL = 0;//是延续进路
    int m_bZHJL = 0;//是组合进路（组合进路的主进路此标志为1）
    QString m_strXHDBegin;//进路始端
    QString m_strXHDEnd;//进路终端
    QString m_strXHDYXEnd;//延续进路终端信号机
    int m_bBTJL = 0;//是变通进路
    QStringList strArrayBTJL;//变通进路数组
    int m_bConfirmed = 0;//确认进路，阶段计划下达更新信息时，确认的进路只更新时间
    int m_nIndexRoute = 0;//匹配的联锁表表索引

    bool m_bRunNow = false;//立即执行
    int m_ntime = 0;//临时计数器
    int m_bDeleteFlag = 0;//删除标志
    int m_bCreateByMan = 0;//是否人工办理进路时生成

    QVector<TrainRouteOrder*> m_vecSonTrainRouteOrder;

public:
    QStringList tempXhdArray;//进路信号机数组
    QStringList tempRouteBtnArray;//进路按钮数组
    //进路命令帧
    unsigned char m_byArrayUDPJLOrderDate[30];
public:
    QString checkMsg;//防错办检查信息
public:
    void SetState(int nState);	//0"等待"、1"正在触发"、2"触发完成"、3"占用"、4"出清"、5"取消"

    bool m_bSx;//上行或下行

    QString m_strRouteDescrip_ZHJL;//组合进路的进路描述
    bool RouteDescrip_ZHJL_Flag;//组合进路展开与否标志位
    int number_ZHJL_Flag;//组合进路展开数目
    int m_nOldRouteState_ZHJL_0;//组合进路小进路0的状态
    int m_nOldRouteState_ZHJL_1;//组合进路小进路1的状态

    bool m_nAutoTouch_ZHJL_0;	//组合进路小进路0的自触
    bool m_nAutoTouch_ZHJL_1;	//组合进路小进路1的自触

    bool DeleteZHJLFlag_0;//删除标志位
    bool DeleteZHJLFlag_1;

    int m_nRowIndex = -1; //所在行号
};

#endif // TRAINROUTEORDER_H
