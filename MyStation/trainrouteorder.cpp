#include "trainrouteorder.h"

#pragma execution_character_set("utf-8")

TrainRouteOrder::TrainRouteOrder()
{
    m_nAutoTouch = false;
    m_nManTouch = false;
    m_bElectric = true;
    m_strRouteState = "等待";
    m_ntime = 0;

    //初始化UDP
    memset(m_byArrayUDPJLOrderDate, 0, 30);
    for (int i = 0; i < 4; i++)
    {
        m_byArrayUDPJLOrderDate[i] = 0xEF;
        m_byArrayUDPJLOrderDate[30 - i - 1] = 0xFE;
    }
    m_byArrayUDPJLOrderDate[4] = 0x1E;
    m_byArrayUDPJLOrderDate[9] = 0x88;//信息分类码 （CTC车务终端->联锁仿真机）
    m_byArrayUDPJLOrderDate[10] = 0x01;//功能按钮类型-列车进路
}

//0"等待"、1"正在触发"、2"触发完成"、3"占用"、4"出清"、5"取消"
void TrainRouteOrder::SetState(int nState)
{
    m_nOldRouteState = nState;
    switch(nState)
    {
    case 0:
        m_strRouteState = "等待";
        break;
    case 1:
        m_strRouteState = "正在触发";
        break;
    case 2:
        m_strRouteState = "触发完成";
        break;
    case 3:
        m_strRouteState = "占用";
        break;
    case 4:
        {
            m_timClean = QDateTime::currentDateTime();
            m_strRouteState = "出清";
        }
        break;
    case 5:
        m_strRouteState = "取消";
        //m_strRouteState =  "已取消";
    default:
        break;
    }
}
