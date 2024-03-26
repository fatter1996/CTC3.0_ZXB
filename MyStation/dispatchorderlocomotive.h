#ifndef DISPATCHORDERLOCOMOTIVE_H
#define DISPATCHORDERLOCOMOTIVE_H

#include <QString>
#include <QDateTime>


//机车信息
struct LocomotiveInfo{
    QString strCheCi;//车次
    QString strLocomotive;//机车
    QDateTime timRecv;//机车接收时间
    int nRecvState = 0;//接收状态 1:接收 0:未签收
};

//机车调度命令
class DispatchOrderLocomotive
{
public:
    DispatchOrderLocomotive();

public:
    int station_id = 0;
    int order_id = 0;//id
    int orderType;//命令种类
    int uNumber = 0;//命令号
    QString strType;//命令类型
    QString strContent;//命令内容
    QString strStation;//车站
    QString strDutyName;//值班人
    QDateTime timCreate;//创建时间
    bool bSend = false;//发送状态，0保存未发送，1已发送
    bool bDel=false;//删除指令
    QDateTime timSend;//发送时间
    QVector<LocomotiveInfo> vectLocmtInfo;//机车接收信息
};

#endif // DISPATCHORDERLOCOMOTIVE_H
