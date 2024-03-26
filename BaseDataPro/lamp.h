#ifndef LAMP_H
#define LAMP_H

#include "BaseData.h"
class lamp: public CBaseData
{
public:
    explicit lamp();

public:
    int m_colorXiqi;//RED;
    int m_colorLuoXia;//BLACK;
    QString DEVName;//灯对应的设备名称
    QStringList DEVName_DJGD;
    QString m_strTips;//提示文字

    bool FCZKMode_lamp; //非常站控
    int nModeState;//模式转换//0为中心控制,1为车站控制,2为车站调车
    int nModeApply;//模式申请//0为中心控制,1为车站控制,2为车站调车//申请状态时,灯显示黄闪
    bool nStateSelect;//按图排路
    bool nPlanCtrl;//计划控制
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
    bool PD180s;//坡道解锁180s 吸起计时3分钟后落下
    bool m_nJWDState;//机务段状态灯
    int m_nTicks;//读秒
    bool m_QJLampFlag = false;

    bool blampshowbjtext=true;//报警灯文字显示标志

    void Initlamp();
    virtual void Draw(QPainter *painter,long nElapsed, double nDiploid,QPoint offset,int type);
    virtual void DrawM(QPainter *painter,long nElapsed, double nDiploid,QPoint offset,int type, int flag = 0);
    void Draw_Th_kb(QPainter *painter,long nElapsed, double nDiploid,QPoint offset);
    void Draw_Th_kbM(QPainter *painter,long nElapsed, double nDiploid,QPoint offset);
    virtual unsigned int getDevType();
    QColor getLightState(int nType,int nElapsed);
    bool GetMousePoint(QPoint p);
    bool GetLampType(QString str);//获取灯类型 用于报警灯文字显示
    int ViewBtnAndLmapWordSize=12;//界面按钮名称及标识灯文字大小；//文字大小
    int ViewBtnAndLmapSize=16;//界面按钮名称及标识灯文字大小；按钮（指示灯大小）
//成员变量声明
private:

};

#endif // LAMP_H
