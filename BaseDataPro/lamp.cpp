#include "lamp.h"

lamp::lamp()
{
    m_colorXiqi = 0;//RED;
    m_colorLuoXia = 0;//BLACK;

    FCZKMode_lamp = true;
    nModeState = 0;
    nModeApply = 0xFF;
    nStateSelect = true;//按图排路
    nPlanCtrl = true;//计划控制
    m_nComCTCCenter = true;//中心通信
    m_nComZLJState = true;//自律机通信
    m_bAllowZH = true;//允许回转
    m_nComLKState = true;//列控通信
    m_nX180 = false;//延迟解锁3分钟 吸起计时3分钟后落下
    m_nX30s = false;//延迟解锁30钟 吸起计时30钟后落下
    m_nS180 = false;//延迟解锁3分钟 吸起计时3分钟后落下
    m_nS30s = false;//延迟解锁30钟 吸起计时3分钟后落下
    m_nX240 = false;//延迟解锁4分钟 吸起计时3分钟后落下
    m_nX60s = false;//延迟解锁60s 吸起计时30钟后落下
    m_nS240 = false;//延迟解锁4分钟 吸起计时3分钟后落下
    m_nS60s = false;//延迟解锁60s 吸起计时3分钟后落下
    PD180s = false;
    m_strTips = "";



}
void lamp::Initlamp()
{
    QString ch = ",";
    StringSplit(DEVName, ch, DEVName_DJGD);

    if("CTC_MODE_CENTER" == DEVName_DJGD[0])
    {
        m_strTips = "中心操作方式";
    }
    else if("CTC_MODE_NORMAL" == DEVName_DJGD[0])
    {
        m_strTips = "车站调车操作方式";
    }
    else if("CTC_MODE_STATION" == DEVName_DJGD[0])
    {
        m_strTips = "车站操作方式";
    }
}
void lamp::Draw(QPainter *painter,long nElapsed, double nDiploid,QPoint offset,int type)
{
    if(type==0x55)
    {
        Draw_Th_kb(painter,nElapsed, nDiploid,offset);
    }
}
void lamp::DrawM(QPainter *painter,long nElapsed, double nDiploid,QPoint offset,int type, int flag)
{
    if(type==0x55)
    {
        Draw_Th_kbM(painter,nElapsed, nDiploid,offset);
    }
}
void lamp::Draw_Th_kb(QPainter *painter,long nElapsed, double nDiploid,QPoint offset)
{
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, true);
    QPen pen;
    QBrush newBrushBK;
    newBrushBK.setStyle(Qt::SolidPattern);
    newBrushBK.setColor(Qt::NoBrush);

    if(!gIsConnectInterlock)
    {
        newBrushBK.setColor(BLACK);
    }
    else
    {
        if("CTC_MODE_CENTER" == DEVName_DJGD[0])
        {
            if(!FCZKMode_lamp)
            {
                if(nModeState == 0)
                {
                    newBrushBK.setColor(getLightState(m_colorXiqi,nElapsed));
                }
                else if(nModeApply == 0)//处于申请状态
                {
                    newBrushBK.setColor(getLightState(8,nElapsed));
                }
                else
                {
                    newBrushBK.setColor(getLightState(m_colorLuoXia,nElapsed));
                }
            }
            else
            {
                newBrushBK.setColor(getLightState(m_colorLuoXia,nElapsed));
            }
        }
        else if("CTC_MODE_STATION" == DEVName_DJGD[0])
        {
            if(!FCZKMode_lamp)
            {
                if(nModeState == 1)
                {
                    newBrushBK.setColor(getLightState(m_colorXiqi,nElapsed));
                }
                else if(nModeApply == 1)//处于申请状态
                {
                    newBrushBK.setColor(getLightState(8,nElapsed));
                }
                else
                {
                    newBrushBK.setColor(getLightState(m_colorLuoXia,nElapsed));
                }
            }
            else
            {
                newBrushBK.setColor(getLightState(m_colorLuoXia,nElapsed));
            }
        }
        else if("CTC_MODE_NORMAL" == DEVName_DJGD[0])
        {
            if(!FCZKMode_lamp)
            {
                if(nModeState == 2)
                {
                    newBrushBK.setColor(getLightState(m_colorXiqi,nElapsed));
                }
                else if(nModeApply == 2)//处于申请状态
                {
                    newBrushBK.setColor(getLightState(8,nElapsed));
                }
                else
                {
                    newBrushBK.setColor(getLightState(m_colorLuoXia,nElapsed));
                }
            }
            else
            {
                newBrushBK.setColor(getLightState(m_colorLuoXia,nElapsed));
            }
        }
        else if("CTC_MODE_6502" == DEVName_DJGD[0])
        {
            //非常站控
            if(FCZKMode_lamp == true)
            {
                newBrushBK.setColor(getLightState(m_colorXiqi,nElapsed));
            }
            else
            {
                newBrushBK.setColor(getLightState(m_colorLuoXia,nElapsed));
            }
        }
        else if("BUILDROUTE_BYPLAN" == DEVName_DJGD[0])
        {
            /*按图排路*/
            if(nStateSelect == true)
            {
                newBrushBK.setColor(getLightState(m_colorXiqi,nElapsed));
            }
            else
            {
                newBrushBK.setColor(getLightState(m_colorLuoXia,nElapsed));
            }
        }
        else if("CONTROL_BYPLAN" == DEVName_DJGD[0])
        {
            /*计划控制*/
            if(nPlanCtrl == true)
            {
                newBrushBK.setColor(getLightState(m_colorXiqi,nElapsed));
            }
            else
            {
                newBrushBK.setColor(getLightState(m_colorLuoXia,nElapsed));
            }
        }
        else if("COMM_TOCENTER" == DEVName_DJGD[0])
        {
            //中心通信
            if(m_nComCTCCenter == true)
            {
                newBrushBK.setColor(getLightState(m_colorXiqi,nElapsed));
            }
            else
            {
                newBrushBK.setColor(getLightState(m_colorLuoXia,nElapsed));
            }
        }
        else if("COMM_TOINTERLOCK" == DEVName_DJGD[0])
        {
            //自律机通信
            if(m_nComZLJState == true)
            {
                newBrushBK.setColor(getLightState(m_colorXiqi,nElapsed));
            }
        }
        else if("PERMIT_BACK" == DEVName_DJGD[0])
        {
            //允许转回, 非常站控模式下，没有按钮按下则亮黄灯 20181111
            if((FCZKMode_lamp == true) && (m_bAllowZH == false))
            {
                newBrushBK.setColor(getLightState(m_colorXiqi,nElapsed));
            }
            else
            {
                newBrushBK.setColor(getLightState(m_colorLuoXia,nElapsed));
            }
        }
        else if("TRAFFIC_CONTROL" == DEVName_DJGD[0])
        {
            //列控通信
            if(m_nComLKState == true)
            {
                newBrushBK.setColor(getLightState(m_colorXiqi,nElapsed));
            }
        }
        //报警灯
        else if("X3m" == DEVName_DJGD[0])//"X3分"
        {
            //延迟解锁3分钟 吸起计时3分钟后落下
            if(m_nX180 == true)
            {
                //pFrame->m_nX180 = FALSE;
                newBrushBK.setColor(getLightState(m_colorXiqi,nElapsed));
            }
            else
            {
                newBrushBK.setColor(getLightState(m_colorLuoXia,nElapsed));
            }
        }
        else if("X30s" == DEVName_DJGD[0])
        {
            //延迟解锁30s 吸起计时30s后落下
            if(m_nX30s == true)
            {
                newBrushBK.setColor(getLightState(m_colorXiqi,nElapsed));
            }
            else
            {
                newBrushBK.setColor(getLightState(m_colorLuoXia,nElapsed));
            }
        }
        else if("S3m" == DEVName_DJGD[0])//"S3分"
        {
            //延迟解锁3分钟 吸起计时3分钟后落下

            /*按钮处于按下状态 开始计时*/
            if(m_nS180 == true)
            {
                newBrushBK.setColor(getLightState(m_colorXiqi,nElapsed));
            }
            else
            {
                newBrushBK.setColor(getLightState(m_colorLuoXia,nElapsed));
            }
        }
        else if("S30s" == DEVName_DJGD[0])
        {
            //延迟解锁30钟 吸起计时30s后落下

            /*按钮处于按下状态 开始计时*/
            if(m_nS30s == true)
            {
                newBrushBK.setColor(getLightState(m_colorXiqi,nElapsed));
            }
            else
            {
                newBrushBK.setColor(getLightState(m_colorLuoXia,nElapsed));
            }
        }
        else if("X4m" == DEVName_DJGD[0])//"X4分"
        {
            //延迟解锁4分钟 吸起计时4分钟后落下
            if(m_nX240 == true)
            {
                newBrushBK.setColor(getLightState(m_colorXiqi,nElapsed));
            }
            else
            {
                newBrushBK.setColor(getLightState(m_colorLuoXia,nElapsed));
            }
        }
        else if("X60s" == DEVName_DJGD[0])
        {
            //延迟解锁60钟 吸起计时60s后落下
            if(m_nX60s == true)
            {
                newBrushBK.setColor(getLightState(m_colorXiqi,nElapsed));
            }
            else
            {
                newBrushBK.setColor(getLightState(m_colorLuoXia,nElapsed));
            }
        }
        else if("S4m" == DEVName_DJGD[0])//"S4分"
        {
            //延迟解锁4分钟 吸起计时4分钟后落下

            /*按钮处于按下状态 开始计时*/
            if(m_nS240 == true)
            {
                newBrushBK.setColor(getLightState(m_colorXiqi,nElapsed));
            }
            else
            {
                newBrushBK.setColor(getLightState(m_colorLuoXia,nElapsed));
            }
        }
        else if("S60s" == DEVName_DJGD[0])
        {
            //延迟解锁60s 吸起计时60s后落下

            /*按钮处于按下状态 开始计时*/
            if(m_nS60s == true)
            {
                newBrushBK.setColor(getLightState(m_colorXiqi,nElapsed));
            }
            else
            {
                newBrushBK.setColor(getLightState(m_colorLuoXia,nElapsed));
            }
        }
        else if("PD180s" == DEVName_DJGD[0])
        {
            if(PD180s == true)
            {
                newBrushBK.setColor(getLightState(m_colorXiqi,nElapsed));
            }
            else
            {
                newBrushBK.setColor(getLightState(m_colorLuoXia,nElapsed));
            }
        }
        else if("DISPLAYLAMP_QJ" == DEVName_DJGD[0])
        {
            if(m_QJLampFlag == true)
            {
                newBrushBK.setColor(getLightState(m_colorXiqi,nElapsed));
            }
            else
            {
                newBrushBK.setColor(getLightState(m_colorLuoXia,nElapsed));
            }

        }
        //报警灯
        else
        {
            newBrushBK.setColor(getLightState(m_colorLuoXia,nElapsed));
        }

    }






//    newBrushBK.setColor(getLightState(m_colorXiqi,nElapsed));
    painter->setBrush(newBrushBK);

    pen.setColor(THIN_GRAY);
    pen.setWidth(2);
    painter->setPen(pen);
    painter->drawEllipse(pCenter.x() * nDiploid,pCenter.y() * nDiploid,16 * nDiploid,16 * nDiploid);
//    m_devRect=QRect(pCenter.x() * nDiploid,pCenter.y() * nDiploid, 16 * nDiploid, 16 * nDiploid);
    //painter->drawEllipse(pCenter.x() * nDiploid,pCenter.y() * nDiploid,ViewBtnAndLmapSize * nDiploid,ViewBtnAndLmapSize * nDiploid);
    m_devRect=QRect(pCenter.x() * nDiploid,pCenter.y() * nDiploid, ViewBtnAndLmapSize * nDiploid, ViewBtnAndLmapSize * nDiploid);
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, false);
    //绘制文字
    pen.setColor(WHITE);
    pen.setWidth(1);
    QFont font;
    font.setWeight(50);//字体宽度,0~99
    //font.setPointSize(12 * nDiploid);//字体高度,10表示高度为10个像素点
    font.setPointSize(ViewBtnAndLmapWordSize * nDiploid);//字体高度,10表示高度为10个像素点

    painter->setPen(pen);
    painter->setFont(font);
    QBrush BrushBK;
    BrushBK.setStyle(Qt::SolidPattern);
    BrushBK.setColor(Qt::NoBrush);
    painter->setBrush(BrushBK);
    QRectF rectLightnWord_X;
    rectLightnWord_X.setLeft(pCenter.x() * nDiploid-70 * nDiploid);
    rectLightnWord_X.setTop(pCenter.y() * nDiploid+18 * nDiploid);
    rectLightnWord_X.setRight(pCenter.x() * nDiploid+85 * nDiploid);
    rectLightnWord_X.setBottom(pCenter.y() * nDiploid+38 * nDiploid);
    if(blampshowbjtext)
        painter->drawText(rectLightnWord_X,Qt::AlignHCenter|Qt::AlignVCenter,m_strName);

    if(m_RangeVisible)
    {
        //绘制文字
        pen.setColor(YELLOW);
        QFont font1;
        font1.setWeight(80);//字体宽度,0~99
        font1.setPointSize(12 * nDiploid);//字体高度,10表示高度为10个像素点
        painter->setPen(pen);
        painter->setFont(font1);
        QBrush BrushBK1;
        BrushBK1.setStyle(Qt::SolidPattern);
        BrushBK1.setColor(Qt::NoBrush);
        painter->setBrush(BrushBK1);
        QRectF rectLightnWord_X1;
        rectLightnWord_X1.setLeft(pCenter.x() * nDiploid-70 * nDiploid);
        rectLightnWord_X1.setTop(pCenter.y() * nDiploid-28 * nDiploid);
        rectLightnWord_X1.setRight(pCenter.x() * nDiploid+85 * nDiploid);
        rectLightnWord_X1.setBottom(pCenter.y() * nDiploid-8 * nDiploid);
        painter->drawText(rectLightnWord_X1,Qt::AlignHCenter|Qt::AlignVCenter,m_strTips);
    }
}
void lamp::Draw_Th_kbM(QPainter *painter,long nElapsed, double nDiploid,QPoint offset)
{
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, true);
    QPen pen;
    QBrush newBrushBK;
    newBrushBK.setStyle(Qt::SolidPattern);
    newBrushBK.setColor(Qt::NoBrush);

    if(!gIsConnectInterlock)
    {
        newBrushBK.setColor(BLACK);
    }
    else
    {
        if("CTC_MODE_CENTER" == DEVName_DJGD[0])
        {
            if(!FCZKMode_lamp)
            {
                if(nModeState == 0)
                {
                    newBrushBK.setColor(getLightState(m_colorXiqi,nElapsed));
                }
                else if(nModeApply == 0)//处于申请状态
                {
                    newBrushBK.setColor(getLightState(8,nElapsed));
                }
                else
                {
                    newBrushBK.setColor(getLightState(m_colorLuoXia,nElapsed));
                }
            }
            else
            {
                newBrushBK.setColor(getLightState(m_colorLuoXia,nElapsed));
            }
        }
        else if("CTC_MODE_STATION" == DEVName_DJGD[0])
        {
            if(!FCZKMode_lamp)
            {
                if(nModeState == 1)
                {
                    newBrushBK.setColor(getLightState(m_colorXiqi,nElapsed));
                }
                else if(nModeApply == 1)//处于申请状态
                {
                    newBrushBK.setColor(getLightState(8,nElapsed));
                }
                else
                {
                    newBrushBK.setColor(getLightState(m_colorLuoXia,nElapsed));
                }
            }
            else
            {
                newBrushBK.setColor(getLightState(m_colorLuoXia,nElapsed));
            }
        }
        else if("CTC_MODE_NORMAL" == DEVName_DJGD[0])
        {
            if(!FCZKMode_lamp)
            {
                if(nModeState == 2)
                {
                    newBrushBK.setColor(getLightState(m_colorXiqi,nElapsed));
                }
                else if(nModeApply == 2)//处于申请状态
                {
                    newBrushBK.setColor(getLightState(8,nElapsed));
                }
                else
                {
                    newBrushBK.setColor(getLightState(m_colorLuoXia,nElapsed));
                }
            }
            else
            {
                newBrushBK.setColor(getLightState(m_colorLuoXia,nElapsed));
            }
        }
        else if("CTC_MODE_6502" == DEVName_DJGD[0])
        {
            //非常站控
            if(FCZKMode_lamp == true)
            {
                newBrushBK.setColor(getLightState(m_colorXiqi,nElapsed));
            }
            else
            {
                newBrushBK.setColor(getLightState(m_colorLuoXia,nElapsed));
            }
        }
        else if("BUILDROUTE_BYPLAN" == DEVName_DJGD[0])
        {
            /*按图排路*/
            if(nStateSelect == true)
            {
                newBrushBK.setColor(getLightState(m_colorXiqi,nElapsed));
            }
            else
            {
                newBrushBK.setColor(getLightState(m_colorLuoXia,nElapsed));
            }
        }
        else if("CONTROL_BYPLAN" == DEVName_DJGD[0])
        {
            /*计划控制*/
            if(nPlanCtrl == true)
            {
                newBrushBK.setColor(getLightState(m_colorXiqi,nElapsed));
            }
            else
            {
                newBrushBK.setColor(getLightState(m_colorLuoXia,nElapsed));
            }
        }
        else if("COMM_TOCENTER" == DEVName_DJGD[0])
        {
            //中心通信
            if(m_nComCTCCenter == true)
            {
                newBrushBK.setColor(getLightState(m_colorXiqi,nElapsed));
            }
            else
            {
                newBrushBK.setColor(getLightState(m_colorLuoXia,nElapsed));
            }
        }
        else if("COMM_TOCENTER" == DEVName_DJGD[0])
        {
            //中心通信
            if(m_nComCTCCenter == true)
            {
                newBrushBK.setColor(getLightState(m_colorXiqi,nElapsed));
            }
            else
            {
                newBrushBK.setColor(getLightState(m_colorLuoXia,nElapsed));
            }
        }
        else if("COMM_TOINTERLOCK" == DEVName_DJGD[0])
        {
            //自律机通信
            if(m_nComZLJState == true)
            {
                newBrushBK.setColor(getLightState(m_colorXiqi,nElapsed));
            }
        }
        else if("TRAFFIC_CONTROL" == DEVName_DJGD[0])
        {
            //列控通信
            if(m_nComLKState == true)
            {
                newBrushBK.setColor(getLightState(m_colorXiqi,nElapsed));
            }
        }
        else
        {
            //newBrushBK.setColor(getLightState(m_colorLuoXia,nElapsed));
            return;
        }
    }

//    newBrushBK.setColor(getLightState(m_colorXiqi,nElapsed));
    painter->setBrush(newBrushBK);

    pen.setColor(THIN_GRAY);
    painter->setPen(pen);
    painter->drawEllipse(pCenter.x() * nDiploid,pCenter.y() * nDiploid,16 * nDiploid,16 * nDiploid);
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, false);
    //绘制文字
    pen.setColor(WHITE);
    QFont font;
    font.setWeight(50);//字体宽度,0~99
    font.setPointSize(12 * nDiploid);//字体高度,10表示高度为10个像素点
    painter->setPen(pen);
    painter->setFont(font);
    QBrush BrushBK;
    BrushBK.setStyle(Qt::SolidPattern);
    BrushBK.setColor(Qt::NoBrush);
    painter->setBrush(BrushBK);
    QRectF rectLightnWord_X;
    rectLightnWord_X.setLeft(pCenter.x() * nDiploid-70 * nDiploid);
    rectLightnWord_X.setTop(pCenter.y() * nDiploid+18 * nDiploid);
    rectLightnWord_X.setRight(pCenter.x() * nDiploid+85 * nDiploid);
    rectLightnWord_X.setBottom(pCenter.y() * nDiploid+38 * nDiploid);
    painter->drawText(rectLightnWord_X,Qt::AlignHCenter|Qt::AlignVCenter,m_strName);
}
unsigned int lamp::getDevType()
{
    return Dev_LAMP;
}
QColor lamp::getLightState(int nType,int nElapsed)//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝）
{
    QColor color;
    color = BLACK;
    switch(nType)
    {
    case 0:
        {
            color = BLACK;
            return color;
            break;
        }
    case 1:
        {
            color = RED;
            return color;
            break;
        }
    case 2:
        {
            color = GREEN;
            return color;
            break;
        }
    case 3:
        {
            color = WHITE;
            return color;
            break;
        }
    case 4:
        {
            color = YELLOW;
            return color;
            break;
        }
    case 5:
        {
            if(nElapsed%2 == 0)
                color = RED;
            else
                color = BLACK;
            return color;
            break;
        }
    case 6:
        {
            if(nElapsed%2 == 0)
                color = GREEN;
            else
                color = BLACK;
            return color;
            break;
        }
    case 7:
        {
            if(nElapsed%2 == 0)
                color = WHITE;
            else
                color = BLACK;
            return color;
            break;
        }
    case 8:
        {
            if(nElapsed%2 == 0)
                color = YELLOW;
            else
                color = BLACK;
            return color;
            break;
        }
    case 9:
        {
            color = BLUE;
            return color;
            break;
        }
    case 10:
        {
            color = THIN_GRAY;
            return color;
            break;
        }

    }
    return color;
}
bool lamp::GetMousePoint(QPoint p)
{
    if ((m_devRect.contains(p)))
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool lamp::GetLampType(QString str)
{
    if(str=="CTC_MODE_CENTER"||str=="CTC_MODE_STATION"||str=="CTC_MODE_NORMAL"
            ||str=="CTC_MODE_6502"||str=="BUILDROUTE_BYPLAN"||str=="CONTROL_BYPLAN"
            ||str=="COMM_TOCENTER"||str=="COMM_TOINTERLOCK"||str=="PERMIT_BACK"||str=="TRAFFIC_CONTROL")
        return false;
    else
        return true;


}
