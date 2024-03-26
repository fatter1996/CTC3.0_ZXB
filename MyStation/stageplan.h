#ifndef STAGEPLAN_H
#define STAGEPLAN_H

#include <QString>
#include <QDateTime>

//阶段计划详情
class StagePlan
{
public:
    StagePlan();

public:
    int detail_id = 0;//阶段计划详情id
    int station_id = 0;//车站id
    int plan_id = 0;//批计划id
    int m_nPlanNumber = 0;//计划号
    QDateTime m_timJHRcv;//计划接收时间
    int m_btStagePlanKind = 0;//阶段计划类型（增加或删除）
    int m_btBeginOrEndFlg = 0;//始发（终到）标志位  0xAA正常(接发) 0xBB始发 0xCC终到 0xDD通过
    QString m_strDispatchDesk;//调度台名称
    int m_nStateSignPlan = 0;//签收状态

    QString m_strReachTrainNum;//列车接车车次
    QString m_strReachTrainNumOld;//列车接车车次
    QString m_strRecvTrainTrack;//接车股道
    int m_nRecvTrainTrack = 0;//接车股道设备号
    QDateTime m_timProvReachStation;//规定到站时间
    QString m_strXHD_JZk;//进站口信号机
    int m_nCodeReachStaEquip = 0;//进站口信号机设备号

    QString m_strDepartTrainNum;//列车发车车次
    QString m_strDepartTrainNumOld;//列车发车车次
    QString m_strDepartTrainTrack;//发车股道
    int m_nDepartTrainTrack = 0;//发车股道设备号
    QDateTime m_timProvDepaTrain;//规定出发时间
    QString m_strXHD_CZk;//出站口信号机
    int m_nCodeDepartStaEquip = 0;//出站口信号机设备号

    int m_bDeleteFlag = 0;//删除标志
    int m_bElectric = 0;//电力
    int m_nLevelCX = 0;//超限等级
    int m_nLHFlg = 0;//列车的客货标志
    int m_nIndexLCLX = 0;//列车类型序号（管内动车组、通勤列车等）
    QString m_strLCLX;
    int m_nIndexYXLX = 0;//运行类型序号（动车组、快速旅客列车等）
    QString m_strYXLX;
    int bXianLuSuo = 0;//线路所
};

#endif // STAGEPLAN_H
