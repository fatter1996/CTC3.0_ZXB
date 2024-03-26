#include "mystation.h"

#include <QTextCodec>
#include <QtDebug>
#include <Windows.h>
#include <QMessageBox>
#include "Log/log.h"
#pragma execution_character_set("utf-8")

MyStation::MyStation()
{
    isThreadStart = false;
    ticksOfThread = 0;
    nDiploid = 1.0f;
    nDiploidCheCi = 1.0f;
    nDiploidMuti = 1.0f;
    nDiploidCheCiMuti = 1.0f;
    m_nFCZKMode = false; //非常站控
    m_bSDJS = false;//上电解锁
    m_bGZTZ = false;//故障通知
    m_nModeState = 1;//模式转换//0为中心控制,1为车站控制,2为车站调车
    nStateSelect = false;//按图排路
    nPlanCtrl = false;//计划控制
    m_nComCTCCenter = false;//中心通信
    m_nComZLJState = false;//自律机通信
    m_bAllowZH = false;//允许回转
    m_nComLKState = false;//列控通信
    m_nX180 = false;//延迟解锁3分钟 吸起计时3分钟后落下
    m_nX30s = false;//延迟解锁30钟 吸起计时30钟后落下
    m_nS180 = false;//延迟解锁3分钟 吸起计时3分钟后落下
    m_nS30s = false;//延迟解锁30钟 吸起计时3分钟后落下
    m_nX240 = false;//延迟解锁4分钟 吸起计时3分钟后落下
    m_nX60s = false;//延迟解锁60s 吸起计时30钟后落下
    m_nS240 = false;//延迟解锁4分钟 吸起计时3分钟后落下
    m_nS60s = false;//延迟解锁60s 吸起计时3分钟后落下
    PD180s = false;
    m_nJWDState = false;//机务段状态灯
    m_nPDJSState = false;//坡道解锁
    commToServer = 0;
    m_nSXYDZS = false;//上行引导总锁
    m_nXXYDZS = false;//下行引导总锁
    m_nQCYDZS = false;//全场引导总锁
    m_nRoutePermission = 0;
    ANFlashFlag = 0;
    ButtonFlashDown = false;
    StaConfigInfo.JCKCount = 0;
    tts = new QTextToSpeech(0);

//    GDWordColor = 0xFF;//股道文字颜色
//    GDWordSize = 0xFF;//股道文字大小
//    GDDCWordColor_DW = 0xFF;//道岔文字颜色
//    GDDCWordColor_FW = 0xFF;//道岔文字颜色
//    GDDCWordColor_SK = 0xFF;//道岔文字颜色
//    GDDCWordSize = 0xFF;//道岔文字大小
//    XHDWordColor = 0xFF;//信号灯文字颜色
//    XHDWordSize = 0xFF;//信号灯文字大小
//    DCQDWordSize= 0xFF;//道岔区段文字
//    DirWordSize= 0xFF;//方向文字:文字大小;
//    BSWordSize= 0xFF;

    GDWordColor = 3;//股道文字颜色
    GDWordSize = 10;//股道文字大小
    GDDCWordColor_DW = 2;//道岔文字颜色
    GDDCWordColor_FW = 4;//道岔文字颜色
    GDDCWordColor_SK = 1;//道岔文字颜色
    GDDCWordSize = 10;//道岔文字大小
    XHDWordColor = 3;//信号灯文字颜色
    XHDWordSize = 10;//信号灯文字大小
    DCQDWordSize= 10;//道岔区段文字
    DirWordSize= 12;//方向文字:文字大小;
    BSWordSize= 12;
}
MyStation::~MyStation()
{
    isThreadStart = false;
    Sleep(500);
    //发信结束信号
    emit endWorkSignal();
}
//线程关联槽
void MyStation::startWorkSlot()
{
    int maxInt = 99999999;
    ticksOfThread = 0;
    while (isThreadStart)
    {
        ticksOfThread++;
        //防溢出处理
        if(ticksOfThread > maxInt)
        {
            ticksOfThread=0;
        }

        //线程按照休眠250ms计算的话，1000ms发送一次
        if(ticksOfThread%4==0)
        {
            //向教师机发送心跳信息
            //this->sendHeartBeatToTeacher();
        }

        //更新车次活跃信息
        this->UpdateStationCheCiInfo(ticksOfThread);
        //更新车次框的显示
        this->UpdateTrainNumWnd();

        QThread::msleep(400);//250
    }
}
//绘制站场
void MyStation::draw(QPainter *painter, long nElapsed, double factor, bool isMuti)
{
    if(isMuti)
    {
        this->drawMyStation(painter,nElapsed,this->nDiploidMuti * factor, this->nDiploidCheCiMuti * factor, isMuti);
    }
    else
    {
        this->drawMyStation(painter,nElapsed,this->nDiploid * factor, this->nDiploidCheCi * factor, isMuti);
    }
}
//站场放大缩小
void MyStation::SetDiploid(int n, double &nDiploid)
{
    if(n==1)//+
    {
        if(nDiploid < 2)
            nDiploid += 0.1;
    }
    else if(n==2)//-
    {
        if(nDiploid > 0.4)
            nDiploid -= 0.1;
    }
    else
    {
        nDiploid = 1;
    }
}
//根据道岔分路不良状态设置道岔区段名称显示颜色
void MyStation::SetDCQDTextColor()
{
    //根据道岔分路不良状态设置道岔区段名称颜色
    CGDDC *pCGDDC=nullptr;
    CGDDC *pCGDDC1=nullptr;
    CGDDC *pCGDDC2=nullptr;
    CText *text=nullptr;
    bool b1=false;
    bool b2=false;
    for(int i=0;i<DevArray.size();i++)
    {
        if(DevArray[i]->getDevType() == Dev_TEXT)
        {
            text=(CText*)DevArray[i];
            QString cc=text->getName();
            if(text->getName()=="516-518DG")
            {
                int a;
                a=0;
            }
            pCGDDC1=nullptr;
            pCGDDC2=nullptr;
            for(int j=0;j<DevArray.size();j++)
            {
                if(DevArray[j]->getDevType() == Dev_DC)
                {
                    pCGDDC=  (CGDDC*)DevArray[j];
                    if(pCGDDC->getDGName()==text->getName())
                    {
                        bool b=pCGDDC->getFLBLOrKX();
                        bool c=text->bDCQDWordColor;
                        if(pCGDDC1==nullptr)
                        {
                            pCGDDC1=pCGDDC;
                            b1=pCGDDC1->getFLBLOrKX();
                        }
                        else
                        {
                             pCGDDC2=pCGDDC;
                             b2=pCGDDC2->getFLBLOrKX();
                        }
                    }
                }
            }
            if(pCGDDC1!=nullptr)
            {
                if(pCGDDC2!=nullptr)
                {
                    text->bDCQDWordColor= pCGDDC1->getFLBLOrKX()|pCGDDC2->getFLBLOrKX();
                }
                else
                {
                    text->bDCQDWordColor=pCGDDC1->getFLBLOrKX();
                }
            }
        }
    }
}
//绘制CTC站场
void MyStation::drawMyStation(QPainter *painter, long nElapsed, double nDiploid, double nDiploidCheCi, bool bMulti)
{
    SetDCQDTextColor();
    //站场设备-非信号机
    int JmType = 0x55;
    for(int i=0;i<DevArray.size();i++)
    {
        if((DevArray[i]->getDevType() != Dev_XH)&&((DevArray[i]->getDevType() != Dev_DC)))
        //if(DevArray[i]->getDevType() != Dev_XH)
        {
            DevArray[i]->Draw(painter,nElapsed,nDiploid,offsetPt,JmType);
        }
    }
    //站场设备-道岔
    for(int i=0;i<DevArray.size();i++)
    {
        if(DevArray[i]->getDevType() == Dev_DC)
        {
            if(bMulti)
            {
                ((CGDDC*)DevArray[i])->isJGGZ = false;
                ((CGDDC*)DevArray[i])->isXGGZ = false;
            }
            if(((CGDDC*)DevArray[i])->isFS)
            {
                ((CGDDC*)DevArray[i])->bDCTextShow = true;
            }
            DevArray[i]->Draw(painter,nElapsed,nDiploid,offsetPt,JmType);
        }
    }

    //站场设备-信号机
    for(int i=0;i<DevArray.size();i++)
    {
        if(DevArray[i]->getDevType() == Dev_XH)
        {
            DevArray[i]->Draw(painter,nElapsed,nDiploid,offsetPt,JmType);
        }
    }
    //千分之六标志
    for(int i=0;i<vectSymbol6PerThousand.size();i++)
    {
        QPoint pt = offsetPt;
        if(isOverturn)
        {
            pt.setX(pCenter.x() * 2 - vectSymbol6PerThousand[i]->pCenterConst.x() + offsetPt.x());
            pt.setY(pCenter.y() * 2 - vectSymbol6PerThousand[i]->pCenterConst.y() + offsetPt.y() - 32);
        }
        vectSymbol6PerThousand[i]->Draw(painter,nElapsed,nDiploid,pt, isOverturn);
    }
    //指示灯
    for(int i=0;i<LampArray.size();i++)
    {
        if(LampArray[i]->getDevType() == Dev_LAMP)
        {
            LampArray[i]->Draw(painter,nElapsed,nDiploid,offsetPt,JmType);
        }
    }
    //站间透明不显示下面内容
    if(!bMulti)
    {
        //自动闭塞
        for(int i=0;i<ZDBSArray.size();i++)
        {
            if(ZDBSArray[i]->getDevType() == Dev_ZDBS)
            {
                ZDBSArray[i]->Draw(painter,nElapsed,nDiploid,offsetPt,JmType);
            }
        }

        //计轴
        for(int i=0;i<JZArray.size();i++)
        {
            if(JZArray[i]->getDevType() == Dev_JZ)
            {
                JZArray[i]->Draw(painter,nElapsed,nDiploid,offsetPt,JmType);
            }
        }
        //场联
        for(int i=0;i<CLArray.size();i++)
        {
            if(CLArray[i]->getDevType() == Dev_CL)
            {
                CLArray[i]->Draw(painter,nElapsed,nDiploid,offsetPt,JmType);
            }
        }
        //功能按钮
        for(int i=0;i<FuncBtnArray.size();i++)
        {
            if(FuncBtnArray[i]->getDevType() == Dev_FUNCB)
            {
                FuncBtnArray[i]->Draw(painter,nElapsed,nDiploid,offsetPt,JmType);
            }
        }
        //道岔故障按钮
        for(int i=0;i<DCBtnArray.size();i++)
        {
            if(DCBtnArray[i]->getDevType() == Dev_DCBTN)
            {
                DCBtnArray[i]->Draw(painter,nElapsed,nDiploid,offsetPt,JmType);
            }
        }
    }
    //半自动闭塞
    for(int i=0;i<BZDBSArray.size();i++)
    {
        if(BZDBSArray[i]->getDevType() == Dev_BZDBS)
        {
            BZDBSArray[i]->Draw(painter,nElapsed,nDiploid,offsetPt,JmType);
        }
    }
    //站台
    for(int i=0;i<StationPlatformOtherArray.size();i++)
    {
        if(StationPlatformOtherArray[i]->getDevType() == Dev_ZT)
        {
            StationPlatformOtherArray[i]->Draw(painter,nElapsed,nDiploid,offsetPt,JmType);
        }
    }
    //进路预告窗
    for(int i=0;i<vectRoutePreWnd.size();i++)
    {
        vectRoutePreWnd[i]->Draw(painter,nElapsed,nDiploid,offsetPt);
    }
    //文字标注
    for(int i=0;i<vectTextAnnotation.size();i++)
    {
        vectTextAnnotation[i]->Draw(painter, nElapsed, nDiploid, offsetPt, isOverturn);
    }
    //绘制鼠标移动提示信息
    for(int i=0;i<DevArray.size();i++)
    {
        DevArray[i]->Draw_ToolTip(painter,nDiploid);
    }
}

//绘制CTC站场安全状态
void MyStation::drawMyStationSafe()
{
    //站场设备-非信号机
    for(int i=0;i<DevArray.size();i++)
    {
        //if((DevArray[i]->getDevType() != Dev_XH)&&((DevArray[i]->getDevType() != Dev_DC)))
        if(DevArray[i]->getDevType() != Dev_XH)
        {
            DevArray[i]->setDevStateToSafe();
        }
    }
    //站场设备-信号机
    for(int i=0;i<DevArray.size();i++)
    {
        if(DevArray[i]->getDevType() == Dev_XH)
        {
            DevArray[i]->setDevStateToSafe();
        }
    }
}
//读取站场设备数据
bool MyStation::readStationDev(QString fileName, MyStation *pMyStation)
{
    QFileInfo fileInfo(fileName);
    if (!fileInfo.isFile())
    {
        QLOG_ERROR()<<QString::fromLocal8Bit("File %1 not existed!").arg(fileName);
        return false;
    }

    QFile file(fileName);
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //QString msg = "打开文件失败+"+fileName;
        //QMessageBox::warning(NULL,VERSION,msg);
        //QLOG_ERROR()<<msg;
        QLOG_ERROR()<<QString("Open file failed! %1").arg(fileName);
        return false;
    }
    while(!file.atEnd())
    {
        QByteArray line = file.readLine();
        QString str(line);
        QString getString;
        if(str.left(4) == "####")
        {
            break;
        }
        else if(str.left(2) == "//")
        {
            continue;
        }
        else if(str.left(4) == "##XH")
        {
            CXHD *xhd = new CXHD();
            while(!file.atEnd())
            {
                QByteArray line1 = file.readLine();
                QString m_str(line1);
                if (m_str.left(12)=="xhd.m_nType=")
                {
                    xhd->setType((m_str.mid(12,m_str.indexOf(";")-m_str.indexOf("=")-1)).toInt());
                }
                else if (m_str.left(14)=="xhd.m_strName=")
                {
                    xhd->setName(m_str.mid(15,m_str.indexOf(";")-m_str.indexOf("=")-3));
                }
                else if (m_str.left(12)=="xhd.m_nCode=")
                {
                    getString = m_str.mid(12,m_str.indexOf(";")-m_str.indexOf("=")-1);
                    int nCodee = StringToHex(getString);//"0x10"
                    xhd->setCode(nCodee);
                }
                else if (m_str.left(16)=="xhd.Module_Code=")
                {
                    getString = m_str.mid(16,m_str.indexOf(";")-m_str.indexOf("=")-1);
                    int nCodee = StringToHex(getString);//"0x10"
                }
                else if (m_str.left(11)=="xhd.center=")
                {
                    xhd->setCenterPt(StringToPoint(m_str.mid(11,m_str.indexOf(";")-m_str.indexOf("=")-1)));
                }
                else if (m_str.left(15)=="xhd.m_nXHDType=")
                {
                    xhd->setXHDType(m_str.mid(15,m_str.indexOf(";")-m_str.indexOf("=")-1));
                }
                else if (m_str.left(15)=="xhd.m_textRect=")
                {
                    xhd->setTextRect(StringToRect(m_str.mid(15,m_str.indexOf(";")-m_str.indexOf("=")-1)));
                }
                else if (m_str.left(10)=="xhd.m_nSX=")
                {
                    xhd->setSX((m_str.mid(10,m_str.indexOf(";")-m_str.indexOf("=")-1)).toInt());
                }
                else if (m_str.left(15)=="xhd.SignalType=")
                {
                    xhd->setSignalType((m_str.mid(15,m_str.indexOf(";")-m_str.indexOf("=")-1)).toInt());
                }
                else if (m_str.left(17)=="xhd.D_B_C_Signal=")
                {
                }
                else if (m_str.left(17)=="xhd.DC_LC_Signal=")
                {
                }
                else if (m_str.left(13)=="xhd.safeLamp=")
                {
                    xhd->setSafeLamp(m_str.mid(13,m_str.indexOf(";")-m_str.indexOf("=")-1));
                }
                else if (m_str.left(11)=="xhd.isHigh=")
                {
                    //xhd->setIsHigh((m_str.mid(11,m_str.indexOf(";")-m_str.indexOf("=")-1)).toInt());
                    QString strtype=m_str.mid(11,m_str.indexOf(";")-m_str.indexOf("=")-1);
                    bool bhigh=(strtype=="TRUE") ? true : false;
                    xhd->setIsHigh(bhigh);
                }
                else if (m_str.left(11)=="xhd.isYDSD=")
                {
                    xhd->setIsYDSD((m_str.mid(11,m_str.indexOf(";")-m_str.indexOf("=")-1)).toInt());
                }
                else if (m_str.left(4)=="ADD_")
                {
                    break;
                }
            }
           // xhd->XHDInit(0x55);
            pMyStation->DevArray.append(xhd);
           // delete xhd;
        }
        else if(str.left(4)=="##DC")
        {
            CGDDC *gddc=new CGDDC();
            while(!file.atEnd())
            {
//                QByteArray line1 = file.readLine();
//                QString m_str(line1);
                QByteArray line1 = file.readLine();
                QString m_str = codec->toUnicode(line1);
                if(m_str.contains("H1"))
                {
                    int a;
                    a=0;
                }
                if (m_str.left(13)=="gddc.m_nType=")
                {
                    gddc->setType((m_str.mid(13,m_str.indexOf(";")-m_str.indexOf("=")-1)).toInt());
                }
                else if (m_str.left(15)=="gddc.m_strName=")
                {
                    gddc->setName(m_str.mid(16,m_str.indexOf(";")-m_str.indexOf("=")-3));
                }
                else if (m_str.left(13)=="gddc.m_nCode=")
                {
                    getString = m_str.mid(13,m_str.indexOf(";")-m_str.indexOf("=")-1);
                    int nCodee = StringToHex(getString);//例"0x10"
                    gddc->setCode(nCodee);
                }
                else if (m_str.left(15)=="gddc.m_nQDCode=")
                {
                    getString = m_str.mid(15,m_str.indexOf(";")-m_str.indexOf("=")-1);
                    int nCodee = StringToHex(getString);//例"0x10"
                    gddc->setQDCode(nCodee);
                }
                else if (m_str.left(13)=="gddc.m_nCxjy=")
                {
                    gddc->setCxjy((m_str.mid(13,m_str.indexOf(";")-m_str.indexOf("=")-1)).toInt());
                }
                else if (m_str.left(15)=="gddc.m_nDSCode=")
                {
                }
                else if (m_str.left(17)=="gddc.Module_Code=")
                {
                }
                else if (m_str.left(17)=="gddc.m_nQDMKCode=")
                {
                    int a;
                    a=0;
                }
                else if (m_str.left(10)=="gddc.m_nZ=")
                {
                    gddc->setZ((m_str.mid(10,m_str.indexOf(";")-m_str.indexOf("=")-1)).toInt());
                }
                else if (m_str.left(11)=="gddc.m_nSX=")
                {
                    gddc->setSX((m_str.mid(11,m_str.indexOf(";")-m_str.indexOf("=")-1)).toInt());
                }
                else if (m_str.left(12)=="gddc.m_nJyj=")
                {
                    gddc->setJyj((m_str.mid(12,m_str.indexOf(";")-m_str.indexOf("=")-1)).toInt());
                }
                else if (m_str.left(17)=="gddc.oneToMore=")
                {
                }
                else if (m_str.left(16)=="gddc.m_textRect=")
                {
                    gddc->setTextRect(StringToRect(m_str.mid(16,m_str.indexOf(";")-m_str.indexOf("=")-1)));
                }
                else if (m_str.left(8)=="gddc.p1=")
                {
                    gddc->setp1(StringToPoint(m_str.mid(8,m_str.indexOf(";")-m_str.indexOf("=")-1)));
                }
                else if (m_str.left(8)=="gddc.p2=")
                {
                    gddc->setp2(StringToPoint(m_str.mid(8,m_str.indexOf(";")-m_str.indexOf("=")-1)));
                }
                else if (m_str.left(8)=="gddc.p3=")
                {
                    gddc->setp3(StringToPoint(m_str.mid(8,m_str.indexOf(";")-m_str.indexOf("=")-1)));
                }
                else if (m_str.left(8)=="gddc.p4=")
                {
                    gddc->setp4(StringToPoint(m_str.mid(8,m_str.indexOf(";")-m_str.indexOf("=")-1)));
                }
                else if (m_str.left(8)=="gddc.p5=")
                {
                    gddc->setp5(StringToPoint(m_str.mid(8,m_str.indexOf(";")-m_str.indexOf("=")-1)));
                }
                else if (m_str.left(8)=="gddc.p6=")
                {
                    gddc->setp6(StringToPoint(m_str.mid(8,m_str.indexOf(";")-m_str.indexOf("=")-1)));
                }
                else if (m_str.left(9)=="gddc.p12=")
                {
                    gddc->setp12(StringToPoint(m_str.mid(9,m_str.indexOf(";")-m_str.indexOf("=")-1)));
                }
                else if (m_str.left(9)=="gddc.p34=")
                {
                    gddc->setp34(StringToPoint(m_str.mid(9,m_str.indexOf(";")-m_str.indexOf("=")-1)));
                }
                else if (m_str.left(9)=="gddc.p56=")
                {
                    gddc->setp56(StringToPoint(m_str.mid(9,m_str.indexOf(";")-m_str.indexOf("=")-1)));
                }
                else if (m_str.left(12)=="gddc.center=")
                {
                    gddc->setCenterPt(StringToPoint(m_str.mid(12,m_str.indexOf(";")-m_str.indexOf("=")-1)));
                }
                else if (m_str.left(10)=="gddc.pZ12=")
                {
                    gddc->setpz12(StringToPoint(m_str.mid(10,m_str.indexOf(";")-m_str.indexOf("=")-1)));
                }
                else if (m_str.left(10)=="gddc.pZ34=")
                {
                    gddc->setpz34(StringToPoint(m_str.mid(10,m_str.indexOf(";")-m_str.indexOf("=")-1)));
                }
                else if (m_str.left(10)=="gddc.pZ56=")
                {
                    gddc->setpz56(StringToPoint(m_str.mid(10,m_str.indexOf(";")-m_str.indexOf("=")-1)));
                }
                else if (m_str.left(15)=="gddc.m_bMainGD=")
                {
                    gddc->setIsMainGD((m_str.mid(15,m_str.indexOf(";")-m_str.indexOf("=")-1)).toInt());
                }
                else if (m_str.left(10)=="gddc.m_nQ=")
                {
                    gddc->setCQdev((m_str.mid(10,m_str.indexOf(";")-m_str.indexOf("=")-1)).toInt());
                }
                else if (m_str.left(10)=="gddc.m_nD=")
                {
                    gddc->setDWdev((m_str.mid(10,m_str.indexOf(";")-m_str.indexOf("=")-1)).toInt());
                }
                else if (m_str.left(10)=="gddc.m_nF=")
                {
                    gddc->setFWdev((m_str.mid(10,m_str.indexOf(";")-m_str.indexOf("=")-1)).toInt());
                }
                else if (m_str.left(4)=="ADD_")
                {
                    break;
                }
            }
            gddc->GDDCInit(0x55);
            pMyStation->DevArray.append(gddc);
           // delete  gddc;
        }
        else if(str.left(4)=="##GD")
        {
            CGD *gd=new CGD();
            while(!file.atEnd())
            {
                QByteArray line1 = file.readLine();
                QString m_str(line1);
                if (m_str.left(11)=="gd.m_nType=")
                {
                    gd->setType((m_str.mid(11,m_str.indexOf(";")-m_str.indexOf("=")-1)).toInt());
                }
                else if (m_str.left(13)=="gd.m_strName=")
                {
                    gd->setName(m_str.mid(14,m_str.indexOf(";")-m_str.indexOf("=")-3));
                }
                else if (m_str.left(11)=="gd.m_nCode=")
                {
                    getString = m_str.mid(11,m_str.indexOf(";")-m_str.indexOf("=")-1);
                    int nCodee = StringToHex(getString);//例"0x10"
                    gd->setCode(nCodee);
                }
                else if (m_str.left(15)=="gd.Module_Code=")
                {
                }
                else if (m_str.left(11)=="gd.GD_Type=")
                {
                    gd->setGDType(m_str.mid(11,m_str.indexOf(";")-m_str.indexOf("=")-1));
                }
                else if (m_str.left(8)=="gd.m_nZ=")
                {
                    gd->setZ((m_str.mid(8,m_str.indexOf(";")-m_str.indexOf("=")-1)).toInt());
                }
                else if (m_str.left(9)=="gd.m_nSX=")
                {
                    gd->setSX((m_str.mid(9,m_str.indexOf(";")-m_str.indexOf("=")-1)).toInt());
                }
                else if (m_str.left(14)=="gd.m_QJXHFlag=")
                {
                    getString = m_str.mid(14,m_str.indexOf(";")-m_str.indexOf("=")-1);
                    gd->isZDQJXHDDirFlag=true;
                    gd->isZDQJXHDRight = ((getString == "0")?(false):(true));
                }
                else if (m_str.left(14)=="gd.m_textRect=")
                {
                    gd->setTextRect(StringToRect(m_str.mid(14,m_str.indexOf(";")-m_str.indexOf("=")-1)));
                }
                else if (m_str.left(6)=="gd.p1=")
                {
                    gd->setp1(StringToPoint(m_str.mid(6,m_str.indexOf(";")-m_str.indexOf("=")-1)));
                }
                else if (m_str.left(6)=="gd.p2=")
                {
                    gd->setp2(StringToPoint(m_str.mid(6,m_str.indexOf(";")-m_str.indexOf("=")-1)));
                }
                else if (m_str.left(6)=="gd.p3=")
                {
                    gd->setp3(StringToPoint(m_str.mid(6,m_str.indexOf(";")-m_str.indexOf("=")-1)));
                }
                else if (m_str.left(6)=="gd.p4=")
                {
                    gd->setp4(StringToPoint(m_str.mid(6,m_str.indexOf(";")-m_str.indexOf("=")-1)));
                }
                else if (m_str.left(7)=="gd.p12=")
                {
                    gd->setp12(StringToPoint(m_str.mid(7,m_str.indexOf(";")-m_str.indexOf("=")-1)));
                }
                else if (m_str.left(7)=="gd.p34=")
                {
                    gd->setp34(StringToPoint(m_str.mid(7,m_str.indexOf(";")-m_str.indexOf("=")-1)));
                }
                else if (m_str.left(8)=="gd.pz12=")
                {
                    gd->setpz12(StringToPoint(m_str.mid(8,m_str.indexOf(";")-m_str.indexOf("=")-1)));
                }
                else if (m_str.left(8)=="gd.pz34=")
                {
                    gd->setpz34(StringToPoint(m_str.mid(8,m_str.indexOf(";")-m_str.indexOf("=")-1)));
                }
                else if (m_str.left(10)=="gd.center=")
                {
                    gd->setCenterPt(StringToPoint(m_str.mid(10,m_str.indexOf(";")-m_str.indexOf("=")-1)));
                }
                else if (m_str.left(12)=="gd.QJXHName=")
                {
                }
                else if (m_str.left(10)=="gd.QJXHPt=")
                {
                }
                else if (m_str.left(4)=="ADD_")
                {
                    break;
                }
            }
            gd->GDInit(0x55);
            pMyStation->DevArray.append(gd);
            //delete gd;
        }
        else if(str.left(8)=="##BuRect")
        {
            CTG *tg=new CTG();
            while(!file.atEnd())
            {
                QByteArray line1 = file.readLine();
                QString m_str(line1);
                if (m_str.left(15)=="BuRect.m_nType=")
                {
                    tg->setType((m_str.mid(15,m_str.indexOf(";")-m_str.indexOf("=")-1)).toInt());
                }
                else if (m_str.left(17)=="BuRect.m_strName=")
                {
                    tg->setName(m_str.mid(18,m_str.indexOf(";")-m_str.indexOf("=")-3));
                }
                else if (m_str.left(15)=="BuRect.m_nCode=")
                {
                    getString = m_str.mid(15,m_str.indexOf(";")-m_str.indexOf("=")-1);
                    int nCodee = StringToHex(getString);//例"0x10"
                    tg->setCode(nCodee);
                }
                else if (m_str.left(10)=="BuRect.p1=")
                {
                    tg->setCenterPt(StringToPoint(m_str.mid(10,m_str.indexOf(";")-m_str.indexOf("=")-1)));
                }
                else if (m_str.left(18)=="BuRect.m_textRect=")
                {
                    tg->setTextRect(StringToRect(m_str.mid(18,m_str.indexOf(";")-m_str.indexOf("=")-1)));
                }
                else if (m_str.left(14)=="BuRect.m_nTZB=")
                {
                }
                else if (m_str.left(4)=="ADD_")
                {
                    break;
                }
            }
            pMyStation->DevArray.append(tg);
            //delete tg;
        }
        else if(str.left(4)=="##TX")
        {
            CText *text=new CText();
            while(!file.atEnd())
            {
                QByteArray line1 = file.readLine();
                //QString m_str(line1);
                QString m_str = codec->toUnicode(line1);
                if (m_str.left(14)=="txt.m_strName=")
                {
                    text->setName(m_str.mid(15,m_str.indexOf(";")-m_str.indexOf("=")-3));
                }
                else if (m_str.left(13)=="txt.m_DCname=")
                {
                    int a=m_str.length();
                    if(m_str.length()>17)//非道岔区段文字
                    {
                        text->setTextType("DCQDNAMETEXT");
                    }
                    text->setGLDCNode(pMyStation->getDCNodeFromDCName(m_str.mid(14,m_str.indexOf(";")-m_str.indexOf(";")-3)));
                }
                else if (m_str.left(12)=="txt.m_nSize=")
                {
                    text->setTextSize((m_str.mid(12,m_str.indexOf(";")-m_str.indexOf("=")-1)).toInt());
                }
                else if (m_str.left(15)=="txt.m_textRect=")
                {
                    text->setTextRect(StringToRect(m_str.mid(15,m_str.indexOf(";")-m_str.indexOf("=")-1)));
                }
                else if (m_str.left(10)=="txt.color=")
                {
                    text->setTextColor(m_str.mid(10,m_str.indexOf(";")-m_str.indexOf("=")-1));
                }
                else if (m_str.left(4)=="ADD_")
                {
                    if(text->getTextSize() >= 30)
                    {
                        pMyStation->setStationName(text->getName());
                        text->setTextType("STATIONNAMETEXT");
                    }
                    break;
                }
            }
            text->TextInit();
            pMyStation->DevArray.append(text);
          //  delete text;
        }
        else if(str.left(5)=="##JTX")
        {
            CJTX *jtx=new CJTX();
            while(!file.atEnd())
            {
                QByteArray line1 = file.readLine();
                QString m_str(line1);
                if (m_str.left(13)=="jtx.JTX_Type=")
                {
                    jtx->setJTXType((m_str.mid(13,m_str.indexOf(";")-m_str.indexOf("=")-1)).toInt());
                }
                else if (m_str.left(15)=="jtx.m_strName=")
                {
                    jtx->setName(m_str.mid(16,m_str.indexOf(";")-m_str.indexOf("=")-3));
                }
                else if (m_str.left(12)=="jtx.m_nType=")
                {
                    jtx->setType((m_str.mid(12,m_str.indexOf(";")-m_str.indexOf("=")-1)).toInt());
                }
                else if (m_str.left(7)=="jtx.p1=")
                {
                    jtx->setp1(StringToPoint(m_str.mid(7,m_str.indexOf(";")-m_str.indexOf("=")-1)));
                }
                else if (m_str.left(7)=="jtx.p2=")
                {
                    jtx->setp2(StringToPoint(m_str.mid(7,m_str.indexOf(";")-m_str.indexOf("=")-1)));
                }
                else if (m_str.left(7)=="jtx.p3=")
                {
                    jtx->setp3(StringToPoint(m_str.mid(7,m_str.indexOf(";")-m_str.indexOf("=")-1)));
                }
                else if (m_str.left(7)=="jtx.p4=")
                {
                    jtx->setp4(StringToPoint(m_str.mid(7,m_str.indexOf(";")-m_str.indexOf("=")-1)));
                }
                else if (m_str.left(11)=="jtx.center=")
                {
                    jtx->setCenterPt(StringToPoint(m_str.mid(11,m_str.indexOf(";")-m_str.indexOf("=")-1)));
                }
                else if (m_str.left(4)=="ADD_")
                {
                    break;
                }
            }
            jtx->JTXInit();
            pMyStation->DevArray.append(jtx);
            // delete jtx;
        }
        else if(str.left(4)=="##QD")
        {
            CQD *qd=new CQD();
            QByteArray line1 = file.readLine();
            QString m_str(line1);
            if (m_str.left(15)=="qd.m_nChildNum=")
            {
                qd->setChildNum((m_str.mid(15,m_str.indexOf(";")-m_str.indexOf("=")-1)).toInt());
            }
            for (int j=0;j<qd->getChildNum();j++)
            {
                QByteArray line2 = file.readLine();
                QString child_str(line2);
                QString subStr=QString("qd.m_nChild[%1]=").arg(j);
                if (child_str.left(15)==subStr)
                {
                    qd->setChild(j,(child_str.mid(15,child_str.indexOf(";")-child_str.indexOf("=")-1)).toInt());
                }
            }
            pMyStation->DevArray.append(qd);
            //delete qd;
        }
    }
    //关闭文件
    file.close();
    getDGNameFromText();
    return true;
}
//读取站场配置数据
bool MyStation::readStationConfig(QString fileName, MyStation *pMyStation)
{
    int code=0;
    QString m_str;
    QString  getString;
    QFileInfo fileInfo(fileName);
    if (!fileInfo.isFile())
    {
        QLOG_ERROR()<<QString::fromLocal8Bit("File %1 not existed!").arg(fileName);
        return false;
    }
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    m_str = file.readLine();
    while(m_str.left(4)!="####")
    {
        m_str = file.readLine();
        //忽略注释和空格
        if(m_str.left(2) == "//" || m_str.trimmed() == "")
        {
            continue;
        }

        if(m_str.left(11) == "STATIONNAME:")
        {
            getString = m_str.mid(11,m_str.indexOf(";")-m_str.indexOf(":")-1);
            StaConfigInfo.strStaName = getString;
        }
        else if(m_str.left(10) == "STATIONID:")
        {
            getString = m_str.mid(10,m_str.indexOf(";")-m_str.indexOf(":")-1);
            StaConfigInfo.StationID = getString.toInt();
        }
        //站场界面大小
        else if(m_str.left(12) == "STATIONSIZE:")
        {
            getString = m_str.mid(12,m_str.indexOf(";")-m_str.indexOf(":")-1);
            QStringList strlist=getString.split("|");
            if(strlist.size()==2)
            {
                int nwidth=strlist[0].toUInt();
                int nheight=strlist[1].toUInt();
                StaConfigInfo.initialSize.setWidth(nwidth);
                StaConfigInfo.initialSize.setHeight(nheight);
            }
        }
        else if(m_str.left(6) == "ABNUM:")
        {
            getString = m_str.mid(6,m_str.indexOf(";")-m_str.indexOf(":")-1);
            ABNUM = getString.toInt();
        }
        else if(m_str.left(8) == "ZXBPATH:")
        {
            strZXBPath = m_str.mid(8,m_str.indexOf(";")-m_str.indexOf(":")-1);
        }
        else if(m_str.left(9) == "SHOWVIEW:")
        {
            getString = m_str.mid(9,m_str.indexOf(";")-m_str.indexOf(":")-1);
            nShowView = getString.toInt();
        }
        else if(m_str.left(8) == "XHDDSBJ:")
        {
            getString = m_str.mid(8,m_str.indexOf(";")-m_str.indexOf(":")-1);
            StaConfigInfo.bXHDDSBJStatus = getString.toInt();
        }
        else if (m_str.left(15) == "CHGMODENEEDJSJ:")
        {
            //读取配置-控制模式转换
            getString = m_str.mid(15, m_str.indexOf(";") - m_str.indexOf(":") - 1);
            StaConfigInfo.bChgModeNeedJSJ = getString.toInt() == 1 ? true : false;
        }
//        else if (m_str.left(9) == "CLPZFLAG:")
//        {
//            //场联配置
//            getString = m_str.mid(9, m_str.indexOf(";") - m_str.indexOf(":") - 1);
//            StaConfigInfo.HaveCLPZFlag = getString.toInt() == 1 ? true : false;
//        }
        else if(m_str.left(11) == "ZHANTAINUM:")//站台
        {
            getString = m_str.mid(11,m_str.indexOf(";")-m_str.indexOf(":")-1);
            ZHANTAICount = getString.toInt();

            for (int j = 0; j < ZHANTAICount; j++)
            {
                QString str1, str2, str3, str4;
                str1=QString("ZHANTAI_TYPE%1:").arg(j+1);
                str2=QString("ZHANTAI_LENGTH%1:").arg(j+1);
                str3=QString("ZHANTAI_POINT%1:").arg(j+1);
                str4=QString("ZHANTAI_HIGH%1:").arg(j+1);

                m_str = file.readLine();
                if(m_str.left(str4.length()) == str4)//14
                {
                    getString = m_str.mid(str4.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    ZHANTAIInfo[j].High = getString.toInt();
                }

                m_str = file.readLine();
                if(m_str.left(str1.length()) == str1)//14
                {
                    getString = m_str.mid(str1.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    ZHANTAIInfo[j].Type = getString;
                }

                m_str = file.readLine();
                if(m_str.left(str2.length()) == str2)//16
                {
                    getString = m_str.mid(str2.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    ZHANTAIInfo[j].Length = getString.toInt();
                }

                m_str = file.readLine();
                if(m_str.left(str3.length()) == str3)//15
                {
                    getString = m_str.mid(str3.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    ZHANTAIInfo[j].ptZT = StringToPoint(getString);
                }
            }
        }
        else if(m_str.left(10) == "XINHAOLOU:")//信号楼坐标，是否朝北
        {
            getString = m_str.mid(10,m_str.indexOf(";")-m_str.indexOf(":")-1);
            QString ch = "|";
            QStringList strArr;
            int c = StringSplit(getString, ch, strArr);
            if (3 <= c)
            {
                bShowXinHaoLou = strArr[0].toInt();
                XinHaoLouPt = StringToPoint(strArr[1]);
                XinHaoLouFaceNorth = strArr[2].toInt();
            }
        }
        else if (m_str.left(5) == "TEXT:")//文本，颜色|左上角|文本
        {
            getString = m_str.mid(5,m_str.indexOf(";")-m_str.indexOf(":")-1);
            QString ch = "|";
            QStringList strArr;
            int c = StringSplit(getString, ch, strArr);
            if (3 == c)
            {
                TEXT_T txt;
                txt.nColorType = strArr[0].toInt();
                txt.ptLT = StringToPoint(strArr[1]);
                QString name = strArr[2];
                txt.strText = name;
                vectText.push_back(txt);
            }
        }
        else if (m_str.left(5) == "LINE:")//线条，颜色|点1|点2|线宽
        {
            getString = m_str.mid(5,m_str.indexOf(";")-m_str.indexOf(":")-1);
            QString ch = "|";
            QStringList strArr;
            int c = StringSplit(getString, ch, strArr);
            if (3 == c)
            {
                LINES_T line;
                line.nColorType = strArr[0].toInt();
                line.pt1 = StringToPoint(strArr[1]);
                line.pt2 = StringToPoint(strArr[2]);
                line.LineWidth = 0;
                line.LineState = 1;
                vectLine.push_back(line);
            }
            else if(4 == c)
            {
                LINES_T line;
                line.nColorType = strArr[0].toInt();
                line.pt1 = StringToPoint(strArr[1]);
                line.pt2 = StringToPoint(strArr[2]);
                line.LineWidth = strArr[3].toInt();
                line.LineState = 1;
                vectLine.push_back(line);
            }
            else if(5 == c)
            {
                LINES_T line;
                line.nColorType = strArr[0].toInt();
                line.pt1 = StringToPoint(strArr[1]);
                line.pt2 = StringToPoint(strArr[2]);
                line.LineWidth = strArr[3].toInt();
                line.LineState = strArr[4].toInt();;
                vectLine.push_back(line);
            }
        }
        else if (m_str.left(15) == "RectanglePoint:")//矩形，左上角|右下角
        {
            getString = m_str.mid(15,m_str.indexOf(";")-m_str.indexOf(":")-1);
            QString ch = "|";
            QStringList strArr;
            int c = StringSplit(getString, ch, strArr);
            if (3 == c)
            {
                RECT_T rect;
                rect.nColorType = strArr[0].toInt();
                rect.pt1 = StringToPoint(strArr[1]);
                rect.pt2 = StringToPoint(strArr[2]);
                rect.LineWidth = 0;
                rect.LineState = 1;
                vectRect.push_back(rect);
            }
            else if(4 == c)
            {
                RECT_T rect;
                rect.nColorType = strArr[0].toInt();
                rect.pt1 = StringToPoint(strArr[1]);
                rect.pt2 = StringToPoint(strArr[2]);
                rect.LineWidth = strArr[3].toInt();
                rect.LineState = 1;
                vectRect.push_back(rect);
            }
            else if(5 == c)
            {
                RECT_T rect;
                rect.nColorType = strArr[0].toInt();
                rect.pt1 = StringToPoint(strArr[1]);
                rect.pt2 = StringToPoint(strArr[2]);
                rect.LineWidth = strArr[3].toInt();
                rect.LineState = strArr[4].toInt();
                vectRect.push_back(rect);
            }
        }
        else if(m_str.left(17) == "DCBTNDISPLAYNAME:")
        {
            getString = m_str.mid(17,m_str.indexOf(";")-m_str.indexOf(":")-1);
            QStringList str;
            QString ch = "|";
            int c = StringSplit(getString, ch, str);
            if(2 == c)
            {
                StaConfigInfo.DCGZBtnNameJG = str[0];
                StaConfigInfo.DCGZBtnNameXG = str[1];
            }
        }
        else if(m_str.left(19) == "LEFTDCBTNBASEPOINT:")
        {
            getString = m_str.mid(19,m_str.indexOf(";")-m_str.indexOf(":")-1);
            QStringList str;
            QString ch = "|";
            int c = StringSplit(getString, ch, str);
            if(4 == c)
            {
                StaConfigInfo.LeftDCGZBtnPt = StringToPoint(str[0]);
                StaConfigInfo.LeftLineFeed = str[1].toInt();
                StaConfigInfo.LeftDCGZStyle = str[2].toInt();
                StaConfigInfo.LeftDCGZnewlineBRight = str[3].toInt();
            }
        }
        else if(m_str.left(11) == "LEFTDCLIST:")
        {
            getString = m_str.mid(11,m_str.indexOf(";")-m_str.indexOf(":")-1);
            QString ch = ",";
            StringSplit(getString, ch, StaConfigInfo.LeftDCArray);
        }
        else if(m_str.left(20) == "RIGHTDCBTNBASEPOINT:")
        {
            getString = m_str.mid(20,m_str.indexOf(";")-m_str.indexOf(":")-1);
            QStringList str;
            QString ch = "|";
            int c = StringSplit(getString, ch, str);
            if(4 == c)
            {
                StaConfigInfo.RightDCGZBtnPt = StringToPoint(str[0]);
                StaConfigInfo.RightLineFeed = str[1].toInt();
                StaConfigInfo.RightDCGZStyle = str[2].toInt();
                StaConfigInfo.RightDCGZnewlineBRight = str[3].toInt();
            }
        }
        else if(m_str.left(12) == "RIGHTDCLIST:")
        {
            getString = m_str.mid(12,m_str.indexOf(";")-m_str.indexOf(":")-1);
            QString ch = ",";
            int c = StringSplit(getString, ch, StaConfigInfo.RightDCArray);
        }
        else if (m_str.left(7) == "QFZ6PD:")//6‰坡道定义//坐标|方向0左1右
        {
            getString = m_str.mid(7,m_str.indexOf(";")-m_str.indexOf(":")-1);
            QStringList strArr;
            int c = StringSplit(getString, "|", strArr);
            if (2 == c)
            {
                Symbol6PerThousand* symb6 = new Symbol6PerThousand;
                symb6->pCenterConst = StringToPoint(strArr[0]);
                symb6->bRight = strArr[1].toInt()==1?true:false;
                vectSymbol6PerThousand.append(symb6);
            }
        }
        else if (m_str.left(7) == "GDWord:")
        {
            getString = m_str.mid(7,m_str.indexOf(";")-m_str.indexOf(":")-1);
            GDWordSize=getString.toInt();
        }
        else if (m_str.left(9) == "GDDCWord:")
        {
            getString = m_str.mid(9,m_str.indexOf(";")-m_str.indexOf(":")-1);
            GDDCWordSize=getString.toInt();
        }
        else if (m_str.left(8) == "XHDWord:")
        {
            getString = m_str.mid(8,m_str.indexOf(";")-m_str.indexOf(":")-1);
            XHDWordSize=getString.toInt();
        }
        else if (m_str.left(9) == "DCQDWord:")//道岔区段文字:文字大小
        {
            getString = m_str.mid(9,m_str.indexOf(";")-m_str.indexOf(":")-1);
            DCQDWordSize=getString.toInt();
        }
        else if (m_str.left(8) == "DirWord:")//方向文字:文字大小;
        {
            getString = m_str.mid(8,m_str.indexOf(";")-m_str.indexOf(":")-1);
            DirWordSize=getString.toInt();
        }
        else if (m_str.left(7) == "BSWord:")//自动闭塞半自动闭塞功能按钮文字:文字大小;
        {
            getString = m_str.mid(7,m_str.indexOf(";")-m_str.indexOf(":")-1);
            BSWordSize=getString.toInt();
        }
        else if(m_str.left(7) == "CLWord:") //字体大小
        {
            CLWordSize = (m_str.mid(7,m_str.indexOf(";")-m_str.indexOf(":")-1)).toInt();
        }
        else if (m_str.left(19) == "ViewBtnAndLmapWord:")//界面按钮名称及标识灯文字大小；//文字大小|按钮（指示灯大小）
        {
            getString = m_str.mid(19,m_str.indexOf(";")-m_str.indexOf(":")-1);
//            QString ch = "|";
//            QStringList strArr;
//            int c = StringSplit(getString, ch, strArr);
//            if (2 == c)
//            {
//                ViewBtnAndLmapWordSize = strArr[0].toInt();
//                ViewBtnAndLmapSize = strArr[1].toInt();
//                if(ViewBtnAndLmapWordSize>12)
//                {
//                    ViewBtnAndLmapWordSize=12;
//                }
//                if(ViewBtnAndLmapSize>16)
//                {
//                    ViewBtnAndLmapSize=16;
//                }
//            }
            if(getString.contains("|"))
            {
               ViewBtnAndLmapWordSize=12;
            }
            else
            {
                ViewBtnAndLmapWordSize=getString.toInt();
                if(ViewBtnAndLmapWordSize>12)
                {
                    ViewBtnAndLmapWordSize=12;
                }
            }

        }
        //用户信息
        else if (m_str.left(5) == "USER:")
        {
            getString = m_str.mid(5,m_str.indexOf(";")-m_str.indexOf(":")-1);
            QString ch = "|";
            QStringList strArr;
            int c = StringSplit(getString, ch, strArr);
            if (2 == c)
            {
                UserInfo mUserInfo;
                mUserInfo.Init();
                mUserInfo.strUserName=strArr[0];
                mUserInfo.strPassWord=strArr[1];
                mUserInfoVectSTA.append(mUserInfo);
                emit GetUserInfo(strArr[0],strArr[1]);
            }
        }
        //可坡道解锁的列车信号机
        else if (m_str.left(15) == "CAN_PDJS_LCXHJ:")
        {
            getString = m_str.mid(15,m_str.indexOf(";")-m_str.indexOf(":")-1);
            QStringList strArr;
            QString ch = ",";
            int c = StringSplit(getString, ch, strArr);
            if(getString!="")
            {
                strPDJSXHJ=strArr;
            }
            c=c;
        }
        //点灯灭灯按钮显示配置
        else if (m_str.left(5) == "DDMD:")
        {
            getString = m_str.mid(5,m_str.indexOf(";")-m_str.indexOf(":")-1);
            QStringList strArr;
            QString ch = "|";
            int c = StringSplit(getString, ch, strArr);
            if(c==2)
            {
                for(int i=0;i<c;i++)
                {
                    if(strArr[0]=="1")
                    {
                        nDDMDStateS=true;
                    }
                    else
                    {
                        nDDMDStateS=false;
                    }
                    if(strArr[1]=="1")
                    {
                        nDDMDStateX=true;
                    }
                    else
                    {
                        nDDMDStateX=false;
                    }
                }
                bDDMDFalg=nDDMDStateS|nDDMDStateX;
            }
            //bDDMDFalg=strArr;
            c=c;
        }
        //可见设置中的计数器功能是否可用
        else if(m_str.left(14)=="CHGVISIBLEJSQ:")
        {
            getString = m_str.mid(14, m_str.indexOf(";") - m_str.indexOf(":") - 1);
            QStringList strArr;
            QString ch = "|";
            int c = StringSplit(getString, ch, strArr);
            if(c == 2)
            {
                StaConfigInfo.bCanChgVisleJSQ = ((strArr[0].toInt())==1) ? true : false;
                StaConfigInfo.bVisleJSQ = ((strArr[1].toInt())==1) ? true : false;
            }
        }
        else if(m_str.left(10) == "QJXHDSHOW:")
        {
            getString = m_str.mid(10, m_str.indexOf(";") - m_str.indexOf(":") - 1);
            StaConfigInfo.bQJxhdShow = getString.toInt();
        }
        else if(m_str.left(14) == "STATIONSXLORR:")
        {
            //getString = m_str.Mid(14,sizeof(m_str)-1);
            getString = m_str.mid(14, m_str.indexOf(";") - m_str.indexOf(":") - 1);
            StaConfigInfo.bStaSXLORR = getString.toInt();
        }
        else if (m_str.left(8) == "JCK|FCK:")
        {
            getString = m_str.mid(8, m_str.indexOf(";") - m_str.indexOf(":") - 1);
            QStringList str;
            int c = StringSplit(getString, "|", str);
            if(c==5)
            {
                int j = pMyStation->StaConfigInfo.JCKCount;
                JFCK_T JFCK;
                JFCK.strJCKName = str[0];
                JFCK.strFCKName = str[1];
                //pMyStation->StaConfigInfo.JFCKInfo[j].strJCKName = str[0];
                //pMyStation->StaConfigInfo.JFCKInfo[j].strFCKName = str[1];
                QString name = str[2];
                JFCK.strDirectSta = name;
                JFCK.bJCKAdvance = str[3].toInt();
                JFCK.bFCKAdvance = str[4].toInt();
                //pMyStation->StaConfigInfo.JFCKInfo[j].strDirectSta = name;
                pMyStation->StaConfigInfo.JFCKInfo.append(JFCK);
                pMyStation->StaConfigInfo.JCKCount++;
            }
        }
        else if(m_str.left(9)=="AQXDCNUM:")//安全线道岔个数
        {
            getString = m_str.mid(9, m_str.indexOf(";") - m_str.indexOf(":") - 1);
            StaConfigInfo.AQXDCNUM = getString.toInt();
            code = 0;
        }
        else if(m_str.left(7)=="##AQXDC")//安全线道岔报警灯
        {
            for(int index=0;index<5;index++)
            {
                m_str = file.readLine();
                if(m_str.left(10)=="SHOWPOINT:")
                {
                    getString = m_str.mid(10, m_str.indexOf(";") - m_str.indexOf(":") - 1);
                    StaConfigInfo.AQXBJDCarray[code].p = StringToPoint(getString);
                }
                else if(m_str.left(9)=="SHOWNAME:")
                {
                    getString = m_str.mid(9, m_str.indexOf(";") - m_str.indexOf(":") - 1);
                    StaConfigInfo.AQXBJDCarray[code].DCname = getString;
                }
                else if(m_str.left(13)=="SHOWNAMETYPE:")
                {
                    getString = m_str.mid(13, m_str.indexOf(";") - m_str.indexOf(":") - 1);
                    StaConfigInfo.AQXBJDCarray[code].m_nShowNameType = getString.toInt();
                }
                else if(m_str.left(7)=="DCNODE:")
                {
                    getString = m_str.mid(7, m_str.indexOf(";") - m_str.indexOf(":") - 1);
                    StaConfigInfo.AQXBJDCarray[code].m_ncode = getString.toInt();
                }
                else if(m_str.left(9)=="SHOWTYPE:")
                {
                    getString = m_str.mid(9, m_str.indexOf(";") - m_str.indexOf(":") - 1);
                    StaConfigInfo.AQXBJDCarray[code].m_nShowType = getString.toInt();
                }
            }
            code++;
        }
        else if (m_str.left(4) == "YXT_")
        {
            if(m_str.left(19) == "YXT_MINITE_DISTANCE")
            {
                getString = m_str.mid(20, m_str.indexOf(";") - m_str.indexOf(":") - 1);
                TrainDiagram.miniteDistance = getString.toInt();
            }
            else if(m_str.left(20) == "YXT_STATION_DISTANCE")
            {
                getString = m_str.mid(21, m_str.indexOf(";") - m_str.indexOf(":") - 1);
                TrainDiagram.stationDistance = getString.toInt();
            }
            else if(m_str.left(17) == "YXT_LINE_DISTANCE")
            {
                getString = m_str.mid(18, m_str.indexOf(";") - m_str.indexOf(":") - 1);
                TrainDiagram.lintDistance = getString.toInt();
            }
        }
        else if (m_str.left(11) == "ISHAVEPXRJ:")
        {
            getString = m_str.mid(11, m_str.indexOf(";") - m_str.indexOf(":") - 1);
            StaConfigInfo.isHavePXRJ = getString=="1"?true:false;
        }
        else if(m_str.left(14) == "DlgDCTimeShow:")
        {
            getString = m_str.mid(14, m_str.indexOf(";") - m_str.indexOf(":") - 1);
            StaConfigInfo.DlgDCTimeShow = getString=="1"?true:false;
        }
        else if(m_str.left(11) == "SpeechFlag:")//语音标志
        {
            getString = m_str.mid(11, m_str.indexOf(";") - m_str.indexOf(":") - 1);
            StaConfigInfo.bSpeechFlag = getString=="1"?true:false;
        }
        else if(m_str.left(5)=="ZHJL:")
        {
            getString = m_str.mid(5,m_str.indexOf(";")-m_str.indexOf(":")-1);
            ZHJL zhjl_TEMP;
            QStringList str;
            QStringList str_1;
            QStringList str_2;
            QString ch = "|";
            QString ch1 = ",";

            int c = StringSplit(getString, ch, str);
            int d=0;
            int e=0;
            for(int i=0;i<c;i++)
            {
                if(i==0)
                {
                    d = StringSplit(str[0], ch1, str_1);
                    zhjl_TEMP.ZHJL_RouteDescrip=str[0];//进路描述的总始端+总终端
                    //zhjl_TEMP.ZHJL_RouteDescrip.replace(",","-");
                    if(d==2)
                    {
                        zhjl_TEMP.ZHJL_SDXH = str_1[0];//进路描述的总始端
                        zhjl_TEMP.ZHJL_ZDXH = str_1[1];//进路描述的总终端
                    }
                }
                else
                {
                    e = StringSplit(str[i], ch1, str_2);
                    zhjl_TEMP.ZHJL_RouteDescrip_ZTJL=zhjl_TEMP.ZHJL_RouteDescrip;//组合进路的显示的进路描述
                    //zhjl_TEMP.ZHJL_RouteDescrip_ZTJL.replace(",","-");

                    zhjl_TEMP.ZHJL_SDXH_JL[i-1]=str_2[0];
                    zhjl_TEMP.ZHJL_ZDXH_JL[i-1]=str_2[1];
                    zhjl_TEMP.ZHJL_RouteDescrip_JL[i-1]=zhjl_TEMP.ZHJL_SDXH_JL[i-1]+"-"+zhjl_TEMP.ZHJL_ZDXH_JL[i-1];
                }
            }
            zhjl_TEMP.ZHJL_SonRouteNum = c-1;
            vectZHJL.push_back(zhjl_TEMP);
        }
        else if(m_str.left(11) == "ZHJLUNFLOD:")//组合进路是否默认展开
        {
            getString = m_str.mid(11,m_str.indexOf(";")-m_str.indexOf(":")-1);
            ZHJL_UNFLOD = getString=="1"?true:false;
        }
        else if(m_str.left(12) == "FunBtnState:")//组合进路是否默认展开
        {
            getString = m_str.mid(12,m_str.indexOf(";")-m_str.indexOf(":")-1);
            nFunBtnState = getString.toInt(0, 16);
        }
        else if (m_str.left(5) == "ZDBS:")
        {
            //自动闭塞
            getString = m_str.mid(5, m_str.indexOf(";") - m_str.indexOf(":") - 1);
            CZDBS *pSpaceArrowZDBS = new CZDBS();
            ZDBSInfo[ZDBSConfigNum].strZDJuncXHD = getString.trimmed();
            pSpaceArrowZDBS->m_strName = getString.trimmed();
            ZDBSArray.append(pSpaceArrowZDBS);
            ZDBSConfigNum++;
        }
        else if (m_str.left(6) == "BZDBS:")
        {
            //半自动闭塞
            QStringList strArr;
            getString = m_str.mid(6, m_str.indexOf(";") - m_str.indexOf(":") - 1);
            int count = StringSplit(getString, "|", strArr);
            if(2 == count)
            {
                CBZDBS *pSpaceArrowBZDBS = new CBZDBS();
                BZDBSInfo[BZDBSConfigNum].strBZDJuncXHD = getString.trimmed();
                pSpaceArrowBZDBS->m_strName = strArr[0].trimmed();
                bool ok;
                pSpaceArrowBZDBS->setCode(strArr[1].trimmed().toInt(&ok, 16));//信号机
                BZDBSArray.append(pSpaceArrowBZDBS);
                BZDBSConfigNum++;
            }
        }
        else if (m_str.left(7) == "JIZHOU:")
        {
            //计轴
            getString = m_str.mid(7, m_str.indexOf(";") - m_str.indexOf(":") - 1);
            CJZ *pSpaceArrowJZ = new CJZ();
            JZInfo[JZConfigNum].strJZJuncXHD = getString.trimmed();
            pSpaceArrowJZ->m_strName = getString.trimmed();
            JZArray.append(pSpaceArrowJZ);
            JZConfigNum++;
        }
        else if (m_str.left(3) == "CL:")
        {
            //场联
            getString = m_str.mid(3, m_str.indexOf(";") - m_str.indexOf(":") - 1);
            cl *pSpaceArrowCL = new cl();
            CLInfo[CLConfigNum].strCLJuncXHD = getString.trimmed();
            pSpaceArrowCL->m_strName = getString.trimmed();
            CLArray.append(pSpaceArrowCL);
            CLConfigNum++;
        }
        else if (m_str.left(9) == "Antislip:")//组合进路是否默认展开
        {
            strAntislip = m_str.mid(9,m_str.indexOf(";")-m_str.indexOf(":")-1);
        }
        else if (m_str.left(9) == "Sleepers:")//组合进路是否默认展开
        {
            bSleepers = m_str.mid(9,m_str.indexOf(";")-m_str.indexOf(":")-1).toInt();
        }
    }
//    InitZTPos(pMyStation);
//    InitDcGZBtn();
    return true;
}

//读取联锁表
bool MyStation::readInterlockTable(QString fileName, MyStation *pStation)
{
    QFileInfo fileInfo(fileName);
    if (!fileInfo.isFile())
    {
        qDebug() << QString::fromLocal8Bit("File %1 not existed!").arg(fileName);
        return false;
    }

    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Open file failed！"+fileName;
        return false;
    }

    while(!file.atEnd())
    {
        QByteArray line = file.readLine();
        //QString m_str(line);
        QString m_str = codec->toUnicode(line);
        QString getString;
        if(m_str.left(4) == "####")
        {
            break;
        }
        //忽略注释和空格
        else if(m_str.left(2) == "//" || m_str.trimmed() == "")
        {
            continue;
        }
        else
        {
            getString = m_str.mid(0,m_str.indexOf(";"));
            getString.replace(" ", "");
            QStringList strArr;
            QString ch = "|";
            int c = StringSplit(getString, ch, strArr);
            if(c >= 7)
            {
                InterlockRoute* pRut = new InterlockRoute;
                pRut->Number = strArr[0];//序号
                pRut->Type   = strArr[1];//类型
                StringSplit(strArr[2], ",", pRut->BtnArr);//按钮
                pRut->strXHD = strArr[3];//信号机
                StringSplit(strArr[4], ",", pRut->DCArr);//道岔
                StringSplit(strArr[5], ",", pRut->QDArr);//区段
                StringSplit(strArr[6], ",", pRut->DDXHDArr);//敌对信号
                pStation->vectInterlockRoute.append(pRut);
            }
        }
    }
    //关闭文件
    file.close();
    return true;
}

//读取自动闭塞配置数据
bool MyStation::readBSConfig(QString fileName, MyStation *pMyStation)
{
    QString m_str;
    QString  getString;
    QFileInfo fileInfo(fileName);
    if (!fileInfo.isFile())
    {
        QLOG_ERROR()<<QString::fromLocal8Bit("File %1 not existed!").arg(fileName);
        return false;
    }
    QString ErrMsg = QString("%1配置数据格式错误：").arg(fileName);
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    m_str = file.readLine();
    while(m_str.left(5)!="##END")
    {
        //自动闭塞
        m_str = file.readLine();
        //忽略注释和空格
        if(m_str.left(2) == "//" || m_str.trimmed() == "")
        {
            continue;
        }
        else if(m_str.left(8) == "ZDBSNUM:")
        {
            ZDBSConfigNum=((m_str.mid(8,m_str.indexOf(";")-m_str.indexOf(":")-1)).toInt());
            for (int j = 0; j< ZDBSConfigNum; j++)
            {
                QString str1, str2, str3, str4, str5, str6, str7, str8, str9, str10, str11, str12, str13;
                str1="ZDBS_JUNCXHD:"; //QString("ZDBS_JUNCXHD%1:").arg(j+1);
                str2="ZDBS_BUTTONFORM:"; //QString("ZDBS_BUTTONFORM%1:").arg(j+1);
                str3="ZDBS_ARROWPOINT:"; //QString("ZDBS_ARROWPOINT%1:").arg(j+1);
                str4="ZDBS_NAMEBASEPOINT:"; //QString("ZDBS_NAMEBASEPOINT%1:").arg(j+1);
                str5="ZDBS_FK:"; //QString("ZDBS_FK%1:").arg(j+1);
                str6="ZDBS_ZFZAN:"; //QString("ZDBS_ZFZAN%1:").arg(j+1);
                str7="ZDBS_JCFZAN:"; //QString("ZDBS_JCFZAN%1:").arg(j+1);
                str8="ZDBS_FCFZAN:"; //QString("ZDBS_FCFZAN%1:").arg(j+1);
                str9="ZDBS_YXGFAN:"; //QString("ZDBS_YXGFAN%1:").arg(j+1);
                str10="ZDBS_QJD:"; //QString("ZDBS_QJD%1:").arg(j+1);
                str11="ZDBS_FZD:"; //QString("ZDBS_FZD%1:").arg(j+1);
                str12="ZDBS_SGD:"; //QString("ZDBS_SGD%1:").arg(j+1);
                str13="ZDBS_YXFC:"; //QString("ZDBS_YXFC%1:").arg(j+1);

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str1.length()) == str1)
                {
                    getString = m_str.mid(str1.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    ZDBSInfo[j].strZDJuncXHD = getString;
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str2.length()) == str2)
                {
                    getString = m_str.mid(str2.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    ZDBSInfo[j].ButtonForm = getString.toInt();
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str3.length()) == str3)
                {
                    getString = m_str.mid(str3.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    ZDBSInfo[j].ptZDBox = StringToPoint(getString);
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str4.length()) == str4)
                {
                    getString = m_str.mid(str4.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    QString ch = "|";
                    QStringList strArr;
                    int c = StringSplit(getString, ch, strArr);
                    if (3 <= c)
                    {
                        ZDBSInfo[j].haveWord_Flag = strArr[0].toInt();
                        ZDBSInfo[j].ptWord = StringToPoint(strArr[1]);
                        ZDBSInfo[j].WordName = strArr[2];
                    }
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str5.length()) == str5)
                {
                    getString = m_str.mid(str5.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    QString ch = "|";
                    QStringList strArr;
                    int c = StringSplit(getString, ch, strArr);
                    if (4 <= c)
                    {
                        ZDBSInfo[j].ZDBS_FK_Flag = strArr[0].toInt();
                        ZDBSInfo[j].ptZDBS_FK = StringToPoint(strArr[1]);
                        ZDBSInfo[j].ZDBS_FK_length = strArr[2].toInt();
                        ZDBSInfo[j].ZDBS_FK_breadth = strArr[3].toInt();
                    }
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str6.length()) == str6)
                {
                    getString = m_str.mid(str6.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    QString ch = "|";
                    QStringList strArr;
                    int c = StringSplit(getString, ch, strArr);
                    if (3 <= c)
                    {
                        ZDBSInfo[j].ZFZANFlag = strArr[0].toInt();
                        ZDBSInfo[j].ZFZANPT = StringToPoint(strArr[1]);
                        ZDBSInfo[j].ZFZANName = strArr[2];
                    }
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str7.length()) == str7)//21
                {
                    getString = m_str.mid(str7.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    QString ch = "|";
                    QStringList strArr;
                    int c = StringSplit(getString, ch, strArr);
                    if (3 <= c)
                    {
                        ZDBSInfo[j].JCFZANFlag = strArr[0].toInt();
                        ZDBSInfo[j].JCFZANPT = StringToPoint(strArr[1]);
                        ZDBSInfo[j].JCFZANName = strArr[2];
                    }
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str8.length()) == str8)//21
                {
                    getString = m_str.mid(str8.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    QString ch = "|";
                    QStringList strArr;
                    int c = StringSplit(getString, ch, strArr);
                    if (3 <= c)
                    {
                        ZDBSInfo[j].FCFZANFlag = strArr[0].toInt();
                        ZDBSInfo[j].FCFZANPT = StringToPoint(strArr[1]);
                        ZDBSInfo[j].FCFZANName = strArr[2];
                    }
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str9.length()) == str9)//21
                {
                    getString = m_str.mid(str9.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    QString ch = "|";
                    QStringList strArr;
                    int c = StringSplit(getString, ch, strArr);
                    if (3 <= c)
                    {
                        ZDBSInfo[j].YXGFANFlag = strArr[0].toInt();
                        ZDBSInfo[j].YXGFANPT = StringToPoint(strArr[1]);
                        ZDBSInfo[j].YXGFANName = strArr[2];
                    }
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str10.length()) == str10)//21
                {
                    getString = m_str.mid(str10.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    QString ch = "|";
                    QStringList strArr;
                    int c = StringSplit(getString, ch, strArr);
                    if (3 <= c)
                    {
                        ZDBSInfo[j].QJDFlag = strArr[0].toInt();
                        ZDBSInfo[j].QJDPT = StringToPoint(strArr[1]);
                        ZDBSInfo[j].QJDName = strArr[2];
                    }
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str11.length()) == str11)
                {
                    getString = m_str.mid(str11.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    QString ch = "|";
                    QStringList strArr;
                    int c = StringSplit(getString, ch, strArr);
                    if (3 <= c)
                    {
                        ZDBSInfo[j].FZDFlag = strArr[0].toInt();
                        ZDBSInfo[j].FZDPT = StringToPoint(strArr[1]);
                        ZDBSInfo[j].FZDName = strArr[2];
                    }
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str12.length()) == str12)
                {
                    getString = m_str.mid(str12.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    QString ch = "|";
                    QStringList strArr;
                    int c = StringSplit(getString, ch, strArr);
                    if (3 <= c)
                    {
                        ZDBSInfo[j].SGDFlag = strArr[0].toInt();
                        ZDBSInfo[j].SGDPT = StringToPoint(strArr[1]);
                        ZDBSInfo[j].SGDName = strArr[2];
                    }
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str13.length()) == str13)
                {
                    getString = m_str.mid(str13.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    QString ch = "|";
                    QStringList strArr;
                    int c = StringSplit(getString, ch, strArr);
                    if (3 <= c)
                    {
                        ZDBSInfo[j].YXFCDFlag = strArr[0].toInt();
                        ZDBSInfo[j].YXFCDPT = StringToPoint(strArr[1]);
                        ZDBSInfo[j].YXFCDName = strArr[2];
                    }
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }
            }
        }

        //半自动闭塞
        else if(m_str.left(9) == "BZDBSNUM:")
        {
            BZDBSConfigNum=((m_str.mid(9,m_str.indexOf(";")-m_str.indexOf(":")-1)).toInt());
            for (int j = 0; j< BZDBSConfigNum; j++)
            {
                QString str1, str2, str3, str4, str5, str6, str7, str8, str9/*, str10, str11, str12, str13, str14*/;
                str1="BZDBS_JUNCXHD:"; //QString("BZDBS_JUNCXHD%1:").arg(j+1);
                str2="BZDBS_CODE:"; //QString("BZDBS_CODE%1:").arg(j+1);
                str3="BZDBS_BUTTONFORM:"; //QString("BZDBS_BUTTONFORM%1:").arg(j+1);
                str4="BZDBS_ARROWPOINT:"; //QString("BZDBS_ARROWPOINT%1:").arg(j+1);
                str5="BZDBS_NAMEBASEPOINT:"; //QString("BZDBS_NAMEBASEPOINT%1:").arg(j+1);
                str6="BZDBS_FK:"; //QString("BZDBS_FK%1:").arg(j+1);
                str7="BZDBS_BSAN:"; //QString("BZDBS_BSAN%1:").arg(j+1);
                str8="BZDBS_SGAN:"; //QString("BZDBS_SGAN%1:").arg(j+1);
                str9="BZDBS_FYAN:"; //QString("BZDBS_FYAN%1:").arg(j+1);
//                str10=QString("BZDBS_QHBSAN%1:").arg(j+1);
//                str11=QString("BZDBS_ZYD%1:").arg(j+1);
//                str12=QString("BZDBS_KXD%1:").arg(j+1);
//                str13=QString("BZDBS_RGBSD%1:").arg(j+1);
//                str14=QString("BZDBS_ZDBSD%1:").arg(j+1);

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str1.length()) == str1)
                {
                    getString = m_str.mid(str1.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    BZDBSInfo[j].strBZDJuncXHD = getString;
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str2.length()) == str2)
                {
                    getString = m_str.mid(str2.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    BZDBSInfo[j].m_nCode = StringToHex(getString);
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str3.length()) == str3)
                {
                    getString = m_str.mid(str3.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    BZDBSInfo[j].ButtonForm = getString.toInt();
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str4.length()) == str4)
                {
                    getString = m_str.mid(str4.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    BZDBSInfo[j].ptBZDBSArrow = StringToPoint(getString);
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str5.length()) == str5)
                {
                    getString = m_str.mid(str5.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    QString ch = "|";
                    QStringList strArr;
                    int c = StringSplit(getString, ch, strArr);
                    if (3 <= c)
                    {
                        BZDBSInfo[j].haveBZDWord_Flag = strArr[0].toInt();
                        BZDBSInfo[j].ptBZDWord = StringToPoint(strArr[1]);
                        BZDBSInfo[j].BZDWordName = strArr[2];
                    }
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str6.length()) == str6)
                {
                    getString = m_str.mid(str6.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    QString ch = "|";
                    QStringList strArr;
                    int c = StringSplit(getString, ch, strArr);
                    if (4 <= c)
                    {
                        BZDBSInfo[j].BZDBS_FK_Flag = strArr[0].toInt();
                        BZDBSInfo[j].ptBZDBS_FK = StringToPoint(strArr[1]);
                        BZDBSInfo[j].BZDBS_FK_length = strArr[2].toInt();
                        BZDBSInfo[j].BZDBS_FK_breadth = strArr[3].toInt();
                    }
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str7.length()) == str7)
                {
                    getString = m_str.mid(str7.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    QString ch = "|";
                    QStringList strArr;
                    int c = StringSplit(getString, ch, strArr);
                    if (3 <= c)
                    {
                        BZDBSInfo[j].BSANFlag = strArr[0].toInt();
                        BZDBSInfo[j].BSANPT = StringToPoint(strArr[1]);
                        BZDBSInfo[j].BSANName = strArr[2];
                    }
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str8.length()) == str8)//21
                {
                    getString = m_str.mid(str8.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    QString ch = "|";
                    QStringList strArr;
                    int c = StringSplit(getString, ch, strArr);
                    if (3 <= c)
                    {
                        BZDBSInfo[j].SGANFlag = strArr[0].toInt();
                        BZDBSInfo[j].SGANPT = StringToPoint(strArr[1]);
                        BZDBSInfo[j].SGANName = strArr[2];
                    }
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str9.length()) == str9)//21
                {
                    getString = m_str.mid(str9.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    QString ch = "|";
                    QStringList strArr;
                    int c = StringSplit(getString, ch, strArr);
                    if (3 <= c)
                    {
                        BZDBSInfo[j].FYANFlag = strArr[0].toInt();
                        BZDBSInfo[j].FYANPT = StringToPoint(strArr[1]);
                        BZDBSInfo[j].FYANName = strArr[2];
                    }
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }

//                m_str = file.readLine();//逐行读取文件
//                if(m_str.left(str10.length()) == str10)//21
//                {
//                    getString = m_str.mid(str10.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
//                    QString ch = "|";
//                    QStringList strArr;
//                    int c = StringSplit(getString, ch, strArr);
//                    if (3 <= c)
//                    {
//                        BZDBSInfo[j].QHBSANFlag = strArr[0].toInt();
//                        BZDBSInfo[j].QHBSANPT = StringToPoint(strArr[1]);
//                        BZDBSInfo[j].QHBSANName = strArr[2];
//                    }
//                }

//                m_str = file.readLine();//逐行读取文件
//                if(m_str.left(str11.length()) == str11)//21
//                {
//                    getString = m_str.mid(str11.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
//                    QString ch = "|";
//                    QStringList strArr;
//                    int c = StringSplit(getString, ch, strArr);
//                    if (3 <= c)
//                    {
//                        BZDBSInfo[j].ZYDFlag = strArr[0].toInt();
//                        BZDBSInfo[j].ZYDPT = StringToPoint(strArr[1]);
//                        BZDBSInfo[j].ZYDName = strArr[2];
//                    }
//                }

//                m_str = file.readLine();//逐行读取文件
//                if(m_str.left(str12.length()) == str12)
//                {
//                    getString = m_str.mid(str12.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
//                    QString ch = "|";
//                    QStringList strArr;
//                    int c = StringSplit(getString, ch, strArr);
//                    if (3 <= c)
//                    {
//                        BZDBSInfo[j].KXDFlag = strArr[0].toInt();
//                        BZDBSInfo[j].KXDPT = StringToPoint(strArr[1]);
//                        BZDBSInfo[j].KXDName = strArr[2];
//                    }
//                }

//                m_str = file.readLine();//逐行读取文件
//                if(m_str.left(str13.length()) == str13)
//                {
//                    getString = m_str.mid(str13.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
//                    QString ch = "|";
//                    QStringList strArr;
//                    int c = StringSplit(getString, ch, strArr);
//                    if (3 <= c)
//                    {
//                        BZDBSInfo[j].RGBSDFlag = strArr[0].toInt();
//                        BZDBSInfo[j].RGBSDPT = StringToPoint(strArr[1]);
//                        BZDBSInfo[j].RGBSDName = strArr[2];
//                    }
//                }

//                m_str = file.readLine();//逐行读取文件
//                if(m_str.left(str14.length()) == str14)
//                {
//                    getString = m_str.mid(str14.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
//                    QString ch = "|";
//                    QStringList strArr;
//                    int c = StringSplit(getString, ch, strArr);
//                    if (3 <= c)
//                    {
//                        BZDBSInfo[j].ZDBSDFlag = strArr[0].toInt();
//                        BZDBSInfo[j].ZDBSDPT = StringToPoint(strArr[1]);
//                        BZDBSInfo[j].ZDBSDName = strArr[2];
//                    }
//                }
            }
        }

        //计轴
        else if(m_str.left(6) == "JZNUM:")
        {
            JZConfigNum=((m_str.mid(6,m_str.indexOf(";")-m_str.indexOf(":")-1)).toInt());
            for (int j = 0; j< JZConfigNum; j++)
            {
                QString str1, str2, str3, str4, str5, str6, str7, str8, str9, str10, str11, str12, str13, str14;
                str1="JZ_JUNCXHD:"; //QString("JZ_JUNCXHD%1:").arg(j+1);
                str2="JZ_BUTTONFORM:"; //QString("JZ_BUTTONFORM%1:").arg(j+1);
                str3="JZ_ARROWPOINT:"; //QString("JZ_ARROWPOINT%1:").arg(j+1);
                str4="JZ_NAMEBASEPOINT:"; //QString("JZ_NAMEBASEPOINT%1:").arg(j+1);
                str5="JZ_FK:"; //QString("JZ_FK%1:").arg(j+1);
                str6="JZ_JZSYAN:"; //QString("JZ_JZSYAN%1:").arg(j+1);
                str7="JZ_JZTYAN:"; //QString("JZ_JZTYAN%1:").arg(j+1);
                str8="JZ_JZFLAN:"; //QString("JZ_JZFLAN%1:").arg(j+1);
                str9="JZ_BSQHAN:"; //QString("JZ_BSQHAN%1:").arg(j+1);
                str10="JZ_JZSYD:"; //QString("JZ_JZSYD%1:").arg(j+1);
                str11="JZ_JZTYD:"; //QString("JZ_JZTYD%1:").arg(j+1);
                str12="JZ_JZFLD:"; //QString("JZ_JZFLD%1:").arg(j+1);
                str13="JZ_JZBJD:"; //QString("JZ_JZBJD%1:").arg(j+1);
                str14="JZ_QJD:"; //QString("JZ_JZBJD%1:").arg(j+1);

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str1.length()) == str1)
                {
                    getString = m_str.mid(str1.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    JZInfo[j].strJZJuncXHD = getString;
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str2.length()) == str2)
                {
                    getString = m_str.mid(str2.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    JZInfo[j].ButtonForm = getString.toInt();
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str3.length()) == str3)
                {
                    getString = m_str.mid(str3.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    JZInfo[j].ptJZArrow = StringToPoint(getString);
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str4.length()) == str4)
                {
                    getString = m_str.mid(str4.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    QString ch = "|";
                    QStringList strArr;
                    int c = StringSplit(getString, ch, strArr);
                    if (3 <= c)
                    {
                        JZInfo[j].haveJZWord_Flag = strArr[0].toInt();
                        JZInfo[j].ptJZWord = StringToPoint(strArr[1]);
                        JZInfo[j].JZWordName = strArr[2];
                    }
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str5.length()) == str5)
                {
                    getString = m_str.mid(str5.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    QString ch = "|";
                    QStringList strArr;
                    int c = StringSplit(getString, ch, strArr);
                    if (4 <= c)
                    {
                        JZInfo[j].JZ_FK_Flag = strArr[0].toInt();
                        JZInfo[j].ptJZ_FK = StringToPoint(strArr[1]);
                        JZInfo[j].JZ_FK_length = strArr[2].toInt();
                        JZInfo[j].JZ_FK_breadth = strArr[3].toInt();
                    }
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str6.length()) == str6)
                {
                    getString = m_str.mid(str6.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    QString ch = "|";
                    QStringList strArr;
                    int c = StringSplit(getString, ch, strArr);
                    if (3 <= c)
                    {
                        JZInfo[j].JZSYANFlag = strArr[0].toInt();
                        JZInfo[j].JZSYANPT = StringToPoint(strArr[1]);
                        JZInfo[j].JZSYANName = strArr[2];
                    }
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str7.length()) == str7)//21
                {
                    getString = m_str.mid(str7.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    QString ch = "|";
                    QStringList strArr;
                    int c = StringSplit(getString, ch, strArr);
                    if (3 <= c)
                    {
                        JZInfo[j].JZTYANFlag = strArr[0].toInt();
                        JZInfo[j].JZTYANPT = StringToPoint(strArr[1]);
                        JZInfo[j].JZTYANName = strArr[2];
                    }
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str8.length()) == str8)//21
                {
                    getString = m_str.mid(str8.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    QString ch = "|";
                    QStringList strArr;
                    int c = StringSplit(getString, ch, strArr);
                    if (3 <= c)
                    {
                        JZInfo[j].JZFLANFlag = strArr[0].toInt();
                        JZInfo[j].JZFLANPT = StringToPoint(strArr[1]);
                        JZInfo[j].JZFLANName = strArr[2];
                    }
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str9.length()) == str9)//21
                {
                    getString = m_str.mid(str9.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    QString ch = "|";
                    QStringList strArr;
                    int c = StringSplit(getString, ch, strArr);
                    if (3 <= c)
                    {
                        JZInfo[j].BSQHANFlag = strArr[0].toInt();
                        JZInfo[j].BSQHANPT = StringToPoint(strArr[1]);
                        JZInfo[j].BSQHANName = strArr[2];
                    }
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str10.length()) == str10)//21
                {
                    getString = m_str.mid(str10.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    QString ch = "|";
                    QStringList strArr;
                    int c = StringSplit(getString, ch, strArr);
                    if (3 <= c)
                    {
                        JZInfo[j].JZSYDFlag = strArr[0].toInt();
                        JZInfo[j].JZSYDPT = StringToPoint(strArr[1]);
                        JZInfo[j].JZSYDName = strArr[2];
                    }
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str11.length()) == str11)
                {
                    getString = m_str.mid(str11.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    QString ch = "|";
                    QStringList strArr;
                    int c = StringSplit(getString, ch, strArr);
                    if (3 <= c)
                    {
                        JZInfo[j].JZTYDFlag = strArr[0].toInt();
                        JZInfo[j].JZTYDPT = StringToPoint(strArr[1]);
                        JZInfo[j].JZTYDName = strArr[2];
                    }
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str12.length()) == str12)
                {
                    getString = m_str.mid(str12.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    QString ch = "|";
                    QStringList strArr;
                    int c = StringSplit(getString, ch, strArr);
                    if (3 <= c)
                    {
                        JZInfo[j].JZFLDFlag = strArr[0].toInt();
                        JZInfo[j].JZFLDPT = StringToPoint(strArr[1]);
                        JZInfo[j].JZFLDName = strArr[2];
                    }
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str13.length()) == str13)
                {
                    getString = m_str.mid(str13.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    QString ch = "|";
                    QStringList strArr;
                    int c = StringSplit(getString, ch, strArr);
                    if (3 <= c)
                    {
                        JZInfo[j].JZBJDFlag = strArr[0].toInt();
                        JZInfo[j].JZBJDPT = StringToPoint(strArr[1]);
                        JZInfo[j].JZBJDName = strArr[2];
                    }
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str14.length()) == str14)
                {
                    getString = m_str.mid(str14.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    QString ch = "|";
                    QStringList strArr;
                    int c = StringSplit(getString, ch, strArr);
                    if (3 <= c)
                    {
                        JZInfo[j].QJFlag = strArr[0].toInt();
                        JZInfo[j].QJPT = StringToPoint(strArr[1]);
                        JZInfo[j].QJDName = strArr[2];
                    }
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }
            }
        }
        else if(m_str.left(15) == "APPTRAINWINDOW:")
        {
            getString = m_str.mid(15,m_str.indexOf(";")-m_str.indexOf(":")-1);
            QStringList str;
            QString ch = "|";
            int c = StringSplit(getString, ch, str);
            if (4 == c)
            {
                RoutePreWnd *pRoutePreWnd = new RoutePreWnd;
//                pRoutePreWnd->m_bVisible = str[0].toInt()==1?true:false;
                pRoutePreWnd->bUpSide = str[1].toInt()==1?true:false;
                pRoutePreWnd->pBasePointConst = StringToPoint(str[2]);
                pRoutePreWnd->juncXHDName = str[3];
                pRoutePreWnd->juncXHDCode = this->GetCodeByStrName(pRoutePreWnd->juncXHDName);
                vectRoutePreWnd.append(pRoutePreWnd);
            }
        }
        //场联
        else if(m_str.left(6) == "CLNUM:")
        {
            CLConfigNum=((m_str.mid(6,m_str.indexOf(";")-m_str.indexOf(":")-1)).toInt());

            for (int j = 0; j< CLConfigNum; j++)
            {
                QString str1, str2, str3, str4, str5, str6, str7, str8, str9, str10, str11, str12, str13;
                str1=QString("CL_JUNCXHD:");
                str2=QString("CL_LEFTARROWPOINT:");
                str3=QString("CL_RIGHTARROWPOINT:");
                str4=QString("CL_NAMEBASEPOINT:");
                str5=QString("CL_FK:");
                str6=QString("CL_AFJ:");
                str7=QString("CL_BFJ:");
                str8=QString("CL_CFJ:");
                str9=QString("CL_JGJ:");

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str1.length()) == str1)
                {
                    getString = m_str.mid(str1.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    CLInfo[j].strCLJuncXHD = getString;
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str2.length()) == str2)
                {
                    getString = m_str.mid(str2.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    QString ch = "|";
                    QStringList strArr;
                    int c = StringSplit(getString, ch, strArr);
                    if (2 <= c)
                    {
                        CLInfo[j].HaveLeftArrow = strArr[0].toInt();
                        CLInfo[j].ptCLLEFTArrow = StringToPoint(strArr[1]);
                    }
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str3.length()) == str3)
                {
                    getString = m_str.mid(str3.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    QString ch = "|";
                    QStringList strArr;
                    int c = StringSplit(getString, ch, strArr);
                    if (2 <= c)
                    {
                        CLInfo[j].HaveRightArrow = strArr[0].toInt();
                        CLInfo[j].ptCLRIGHTArrow = StringToPoint(strArr[1]);
                    }
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str4.length()) == str4)
                {
                    getString = m_str.mid(str4.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    QString ch = "|";
                    QStringList strArr;
                    int c = StringSplit(getString, ch, strArr);
                    if (3 <= c)
                    {
                        CLInfo[j].haveCLWord_Flag = strArr[0].toInt();
                        CLInfo[j].ptCLWord = StringToPoint(strArr[1]);
                        CLInfo[j].CLWordName = strArr[2];
                    }
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str5.length()) == str5)
                {
                    getString = m_str.mid(str5.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    QString ch = "|";
                    QStringList strArr;
                    int c = StringSplit(getString, ch, strArr);
                    if (4 <= c)
                    {
                        CLInfo[j].CL_FK_Flag = strArr[0].toInt();
                        CLInfo[j].ptCL_FK = StringToPoint(strArr[1]);
                        CLInfo[j].CL_FK_length = strArr[2].toInt();
                        CLInfo[j].CL_FK_breadth = strArr[3].toInt();
                    }
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str6.length()) == str6)
                {
                    getString = m_str.mid(str6.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    QString ch = "|";
                    QStringList strArr;
                    int c = StringSplit(getString, ch, strArr);
                    if (5 <= c)
                    {
                        CLInfo[j].AFJFlag = strArr[0].toInt();
                        CLInfo[j].AFJPT = StringToPoint(strArr[1]);
                        CLInfo[j].AFJName = strArr[2];
                        CLInfo[j].AFJLightForm = strArr[3].toInt();
                        CLInfo[j].AFJTextPos = strArr[4].toInt();
                    }
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str7.length()) == str7)
                {
                    getString = m_str.mid(str7.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    QString ch = "|";
                    QStringList strArr;
                    int c = StringSplit(getString, ch, strArr);
                    if (5 <= c)
                    {
                        CLInfo[j].BFJFlag = strArr[0].toInt();
                        CLInfo[j].BFJPT = StringToPoint(strArr[1]);
                        CLInfo[j].BFJName = strArr[2];
                        CLInfo[j].BFJLightForm = strArr[3].toInt();
                        CLInfo[j].BFJTextPos = strArr[4].toInt();
                    }
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str8.length()) == str8)
                {
                    getString = m_str.mid(str8.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    QString ch = "|";
                    QStringList strArr;
                    int c = StringSplit(getString, ch, strArr);
                    if (5 <= c)
                    {
                        CLInfo[j].CFJFlag = strArr[0].toInt();
                        CLInfo[j].CFJPT = StringToPoint(strArr[1]);
                        CLInfo[j].CFJName = strArr[2];
                        CLInfo[j].CFJLightForm = strArr[3].toInt();
                        CLInfo[j].CFJTextPos = strArr[4].toInt();
                    }
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }

                m_str = file.readLine();//逐行读取文件
                //数据间的注释
                while(m_str.left(2) == "//")
                {
                    m_str = file.readLine();//逐行读取文件
                }
                if(m_str.left(str9.length()) == str9)
                {
                    getString = m_str.mid(str9.length(),m_str.indexOf(";")-m_str.indexOf(":")-1);
                    QString ch = "|";
                    QStringList strArr;
                    int c = StringSplit(getString, ch, strArr);
                    if (5 <= c)
                    {
                        CLInfo[j].JGJFlag = strArr[0].toInt();
                        CLInfo[j].JGJPT = StringToPoint(strArr[1]);
                        CLInfo[j].JGJName = strArr[2];
                        CLInfo[j].JGJLightForm = strArr[3].toInt();
                        CLInfo[j].JGJTextPos = strArr[4].toInt();
                    }
                }
                else
                {
                    QLOG_ERROR() << ErrMsg<<m_str;
                }
            }
        }
    }
//    InitZDBSPos(pMyStation);
//    InitBZDBSPos(pMyStation);
//    InitJZPos(pMyStation);
//    InitATWPos(pMyStation);
    return true;
}
void MyStation::InitZDBSPos(MyStation *pMyStation)
{
    //自动闭塞
    if(ZDBSConfigNum >0)
    {
        for(int i = 0; i < ZDBSConfigNum; i++)
        {
            CZDBS *pSpaceArrowZDBS = new CZDBS();
            pSpaceArrowZDBS->m_strName = ZDBSInfo[i].strZDJuncXHD;//信号机
            pSpaceArrowZDBS->ButtonForm = ZDBSInfo[i].ButtonForm;//按钮样式
            pSpaceArrowZDBS->bStaSXLORR = StaConfigInfo.bStaSXLORR;
            //通过自动闭塞信号机名称获取对应关联信号机code
            if(GetXHCodeByName(pSpaceArrowZDBS->m_strName)!=-1)
            {
               pSpaceArrowZDBS->setGLXH(GetXHCodeByName(pSpaceArrowZDBS->m_strName));
            }
            pSpaceArrowZDBS->m_nSX = ZDBSisSX(pSpaceArrowZDBS->m_strName);

            if( pSpaceArrowZDBS->m_nSX)
            {
                pSpaceArrowZDBS->ArrowLeftName = "发车";
                pSpaceArrowZDBS->ArrowRightName = "接车";
            }
            else
            {
                pSpaceArrowZDBS->ArrowLeftName = "接车";
                pSpaceArrowZDBS->ArrowRightName = "发车";
            }
            pSpaceArrowZDBS->setArrowPt(ZDBSInfo[i].ptZDBox);//箭头坐标

            pSpaceArrowZDBS->haveWord = ZDBSInfo[i].haveWord_Flag;//文字
            pSpaceArrowZDBS->ptWord = ZDBSInfo[i].ptWord;
            pSpaceArrowZDBS->WordName = ZDBSInfo[i].WordName;

            pSpaceArrowZDBS->haveFK = ZDBSInfo[i].ZDBS_FK_Flag;//方框
            pSpaceArrowZDBS->ptFK = ZDBSInfo[i].ptZDBS_FK;
            pSpaceArrowZDBS->FK_length = ZDBSInfo[i].ZDBS_FK_length;
            pSpaceArrowZDBS->FK_breath = ZDBSInfo[i].ZDBS_FK_breadth;

            pSpaceArrowZDBS->haveZFZAN = ZDBSInfo[i].ZFZANFlag;
            pSpaceArrowZDBS->setZFZANPt(ZDBSInfo[i].ZFZANPT);
            pSpaceArrowZDBS->ZFZAN_Name = ZDBSInfo[i].ZFZANName;

            pSpaceArrowZDBS->haveJCFZAN = ZDBSInfo[i].JCFZANFlag;
            pSpaceArrowZDBS->setJCFZANPt(ZDBSInfo[i].JCFZANPT);
            pSpaceArrowZDBS->JCFZAN_Name = ZDBSInfo[i].JCFZANName;

            pSpaceArrowZDBS->haveFCFZAN = ZDBSInfo[i].FCFZANFlag;
            pSpaceArrowZDBS->setFCFZANPt(ZDBSInfo[i].FCFZANPT);
            pSpaceArrowZDBS->FCFZAN_Name = ZDBSInfo[i].FCFZANName;

            pSpaceArrowZDBS->setIsNX(ZDBSInfo[i].YXGFANFlag);//m_bHaveGFBtn
            pSpaceArrowZDBS->setGFANPt(ZDBSInfo[i].YXGFANPT);
            pSpaceArrowZDBS->GFAN_Name = ZDBSInfo[i].YXGFANName;

            pSpaceArrowZDBS->haveQJ = ZDBSInfo[i].QJDFlag;
            pSpaceArrowZDBS->setBSD_QJPt(ZDBSInfo[i].QJDPT);
            pSpaceArrowZDBS->QJ_Name = ZDBSInfo[i].QJDName;

            pSpaceArrowZDBS->haveFZ = ZDBSInfo[i].FZDFlag;
            pSpaceArrowZDBS->setBSD_FZPt(ZDBSInfo[i].FZDPT);
            pSpaceArrowZDBS->FZ_Name = ZDBSInfo[i].FZDName;

            pSpaceArrowZDBS->haveSG = ZDBSInfo[i].SGDFlag;
            pSpaceArrowZDBS->setBSD_SGPt(ZDBSInfo[i].SGDPT);
            pSpaceArrowZDBS->SG_Name = ZDBSInfo[i].SGDName;

            pSpaceArrowZDBS->haveYXFC = ZDBSInfo[i].YXFCDFlag;
            pSpaceArrowZDBS->setBSD_YXFCPt(ZDBSInfo[i].YXFCDPT);
            pSpaceArrowZDBS->YXFC_Name = ZDBSInfo[i].YXFCDName;

            pSpaceArrowZDBS->bVisleJSQ = StaConfigInfo.bVisleJSQ;
            if(BSWordSize!=0xff)
            {
               pSpaceArrowZDBS->BSWordSize = BSWordSize;
            }
            ZDBSArray.append(pSpaceArrowZDBS);
        }
    }
}
void MyStation::InitBZDBSPos(MyStation *pMyStation)
{
    //半自动闭塞
    if(BZDBSConfigNum >0)
    {
        for(int i = 0; i < BZDBSConfigNum; i++)
        {
            CBZDBS *pSpaceArrowBZDBS = new CBZDBS;
            pSpaceArrowBZDBS->m_strName = BZDBSInfo[i].strBZDJuncXHD;//信号机
            pSpaceArrowBZDBS->setCode(BZDBSInfo[i].m_nCode);//信号机
            pSpaceArrowBZDBS->ButtonForm = BZDBSInfo[i].ButtonForm;//按钮样式
            pSpaceArrowBZDBS->bStaSXLORR = StaConfigInfo.bStaSXLORR;

            pSpaceArrowBZDBS->m_nSX = ZDBSisSX(pSpaceArrowBZDBS->m_strName);

            if( pSpaceArrowBZDBS->m_nSX)
            {
                pSpaceArrowBZDBS->ArrowLeftName = "发车";
                pSpaceArrowBZDBS->ArrowRightName = "接车";
            }
            else
            {
                pSpaceArrowBZDBS->ArrowLeftName = "接车";
                pSpaceArrowBZDBS->ArrowRightName = "发车";
            }
            pSpaceArrowBZDBS->setArrowPt(BZDBSInfo[i].ptBZDBSArrow);//箭头坐标

            pSpaceArrowBZDBS->haveWord = BZDBSInfo[i].haveBZDWord_Flag;//文字
            pSpaceArrowBZDBS->ptWord = BZDBSInfo[i].ptBZDWord;
            pSpaceArrowBZDBS->WordName = BZDBSInfo[i].BZDWordName;

            pSpaceArrowBZDBS->haveFK = BZDBSInfo[i].BZDBS_FK_Flag;//方框
            pSpaceArrowBZDBS->ptFK = BZDBSInfo[i].ptBZDBS_FK;
            pSpaceArrowBZDBS->FK_length = BZDBSInfo[i].BZDBS_FK_length;
            pSpaceArrowBZDBS->FK_breath = BZDBSInfo[i].BZDBS_FK_breadth;

            pSpaceArrowBZDBS->haveBSAN = BZDBSInfo[i].BSANFlag;
            pSpaceArrowBZDBS->setBSANPt(BZDBSInfo[i].BSANPT);
            pSpaceArrowBZDBS->BSAN_Name = BZDBSInfo[i].BSANName;

            pSpaceArrowBZDBS->haveSGAN = BZDBSInfo[i].SGANFlag;
            pSpaceArrowBZDBS->setSGANPt(BZDBSInfo[i].SGANPT);
            pSpaceArrowBZDBS->SGAN_Name = BZDBSInfo[i].SGANName;

            pSpaceArrowBZDBS->haveFYAN = BZDBSInfo[i].FYANFlag;
            pSpaceArrowBZDBS->setFYANPt(BZDBSInfo[i].FYANPT);
            pSpaceArrowBZDBS->FYAN_Name = BZDBSInfo[i].FYANName;

//            pSpaceArrowBZDBS->haveBSQHAN = BZDBSInfo[i].QHBSANFlag;//m_bHaveGFBtn
//            pSpaceArrowBZDBS->setBSQHANPt(BZDBSInfo[i].QHBSANPT);
//            pSpaceArrowBZDBS->BSQHAN_Name = BZDBSInfo[i].QHBSANName;

//            pSpaceArrowBZDBS->haveZYD = BZDBSInfo[i].ZYDFlag;
//            pSpaceArrowBZDBS->setBSD_ZYPt(BZDBSInfo[i].ZYDPT);
//            pSpaceArrowBZDBS->ZYD_Name = BZDBSInfo[i].ZYDName;

//            pSpaceArrowBZDBS->haveKXD = BZDBSInfo[i].KXDFlag;
//            pSpaceArrowBZDBS->setBSD_KXPt(BZDBSInfo[i].KXDPT);
//            pSpaceArrowBZDBS->KXD_Name = BZDBSInfo[i].KXDName;

//            pSpaceArrowBZDBS->haveRGBSD = BZDBSInfo[i].RGBSDFlag;
//            pSpaceArrowBZDBS->setBSD_RGBSPt(BZDBSInfo[i].RGBSDPT);
//            pSpaceArrowBZDBS->RGBSD_Name = BZDBSInfo[i].RGBSDName;

//            pSpaceArrowBZDBS->haveZDBSD = BZDBSInfo[i].ZDBSDFlag;
//            pSpaceArrowBZDBS->setBSD_ZDBSPt(BZDBSInfo[i].ZDBSDPT);
//            pSpaceArrowBZDBS->ZDBSD_Name = BZDBSInfo[i].ZDBSDName;

            pSpaceArrowBZDBS->bVisleJSQ = StaConfigInfo.bVisleJSQ;
            if(BSWordSize!=0xff)
            {
                pSpaceArrowBZDBS->BSWordSize = BSWordSize;
            }
            BZDBSArray.append(pSpaceArrowBZDBS);
        }
    }
}

void MyStation::InitJZPos(MyStation *pMyStation)
{
    //计轴
    if(JZConfigNum >0)
    {
        for(int i = 0; i < JZConfigNum; i++)
        {
            CJZ *pSpaceArrowJZ = new CJZ;
            pSpaceArrowJZ->m_strName = JZInfo[i].strJZJuncXHD;//信号机
            pSpaceArrowJZ->ButtonForm = JZInfo[i].ButtonForm;//按钮样式
            //通过计轴信号机名称获取对应关联信号机code
            if(GetXHCodeByName(pSpaceArrowJZ->m_strName)!=-1)
            {
               pSpaceArrowJZ->setGLXH(GetXHCodeByName(pSpaceArrowJZ->m_strName));
               pSpaceArrowJZ->setCode(GetXHCodeByName(pSpaceArrowJZ->m_strName));
            }
            pSpaceArrowJZ->m_nSX = ZDBSisSX(pSpaceArrowJZ->m_strName);

            if( pSpaceArrowJZ->m_nSX)
            {
                pSpaceArrowJZ->ArrowLeftName = "发车";
                pSpaceArrowJZ->ArrowRightName = "接车";
            }
            else
            {
                pSpaceArrowJZ->ArrowLeftName = "接车";
                pSpaceArrowJZ->ArrowRightName = "发车";
            }
            pSpaceArrowJZ->setArrowPt(JZInfo[i].ptJZArrow);//箭头坐标

            pSpaceArrowJZ->haveWord = JZInfo[i].haveJZWord_Flag;//文字
            pSpaceArrowJZ->ptWord = JZInfo[i].ptJZWord;
            pSpaceArrowJZ->WordName = JZInfo[i].JZWordName;

            pSpaceArrowJZ->haveFK = JZInfo[i].JZ_FK_Flag;//方框
            pSpaceArrowJZ->ptFK = JZInfo[i].ptJZ_FK;
            pSpaceArrowJZ->FK_length = JZInfo[i].JZ_FK_length;
            pSpaceArrowJZ->FK_breath = JZInfo[i].JZ_FK_breadth;

            pSpaceArrowJZ->haveJZSYAN = JZInfo[i].JZSYANFlag;
            pSpaceArrowJZ->setJZSYANPt(JZInfo[i].JZSYANPT);
            pSpaceArrowJZ->JZSYAN_Name = JZInfo[i].JZSYANName;

            pSpaceArrowJZ->haveJZTYAN = JZInfo[i].JZTYANFlag;
            pSpaceArrowJZ->setJZTYANPt(JZInfo[i].JZTYANPT);
            pSpaceArrowJZ->JZTYAN_Name = JZInfo[i].JZTYANName;

            pSpaceArrowJZ->haveJZFLAN = JZInfo[i].JZFLANFlag;
            pSpaceArrowJZ->setJZFLANPt(JZInfo[i].JZFLANPT);
            pSpaceArrowJZ->JZFLAN_Name = JZInfo[i].JZFLANName;

            pSpaceArrowJZ->haveBSQHAN = JZInfo[i].BSQHANFlag;//m_bHaveGFBtn
            pSpaceArrowJZ->setBSQHANPt(JZInfo[i].BSQHANPT);
            pSpaceArrowJZ->BSQHAN_Name = JZInfo[i].BSQHANName;

            pSpaceArrowJZ->haveJZSYD = JZInfo[i].JZSYDFlag;
            pSpaceArrowJZ->setBSD_JZSYPt(JZInfo[i].JZSYDPT);
            pSpaceArrowJZ->JZSYD_Name = JZInfo[i].JZSYDName;

            pSpaceArrowJZ->haveJZTYD = JZInfo[i].JZTYDFlag;
            pSpaceArrowJZ->setBSD_JZTYPt(JZInfo[i].JZTYDPT);
            pSpaceArrowJZ->JZTYD_Name = JZInfo[i].JZTYDName;

            pSpaceArrowJZ->haveJZFLD = JZInfo[i].JZFLDFlag;
            pSpaceArrowJZ->setBSD_JZFLPt(JZInfo[i].JZFLDPT);
            pSpaceArrowJZ->JZFLD_Name = JZInfo[i].JZFLDName;

            pSpaceArrowJZ->haveJZBJD = JZInfo[i].JZBJDFlag;
            pSpaceArrowJZ->setBSD_JZBJPt(JZInfo[i].JZBJDPT);
            pSpaceArrowJZ->JZBJD_Name = JZInfo[i].JZBJDName;

            pSpaceArrowJZ->haveQJ = JZInfo[i].QJFlag;
            pSpaceArrowJZ->setBSD_QJPt(JZInfo[i].QJPT);
            pSpaceArrowJZ->QJD_Name = JZInfo[i].QJDName;

            pSpaceArrowJZ->bVisleJSQ = StaConfigInfo.bVisleJSQ;
            if(BSWordSize!=0xff)
            {
                pSpaceArrowJZ->BSWordSize = BSWordSize;
            }
            JZArray.append(pSpaceArrowJZ);
        }
    }
}

void MyStation::InitCLPos(MyStation *pMyStation)
{
    //计轴
    if(CLConfigNum >0)
    {
        for(int i = 0; i < CLConfigNum; i++)
        {
            cl *pSpaceArrowCL = new cl;
            pSpaceArrowCL->m_strName = CLInfo[i].strCLJuncXHD;//信号机
            pSpaceArrowCL->bStaSXLORR = StaConfigInfo.bStaSXLORR;
            //通过计轴信号机名称获取对应关联信号机code
            if(GetXHCodeByName(pSpaceArrowCL->m_strName)!=-1)
            {
               pSpaceArrowCL->m_nGLXH=(GetXHCodeByName(pSpaceArrowCL->m_strName));
               pSpaceArrowCL->setCode(GetXHCodeByName(pSpaceArrowCL->m_strName));
            }
            pSpaceArrowCL->m_nSX = ZDBSisSX(pSpaceArrowCL->m_strName);

            if( pSpaceArrowCL->m_nSX)
            {
                pSpaceArrowCL->ArrowLeftName = "发车";
                pSpaceArrowCL->ArrowRightName = "接车";
            }
            else
            {
                pSpaceArrowCL->ArrowLeftName = "接车";
                pSpaceArrowCL->ArrowRightName = "发车";
            }

            pSpaceArrowCL->HaveLeftArrow=CLInfo[i].HaveLeftArrow;
            pSpaceArrowCL->HaveRightArrow=CLInfo[i].HaveRightArrow;
            pSpaceArrowCL->LEFTArrowPt=(CLInfo[i].ptCLLEFTArrow);//箭头坐标
            pSpaceArrowCL->LEFTArrowPtConst=(CLInfo[i].ptCLLEFTArrow);
            pSpaceArrowCL->RIGHTArrowPt=(CLInfo[i].ptCLRIGHTArrow);//箭头坐标
            pSpaceArrowCL->RIGHTArrowPtConst=(CLInfo[i].ptCLRIGHTArrow);

            pSpaceArrowCL->haveWord = CLInfo[i].haveCLWord_Flag;//文字
            pSpaceArrowCL->ptWord = CLInfo[i].ptCLWord;
            pSpaceArrowCL->WordName = CLInfo[i].CLWordName;

            pSpaceArrowCL->haveFK = CLInfo[i].CL_FK_Flag;//方框
            pSpaceArrowCL->ptFK = CLInfo[i].ptCL_FK;
            pSpaceArrowCL->FK_length = CLInfo[i].CL_FK_length;
            pSpaceArrowCL->FK_breath = CLInfo[i].CL_FK_breadth;

            pSpaceArrowCL->haveAFJ = CLInfo[i].AFJFlag;
            pSpaceArrowCL->p_BSD_AFJ=(CLInfo[i].AFJPT);
            pSpaceArrowCL->p_BSD_AFJConst=(CLInfo[i].AFJPT);
            pSpaceArrowCL->AFJ_Name = CLInfo[i].AFJName;
            pSpaceArrowCL->AFJLightForm = CLInfo[i].AFJLightForm;
            pSpaceArrowCL->AFJTextPos = CLInfo[i].AFJTextPos;

            pSpaceArrowCL->haveBFJ = CLInfo[i].BFJFlag;
            pSpaceArrowCL->p_BSD_BFJ=(CLInfo[i].BFJPT);
            pSpaceArrowCL->p_BSD_BFJConst=(CLInfo[i].BFJPT);
            pSpaceArrowCL->BFJ_Name = CLInfo[i].BFJName;
            pSpaceArrowCL->BFJLightForm = CLInfo[i].BFJLightForm;
            pSpaceArrowCL->BFJTextPos = CLInfo[i].BFJTextPos;

            pSpaceArrowCL->haveCFJ = CLInfo[i].CFJFlag;
            pSpaceArrowCL->p_BSD_CFJ=(CLInfo[i].CFJPT);
            pSpaceArrowCL->p_BSD_CFJConst=(CLInfo[i].CFJPT);
            pSpaceArrowCL->CFJ_Name = CLInfo[i].CFJName;
            pSpaceArrowCL->CFJLightForm = CLInfo[i].CFJLightForm;
            pSpaceArrowCL->CFJTextPos = CLInfo[i].CFJTextPos;

            pSpaceArrowCL->haveJGJ = CLInfo[i].JGJFlag;
            pSpaceArrowCL->p_BSD_JGJ=(CLInfo[i].JGJPT);
            pSpaceArrowCL->p_BSD_JGJConst=(CLInfo[i].JGJPT);
            pSpaceArrowCL->JGJ_Name = CLInfo[i].JGJName;
            pSpaceArrowCL->JGJLightForm = CLInfo[i].JGJLightForm;
            pSpaceArrowCL->JGJTextPos = CLInfo[i].JGJTextPos;

//            pSpaceArrowCL->bVisleJSQ = StaConfigInfo.bVisleJSQ;
            if(BSWordSize!=0xff)
            {
                pSpaceArrowCL->CLWordSize = CLWordSize;
            }
            CLArray.append(pSpaceArrowCL);
        }
    }
}

//读取表示灯配置数据
bool MyStation::readLampFuncBConfig(QString fileName, MyStation *pMyStation)
{
    QString m_str;
    QString  getString;
    QFileInfo fileInfo(fileName);
    if (!fileInfo.isFile())
    {
        QLOG_ERROR()<<QString::fromLocal8Bit("File %1 not existed!").arg(fileName);
        return false;
    }
    QFile file(fileName);
    bool a = file.open(QIODevice::ReadOnly);
    m_str = file.readLine();
    while(m_str.left(5)!="##END")
    {
        m_str = file.readLine();
        //忽略注释和空格
        if(m_str.left(2) == "//" || m_str.trimmed() == "")
        {
            continue;
        }
        if(m_str.left(12) == "DISPLAYLAMP:")//表示灯
        {
            getString = m_str.mid(12,m_str.indexOf(";")-m_str.indexOf(":")-1);
            QStringList str;
            QString ch = "|";
            int c = StringSplit(getString, ch, str);
            if (5 == c)
            {
                LAMP_T lamp_TEMP;
                lamp_TEMP.nStatus_XiQi = str[0].toInt();
                lamp_TEMP.nStatus_LuoXia = str[1].toInt();
                lamp_TEMP.ptLamp = StringToPoint(str[2]);
                QString name = str[3];
                lamp_TEMP.strName = name;
                lamp_TEMP.DEVName = str[4];
                vectLamp.push_back(lamp_TEMP);
            }
        }
        else if(m_str.left(8) == "FUNCBTN:")//表示灯
        {
            getString = m_str.mid(8,m_str.indexOf(";")-m_str.indexOf(":")-1);
            QStringList str;
            QString ch = "|";
            int c = StringSplit(getString, ch, str);
            if (8 == c)//旧版本，没有功能按钮不显示版本
            {
                FuncBtn_T funcBtn_TEMP;
                funcBtn_TEMP.nStatus_XiQi = str[0].toInt();
                funcBtn_TEMP.nStatus_LuoXia = str[1].toInt();
                funcBtn_TEMP.ptFuncBtn = StringToPoint(str[2]);
                QString name = str[3];
                funcBtn_TEMP.strName = name;
                funcBtn_TEMP.DEVName = str[4];
                funcBtn_TEMP.BtnType = str[5];
                funcBtn_TEMP.DEVBtnType = str[6];
                funcBtn_TEMP.haveQF = str[7].toInt();
                funcBtn_TEMP.haveAN=true;
                vectFuncBtn.push_back(funcBtn_TEMP);
            }
            else if (9 == c)//新版本，铅封计数显示，按钮不显示版本
            {
                FuncBtn_T funcBtn_TEMP;
                funcBtn_TEMP.nStatus_XiQi = str[0].toInt();
                funcBtn_TEMP.nStatus_LuoXia = str[1].toInt();
                funcBtn_TEMP.ptFuncBtn = StringToPoint(str[2]);
                QString name = str[3];
                funcBtn_TEMP.strName = name;
                funcBtn_TEMP.DEVName = str[4];
                funcBtn_TEMP.BtnType = str[5];
                funcBtn_TEMP.DEVBtnType = str[6];
                funcBtn_TEMP.haveQF = str[7].toInt();
                funcBtn_TEMP.haveAN = str[8].toInt();
                vectFuncBtn.push_back(funcBtn_TEMP);
            }
        }
    }

    InitLAMPPos(pMyStation);
    InitFuncBtnPos(pMyStation);
    return true;
}
//读取供电臂设置数据
bool MyStation::readPowerSupplyArmData(QString fileName, MyStation *pMyStation)
{
    QString m_str;
    QString  getString;
    QFileInfo fileInfo(fileName);
    // 指定为GBK
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    if (!fileInfo.isFile())
    {
        QLOG_ERROR()<<QString::fromLocal8Bit("File %1 not existed!").arg(fileName);
        return false;
    }
    QFile file(fileName);
    bool a = file.open(QIODevice::ReadOnly);
    m_str = file.readLine();

    while(m_str.left(4)!="####"&& m_str != "")
    {
        PowerStr *powerdata = new PowerStr();
        powerdata->Init();
        //忽略注释和空格
        if(m_str.left(2) == "//" || m_str.trimmed() == "")
        {
            continue;
        }
        else if(m_str.left(3)=="##>")
        {
            QByteArray line = file.readLine();
            m_str = codec->toUnicode(line);
            if(m_str.left(5)=="Name:")
            {
                powerdata->PowerName=m_str.right(m_str.length()-5).replace(";","").replace("\r\n","");
            }
            line = file.readLine();
            m_str = codec->toUnicode(line);
            if(m_str.left(8)=="RangeGD:")
            {
                powerdata->PowerDEVName.clear();
                QString PowerStr;
                PowerStr=m_str.right(m_str.length()-8).replace(";","").replace("\r\n","");
                QString ch = ",";
                powerdata->PowerDEVName=PowerStr.split(ch);
            }
            line = file.readLine();
            m_str = codec->toUnicode(line);
            if(m_str.left(8)=="RangeDC:")
            {
                powerdata->PowerDCName.clear();
                QString PowerStr;
                PowerStr=m_str.right(m_str.length()-8).replace(";","").replace("\r\n","");
                QString ch = ",";
                powerdata->PowerDCName=PowerStr.split(ch);
            }
            v_PowerArray.append(powerdata);
        }
        m_str = file.readLine();
    }
    return true;
}
//读取封锁设备配置数据-股道
void MyStation::readBlockingDevDataGD(QString fileName, MyStation *pMyStation)
{
    QString m_str;
    QString  getString;
    QFileInfo fileInfo(fileName);
    // 指定为GBK
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    if (!fileInfo.isFile())
    {
        QLOG_ERROR()<<QString::fromLocal8Bit("File %1 not existed!").arg(fileName);
        return ;
    }
    QFile file(fileName);
    bool a = file.open(QIODevice::ReadOnly);
    m_str = file.readLine();

    while(m_str.left(4)!="####"&& m_str != "")
    {
        qdData *qddata = new qdData();
        qddata->Init();
        //忽略注释和空格
        if(m_str.left(2) == "//" || m_str.trimmed() == ""||m_str.trimmed() == "\r\n")
        {
            m_str = file.readLine();
            continue;
        }
        else if(m_str.left(3)=="##>")
        {
            QByteArray line = file.readLine();
            m_str = codec->toUnicode(line);
            if(m_str.left(5)=="Name:")
            {
                qddata->qdName=m_str.right(m_str.length()-5).replace(";","").replace("\r\n","");
            }
            line = file.readLine();
            m_str = codec->toUnicode(line);
            if(m_str.left(8)=="RangeGD:")
            {
                  qddata->qdnode=m_str.right(m_str.length()-8).replace(";","").replace("\r\n","").toInt();
            }
            line = file.readLine();
            m_str = codec->toUnicode(line);
            if(m_str.left(8)=="RangeDC:")
            {

            }
            v_GDArray.append(qddata);
        }
        m_str = file.readLine();
    }
    return ;
}
//读取封锁设备配置数据-区间
void MyStation::readBlockingDevDataQJ(QString fileName, MyStation *pMyStation)
{
    QString m_str;
    QString  getString;
    QFileInfo fileInfo(fileName);
    // 指定为GBK
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    if (!fileInfo.isFile())
    {
        QLOG_ERROR()<<QString::fromLocal8Bit("File %1 not existed!").arg(fileName);
        return ;
    }
    QFile file(fileName);
    bool a = file.open(QIODevice::ReadOnly);
    m_str = file.readLine();

    while(m_str.left(4)!="####"&& m_str != "")
    {
        qdData *qddata = new qdData();
        qddata->Init();
        //忽略注释和空格
        if(m_str.left(2) == "//" || m_str.trimmed() == ""||m_str.trimmed() == "\r\n")
        {
            m_str = file.readLine();
            continue;
        }
        else if(m_str.left(3)=="##>")
        {
            QByteArray line = file.readLine();
            m_str = codec->toUnicode(line);
            if(m_str.left(5)=="Name:")
            {
                qddata->qdName=m_str.right(m_str.length()-5).replace(";","").replace("\r\n","");
            }
            line = file.readLine();
            m_str = codec->toUnicode(line);
            if(m_str.left(8)=="RangeGD:")
            {
                qddata->FSDEVName.clear();
                QString strblock;
                strblock=m_str.right(m_str.length()-8).replace(";","").replace("\r\n","");
                QString ch = ",";
                qddata->FSDEVName=strblock.split(ch);
            }
            line = file.readLine();
            m_str = codec->toUnicode(line);
            if(m_str.left(10)=="RangeCode:")
            {
                qddata->FSDEVCode.clear();
                QString strblock;
                strblock=m_str.right(m_str.length()-10).replace(";","").replace("\r\n","");
                QString ch = ",";
                //qddata->FSDEVCode=strblock.split(ch);
                for(int i=0;i<strblock.split(ch).size();i++)
                {
                    qddata->FSDEVCode << strblock.split(ch)[i].toInt();
                }
            }
            v_QJArray.append(qddata);
        }
        m_str = file.readLine();
    }
    return ;
}

void MyStation::InitLAMPPos(MyStation *pMyStation)
{
    for (int i = 0; i < vectLamp.size(); i++)
    {
        lamp *pLamp = new lamp;
        pLamp->m_strName = vectLamp[i].strName;
        pLamp->pCenter = vectLamp[i].ptLamp;
        pLamp->m_colorXiqi = vectLamp[i].nStatus_XiQi;//RED;
        pLamp->m_colorLuoXia = vectLamp[i].nStatus_LuoXia;//BLACK;
        pLamp->DEVName = vectLamp[i].DEVName;
        if(ViewBtnAndLmapWordSize!=0xff)
        {
           pLamp->ViewBtnAndLmapWordSize = ViewBtnAndLmapWordSize;
        }
        if(ViewBtnAndLmapSize!=0xff)
        {
           pLamp->ViewBtnAndLmapSize = ViewBtnAndLmapSize;
        }
        LampArray.append(pLamp);
        pLamp->Initlamp();
    }
}
void MyStation::InitFuncBtnPos(MyStation *pMyStation)
{
    for (int i = 0; i < vectFuncBtn.size(); i++)
    {
        FUNCBUTTON *funcBtn = new FUNCBUTTON;
//        funcBtn->m_colorXiqi = vectFuncBtn[i].nStatus_XiQi;//RED;
//        funcBtn->m_colorLuoXia = vectFuncBtn[i].nStatus_LuoXia;//BLACK;
        funcBtn->Button_Name = vectFuncBtn[i].strName;
        funcBtn->setButtonPt(vectFuncBtn[i].ptFuncBtn);
        funcBtn->DEVName = vectFuncBtn[i].DEVName;
        funcBtn->m_strType = vectFuncBtn[i].BtnType;
        funcBtn->DEVBtnType = vectFuncBtn[i].DEVBtnType;
        funcBtn->haveQF = vectFuncBtn[i].haveQF;
        funcBtn->haveAN = vectFuncBtn[i].haveAN;
        //funcBtn->haveButton = vectFuncBtn[i].haveAN;

        funcBtn->bVisleJSQ = StaConfigInfo.bVisleJSQ;
        if(ViewBtnAndLmapWordSize!=0xff)
        {
           funcBtn->wordsize = ViewBtnAndLmapWordSize;
        }
        if(ViewBtnAndLmapSize!=0xff)
        {
           funcBtn->ViewBtnAndLmapSize = ViewBtnAndLmapSize;
        }
        FuncBtnArray.append(funcBtn);
    }
}

void MyStation::InitZTPos(MyStation *pMyStation)
{
    StationPlatformOther *StationPlatform = new StationPlatformOther;
    StationPlatform->ZHANTAICount = ZHANTAICount;
    StationPlatform->ShowXinHaoLou = bShowXinHaoLou;
    StationPlatform->XinHaoLouFaceNorth = XinHaoLouFaceNorth;
    StationPlatform->ptXinHaoLou = XinHaoLouPt;
    for(int j = 0; j < ZHANTAICount; j++)
    {
        StationPlatform->StaZTInfo[j].m_bVisibleZT = true;
        StationPlatform->StaZTInfo[j].StationIsHigh = ZHANTAIInfo[j].High;
        StationPlatform->StaZTInfo[j].StationType = ZHANTAIInfo[j].Type;
        StationPlatform->StaZTInfo[j].ptStation = ZHANTAIInfo[j].ptZT;
        StationPlatform->StaZTInfo[j].StationZTLenth = ZHANTAIInfo[j].Length;
    }
    for(int i=0;i<vectText.size();i++)
    {
        StationPlatformOther::TEXT_T text_Temp;
        text_Temp.nColorType = vectText[i].nColorType;
        text_Temp.ptLT = vectText[i].ptLT;
        text_Temp.strText = vectText[i].strText;
        StationPlatform->vectText.push_back(text_Temp);
    }
    for(int i=0;i<vectLine.size();i++)
    {
        StationPlatformOther::LINES_T line_Temp;
        line_Temp.nColorType = vectLine[i].nColorType;
        line_Temp.pt1 = vectLine[i].pt1;
        line_Temp.pt2 = vectLine[i].pt2;
        line_Temp.LineWidth = vectLine[i].LineWidth;
        line_Temp.LineState = vectLine[i].LineState;
        StationPlatform->vectLine.push_back(line_Temp);
    }
    for(int i=0;i<vectRect.size();i++)
    {
        StationPlatformOther::RECT_T rect_Temp;
        rect_Temp.nColorType = vectRect[i].nColorType;
        rect_Temp.pt1 = vectRect[i].pt1;
        rect_Temp.pt2 = vectRect[i].pt2;
        rect_Temp.LineWidth = vectRect[i].LineWidth;
        rect_Temp.LineState = vectRect[i].LineState;
        StationPlatform->vectRect.push_back(rect_Temp);
    }
    StationPlatformOtherArray.append(StationPlatform);
}
//初始化DC尖轨、心轨故障按钮
void MyStation::InitDcGZBtn()
{
    //初始化道岔按钮
    QPoint leftpoint  = StaConfigInfo.LeftDCGZBtnPt;
    QPoint rightpoint = StaConfigInfo.RightDCGZBtnPt;
    int leftCount = StaConfigInfo.LeftDCArray.size();
    int rihtCount = StaConfigInfo.RightDCArray.size();
    int leftLineFeed = StaConfigInfo.LeftLineFeed;
    int rihtLineFeed = StaConfigInfo.RightLineFeed;
    int width = 62;//40;
    int height = 60;

    int j = 0;
    for(int i = 0; i < StaConfigInfo.LeftDCArray.size(); i++)
    {
        CDcBtn *dcBtn = new CDcBtn;

        QString temp = StaConfigInfo.LeftDCArray[i];
        dcBtn->m_strDcName = temp.left(temp.length()-2);
        if( "JG" == temp.right(2) )
        {
            dcBtn->m_tType = DCGZ_JG;
            dcBtn->Button_Name = dcBtn->m_strDcName + StaConfigInfo.DCGZBtnNameJG;
        }
        else if("XG" == temp.right(2) )
        {
            dcBtn->m_tType = DCGZ_XG;
            dcBtn->Button_Name = dcBtn->m_strDcName + StaConfigInfo.DCGZBtnNameXG;
        }
        dcBtn->m_nDcCode   = GetCodeByStrName(dcBtn->m_strDcName);
        if((i>0) && (i%leftLineFeed == 0))//(i>7)
        {
            j++;
        }
//		dcBtn->m_pointCenter = QPoint(leftpoint.x() + width*(i%leftLineFeed), leftpoint.y() + height*j);
        dcBtn->setButtonPt(QPoint(leftpoint.x() + width*(i%leftLineFeed), leftpoint.y() + height*j));
        //第二行及以后
        if(0<j && StaConfigInfo.LeftDCGZnewlineBRight)
        {
//			dcBtn->m_pointCenter = QPoint(leftpoint.x() + width*(leftLineFeed-i%leftLineFeed-1), leftpoint.y() + height*j);
            dcBtn->setButtonPt(QPoint(leftpoint.x() + width*(leftLineFeed-i%leftLineFeed-1), leftpoint.y() + height*j));
        }
        //第二行及以后
        if(0<j)
        {
            dcBtn->m_nStyle = StaConfigInfo.LeftDCGZStyle;
        }
        dcBtn->bVisleJSQ=StaConfigInfo.bVisleJSQ;
        DCBtnArray.push_back(dcBtn);
    }
    j=0;
    for(int i = 0; i < StaConfigInfo.RightDCArray.size(); i++)
    {
        CDcBtn *dcBtn = new CDcBtn;

        QString temp = StaConfigInfo.RightDCArray[i];
        dcBtn->m_strDcName = temp.left(temp.length()-2);
        dcBtn->m_nDcCode   = GetCodeByStrName(dcBtn->m_strDcName);
        if(temp.right(2) == "JG")
        {
            dcBtn->m_tType = DCGZ_JG;
            dcBtn->Button_Name = dcBtn->m_strDcName + StaConfigInfo.DCGZBtnNameJG;
        }
        else if(temp.right(2) == "XG")
        {
            dcBtn->m_tType = DCGZ_XG;
            dcBtn->Button_Name = dcBtn->m_strDcName + StaConfigInfo.DCGZBtnNameXG;
        }
        if((i>0) && (i%rihtLineFeed == 0))
        {
            j++;
        }
//		dcBtn->m_pointCenter = QPoint(rightpoint.x() + width*(i%rihtLineFeed), rightpoint.y() + height*j);
        dcBtn->setButtonPt(QPoint(rightpoint.x() + width*(i%rihtLineFeed), rightpoint.y() + height*j));
        if(0<j && StaConfigInfo.RightDCGZnewlineBRight)
        {
//			dcBtn->m_pointCenter = QPoint(rightpoint.x() + width*(rihtLineFeed-i%rihtLineFeed-1), rightpoint.y() + height*j);
            dcBtn->setButtonPt(QPoint(rightpoint.x() + width*(rihtLineFeed-i%rihtLineFeed-1), rightpoint.y() + height*j));
        }
        //第二行及以后
        if(0<j)
        {
            dcBtn->m_nStyle = StaConfigInfo.RightDCGZStyle;
        }
        dcBtn->bVisleJSQ=StaConfigInfo.bVisleJSQ;
        DCBtnArray.push_back(dcBtn);
    }
}
//初始化车次框
void MyStation::InitTrainWnd()
{
    for(auto ement:DevArray)
    {
        if(ement->getDevType() == Dev_GD)
        {
            TrainNumWnd* pTrainNumWnd = new TrainNumWnd;
            CGD *gd=(CGD*)ement;
            //无岔区段没有车次框
            if(gd->getGDType() != JJ_QD && gd->getGDType() != GD_QD)
            {
                continue;
            }
            pTrainNumWnd->m_nPosCode = gd->getCode();
            pTrainNumWnd->pCenterConst = gd->getCenterPt().toPoint();
            if(JJ_QD == gd->getGDType())
            {
                pTrainNumWnd->misqj=true;
                //文字在上，则车次框在下
                if((gd->m_textRect.top()+gd->m_textRect.bottom())/2 < gd->getCenterPt().y())
                {
                    pTrainNumWnd->pCenterConst += QPoint(0, 60/2+2);
                }
                //文字在下，则车次框在上
                else
                {
                    pTrainNumWnd->pCenterConst -= QPoint(0, 60/2+2);
                }
            }
            else
            {
               pTrainNumWnd->misqj=false;
            }
            m_ArrayTrainNumWnd.append(pTrainNumWnd);
        }
    }
}
//初始化站场文字
void MyStation::InitWord()
{
    for(auto ement:DevArray)
    {
        if(ement->getDevType() == Dev_GD)
        {
            CGD *gd=(CGD*)ement;
            if(GDWordColor!=0xFF)
            {
                gd->wordcolor = GDWordColor;
                if(GDWordSize>0)
                {
                    gd->wordsize = GDWordSize;
                }
            }
            gd->bQJxhdShow = StaConfigInfo.bQJxhdShow;
            gd->bStaSXLORR = StaConfigInfo.bStaSXLORR;
        }
        if(ement->getDevType() == Dev_DC)
        {
            CGDDC *gddc=(CGDDC*)ement;
            if(GDDCWordColor_DW!=0xFF)
            {
                gddc->wordcolor_DW = GDDCWordColor_DW;
                gddc->wordcolor_FW = GDDCWordColor_FW;
                gddc->wordcolor_SK = GDDCWordColor_SK;
                if(GDDCWordSize>0)
                {
                    gddc->wordsize = GDDCWordSize;
                }
            }
        }
        if(ement->getDevType() == Dev_XH)
        {
            CXHD *xhd=(CXHD*)ement;
            if(XHDWordColor!=0xFF)
            {
                xhd->wordcolor = XHDWordColor;
                if(XHDWordSize>0)
                {
                    xhd->wordsize = XHDWordSize;
                }
                InitXHDTGButton(xhd);
            }
        }
        if(ement->getDevType() == Dev_TEXT)
        {
            CText *text=(CText*)ement;
            if(DCQDWordSize!=0xFF)
            {
                //text->DCQDWordSize = /*DCQDWordSize*/GDDCWordSize;
                text->DCQDWordSize = DCQDWordSize;
            }
            if(DirWordSize!=0xFF)
            {
                if(DirWordSize>0)
                {
                    text->DirWordSize = DirWordSize;
                }

            }
        }
    }
}
void MyStation::InitXHDTGButton(CXHD *xhd)
{
    for(auto ement:DevArray)
    {
        if(ement->getDevType() == Dev_TG)
        {
            CTG *tg=(CTG*)ement;
            if(xhd->m_strName == tg->m_strName.left(tg->m_strName.length()-2))
            {
                xhd->rectBtnTG_word = tg->getTextRect();
                xhd->TxtTGButton = tg->m_strName;
            }
        }
    }
}
//初始化安全线报警灯
void MyStation::InitAQXBJD()
{
    for(auto ement:DevArray)
    {
        if(ement->getDevType() == Dev_DC)
        {
            CGDDC *gddc=(CGDDC*)ement;
            for(int i=0; i< StaConfigInfo.AQXDCNUM;i++)
            {
                if(StaConfigInfo.AQXBJDCarray[i].m_ncode == gddc->getCode())
                {
                    gddc->SHOWPOINT = StaConfigInfo.AQXBJDCarray[i].p;
                    gddc->SHOWTYPE = StaConfigInfo.AQXBJDCarray[i].m_nShowType;
                    gddc->SHOWNAMETYPE = StaConfigInfo.AQXBJDCarray[i].m_nShowNameType;
                    gddc->SHOWNAME = StaConfigInfo.AQXBJDCarray[i].DCname;
                }
            }
        }
    }
}

//初始化数据
void MyStation::InitData(bool bMulti)
{
    InitWord();
    //InitZDBSPos(this);
    //InitBZDBSPos(this);
    //InitJZPos(this);
    //InitCLPos(this);
    InitZTPos(this);
    InitDcGZBtn();
    if(!bMulti)
    {
        InitTrainWnd();
    }
    InitAQXBJD();
    InitZXBGDData();
}
//根据设备号获取设备名称
QString MyStation::GetStrNameByCode(int nCode)
{
    for(auto ement:DevArray)
    {
        if(ement->getDevType() == Dev_DC)
        {
            CGDDC *gddc=(CGDDC *)ement;
            if(nCode == (int)gddc->getCode())
            {
                return gddc->getName();
            }
        }
        else if(ement->getDevType() == Dev_XH)
        {
            CXHD *xhd=(CXHD *)ement;
            if(nCode == (int)xhd->getCode())
            {
                return xhd->getName();
            }
        }
        else if(ement->getDevType() == Dev_GD)
        {
            CGD *gd=(CGD*)ement;
            if(nCode == (int)gd->getCode())
            {
                return gd->getName();
            }
        }
        else if(ement->getDevType() == Dev_TG)
        {
            CTG *tg=(CTG*)ement;
            if(nCode == (int)tg->getCode())
            {
                return tg->getName().left((tg->getName().length())-2);
            }
        }
    }
    return "未知";
}
//根据无岔区段code获取股道指针
CGD * MyStation::GetGDByCode(int nCode)
 {
     for(auto ement:DevArray)
     {
         if(ement->getDevType() == Dev_GD)
         {
             CGD *gd=(CGD *)ement;
             if(nCode == (int)gd->getCode())
             {
                 return gd;
             }
         }
     }
     return nullptr;
 }
//根据道岔code获取道岔指针
CGDDC * MyStation::GetDCByCode(int nCode)
{
    for(auto ement:DevArray)
    {
        if(ement->getDevType() == Dev_DC)
        {
            CGDDC *gddc=(CGDDC *)ement;
            if(nCode == (int)gddc->getCode())
            {
                return gddc;
            }
        }
    }
    return nullptr;
}
CXHD * MyStation::GetXHDByCode(int nCode)
{
    for(auto ement:DevArray)
    {
        if(ement->getDevType() == Dev_XH)
        {
            CXHD *pXHD=(CXHD *)ement;
            if(nCode == (int)pXHD->getCode())
            {
                return pXHD;
            }
        }
    }
    return nullptr;
}
 //根据设备名称获取设备号
int MyStation::GetCodeByStrName(QString devName)
{
    for(auto ement:DevArray)
    {
        if(ement->getDevType() == Dev_DC)
        {
            CGDDC *gddc=(CGDDC *)ement;
            if(gddc->getName().contains("<"))
            {
                QString name1 = gddc->getName().left(gddc->getName().indexOf("<"));
                QString name2 = gddc->getName().mid(gddc->getName().indexOf("<") + 1, gddc->getName().length() - gddc->getName().indexOf("<") - 2);

                if(name1 == devName || name2 == devName || devName == gddc->getName())
                {
                    return gddc->getCode();
                }
            }
            else
            {
                if(gddc->getName() == devName)
                {
                    return gddc->getCode();
                }
            }

        }
        else if(ement->getDevType() == Dev_XH)
        {
            CXHD *xhd=(CXHD *)ement;
            if(devName == xhd->getName())
            {
                return xhd->getCode();
            }
        }
        else if(ement->getDevType() == Dev_GD)
        {
            CGD *gd=(CGD*)ement;
            if(devName == gd->getName())
            {
                return gd->getCode();
            }
        }
    }
    return -1;
}
//股道在站场数组中的索引（+1的）
int MyStation::GetGDPosInzcArray(int nCode)
{
    for(int i=0; i<DevArray.count(); i++)
    {
        if(DevArray[i]->getDevType() == Dev_GD)
        {
            CGD *gd=(CGD*)DevArray[i];
            if(nCode == (int)gd->getCode())
            {
                return i+1;
            }
        }
    }
    return 0;
}
//信号机是否属于线路所
bool MyStation::IsXHDinXianLuSuo(QString _strXHD)
{
    for(int i=0; i<StaConfigInfo.XianLuSuoXHDArray.count(); i++)
    {
        if(_strXHD == StaConfigInfo.XianLuSuoXHDArray[i])
        {
            return true;
        }
    }
    return false;
}
//根据名称获取设备索引
int MyStation::GetIndexByStrName(QString devName)
{
    for(int i=0; i<DevArray.count(); i++)
    {
        if(DevArray[i]->getDevType() == Dev_DC)
        {
            CGDDC *gddc=(CGDDC *)DevArray[i];
            if(devName == gddc->getName())
            {
                return i;
            }
        }
        else if(DevArray[i]->getDevType() == Dev_XH)
        {
            CXHD *xhd=(CXHD *)DevArray[i];
            if(devName == xhd->getName())
            {
                return i;
            }
        }
        else if(DevArray[i]->getDevType() == Dev_GD)
        {
            CGD *gd=(CGD*)DevArray[i];
            if(devName == gd->getName())
            {
                return i;
            }
        }
    }
    return -1;
}
//设备是否是股道
bool MyStation::DevIsGDByCode(int nCode)
{
    bool rvalue = false;

    for(int i=0; i<DevArray.count(); i++)
    {
        if(DevArray[i]->getDevType() == Dev_GD)
        {
            CGD *pGD = (CGD*)DevArray[i];
            if((nCode == (int)pGD->getCode()) && (pGD->getGDType() == GD_QD))
            {
                rvalue = true;
                break;
            }
        }
    }

    if(rvalue == false)
    {
        //股道和股道配置的相邻区段判断
        QString strDev = GetStrNameByCode(nCode);
        for(int i=0; i<m_nStationGDNode;i++)
        {
            if(strDev == StationGDNode[i].strGDName)
            {
                rvalue = true;
                break;
            }
            else if(strDev == StationGDNode[i].strGDName1 && StationGDNode[i].strGDName1 != "")
            {
                rvalue = true;
                break;
            }
        }
    }

    return rvalue;
}
void MyStation::SetXHJTGBtnState()
{
    CXHD *pXHD=nullptr;
    CTG *pTG=nullptr;
    for(auto ement:DevArray)
    {
        if(ement->getDevType() == Dev_XH)
        {
            pXHD=(CXHD *)ement;
            for(auto ement:DevArray)
            {
                if(ement->getDevType() == Dev_TG)
                {
                    pTG=(CTG *)ement;
                    if(pTG->getName()==pXHD->getName()+"TA")
                    {
                       pXHD->m_TGXHDButtonShowFlag=true;
                       pXHD->TxtTGButton=pTG->getName();
                       pXHD->nTxtTGButtonCode=pTG->getCode();

                    }
                }
            }
            pXHD->XHDInit(0x55);
        }
    }
}
void MyStation::SetXHJPDJSState()
{
    CXHD *pXHD=nullptr;
    for(auto ement:DevArray)
    {
        if(ement->getDevType() == Dev_XH)
        {
            pXHD=(CXHD *)ement;

            //m_PDJSFalg strPDJSXHJ
            for(int i=0;i<strPDJSXHJ.size();i++)
            {
                if(pXHD->getName() == strPDJSXHJ[i])
                {
                    pXHD->m_PDJSFalg=true;
                }
            }
        }
    }
}
//获取设备上下咽喉信息
QString MyStation::GetDevSX(int ntype,int ncode,bool dcqd)
{
    for(auto ement:DevArray)
    {
        if(!dcqd)
        {
            if(ement->getDevType() == ntype&&ement->getCode()==ncode)
            {
                if(ement->getSX()==0)
                {
                    return "X";
                }
                else if(ement->getSX()==1)
                {
                    return "S";
                }
            }
        }
        else
        {
            if(ement->getDevType() == ntype&&ntype==Dev_DC)
            {
                CGDDC *gddc=(CGDDC *)ement;
                if(gddc->getQDCode()==ncode)
                {
                    //ement->getCode()==ncode
                    if(ement->getSX()==0)
                    {
                        return "X";
                    }
                    else if(ement->getSX()==1)
                    {
                        return "S";
                    }
                }

            }
        }

    }
    return "";
}
//破封统计计数
void MyStation::SetQFTJAdd(QString str)
{
    //pCurrStation->mQianFengVect
    for(int i=0;i<mQianFengVect.size();i++)
    {
        if(mQianFengVect[i].StrName==str)
        {
            mQianFengVect[i].Counter++;
            break;
        }
    }
}
//获取按钮设备名称
QString MyStation::GetANNameByCode(int ntype,int nCode)
{
    //自动闭塞
    if(ntype == Dev_ZDBS)
    {
        for(int j=0;j<ZDBSArray.size();j++)
        {
          if(ZDBSArray[j]->getGLXH()==nCode)
          {
              return ZDBSArray[j]->getName();
          }
        }
    }
    //半自动闭塞
    else if(ntype == Dev_BZDBS)
    {
        for(int j=0;j<BZDBSArray.size();j++)
        {
            if(BZDBSArray[j]->getCode()==nCode)
            {
                return BZDBSArray[j]->getName();
            }
        }
    }
    //计轴
    else if(ntype == Dev_JZ)
    {
        for(int j=0;j<JZArray.size();j++)
        {
            if(JZArray[j]->getCode()==nCode)
            {
                return JZArray[j]->getName();
            }
        }
    }
    //功能按钮 上电解锁 语音（预留） 引导总锁
    else if(ntype == Dev_FUNCB)
    {
        for(int j=0;j<FuncBtnArray.size();j++)
        {
            if(FuncBtnArray[j]->getCode()==nCode)
            {
                return FuncBtnArray[j]->getName();
            }
        }
    }
    //功能按钮 尖轨心轨
    else if(ntype == Dev_DCBTN)
     {
         for(int j=0;j<DCBtnArray.size();j++)
         {
             if(DCBtnArray[j]->m_nDcCode==nCode)
             {
                 return DCBtnArray[j]->m_strDcName;
             }
         }
     }
     return "";
}
//信号机及其他数据初始化
void MyStation::SetXHDFlag()
{
    for(auto ement:DevArray)
    {
        if(ement->getDevType() == Dev_XH)
        {
            CXHD *pXHD=(CXHD *)ement;
            int ct = StaConfigInfo.JCKCount;
            for (int t = 0; t < ct; t++)
            {
                if (pXHD->m_strName == StaConfigInfo.JFCKInfo[t].strJCKName)
                {
                    StaConfigInfo.JFCKInfo[t].nJCKCode = pXHD->m_nCode;
                }
                if (pXHD->m_strName == StaConfigInfo.JFCKInfo[t].strFCKName)
                {
                    StaConfigInfo.JFCKInfo[t].nFCKCode = pXHD->m_nCode;
                }
            }
        }
    }

#if 1
    int ct = StaConfigInfo.JCKCount;
    for (int t = 0; t < ct; t++)
    {
        //接车口关联的接近区段获取
        int jckXHDindex = GetIndexByStrName(StaConfigInfo.JFCKInfo[t].strJCKName);
        if(jckXHDindex < 0)
            break;
        CXHD *pxhd=(CXHD *)(DevArray[jckXHDindex]);
        //CString str1 = GetXHDjucJJQDName(pxhd);//第1个接近
        QString str1 = GetXHDjucJJQDName(pxhd->p1, pxhd->p2);//第1个接近
        StaConfigInfo.JFCKInfo[t].strJCKjucJJQD = str1;
        if(str1 != "")
        {
            StaConfigInfo.JFCKInfo[t].strArrJckJJQD.append(str1);
            QString strNext2 = FindNextJJQD(t, str1);//第2个接近
            if(strNext2 != "")
            {
                StaConfigInfo.JFCKInfo[t].strArrJckJJQD.append(strNext2);
                QString strNext3 = FindNextJJQD(t, strNext2);//第3个接近
                if(strNext3 != "")
                {
                    StaConfigInfo.JFCKInfo[t].strArrJckJJQD.append(strNext3);
                    QString strNext4 = FindNextJJQD(t, strNext3);//第4个接近
                    if(strNext4 != "")
                    {
                        StaConfigInfo.JFCKInfo[t].strArrJckJJQD.append(strNext4);
                        QString strNext5 = FindNextJJQD(t, strNext4);//第5个接近
                        if(strNext5 != "")
                        {
                            StaConfigInfo.JFCKInfo[t].strArrJckJJQD.append(strNext5);
                            QString strNext6 = FindNextJJQD(t, strNext5);//第6个接近
                            if(strNext6 != "")
                            {
                                StaConfigInfo.JFCKInfo[t].strArrJckJJQD.append(strNext6);
                                QString strNext7 = FindNextJJQD(t, strNext6);//第7个接近
                                if(strNext7 != "")
                                {
                                    StaConfigInfo.JFCKInfo[t].strArrJckJJQD.append(strNext7);
                                    QString strNext8 = FindNextJJQD(t, strNext7);//第8个接近
                                    if(strNext8 != "")
                                    {
                                        StaConfigInfo.JFCKInfo[t].strArrJckJJQD.append(strNext8);
                                        QString strNext9 = FindNextJJQD(t, strNext8);//第9个接近
                                        if(strNext9 != "")
                                        {
                                            StaConfigInfo.JFCKInfo[t].strArrJckJJQD.append(strNext9);
                                            QString strNext10 = FindNextJJQD(t, strNext9);//第10个接近
                                            if(strNext10 != "")
                                            {
                                                StaConfigInfo.JFCKInfo[t].strArrJckJJQD.append(strNext10);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        //发车口关联的离去区段获取
        int fckXHDindex = GetIndexByStrName(StaConfigInfo.JFCKInfo[t].strFCKName);
        if (fckXHDindex < 0)
            break;
        CXHD *pxhd2 = (CXHD *)(DevArray[fckXHDindex]);
        //str1 = GetXHDjucJJQDName(pxhd);//第1个离去
        QString str11 = GetXHDjucJJQDName(pxhd2->p1, pxhd2->p2);//第1个接近
        StaConfigInfo.JFCKInfo[t].strFCKjucLQQD = str11;
        if (str11 != "")
        {
            StaConfigInfo.JFCKInfo[t].strArrFckLQQD.append(str11);
            QString strNext2 = FindNextJJQD(t, str11);//第2个离去
            if (strNext2 != "")
            {
                StaConfigInfo.JFCKInfo[t].strArrFckLQQD.append(strNext2);
                QString strNext3 = FindNextJJQD(t, strNext2);//第3个离去
                if (strNext3 != "")
                {
                    StaConfigInfo.JFCKInfo[t].strArrFckLQQD.append(strNext3);
                    QString strNext4 = FindNextJJQD(t, strNext3);//第4个离去
                    if (strNext4 != "")
                    {
                        StaConfigInfo.JFCKInfo[t].strArrFckLQQD.append(strNext4);
                        QString strNext5 = FindNextJJQD(t, strNext4);//第5个离去
                        if (strNext5 != "")
                        {
                            StaConfigInfo.JFCKInfo[t].strArrFckLQQD.append(strNext5);
                            QString strNext6 = FindNextJJQD(t, strNext5);//第6个离去
                            if (strNext6 != "")
                            {
                                StaConfigInfo.JFCKInfo[t].strArrFckLQQD.append(strNext6);
                                QString strNext7 = FindNextJJQD(t, strNext6);//第7个离去
                                if (strNext7 != "")
                                {
                                    StaConfigInfo.JFCKInfo[t].strArrFckLQQD.append(strNext7);
                                    QString strNext8 = FindNextJJQD(t, strNext7);//第8个离去
                                    if (strNext8 != "")
                                    {
                                        StaConfigInfo.JFCKInfo[t].strArrFckLQQD.append(strNext8);
                                        QString strNext9 = FindNextJJQD(t, strNext8);//第9个离去
                                        if (strNext9 != "")
                                        {
                                            StaConfigInfo.JFCKInfo[t].strArrFckLQQD.append(strNext9);
                                            QString strNext10 = FindNextJJQD(t, strNext9);//第10个离去
                                            if (strNext10 != "")
                                            {
                                                StaConfigInfo.JFCKInfo[t].strArrFckLQQD.append(strNext10);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
#endif

}
//获取（接车口）信号机关联的接近区段("XHD.h"循环调用修改)
QString MyStation::GetXHDjucJJQDName(QPoint xhdP1, QPoint xhdP2)
{
    //CPoint *pt[]={&pxhd->p1,&pxhd->p2};
    QPoint pt[]={xhdP1,xhdP2};
    for(int i = 0; i < (int)DevArray.count(); i++)
    {
        if(DevArray[i]->getDevType() == Dev_GD)
        {
            CGD *pGD = (CGD*)DevArray[i];
            if(JJ_QD == (int)pGD->getGDType())
            {
                QRect rectGD;
                QPoint pPtGD[]={pGD->p1.toPoint(),pGD->p2.toPoint(),pGD->p3.toPoint(),pGD->p4.toPoint()};
                for(int k=0;k<2;k++)
                {
                    for(int j=0;j<4;j++)
                    {
                        //rectGD.setRect(pPtGD[j].x()-10,pPtGD[j].y()-10,pPtGD[j].x()+10,pPtGD[j].y()+10);
                        rectGD.setRect(pPtGD[j].x()-5,pPtGD[j].y()-5,10,10);
                        //if(PtInRect(rectGD,*pt[k]))
                        //if(PtInRect(rectGD,pt[k]))
                        if(rectGD.contains(pt[k]))
                        {
                            return pGD->m_strName;
                        }
                    }
                }
            }
        }
    }

    return "";
}
QString MyStation::GetXHDjucGDName(QPoint xhdP1, QPoint xhdP2)
{
    //CPoint *pt[]={&pxhd->p1,&pxhd->p2};
    QPoint pt[]={xhdP1,xhdP2};
    for(int i = 0; i < (int)DevArray.count(); i++)
    {
        if(DevArray[i]->getDevType() == Dev_GD)
        {
            CGD *pGD = (CGD*)DevArray[i];
            //if(JJ_QD == (int)pGD->getGDType()
            //        || GD_QD == (int)pGD->getGDType())
            {
                QRect rectGD;
                QPoint pPtGD[]={pGD->p1.toPoint(),pGD->p2.toPoint(),pGD->p3.toPoint(),pGD->p4.toPoint()};
                for(int k=0;k<2;k++)
                {
                    for(int j=0;j<4;j++)
                    {
                        //rectGD.setRect(pPtGD[j].x()-10,pPtGD[j].y()-10,pPtGD[j].x()+10,pPtGD[j].y()+10);
                        rectGD.setRect(pPtGD[j].x()-5,pPtGD[j].y()-5,10,10);
                        //if(PtInRect(rectGD,*pt[k]))
                        //if(PtInRect(rectGD,pt[k]))
                        if(rectGD.contains(pt[k]))
                        {
                            return pGD->m_strName;
                        }
                    }
                }
            }
        }
    }

    return "";
}

//根据第一个接近区段名称查找下一个接近区段
QString MyStation::FindNextJJQD(int index, QString strJJQD)
{
    int indexJJ = GetIndexByStrName(strJJQD);
    if(indexJJ == -1)
    {
        return "";
    }
    CGD *pGD = (CGD*)DevArray[indexJJ];
    QPoint pt[]={pGD->p12.toPoint(),pGD->p34.toPoint()};
    //int m = (int)DevArray.GetSize();
    for(int i = 0; i < (int)DevArray.count(); i++)
    {
        if(DevArray[i]->getDevType() == Dev_GD)
        {
            CGD *pGDNext = (CGD*)DevArray[i];
            if(JJ_QD == (int)pGDNext->getGDType())
            {
                if(strJJQD == pGDNext->m_strName)//不判断自己
                {
                    continue;
                }
                if(FindJJQDFromJCK(index, pGDNext->m_strName))//如何自己已经在接近区段数组中，则跳过
                {
                    continue;
                }
                QPoint pPtGDNext[]={pGDNext->p12.toPoint(),pGDNext->p34.toPoint()};
                for(int k=0;k<2;k++)
                {
                    for(int j=0;j<2;j++)
                    {
                        QRect rectGDNext;
                        //rectGDNext.setRect(pPtGDNext[j].x()-5,pPtGDNext[j].y()-5,pPtGDNext[j].x()+5,pPtGDNext[j].y()+5);
                        rectGDNext.setRect(pPtGDNext[j].x()-5,pPtGDNext[j].y()-5, 10, 10);
                        //if(PtInRect(rectGDNext,*pt[k]))
                        if(rectGDNext.contains(pt[k]))
                        {
                            return pGDNext->m_strName;
                            break;
                        }
                    }
                }
            }
        }
    }

    return  "";
}

QStringList MyStation::getGDQDListByXHD(CXHD* pXHD)
{
    int index = -1;
    for (int i = 0; i < StaConfigInfo.JCKCount; i++)
    {
        if(StaConfigInfo.JFCKInfo.at(i).strJCKName == pXHD->m_strName
                || StaConfigInfo.JFCKInfo.at(i).strFCKName == pXHD->m_strName)
        {
            index = i;
            break;
        }
    }

    QStringList strGDQDList;
    QString QDName = GetXHDjucGDName(pXHD->p1, pXHD->p2);
    strGDQDList.append(QDName);
    if(index >= 0)
    {
        while (QDName != "") {
            QDName = FindNextJJQD(index, QDName);
            if(QDName != "")
                strGDQDList.append(QDName);
        }
    }
    return strGDQDList;
}
//在接车口接近区段能否找到该接近区段
bool MyStation::FindJJQDFromJCK(int index, QString strJJQD)
{
    for(int i=0;i<StaConfigInfo.JFCKInfo[index].strArrJckJJQD.count();i++)
    {
        if(strJJQD == StaConfigInfo.JFCKInfo[index].strArrJckJJQD[i])
        {
            return true;
        }
    }
    for(int i=0;i<StaConfigInfo.JFCKInfo[index].strArrFckLQQD.count();i++)
    {
        if(strJJQD == StaConfigInfo.JFCKInfo[index].strArrFckLQQD[i])
        {
            return true;
        }
    }

    return false;
}
//初始化本站变通进路基础信息
void MyStation::initBTJLinfo()
{
    //变通进路初始化
    for (int i = 0; i < StaConfigInfo.BTJLCount; i++)
    {
        QString btjlInfo = StaConfigInfo.BTJLArray[i];
        QStringList strArray;
        QString ch = "|";
        int c = StringSplit(btjlInfo, ch, strArray);
        BTJL btjl;
        for(int j=0; j<c ;j++)
        {
            btjl.vectBTJLChild.append(strArray[j]);
        }
        vectBTJL.append(btjl);
    }
}

//联锁状态解析
void MyStation::updateDevStateOfLS(unsigned char *array)
{
    //commToServer = 0;
    setGD();
    int dcNum=(array[10]);
    int xhdNum=(array[11]);
    int qdNum=(array[12]);
    int count=12;
    //道岔状态
    int num=0;
    for(auto ement:DevArray)
    {
        if(num>dcNum)
        {
            break;
        }
        if(ement->getDevType() == Dev_DC)
        {
            CGDDC *gddc=(CGDDC *)ement;
            count++;
            int state_dc_0=array[count];//第0个字节解析
            count++;
            int state_dc_1=array[count];//第1个字节解析
            count++;
            int state_dc_2=array[count];//第2个字节解析
            count++;
            int state_dc_3=array[count];//第3个字节解析
            updateGDDC_StateOfLS(gddc,state_dc_0,state_dc_1,state_dc_2,state_dc_3);
            num++;
        }
    }
    //信号机状态
    num=0;
    for(auto ement:DevArray)
    {
        if(num>xhdNum)
        {
            break;
        }
        if(ement->getDevType() == Dev_XH)
        {
            CXHD *xhd=(CXHD *)ement;
            count++;
            int state_xhd_0=array[count];//第0个字节解析
            count++;
            int state_xhd_1=array[count];//第1个字节解析
            count++;
            int state_xhd_2=array[count];//第2个字节解析
            count++;
            int state_xhd_3=array[count];//第3个字节解析
            updateXHD_StateOfLS(xhd,state_xhd_0,state_xhd_1,state_xhd_2,state_xhd_3);
            num++;
        }
    }
    //股道状态
    num=0;
    for(auto ement:DevArray)
    {
        if(num>qdNum)
        {
            break;
        }
        if(ement->getDevType() == Dev_GD)
        {
            CGD *gd=(CGD *)ement;
            count++;
            int state_gd_0=array[count];
            count++;
            int state_gd_1=array[count];
            count++;
            int state_gd_2=array[count];
            updateGD_StateOfLS(gd,state_gd_0,state_gd_1,state_gd_2);
            num++;
        }
    }

    //自动闭塞状态
    num=0;
    for(auto ement:ZDBSArray)
    {
        if(ement->getDevType() == Dev_ZDBS)
        {
            count++;
            int state_ZDBS_0=array[count];
            count++;
            int state_ZDBS_1=array[count];
            count++;
            int state_ZDBS_2=array[count];
            count++;
            int state_ZDBS_3=array[count];
            count++;
            int state_ZDBS_4=array[count];
            count++;
            int state_ZDBS_5=array[count];
            updateZDBS_StateOfLS(state_ZDBS_0,state_ZDBS_1,state_ZDBS_2,state_ZDBS_3,state_ZDBS_4,state_ZDBS_5);
            num++;
        }
    }
    //半自动闭塞状态
    num=0;
    for(auto ement:BZDBSArray)
    {
        if(ement->getDevType() == Dev_BZDBS)
        {
            count++;
            int state_BZDBS_0=array[count];
            count++;
            int state_BZDBS_1=array[count];
            count++;
            int state_BZDBS_2=array[count];
            count++;
            int state_BZDBS_3=array[count];
            updateBZDBS_StateOfLS(state_BZDBS_0,state_BZDBS_1,state_BZDBS_2,state_BZDBS_3);
            num++;
        }
    }
    //计轴状态
    num=0;
    for(auto ement:JZArray)
    {
        if(ement->getDevType() == Dev_JZ)
        {
            count++;
            int state_JZ_0=array[count];
            count++;
            int state_JZ_1=array[count];
            count++;
            int state_JZ_2=array[count];
            count++;
            int state_JZ_3=array[count];
            updateJZ_StateOfLS(state_JZ_0,state_JZ_1,state_JZ_2,state_JZ_3);
            num++;
        }
    }
    //场联状态
    num=0;
    for(auto ement:CLArray)
    {
        if(ement->getDevType() == Dev_CL)
        {
            count++;
            int state_CL_0=array[count];
            count++;
            int state_CL_1=array[count];
            count++;
            int state_CL_2=array[count];
            count++;
            int state_CL_3=array[count];
            updateCL_StateOfLS(state_CL_0,state_CL_1,state_CL_2,state_CL_3);
            num++;
        }
    }
    //功能按钮状态
    num=0;

    //预留2个空白
    count += 2;
    //************* 其他状态 *************
    //非常站控及模式转换
    count++;
    bool bFCZK = true;
    int modestate = 1;
    if(array[count] == 0x22)//联锁非常站控模式
    {
        bFCZK = true;
    }
    else//CTC模式
    {
        bFCZK = false;
        modestate=array[count];
    }
    if((bFCZK != m_nFCZKMode)||((modestate == 1)&&(m_nModeState==0))/*||modestate==0*/)//模式切换了
    {
        m_bModeChanged = true;
    }
    else
    {
        m_bModeChanged = false;
    }
    m_nModeState = modestate;
    m_nFCZKMode = bFCZK;

    //按图排路模式状态及计划控制模式
    count++;
    //第1位排路模式
    if((int)(array[count]&0x01) == 0x01)
    {
        nStateSelect = true;
    }
    else
    {
        nStateSelect = false;
    }
    //第2位计划控制
    if((int)(array[count]&0x02) == 0x02)
    {
        nPlanCtrl = true;
    }
    else
    {
        nPlanCtrl = false;
    }
    //第3位阶段计划自动签收
    if((int)(array[count]&0x04) == 0x04)
    {
        AutoSignStage = true;
    }
    else
    {
        AutoSignStage = false;
    }
    //咽喉总锁状态
    count++;
    if((array[count]&0x01) == 0x01)//
    {
        m_nSXYDZS = true;
    }
    else
    {
        m_nSXYDZS = false;
    }
    if((array[count]&0x02) == 0x02)//
    {
        m_nXXYDZS = true;
    }
    else
    {
        m_nXXYDZS = false;
    }
    //引导总锁状态处理
    for(auto ement:DevArray)
    {
        if(ement->getDevType() == Dev_DC)
        {
            CGDDC *gddc=(CGDDC *)ement;
            if(m_nSXYDZS)
            {
                gddc->m_nDCSXAllLock |= GDDC_S_LOCK;
            }
            else
            {
                gddc->m_nDCSXAllLock = gddc->m_nDCSXAllLock&(~GDDC_S_LOCK);
            }
            if(m_nXXYDZS)
            {
                gddc->m_nDCSXAllLock |= GDDC_X_LOCK;
            }
            else
            {
                gddc->m_nDCSXAllLock = gddc->m_nDCSXAllLock&(~GDDC_X_LOCK);
            }
        }
    }

    //允许转回灯  0为点灯，1为灭灯
    count++;
    if((array[count]&0x01) == 0x01)//
    {
        m_bAllowZH = false;
    }
    else
    {
        m_bAllowZH = true;
    }

    //坡道解锁倒计时180秒计数
    count++;
    if(StaConfigInfo.YXJLBeginNum > 0)
    {
        int jsq = (int)array[count];
        {
            lamp *pLamp;
            for(int i = 0; i < LampArray.size(); i++)
            {
                if(LampArray[i]->getDevType() == Dev_LAMP)
                {
                    pLamp = (lamp*)LampArray[i];
                    if("PD180s" == pLamp->DEVName_DJGD[0])
                    {
                        if(jsq > 0)
                        {
                            pLamp->PD180s = true;
                            pLamp->m_nTicks = jsq;
                        }
                        else
                        {
                            pLamp->PD180s = false;
                            pLamp->m_nTicks = 0;
                        }
                        break;
                    }
                }
            }
        }
    }

    //S人解180s
    count++;
    if(array[count]>0)
    {
        m_nS180 = true;
    }
    else
    {
        m_nS180 = false;
    }

    //S人解30s
    count++;
    if(array[count]>0)
    {
        m_nS30s = true;
    }
    else
    {
        m_nS30s = false;
    }

    //X人解180s
    count++;
    if(array[count]>0)
    {
        m_nX180 = true;
    }
    else
    {
        m_nX180 = false;
    }

    //X人解30s
    count++;
    if(array[count]>0)
    {
        m_nX30s = true;
    }
    else
    {
        m_nX30s = false;
    }

    //上电解锁状态
    count++;
    if((array[count]&0x01) == 0x01)
    {
        m_bSDJS = true;
    }
    else
    {
        m_bSDJS = false;
    }
    //通信状态
    count++;
    if((array[count]&0x01)  == 0x01)
    {
        m_nComCTCToLS = true;
    }
    else
    {
        m_nComCTCToLS = false;
    }
    if((array[count]&0x02)  == 0x01)
    {
        m_nComCTCCenter = true;
    }
    else
    {
        m_nComCTCCenter = false;
    }

    //进路权限
    count++;
    if((array[count]&0x01)  == 0x01)
    {
        m_nRoutePermission = 1;
    }
    else if((array[count]&0x02)  == 0x02)
    {
        m_nRoutePermission = 2;
    }
    else
    {
        m_nRoutePermission = 0;
    }

    //预留2个空白
    count+=2;

    for(int i=0;i<LampArray.size();i++)
    {
        if(LampArray[i]->getDevType() == Dev_LAMP)
        {
            lamp *lamp_Temp = (lamp *)LampArray[i];
            updatelamp_StateOfLS(lamp_Temp);
        }
    }
    for(int i=0;i<FuncBtnArray.size();i++)
    {
        if(FuncBtnArray[i]->getDevType() == Dev_FUNCB)
        {
            FUNCBUTTON *FUN_Temp = (FUNCBUTTON *)FuncBtnArray[i];
            updateFUNCButton_StateOfLS(FUN_Temp);
        }
    }
    setDCGZButtonState();

    //登录用户个数(最多2个，第1个为当前用户)
    count++;
    int userCount = array[count];//this->vectLoginUser.size();
    int userNum = mUserInfoVectSTA.size();
    if((userCount>0)&&(userCount<=userNum))
    {
        for(int i=0; i<userCount; i++)
        {
            //登录名长度
            count++;
            int userNameLen = array[count];
            //登录名
            count++;
            unsigned char strText[40];
            memset(strText,0,sizeof(strText));
            memcpy(strText, &array[count], userNameLen);
            QString loginName = UnsignedCharArrayToString(strText);
            //登录状态，0未登录，1登录，2注销
            count += userNameLen;
            int nstate=array[count];
            //TODO:用户登录 占线版暂时不考虑用户登录

            //emit UserUpDate(i,loginName,nstate);
        }
    }

    //预留2个空白
    count+=2;
    count++;//下一个状态直接取值

    //预告信息标志码
    int preFlag = array[count++];
    if(0xAA == preFlag)
    {
        //接发车口（进路预告窗）数目
        int preCount = array[count++];
        for(int i=0; i<preCount; i++)
        {
            RoutePreWnd *pRoutePreWndTemp = new RoutePreWnd;
            //进路窗关联信号机设备号
            pRoutePreWndTemp->juncXHDCode = (int)(array[count] | (array[count+1]<<8));
            count+=2;
            //进路序列个数
            int routeCount = array[count];
            count+=1;
            //进路序列详细信息
            for(int j=0; j<routeCount; j++)
            {
                RoutePreWnd::RouteInfo routeInfo;
                //进路序列id
                routeInfo.route_id = (int)(array[count] | (array[count+1]<<8));
                count+=2;
                //车次
                {
                    //车次长度
                    int lenCC = (int)(array[count]&0xFF);
                    count += 1;
                    //车次
                    unsigned char strText[40];
                    memset(strText,0,sizeof(strText));
                    memcpy(strText, &array[count], lenCC);
                    QString strCC = UnsignedCharArrayToString(strText);
                    routeInfo.CheCi = strCC;
                    count += lenCC;
                }
                //股道
                {
                    //股道长度
                    int lenGD = (int)(array[count]&0xFF);
                    count += 1;
                    //股道
                    unsigned char strText[40];
                    memset(strText,0,sizeof(strText));
                    memcpy(strText, &array[count], lenGD);
                    QString strGD = UnsignedCharArrayToString(strText);
                    routeInfo.GDName = strGD;
                    count += lenGD;
                }
                //客货类型
                routeInfo.m_nKHType=array[count];
                count+=1;
                //进路接发车类型
                routeInfo.routeType=array[count];
                count+=1;
                //进路状态
                routeInfo.routeState=array[count];
                count+=1;
                //增加一条
                pRoutePreWndTemp->vectRouteInfo.append(routeInfo);
            }
            //区间逻辑检查状态
            pRoutePreWndTemp->statusQJLJJC=array[count];
            count+=1;

            vectRoutePreWndTemp.append(pRoutePreWndTemp);
        }
    }
    UpdateRoutePreWndInfo();

}

void MyStation::updateGDDC_StateOfLS(CGDDC *gddc, int state_0, int state_1, int state_2, int state_3)
{
    //第0字节:
    if((state_0 & 0x01) == 0x01)//占用状态
    {
         gddc->setState(QDZY);
    }
    else if((state_0 & 0x01) == 0x00)//出清状态
    {
        if((state_0 & 0x02) == 0x02)//区段锁闭
        {
             gddc->setState(QDSB);
        }
        else if((state_0 & 0x02) == 0x00)//
        {
             gddc->setState(QDKX);
        }
    }
    if((state_0 & 0x04) == 0x04)//单锁
    {
        gddc->setIsDS(true);
    }
    else if((state_0 & 0x04) == 0x00)
    {
        gddc->setIsDS(false);
    }
    if((state_0 & 0x08) == 0x08)//单封
    {
        gddc->setIsFS(true);
    }
    else if((state_0 & 0x08) == 0x00)
    {
        gddc->setIsFS(false);
    }
    if((state_0 & 0x30) == 0x30)
    {
        gddc->setDCWZ(DCSK);
        gddc->setDCState(0);
    }
    else if((state_0 & 0x10) == 0x10)//定位
    {
        gddc->setDCWZ(DCDW);
        gddc->setDCState(1);
    }
    else if((state_0 & 0x20) == 0x20)//反位
    {
        gddc->setDCWZ(DCFW);
        gddc->setDCState(2);
    }

    //第1字节:
    if((state_1 & 0x01) == 0x01)//尖轨故障
    {
        gddc->setJGGZ(true);
    }
    else if((state_1 & 0x01) == 0x00)
    {
        gddc->setJGGZ(false);
    }

    if((state_1 & 0x02) == 0x02)//心轨故障
    {
        gddc->setXGGZ(true);
    }
    else if((state_1 & 0x02) == 0x00)
    {
        gddc->setXGGZ(false);
    }

//    if(((state_1 & 0x0F)>>2)== 0x01)//定位分路不良
//    {
//        gddc->setIsFLBL_DW(true);
//        gddc->m_nDWFLBLKX=false;
//    }
//    else if(((state_1 & 0x0F)>>2)== 0x02)//定位分路不良空闲
//    {
//        gddc->m_nDWFLBLKX=true;
//    }
//    else if(((state_1 & 0x0F)>>2)== 0x00)//无定位分路不良
//    {
//        gddc->m_nDWFLBLKX=false;
//        gddc->setIsFLBL_DW(false);
//    }
    if(!gddc->bJSJSetFLBLFlagDW)
    {
       gddc->flblStatusDW =(state_1 & 0x0C)>>2;
    }


//    if(((state_1 & 0x30)>>4)== 0x01)//反位分路不良
//    {
//        gddc->setIsFLBL_FW(true);
//        gddc->m_nFWFLBLKX=false;
//    }
//    else if(((state_1 & 0x30)>>4)== 0x02)//反位分路不良空闲
//    {
//        gddc->m_nFWFLBLKX=true;
//    }
//    else if(((state_1 & 0x30)>>4)== 0x00)//无反位分路不良
//    {
//        gddc->m_nFWFLBLKX=false;
//        gddc->setIsFLBL_FW(false);
//    }
     if(!gddc->bJSJSetFLBLFlagFW)
     {
        gddc->flblStatusFW =(state_1 & 0x30)>>4;
     }
    //gddc->flblStatusFW =(state_1 & 0x30)>>4;

//    if(((state_1 & 0xC0)>>6)== 0x01)//岔前分路不良
//    {
//        gddc->setIsFLBL_CQ(true);
//        gddc->m_nCQFLBLKX=false;
//    }
//    else if(((state_1 & 0xC0)>>6)== 0x02)//岔前分路不良空闲
//    {
//        gddc->m_nCQFLBLKX=true;
//    }
//    else if(((state_1 & 0xC0)>>6)== 0x00)//无岔前分路不良
//    {
//        gddc->m_nCQFLBLKX=false;
//        gddc->setIsFLBL_CQ(false);
//    }
      if(!gddc->bJSJSetFLBLFlagCQ)
      {
          gddc->flblStatusCQ =(state_1 & 0xC0)>>6;
      }
    //gddc->flblStatusCQ =(state_1 & 0xC0)>>6;

    if(gddc->getOldState(QDZY) && !gddc->getState(QDZY))
    {
        //分路不良的区段， 在进路出清后， 会自动变为“未确认空闲” 的闪烁状态
//        if((gddc->getIsFLBL_CQ())&&(gddc->m_nCQFLBLKX))
//        {
//            gddc->m_nCQFLBLKX = false;
//        }
//        if((gddc->getIsFLBL_DW())&&(gddc->m_nDWFLBLKX))
//        {
//            gddc->m_nDWFLBLKX = false;
//        }
//        if((gddc->getIsFLBL_FW())&&(gddc->m_nFWFLBLKX))
//        {
//            gddc->m_nFWFLBLKX = false;
//        }
        if(gddc->flblStatusDW == 2)
        {
            gddc->flblStatusDW = 1;
        }
        if(gddc->flblStatusFW == 2)
        {
            gddc->flblStatusFW = 1;
        }
        if(gddc->flblStatusCQ == 2)
        {
            gddc->flblStatusCQ = 1;
        }
        gddc->m_nOldState2 = gddc->m_nOldState;
        gddc->m_nOldState  = gddc->getState();

    }
    if ((gddc->getOldState(QDSB) && gddc->getState(QDKX))
        )
    {
        //分路不良的区段， 在进路出清后， 会自动变为“未确认空闲” 的闪烁状态 20180510
        //进路办理后又取消进路，则自动变为“未确认空闲” 的闪烁状态 2021.1.22
        if(gddc->flblStatusDW == 2)
        {
            gddc->flblStatusDW = 1;
        }
        if(gddc->flblStatusFW == 2)
        {
            gddc->flblStatusFW = 1;
        }
        if(gddc->flblStatusCQ == 2)
        {
            gddc->flblStatusCQ = 1;
        }
        gddc->m_nOldState2 = gddc->m_nOldState;
        gddc->m_nOldState  = gddc->getState();
    }

    //第2字节:
    if((state_2 & 0x01) == 0x01)//定位无电
    {
        gddc->setIsWD_DW(false);
    }
    else if((state_2 & 0x01) == 0x00)
    {
        gddc->setIsWD_DW(true);
    }

    if((state_2 & 0x02) == 0x02)//定位无电
    {
        gddc->setIsWD_FW(false);
    }
    else if((state_2 & 0x02) == 0x00)
    {
        gddc->setIsWD_FW(true);
    }

    if((state_2 & 0x04) == 0x04)//定位无电
    {
        gddc->setIsWD_CQ(false);
    }
    else if((state_2 & 0x04) == 0x00)
    {
        gddc->setIsWD_CQ(true);
    }

    //第3字节:
    gddc->speedLimitStatus = state_3 & 0x0F;

}

void MyStation::updateXHD_StateOfLS(CXHD *xhd, int state_xhd_0,int state_xhd_1,int state_xhd_2,int state_xhd_3)
{
    //第0字节
    if((state_xhd_0 & 0x0f) == 0x01)//开放白灯
    {
        xhd->setXHDState(XHD_BD);
    }
    else if((state_xhd_0 & 0x0f) == 0x02)//开放红白灯
    {
        xhd->setXHDState(XHD_YD);
    }
    else if((state_xhd_0 & 0x0f) == 0x03)//开放绿灯
    {
        xhd->setXHDState(XHD_LD);
    }
    else if((state_xhd_0 & 0x0f) == 0x04)//开放黄灯
    {
        xhd->setXHDState(XHD_UD);
    }
    else if((state_xhd_0 & 0x0f) == 0x05)//开放绿黄灯
    {
        xhd->setXHDState(XHD_LU);
    }
    else if((state_xhd_0 & 0x0f) == 0x06)//开放双黄灯
    {
        xhd->setXHDState(XHD_UU);
    }
    else if((state_xhd_0 & 0x0f)==0x07)	//开放红灯
    {
        xhd->setXHDState(XHD_HD);
    }
    else if((state_xhd_0 & 0x0f) == 0x08)//开放蓝灯
    {
        xhd->setXHDState(XHD_AD);
    }
    else if((state_xhd_0 & 0x0f) == 0x09)//断丝
    {
        xhd->setXHDState(XHD_DS);
    }
    else if((state_xhd_0 & 0x0f) == 0x0A)//黄闪黄
    {
        xhd->setXHDState(XHD_USU);
    }
    else if((state_xhd_0 & 0x0f) == 0x0B)//白闪
    {
        xhd->setXHDState(XHD_BS);
    }
    else if((state_xhd_0 & 0x0f) == 0x0C)//双绿
    {
        xhd->setXHDState(XHD_LL);
    }

    if((state_xhd_0 & 0x10) == 0x10)//点灯灭灯
    {
        xhd->setIsMD(false);
    }
    else if((state_xhd_0 & 0x10) == 0x00)
    {
        xhd->setIsMD(true);
    }
    if((state_xhd_0 & 0x20) == 0x20)//列车按钮封闭
    {
        xhd->setIsLCANFB(true);
    }
    else if((state_xhd_0 & 0x20) == 0x00)
    {
        xhd->setIsLCANFB(false);
    }
    if((state_xhd_0 & 0x40) == 0x40)//调车按钮封闭
    {
        xhd->setIsDCANFB(true);
    }
    else if((state_xhd_0 & 0x40) == 0x00)
    {
        xhd->setIsDCANFB(false);
    }
    if((state_xhd_0 & 0x80) == 0x80)//引导按钮封闭
    {
        xhd->setIsYDANFB(true);
    }
    else if((state_xhd_0 & 0x80) == 0x00)
    {
        xhd->setIsYDANFB(false);
    }
    //第1字节
    if ((state_xhd_1 & 0x01)==0x01)	//红灯灯丝断丝
    {
        xhd->setXHDState(XHD_DS);
        xhd->XHD_ds_HD = true;
    }
    else
    {
        xhd->XHD_ds_HD = false;
    }

    if((state_xhd_1 & 0x02)==0x02)//绿灯灯丝断丝
    {
        //pXHD->setXHDState(XHD_DS);
        xhd->XHD_ds_LD = true;
    }
    else
    {
        xhd->XHD_ds_LD = false;
    }

    if((state_xhd_1 & 0x04)==0x04)//黄灯灯丝断丝
    {
        //pXHD->setXHDState(XHD_DS);
        xhd->XHD_ds_YD=true;
    }
    else
    {
        xhd->XHD_ds_YD=false;
    }

    if((state_xhd_1 & 0x08)==0x08)//白灯灯丝断丝
    {
        //pXHD->setXHDState(XHD_DS);
        xhd->XHD_ds_BD=true;
    }
    else
    {
        xhd->XHD_ds_BD=false;
    }

    if((state_xhd_1 & 0x10)==0x10)//蓝灯灯丝断丝
    {
        //pXHD->setXHDState(XHD_DS);
        xhd->XHD_ds_AD=true;
    }
    else
    {
        xhd->XHD_ds_AD=false;
    }

    if((state_xhd_1 & 0x20)==0x20)//引导白灯灯丝断丝
    {
        //pXHD->setXHDState(XHD_DS);
        xhd->XHD_ds_YBD=true;
    }
    else
    {
        xhd->XHD_ds_YBD=false;
    }

    if((state_xhd_1 & 0x40)==0x40)//预留
    {
        //pXHD->setXHDState(XHD_DS);
    }
    else
    {

    }

    if((state_xhd_1 & 0x80)==0x80)//预留
    {
        //pXHD->setXHDState(XHD_DS);
    }
    else
    {

    }
    //第2字节
    //if((state_xhd_2>0)&&(state_xhd_2<0xFF))
    {
        xhd->setTimeCount(state_xhd_2);
    }
    //第3字节
    if((state_xhd_3 & 0x01)==0x01)//列车按钮终端闪烁
    {
        xhd->isLCANFlash=true;
        xhd->m_nLcIsDown=4;
    }
    else
    {
        xhd->isLCANFlash=false;
    }
    if((state_xhd_3 & 0x02)==0x02)//调车按钮终端闪烁
    {
        xhd->isDCANFlash=true;
        xhd->m_nDcIsDown=4;
    }
    else
    {
        xhd->isDCANFlash=false;
    }
    if((state_xhd_3 & 0x04)==0x04)//变通按钮终端闪烁
    {
        xhd->isBTANFlash=true;
    }
    else
    {
        xhd->isBTANFlash=false;
    }

}

void MyStation::updateGD_StateOfLS(CGD *gd, int state_gd_0, int state_gd_1, int state_gd_2)
{
    //第0字节
    if((state_gd_0 & 0x01) == 0x01)//占用状态
    {
         gd->setState(QDZY);
    }
    else if((state_gd_0 & 0x01) == 0x00)//出清状态
    {
        if((state_gd_0 & 0x02) == 0x02)//区段锁闭
        {
             gd->setState(QDSB);
        }
        else if((state_gd_0 & 0x02) == 0x00)//
        {
             gd->setState(QDKX);
        }
    }
//    if((state_gd_0 & 0x01) == 0x01)//区段占用
//    {
//        gd->setState(QDZY);
////        gd->setQDColor(Qt::red);
//    }
//    else if((state_gd_0 & 0x02) == 0x02)//区段锁闭
//    {
//        gd->setState(QDSB);
////        gd->setQDColor(Qt::white);
//    }
//    else//区段空闲
//    {
//        gd->setState(QDKX);
////        gd->setQDColor(SkyBlue);
//    }
    if((state_gd_0 & 0x04) == 0x04)//股道封锁
    {
        gd->setFS(true);
    }
    else if((state_gd_0 & 0x04) == 0x00)
    {
        gd->setFS(false);
    }

    if((state_gd_0 & 0x10) == 0x10)//股道有电无电,0为无电,1为有电
    {
        gd->setPowerCut(false);
    }
    else if((state_gd_0 & 0x10) == 0x00)
    {
        gd->setPowerCut(true);
    }

    if(((state_gd_0 & 0x60)>>5) == 0x01)//分路不良
    {
        gd->setGDFLBL(true);
        gd->m_nGDFLBLKX = false;
    }
    else if(((state_gd_0 & 0x60)>>5) == 0x02)
    {
        if(gd->getGDType()!=JJ_QD)//接近区段不能设置分路不良空闲
        {
            gd->m_nGDFLBLKX = true;
        }
    }
//    else
//    {
//        gd->setGDFLBL(false);
//        gd->m_nGDFLBLKX = false;
//    }
    else if(!gd->bJSJSetFLBLFlag)
    {
        gd->setGDFLBL(false);
        gd->m_nGDFLBLKX = false;
    }

    if( gd->getOldState(QDZY) && (!gd->getState(QDZY)))
    {
        //分路不良的区段， 在进路出清后， 会自动变为“未确认空闲” 的闪烁状态 20180510
        if( (gd->getGDFLBL()==true) && (gd->m_nGDFLBLKX == true))
        {
            gd->m_nGDFLBLKX  = false;
            gd->m_nOldState2 = gd->m_nOldState;
            gd->m_nOldState  = gd->getState();
        }
    }
    if ((gd->getOldState(QDSB) && gd->getState(QDKX)))
    {
        //AfxMessageBox(pGD->m_strName);
        //分路不良的区段， 在进路出清后， 会自动变为“未确认空闲” 的闪烁状态 20180510
        //进路办理后又取消进路，则自动变为“未确认空闲” 的闪烁状态 2021.1.22
        if ((gd->isGDFLBL == true) && (gd->m_nGDFLBLKX == true))
        {
            gd->m_nGDFLBLKX = false;
            gd->m_nOldState2 = gd->m_nOldState;
            gd->m_nOldState = gd->getState();
        }
    }

    //第1字节
    gd->bsqdfmDirection = state_gd_1 & 0x03;
    gd->bsqdfmCode = (state_gd_1 & 0x7C)>>2;

    //第2字节
    gd->speedLimitStatus = state_gd_2;

}

void MyStation::updateQD_StateOfLS(CQD *qd, int state)
{
    if((state & 0x01) == 0x01)//区段占用
    {
        qd->setState(QDZY);
        setQD_Color(qd, Qt::red);
    }
    else if((state & 0x02) == 0x02)//区段锁闭
    {
        qd->setState(QDSB);
        setQD_Color(qd, Qt::white);
    }
    else//区段空闲
    {
        qd->setState(QDKX);
        setQD_Color(qd, SkyBlue);
    }
}
//解析自动闭塞状态
void MyStation::updateZDBS_StateOfLS(int state_ZDBS_0,int state_ZDBS_1,int state_ZDBS_2,int state_ZDBS_3,int state_ZDBS_4,int state_ZDBS_5)
{
    //第0字节
    //第1字节
    int glXHNum;
    QString XHDName = "";
    glXHNum = (state_ZDBS_0) | (state_ZDBS_1<<8);

    CXHD *xhd;
    CZDBS *ZDBS;
    for(int i=0;i<DevArray.size();i++)
    {
        if(DevArray[i]->getDevType()==Dev_XH)
        {
            xhd = (CXHD *)DevArray[i];
            if(xhd->getCode() == glXHNum)
            {
                XHDName = xhd->m_strName;
            }
        }
    }
    for(int i=0;i<ZDBSConfigNum;i++)
    {
        if(ZDBSArray[i]->getDevType()==Dev_ZDBS)
        {
            ZDBS = (CZDBS *)ZDBSArray[i];
            if(ZDBS->m_strName == XHDName)
            {
                //第2字节
                if((state_ZDBS_2 & 0x01) == 0x01)
                {
//                    ZDBS->setIsDown_ZFZ(2);
                    if(ZDBS->getIsDown_ZFZ()==1)
                    {
                        ZDBS->m_nDownTime_ZFZ_1++;
                        if(ZDBS->m_nDownTime_ZFZ_1>30)
                        {
                            ZDBS->setIsDown_ZFZ(2);
                            ZDBS->m_nDownTime_ZFZ_1=0;
                        }
                    }
                    else
                    {
                        ZDBS->setIsDown_ZFZ(2);
                    }
                }
                else if((state_ZDBS_2 & 0x01) == 0x00)
                {
                    if(ZDBS->getIsDown_ZFZ()==1)
                    {
                        //ZDBS->setIsDown_ZFZ(0);
                        ZDBS->m_nDownTime_ZFZ_1++;
                        if(ZDBS->m_nDownTime_ZFZ_1>30)
                        {
                            ZDBS->setIsDown_ZFZ(0);
                            ZDBS->m_nDownTime_ZFZ_1=0;
                        }
                    }
                    else
                    {
                        ZDBS->setIsDown_ZFZ(0);
                    }
                    //ZDBS->setIsDown_ZFZ(0);
                }
                if((state_ZDBS_2 & 0x02) == 0x02)
                {
                    //ZDBS->setIsDown_FCFZ(2);
                    if(ZDBS->getIsDown_FCFZ()==1)
                    {
                        //ZDBS->setIsDown_FCFZ(0);
                        ZDBS->m_nDownTime_FCFZ_1++;
                        if(ZDBS->m_nDownTime_FCFZ_1>30)
                        {
                            ZDBS->setIsDown_FCFZ(2);
                            ZDBS->m_nDownTime_FCFZ_1=0;
                        }
                    }
                    else
                    {
                        ZDBS->setIsDown_FCFZ(2);
                    }
                }
                else if((state_ZDBS_2 & 0x02) == 0x00)
                {
                    if(ZDBS->getIsDown_FCFZ()==1)
                    {
                        //ZDBS->setIsDown_FCFZ(0);
                        ZDBS->m_nDownTime_FCFZ_1++;
                        if(ZDBS->m_nDownTime_FCFZ_1>30)
                        {
                            ZDBS->setIsDown_FCFZ(0);
                            ZDBS->m_nDownTime_FCFZ_1=0;
                        }
                    }
                    else
                    {
                        ZDBS->setIsDown_FCFZ(0);
                    }
                    //ZDBS->setIsDown_FCFZ(0);
                }
                if((state_ZDBS_2 & 0x04) == 0x04)
                {
//                    ZDBS->setIsDown_JCFZ(2);
                    if(ZDBS->getIsDown_JCFZ()==1)
                    {
                        //ZDBS->setIsDown_JCFZ(0);
                        ZDBS->m_nDownTime_JCFZ_1++;
                        if(ZDBS->m_nDownTime_JCFZ_1>30)
                        {
                            ZDBS->setIsDown_JCFZ(2);
                            ZDBS->m_nDownTime_JCFZ_1=0;
                        }
                    }
                    else
                    {
                        ZDBS->setIsDown_JCFZ(2);
                    }
                }
                else if((state_ZDBS_2 & 0x04) == 0x00)
                {
                    if(ZDBS->getIsDown_JCFZ()==1)
                    {
                        //ZDBS->setIsDown_JCFZ(0);
                        ZDBS->m_nDownTime_JCFZ_1++;
                        if(ZDBS->m_nDownTime_JCFZ_1>30)
                        {
                            ZDBS->setIsDown_JCFZ(0);
                            ZDBS->m_nDownTime_JCFZ_1=0;
                        }
                    }
                    else
                    {
                        ZDBS->setIsDown_JCFZ(0);
                    }
                    //ZDBS->setIsDown_JCFZ(0);
                }
                if((state_ZDBS_2 & 0x08) == 0x08)
                {
//                    ZDBS->setIsDown_GF(2);
                    if(ZDBS->getIsDown_GF()==1)
                    {
                        //ZDBS->setIsDown_GF(0);
                        ZDBS->m_nDownTime_GF_1++;
                        if(ZDBS->m_nDownTime_GF_1>30)
                        {
                            ZDBS->setIsDown_GF(2);
                            ZDBS->m_nDownTime_GF_1=0;
                        }
                    }
                    else
                    {
                        ZDBS->setIsDown_GF(2);
                    }
                }
                else if((state_ZDBS_2 & 0x08) == 0x00)
                {
                    if(ZDBS->getIsDown_GF()==1)
                    {
                        //ZDBS->setIsDown_GF(0);
                        ZDBS->m_nDownTime_GF_1++;
                        if(ZDBS->m_nDownTime_GF_1>30)
                        {
                            ZDBS->setIsDown_GF(0);
                            ZDBS->m_nDownTime_GF_1=0;
                        }
                    }
                    else
                    {
                        ZDBS->setIsDown_GF(0);
                    }
                    //ZDBS->setIsDown_GF(0);
                }

                if((state_ZDBS_2 & 0x70) == 0x10)
                {
                    ZDBS->setArrowStateJC(Arrow_Yellow);
                }
                else if((state_ZDBS_2 & 0x70) == 0x20)
                {
                    ZDBS->setArrowStateJC(Arrow_Red);
                }
                else
                {
                    ZDBS->setArrowStateJC(Arrow_Black);
                }

                if((state_ZDBS_2 & 0x70) == 0x40)
                {
                    ZDBS->setArrowStateFC(Arrow_Green);
                }
                else if((state_ZDBS_2 & 0x70) == 0x50)
                {
                    ZDBS->setArrowStateFC(Arrow_Red);
                }
                else
                {
                    ZDBS->setArrowStateFC(Arrow_Black);
                }

                //第3字节

                //第4字节
                ZDBS->QJDColorType = (state_ZDBS_3 & 0x0F);
                ZDBS->setIsBSDLight_QJ(ZDBS->QJDColorType);

                ZDBS->FZDColorType = (state_ZDBS_3 & 0xF0)>>4;
                ZDBS->setIsBSDLight_FZ(ZDBS->FZDColorType);

                ZDBS->SGDColorType = (state_ZDBS_4 & 0x0F);
                ZDBS->setIsBSDLight_SG(ZDBS->SGDColorType);

                ZDBS->YXFCDColorType = (state_ZDBS_4 & 0xF0)>>4;
                ZDBS->setIsBSDLight_YXFC(ZDBS->YXFCDColorType);

                //第5字节
                state_ZDBS_5;//区间逻辑检查灯尚未完成
            }
        }
    }
}
//解析半自动闭塞状态
void MyStation::updateBZDBS_StateOfLS(int state_BZDBS_0,int state_BZDBS_1,int state_BZDBS_2,int state_BZDBS_3)
{
    //第0字节
    //第1字节
    int glXHNum;
    glXHNum = (state_BZDBS_0) | (state_BZDBS_1<<8);

    CBZDBS *BZDBS;
    for(int i=0;i<BZDBSConfigNum;i++)
    {
        if(BZDBSArray[i]->getDevType()==Dev_BZDBS)
        {
            BZDBS = (CBZDBS *)BZDBSArray[i];
            if(BZDBS->getCode() == glXHNum)
            {
                //第2字节
                if((state_BZDBS_2 & 0x0F) == 0x00)
                {
                    BZDBS->setArrowStateJC(Arrow_Gray);
                }
                else if((state_BZDBS_2 & 0x0F) == 0x01)
                {
                    BZDBS->setArrowStateJC(Arrow_Yellow);
                }
                else if((state_BZDBS_2 & 0x0F) == 0x02)
                {
                    BZDBS->setArrowStateJC(Arrow_Green);
                }
                else if((state_BZDBS_2 & 0x0F) == 0x04)
                {
                    BZDBS->setArrowStateJC(Arrow_Red);
                }
                if((state_BZDBS_2 & 0xF0)>>4 == 0x00)
                {
                    BZDBS->setArrowStateFC(Arrow_Gray);
                }
                else if((state_BZDBS_2 & 0xF0)>>4 == 0x01)
                {
                    BZDBS->setArrowStateFC(Arrow_Yellow);
                }
                else if((state_BZDBS_2 & 0xF0)>>4 == 0x02)
                {
                    BZDBS->setArrowStateFC(Arrow_Green);
                }
                else if((state_BZDBS_2 & 0xF0)>>4 == 0x04)
                {
                    BZDBS->setArrowStateFC(Arrow_Red);
                }
                //第3字节预留

            }
        }
    }
}

//解析计轴状态
void MyStation::updateJZ_StateOfLS(int state_JZ_0,int state_JZ_1,int state_JZ_2,int state_JZ_3)
{
    //第0字节
    //第1字节
    int glXHNum;
    glXHNum = (state_JZ_0) | (state_JZ_1<<8);

    CJZ *JZ;
    for(int i=0;i<JZConfigNum;i++)
    {
        if(JZArray[i]->getDevType()==Dev_JZ)
        {
            JZ = (CJZ *)JZArray[i];
            if(JZ->getCode() == glXHNum)
            {
                //第2字节
                if((state_JZ_2 & 0x01) == 0x01)
                {
                    if(JZ->getIsDown_JZFL()==1)
                    {
                        JZ->m_nDownTime_JZFL_1++;
                        if(JZ->m_nDownTime_JZFL_1>30)
                        {
                            JZ->setIsDown_JZFL(2);
                            JZ->m_nDownTime_JZFL_1=0;
                        }
                    }
                    else
                    {
                        JZ->setIsDown_JZFL(2);
                    }
                }
                else if((state_JZ_2 & 0x01) == 0x00)
                {
                    if(JZ->getIsDown_JZFL()==1)
                    {
                        JZ->m_nDownTime_JZFL_1++;
                        if(JZ->m_nDownTime_JZFL_1>30)
                        {
                            JZ->setIsDown_JZFL(0);
                            JZ->m_nDownTime_JZFL_1=0;
                        }
                    }
                    else
                    {
                        JZ->setIsDown_JZFL(0);
                    }
                }

                if((state_JZ_2 & 0x02) == 0x02)
                {
                    if(JZ->getIsDown_JZSY()==1)
                    {
                        JZ->m_nDownTime_JZSY_1++;
                        if(JZ->m_nDownTime_JZSY_1>30)
                        {
                            JZ->setIsDown_JZSY(2);
                            JZ->m_nDownTime_JZSY_1=0;
                        }
                    }
                    else
                    {
                        JZ->setIsDown_JZSY(2);
                    }
                }
                else if((state_JZ_2 & 0x02) == 0x00)
                {
                    if(JZ->getIsDown_JZSY()==1)
                    {
                        JZ->m_nDownTime_JZSY_1++;
                        if(JZ->m_nDownTime_JZSY_1>30)
                        {
                            JZ->setIsDown_JZSY(0);
                            JZ->m_nDownTime_JZSY_1=0;
                        }
                    }
                    else
                    {
                        JZ->setIsDown_JZSY(0);
                    }
                }

                if((state_JZ_2 & 0x04) == 0x04)
                {
                    if(JZ->getIsDown_JZTY()==1)
                    {
                        JZ->m_nDownTime_JZTY_1++;
                        if(JZ->m_nDownTime_JZTY_1>30)
                        {
                            JZ->setIsDown_JZTY(2);
                            JZ->m_nDownTime_JZTY_1=0;
                        }
                    }
                    else
                    {
                        JZ->setIsDown_JZTY(2);
                    }
                }
                else if((state_JZ_2 & 0x04) == 0x00)
                {
                    if(JZ->getIsDown_JZTY()==1)
                    {
                        JZ->m_nDownTime_JZTY_1++;
                        if(JZ->m_nDownTime_JZTY_1>30)
                        {
                            JZ->setIsDown_JZTY(0);
                            JZ->m_nDownTime_JZTY_1=0;
                        }
                    }
                    else
                    {
                        JZ->setIsDown_JZTY(0);
                    }
                }

                if((state_JZ_2 & 0x08) == 0x08)
                {
                    JZ->setIsBSDLight_JZBJ(true);
                }
                else if((state_JZ_2 & 0x08) == 0x00)
                {
                    JZ->setIsBSDLight_JZBJ(false);
                }

                if((state_JZ_2 & 0x10) == 0x10)
                {
                    JZ->setIsBSDLight_JZFL(true);
                }
                else if((state_JZ_2 & 0x10) == 0x00)
                {
                    JZ->setIsBSDLight_JZFL(false);
                }
                if((state_JZ_2 & 0x20) == 0x20)
                {
                    JZ->setIsBSDLight_JZSY(true);
                }
                else if((state_JZ_2 & 0x20) == 0x00)
                {
                    JZ->setIsBSDLight_JZSY(false);
                }
                if((state_JZ_2 & 0x40) == 0x40)
                {
                    JZ->setIsBSDLight_JZTY(true);
                }
                else if((state_JZ_2 & 0x40) == 0x00)
                {
                    JZ->setIsBSDLight_JZTY(false);
                }
                if((state_JZ_2 & 0x80) == 0x80)
                {
                    JZ->setIsBSDLight_QJ(true);
                }
                else if((state_JZ_2 & 0x80) == 0x00)
                {
                    JZ->setIsBSDLight_QJ(false);
                }
                if((state_JZ_3 & 0x01) == 0x01)
                {
                    //预留
                }
                else if((state_JZ_3 & 0x01) == 0x00)
                {
                    //预留
                }
            }
        }
    }
}

//场联状态
void MyStation::updateCL_StateOfLS(int state_CL_0,int state_CL_1,int state_CL_2,int state_CL_3)
{
    //第0字节
    //第1字节
    int glXHNum;
    glXHNum = (state_CL_0) | (state_CL_1<<8);

    cl *CL;
    for(int i=0;i<CLConfigNum;i++)
    {
        if(CLArray[i]->getDevType()==Dev_CL)
        {
            CL = (cl *)CLArray[i];
            if(CL->getCode() == glXHNum)
            {
                CL->m_nArrowStateJC = 0x00;
                CL->m_nArrowStateFC = 0x00;
                //第2字节
                if((state_CL_2 & 0x70) == 0x00)
                {
                    CL->m_nArrowStateJC=(Arrow_Black);
                }
                else if((state_CL_2 & 0x70) == 0x10)
                {
                    CL->m_nArrowStateJC=(Arrow_Red);
                }
                else if((state_CL_2 & 0x70) == 0x20)
                {
                    CL->m_nArrowStateJC=(Arrow_Green);
                }
                else if((state_CL_2 & 0x70) == 0x30)
                {
                    CL->m_nArrowStateJC=(Arrow_White);
                }
//                else if((state_CL_2 & 0x70) == 0x40)
//                {
//                    CL->m_nArrowStateFC=(Arrow_Black);
//                }
//                else if((state_CL_2 & 0x70) == 0x50)
//                {
//                    CL->m_nArrowStateFC=(Arrow_Red);
//                }
//                else if((state_CL_2 & 0x70) == 0x60)
//                {
//                    CL->m_nArrowStateFC=(Arrow_Green);
//                }
//                else if((state_CL_2 & 0x70) == 0x70)
//                {
//                    CL->m_nArrowStateFC=(Arrow_White);
//                }
                else if((state_CL_2 & 0x70) == 0x40)
                {
                    CL->m_nArrowStateJC=(Arrow_Black);
                }
                else if((state_CL_2 & 0x70) == 0x50)
                {
                    CL->m_nArrowStateJC=(Arrow_Red);
                }
                else if((state_CL_2 & 0x70) == 0x60)
                {
                    CL->m_nArrowStateJC=(Arrow_Green);
                }
                else if((state_CL_2 & 0x70) == 0x70)
                {
                    CL->m_nArrowStateJC=(Arrow_White);
                }

                //监督区间灯JGJ
                if((state_CL_2 & 0x80) == 0x80)
                {
                    CL->JGJColorType = 0x01;
                }
                else
                {
                    CL->JGJColorType = 0;//灭灯
                }

                CL->CFJColorType = state_CL_2 & 0x0F;
                CL->AFJColorType = state_CL_3 & 0x0F;//指示灯的状态
                CL->BFJColorType = (state_CL_3 & 0xF0)>>4;//指示灯的状态
            }
        }
    }
}

//解析指示灯状态
void MyStation::updatelamp_StateOfLS(lamp *lamp_Temp)
{
    lamp_Temp->FCZKMode_lamp = m_nFCZKMode;//非常站控
    lamp_Temp->nModeState = m_nModeState;//模式转换//0为中心控制,1为车站控制,2为车站调车
    lamp_Temp->nStateSelect = nStateSelect;//按图排路
    lamp_Temp->nPlanCtrl = nPlanCtrl;//计划控制
    lamp_Temp->m_nComCTCCenter = m_nComCTCCenter;//中心通信
    lamp_Temp->m_nComZLJState;//自律机通信
    lamp_Temp->m_bAllowZH = m_bAllowZH;//允许回转
    lamp_Temp->m_nComLKState;//列控通信
    lamp_Temp->m_nX180 = m_nX180;//延迟解锁3分钟 吸起计时3分钟后落下
    lamp_Temp->m_nX30s = m_nX30s;//延迟解锁30钟 吸起计时30钟后落下
    lamp_Temp->m_nS180 = m_nS180;//延迟解锁3分钟 吸起计时3分钟后落下
    lamp_Temp->m_nS30s = m_nS30s;//延迟解锁30钟 吸起计时3分钟后落下
    lamp_Temp->m_nX240 = m_nX240;//延迟解锁4分钟 吸起计时3分钟后落下
    lamp_Temp->m_nX60s = m_nX60s;//延迟解锁60s 吸起计时30钟后落下
    lamp_Temp->m_nS240 = m_nS240;//延迟解锁4分钟 吸起计时3分钟后落下
    lamp_Temp->m_nS60s = m_nS60s;//延迟解锁60s 吸起计时3分钟后落下
    //lamp_Temp->PD180s;
    lamp_Temp->m_nJWDState;//机务段状态灯

    //区间灯
    CGD *pGD=nullptr;
    if(lamp_Temp->DEVName_DJGD[0]=="DISPLAYLAMP_QJ")
    {
        for(int j=0;j<lamp_Temp->DEVName_DJGD.size();j++)
        {
            for (int i = 0; i < DevArray.size(); i++)
            {
                if(DevArray[i]->getDevType() == Dev_GD)
                {
                    pGD = (CGD*)(DevArray[i]);
                    if(pGD->m_strName==lamp_Temp->DEVName_DJGD[j])
                    {
                        if(!(pGD->getState(QDKX)))
//                      if(pGD->getState(QDZY))
                        {
                            lamp_Temp->m_QJLampFlag = true;
                        }
                        else
                        {
                            lamp_Temp->m_QJLampFlag = false;
                        }
                    }
                }
            }
        }
    }

}
void MyStation::updateFUNCButton_StateOfLS(FUNCBUTTON *FUNC_Temp)
{
    if(FUNC_Temp->m_strType == "CALLONLOCK")
    {
        if(FUNC_Temp->Button_Name.left(1)=="S")
        {
            if(m_nSXYDZS == true)
            {
//                FUNC_Temp->setIsDown_Button(2);
                if(FUNC_Temp->getIsDown_Button()==1)
                {
                    FUNC_Temp->m_nDownTime_1++;
                    if(FUNC_Temp->m_nDownTime_1>30)
                    {
                        FUNC_Temp->setIsDown_Button(2);
                        FUNC_Temp->m_nDownTime_1=0;
                    }
                }
                else
                {
                    FUNC_Temp->setIsDown_Button(2);
                }
            }
            else
            {
                if(FUNC_Temp->getIsDown_Button()==1)
                {
                    FUNC_Temp->m_nDownTime_1++;
                    if(FUNC_Temp->m_nDownTime_1>30)
                    {
                        FUNC_Temp->setIsDown_Button(0);
                        FUNC_Temp->m_nDownTime_1=0;
                    }
                }
                else
                {
                    FUNC_Temp->setIsDown_Button(0);
                }
                //FUNC_Temp->setIsDown_Button(0);
            }
        }
        else if(FUNC_Temp->Button_Name.left(1)=="X")
        {
            if(m_nXXYDZS == true)
            {
//                FUNC_Temp->setIsDown_Button(2);
                if(FUNC_Temp->getIsDown_Button()==1)
                {
                    FUNC_Temp->m_nDownTime_1++;
                    if(FUNC_Temp->m_nDownTime_1>30)
                    {
                        FUNC_Temp->setIsDown_Button(2);
                        FUNC_Temp->m_nDownTime_1=0;
                    }
                }
                else
                {
                    FUNC_Temp->setIsDown_Button(2);
                }
            }
            else
            {
                if(FUNC_Temp->getIsDown_Button()==1)
                {
                    FUNC_Temp->m_nDownTime_1++;
                    if(FUNC_Temp->m_nDownTime_1>30)
                    {
                        FUNC_Temp->setIsDown_Button(0);
                        FUNC_Temp->m_nDownTime_1=0;
                    }
                }
                else
                {
                    FUNC_Temp->setIsDown_Button(0);
                }
                //FUNC_Temp->setIsDown_Button(0);
            }
        }
        else if(FUNC_Temp->Button_Name.left(1)=="全")
        {
            if((m_nSXYDZS == true)&&(m_nXXYDZS == true))
            {
                //FUNC_Temp->setIsDown_Button(2);
                if(FUNC_Temp->getIsDown_Button()==1)
                {
                    FUNC_Temp->m_nDownTime_1++;
                    if(FUNC_Temp->m_nDownTime_1>30)
                    {
                        FUNC_Temp->setIsDown_Button(2);
                        FUNC_Temp->m_nDownTime_1=0;
                    }
                }
                else
                {
                    FUNC_Temp->setIsDown_Button(2);
                }
            }
            else /*if((m_nSXYDZS == false)&&(m_nXXYDZS == false))*/
            {
                if(FUNC_Temp->getIsDown_Button()==1)
                {
                    FUNC_Temp->m_nDownTime_1++;
                    if(FUNC_Temp->m_nDownTime_1>30)
                    {
                        FUNC_Temp->setIsDown_Button(0);
                        FUNC_Temp->m_nDownTime_1=0;
                    }
                }
                else
                {
                    FUNC_Temp->setIsDown_Button(0);
                }
//                FUNC_Temp->setIsDown_Button(0);
            }
        }
    }
    if(FUNC_Temp->m_strType == "SDJS")
    {
        if(m_bSDJS == true)
        {
//            FUNC_Temp->setIsDown_Button(2);
            if(FUNC_Temp->getIsDown_Button()==1)
            {
                FUNC_Temp->m_nDownTime_1++;
                if(FUNC_Temp->m_nDownTime_1>30)
                {
                    FUNC_Temp->setIsDown_Button(2);
                    FUNC_Temp->m_nDownTime_1=0;
                }
            }
            else
            {
                FUNC_Temp->setIsDown_Button(2);
            }
        }
        else
        {
            if(FUNC_Temp->getIsDown_Button()==1)
            {
                FUNC_Temp->m_nDownTime_1++;
                if(FUNC_Temp->m_nDownTime_1>30)
                {
                    FUNC_Temp->setIsDown_Button(0);
                    FUNC_Temp->m_nDownTime_1=0;
                }
            }
            else
            {
                FUNC_Temp->setIsDown_Button(0);
            }
            //FUNC_Temp->setIsDown_Button(0);
        }
    }
    if(FUNC_Temp->m_strType == "GZTZ")
    {
        if(m_bGZTZ == true)
        {
//            FUNC_Temp->setIsDown_Button(2);
            if(FUNC_Temp->getIsDown_Button()==1)
            {
                FUNC_Temp->m_nDownTime_1++;
                if(FUNC_Temp->m_nDownTime_1>30)
                {
                    FUNC_Temp->setIsDown_Button(2);
                    FUNC_Temp->m_nDownTime_1=0;
                }
            }
            else
            {
                FUNC_Temp->setIsDown_Button(2);
            }
        }
        else
        {
            if(FUNC_Temp->getIsDown_Button()==1)
            {
                FUNC_Temp->m_nDownTime_1++;
                if(FUNC_Temp->m_nDownTime_1>30)
                {
                    FUNC_Temp->setIsDown_Button(0);
                    FUNC_Temp->m_nDownTime_1=0;
                }
            }
            else
            {
                FUNC_Temp->setIsDown_Button(0);
            }
            //FUNC_Temp->setIsDown_Button(0);
        }
    }
}
//设置道岔故障按钮状态
void MyStation::setDCGZButtonState()
{
    CDcBtn *DCBUTTON=nullptr;
    CGDDC *pCGDDC=nullptr;
    for(int j=0;j<DCBtnArray.size();j++)
    {
        if(DCBtnArray[j]->getDevType() == Dev_DCBTN)
        {
            DCBUTTON=(CDcBtn*)(DCBtnArray[j]);
            if(DCBUTTON->m_tType == DCGZ_JG)
            {
                for(int k=0;k<DevArray.size();k++)
                {
                    if(DevArray[k]->getDevType() == Dev_DC)
                    {
                        pCGDDC=(CGDDC*)(DevArray[k]);
                        if (pCGDDC->m_nCode == DCBUTTON->m_nDcCode)
                        {
                            if(pCGDDC->getJGGZ())
                            {
                                DCBUTTON->setIsDown_Button(2);
                            }
                            else
                            {
                                if(DCBUTTON->getIsDown_Button()==1)
                                {
                                    DCBUTTON->m_nDownTime_1++;
                                    if(DCBUTTON->m_nDownTime_1>30)
                                    {
                                        DCBUTTON->setIsDown_Button(0);
                                        DCBUTTON->m_nDownTime_1=0;
                                    }
                                }
                                else
                                {
                                    DCBUTTON->setIsDown_Button(0);
                                }
                            }
                        }
                    }
                }
            }
            else if(DCBUTTON->m_tType == DCGZ_XG)
            {
                for(int k=0;k<DevArray.size();k++)
                {
                    if(DevArray[k]->getDevType() == Dev_DC)
                    {
                        pCGDDC=(CGDDC*)(DevArray[k]);
                        //if (pCGDDC->m_strName == DCBUTTON->m_strDcName)
                        if (pCGDDC->m_nCode == DCBUTTON->m_nDcCode)
                        {
                            if(pCGDDC->getXGGZ())
                            {
                                DCBUTTON->setIsDown_Button(2);
                            }
                            else
                            {
                                if(DCBUTTON->getIsDown_Button()==1)
                                {
                                    DCBUTTON->m_nDownTime_1++;
                                    if(DCBUTTON->m_nDownTime_1>30)
                                    {
                                        DCBUTTON->setIsDown_Button(0);
                                        DCBUTTON->m_nDownTime_1=0;
                                    }
                                }
                                else
                                {
                                    DCBUTTON->setIsDown_Button(0);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
void MyStation::setQD_Color(CQD *qd,QColor color)
{
    if(qd==NULL)
    {
        return;
    }
    for(int i=0;i<qd->getChildNum();i++)
    {
        for(int j=0;j<DevArray.size();j++)
        {
            if(qd->getChild(i) == (int)DevArray[j]->getCode())
            {
                if(DevArray[j]->getDevType() == Dev_DC)
                {
                    CGDDC *gddc;
                    gddc=(CGDDC*)(DevArray[j]);
                    gddc->setQDColor(color);
                    if(qd->getState(QDZY))
                    {
                       gddc->setQDState(QDZY);
                    }
                    else if(qd->getState(QDSB))
                    {
                       gddc->setQDState(QDSB);
                    }
                    else if(qd->getState(QDKX))
                    {
                       gddc->setQDState(QDKX);
                    }
                    break;
                }
                else if(DevArray[j]->getDevType() == Dev_GD)
                {
                    CGD *gd;
                    gd=(CGD*)(DevArray[j]);
                    gd->setQDColor(color);
                    if(qd->getState(QDZY))
                    {
                       gd->setState(QDZY);
                    }
                    else if(qd->getState(QDSB))
                    {
                       gd->setState(QDSB);
                    }
                    else if(qd->getState(QDKX))
                    {
                       gd->setState(QDKX);
                    }
                    break;
                }
            }
        }
    }
}
//接收服务端数据-列车位置信息
void MyStation::recvServerData_TrainPos(unsigned char *recvArray, int recvlen)
{
    Train* pTrainTemp = new Train;
    int nCount=10;
    //车次号长度
    int nNamelength=recvArray[nCount];
    nCount += 1;
    //车次号
    QString strTrainNum="";
    unsigned char strCheCiTemp[21];
    memset(strCheCiTemp,0,sizeof(strCheCiTemp));
    memcpy(strCheCiTemp, &recvArray[11], nNamelength);
    strTrainNum = UnsignedCharArrayToString(strCheCiTemp);
    pTrainTemp->m_strCheCi = strTrainNum;
    nCount += nNamelength;

    //所在股道信号设备号//显示位置
    //int nCode = (int)(recvArray[nCount] | recvArray[nCount+1]<<8);
    pTrainTemp->m_nPosCode = (int)(recvArray[nCount] | recvArray[nCount+1]<<8);
    nCount += 2;
    //实时位置
    //int nCodeReal = nCode;
    //int nCodeReal = (int)(recvArray[nCount] | recvArray[nCount+1]<<8);
    pTrainTemp->m_nPosCodeReal = (int)(recvArray[nCount] | recvArray[nCount+1]<<8);
    nCount += 2;

    //左行0x11  右行0x22 如不能提供该信息则添0x00
    bool bRight;
    if(0x22 == (int)recvArray[nCount])
    {
        bRight = true;//右行
        //checi.bRight = TRUE;//右行
    }
    else
    {
        bRight = false;//左行
        //checi.bRight = FALSE;//左行
    }
    pTrainTemp->m_bRight = bRight;
    nCount += 1;

    //列车是否丢失 0x01 丢失故障 0x00 正常
    bool bLost;
    if((int)recvArray[nCount] == 0x01)
    {
        bLost = true;//车次丢失
        //checi.bLost = TRUE;//车次丢失
    }
    else
    {
        bLost = false;//正常
        //checi.bLost = FALSE;//正常
    }
    pTrainTemp->m_bLost = bLost;
    nCount += 1;

    //车次类型 0x02列车 0x03调车
    int trainType = recvArray[nCount];
    //checi.trainType = StatusArray[nCount];
    pTrainTemp->m_nType = trainType;
    nCount += 1;

    //列车类型 客车、货车
    int LCType;
    if((int)recvArray[nCount] == 0x01)
    {
        LCType = LCTYPE_KC;
        //checi.LCType = LCTYPE_KC;
    }
    else
    {
        LCType = LCTYPE_HC;
        //checi.LCType = LCTYPE_HC;
    }
    pTrainTemp->m_nKHType = LCType;
    nCount += 1;

    //列车是否正在运行
    bool bRunning;
    if((int)recvArray[nCount] == 0x01)
    {
        bRunning = true;
        //checi.bRunning = TRUE;
    }
    else
    {
        bRunning = false;
        //checi.bRunning = FALSE;
    }
    pTrainTemp->m_bRunning = bRunning;
    nCount += 1;

    //电力机车
    bool bElectric;
    if((int)recvArray[nCount] == 0x11)
    {
        bElectric = true;
        //checi.bElectric = TRUE;
    }
    else
    {
        bElectric = false;
        //checi.bElectric = FALSE;
    }
    pTrainTemp->m_bElectric = bElectric;
    nCount += 1;

    //超限
    int nLevelCX = (int)recvArray[nCount];
    //checi.nLevelCX = StatusArray[nCount];
    pTrainTemp->m_nLevelCX = nLevelCX;
    nCount += 1;

    //编组长度
    int nTrainLengh = (int)recvArray[nCount];
    //checi.nTrainLengh = StatusArray[nCount];
    pTrainTemp->m_nLengh = nTrainLengh;
    nCount += 1;

    //速度（预设）
    int nSpeed = (int)(recvArray[nCount] | recvArray[nCount+1]<<8);
    //checi.nSpeed = StatusArray[nCount];
    pTrainTemp->m_nSpeed = nSpeed;
    nCount += 2;

    //列车类型序号（管内动车组、通勤列车等）
    int nIndexLCLX = (int)recvArray[nCount];
    pTrainTemp->m_nIndexLCLX = nIndexLCLX;
    nCount += 1;

    //列车停稳
    if((int)recvArray[nCount] == 0x01)
    {
        pTrainTemp->m_bStop = true;
    }
    else
    {
        pTrainTemp->m_bStop = false;
    }
    nCount += 1;

    //人工标记车次
    if((int)recvArray[nCount] == 0x01)
    {
        pTrainTemp->m_bManType = true;
    }
    else
    {
        pTrainTemp->m_bManType = false;
    }
    nCount += 1;

    //实际车次，包含早晚点
    if(!pTrainTemp->m_bManType)
    {
        //第0~14位为时间
        int overtime = (int)(recvArray[nCount] | ((recvArray[nCount+1]&0x7F)<<8));
        //是否早点,最高位第15位，0为晚点，1为早点
        bool bEarly = (int)(recvArray[nCount+1]>>7) == 1 ? true: false;
        if(bEarly)
        {
            //小于0表示早点
            overtime = 0 - overtime;
        }
        pTrainTemp->m_nOvertime = overtime;
        nCount+=2;
    }

    this->SetCheCiInfo(pTrainTemp);
    delete pTrainTemp;
}
//设置车次信息  车次类型 正在运行 电力机车 超限 编组长度
void MyStation::SetCheCiInfo(Train* pTrainTemp)
{
    Train *pTrain;
    int count = m_ArrayTrain.size();
    int i = 0;
    for(i=0; i<count; i++)
    {
        pTrain = (Train *)m_ArrayTrain[i];
        if(pTrain->m_strCheCi == pTrainTemp->m_strCheCi)//修改车次属性
        {
            pTrain->m_nPosCode     = pTrainTemp->m_nPosCode;
            pTrain->m_nPosCodeReal = pTrainTemp->m_nPosCodeReal;
            pTrain->m_bRight       = pTrainTemp->m_bRight;
            pTrain->m_bLost        = pTrainTemp->m_bLost;
            pTrain->m_nType        = pTrainTemp->m_nType;
            pTrain->m_nKHType      = pTrainTemp->m_nKHType;
            pTrain->m_bRunning     = pTrainTemp->m_bRunning;
            pTrain->m_bElectric    = pTrainTemp->m_bElectric;
            pTrain->m_nLevelCX     = pTrainTemp->m_nLevelCX;
            pTrain->m_nLengh       = pTrainTemp->m_nLengh;
            pTrain->m_nSpeed       = pTrainTemp->m_nSpeed;
            pTrain->m_nIndexLCLX   = pTrainTemp->m_nIndexLCLX;
            pTrain->m_nOvertime    = pTrainTemp->m_nOvertime;
            pTrain->m_bManType     = pTrainTemp->m_bManType;
            pTrain->m_bStop     = pTrainTemp->m_bStop;
            pTrain->m_bActive = true;
            break;
        }
    }
    if(i>=count)//新增车次
    {
        pTrain = new Train;
        pTrain->m_strCheCi     = pTrainTemp->m_strCheCi;
        pTrain->m_nPosCode     = pTrainTemp->m_nPosCode;
        pTrain->m_nPosCodeReal = pTrainTemp->m_nPosCodeReal;
        pTrain->m_bRight       = pTrainTemp->m_bRight;
        pTrain->m_bLost        = pTrainTemp->m_bLost;
        pTrain->m_nType        = pTrainTemp->m_nType;
        pTrain->m_nKHType      = pTrainTemp->m_nKHType;
        pTrain->m_bRunning     = pTrainTemp->m_bRunning;
        pTrain->m_bElectric    = pTrainTemp->m_bElectric;
        pTrain->m_nLevelCX     = pTrainTemp->m_nLevelCX;
        pTrain->m_nLengh       = pTrainTemp->m_nLengh;
        pTrain->m_nSpeed       = pTrainTemp->m_nSpeed;
        pTrain->m_nIndexLCLX   = pTrainTemp->m_nIndexLCLX;
        pTrain->m_nOvertime    = pTrainTemp->m_nOvertime;
        pTrain->m_bManType     = pTrainTemp->m_bManType;
        pTrain->m_bActive  = true;
        m_ArrayTrain.append(pTrain);
    }
}
//设置区段车次
void MyStation::SetQDCheCi()
{
    //int ccCount = m_ArrayTrain.count();
    //int nArraySize =  DevArray.count();//获取站场大小
    CGD *pGD;
    CGDDC *pGDDC;
    Train *pTrain;

    for(int i=0; i< DevArray.count(); i++)
    {
        if(DevArray[i]->getDevType() == Dev_GD)
        {
            bool bCheCi = false;//有无车次
            pGD=(CGD *)(DevArray[i]);
            for(int j=0; j<m_ArrayTrain.size(); j++)
            {
                pTrain = (Train *)m_ArrayTrain[j];
                if(pTrain->m_nPosCode == (int)pGD->m_nCode)
                {
                    bCheCi = true;
                    pGD->m_strCheCiNum = pTrain->m_strCheCi;
                    pGD->m_nCheciLost  = pTrain->m_bLost;
                    pGD->m_bElectric   = pTrain->m_bElectric;
                    pGD->m_nSXCheCi    = pTrain->m_bRight;
                    pGD->m_nKHType     = pTrain->m_nKHType;
                    pGD->m_nSpeed     = pTrain->m_nSpeed;
                    pTrain->pCenterConst = GetPointInTrainNumWndArray(pTrain->m_nPosCode);
                    if(pTrain->pCenterConst.x()<0)
                    {
                        pTrain->pCenterConst = pGD->getCenterPt().toPoint();
                    }
                    //if(pGD->getGDType() == GD_QD && !pTrain->m_bRunning)
                    //仅在股道上判断停稳标志
                    if(this->DevIsGDByCode(pGD->getCode()) && !pTrain->m_bRunning)
                    {
                        pTrain->m_bStop = true;
                    }
                    pGD->m_bLCTW       = pTrain->m_bStop;
                    break;
                }
            }
            if(!bCheCi)//无车次
            {
                pGD->m_strCheCiNum = "";
                //pGD->m_nCheciLost  = FALSE;
                //pGD->m_bElectric   = FALSE;
                //pGD->m_nSXCheCi    = 0；
            }
        }
        //增加向道岔区段设置车次.lwm.2021.10.30
        if(DevArray[i]->getDevType() == Dev_DC)
        {
            bool bCheCi = false;//有无车次
            pGDDC=(CGDDC *)(DevArray[i]);
            for(int j=0; j<m_ArrayTrain.size(); j++)
            {
                pTrain = (Train *)m_ArrayTrain[j];
                if(pTrain->m_nPosCode == (int)pGDDC->getQDCode())
                {
                    pGDDC->m_strCheCiNum = pTrain->m_strCheCi;
                    pGDDC->m_nCheciLost  = pTrain->m_bLost;
                    pGDDC->m_bElectric   = pTrain->m_bElectric;
                    pGDDC->m_nSXCheCi    = pTrain->m_bRight;
                    pGDDC->m_nKHType     = pTrain->m_nKHType;
                    pGDDC->m_nSpeed     = pTrain->m_nSpeed;
                    bCheCi = true;
                    pTrain->pCenterConst = pGDDC->getCenterPt().toPoint();
                    pTrain->m_bStop = false;
                    pGDDC->m_bLCTW       = pTrain->m_bStop;
                    break;
                }
            }
            if(!bCheCi)//无车次
            {
                pGDDC->m_strCheCiNum = "";
                //pGD->m_nCheciLost  = FALSE;
                //pGD->m_bElectric   = FALSE;
                //pGD->m_nSXCheCi    = 0；
            }
        }
    }
}
//更新车次活跃信息
void MyStation::UpdateCheCiInfo()
{
    Train *pTrain;
    for(int i=0; i<m_ArrayTrain.size(); i++)
    {
        pTrain = (Train *)m_ArrayTrain[i];
        pTrain->m_bActive = false;
    }
}
//删除没有的车次
void MyStation::DeleteCheCi()
{
    Train *pTrain;
    for(int i=0; i<m_ArrayTrain.size(); i++)
    {
        pTrain = (Train *)m_ArrayTrain[i];
        if(!pTrain->m_bActive)
        {
            m_ArrayTrain.removeAt(i);
            DeleteCheCi();
            break;
        }
    }
}
//更新激活车次信息
void MyStation::UpdateStationCheCiInfo(int nElaps)
{
    if (nElaps % 2 == 0)
    {
        UpdateCheCiInfo();//更新车次信息
    }
    else
    {
        DeleteCheCi();//删除没有的车次
    }
    SetQDCheCi();//设置车次
}
//根据Code查找相应车次框的位置
QPoint MyStation::GetPointInTrainNumWndArray(int code)
{
    QPoint point = QPoint(-1000,-1000);
    for(int i=0; i<m_ArrayTrainNumWnd.size(); i++)
    {
        TrainNumWnd* pTrainNumWnd = m_ArrayTrainNumWnd[i];
        if(code == pTrainNumWnd->m_nPosCode)
        {
            return pTrainNumWnd->pCenterConst.toPoint();
        }
    }
    return point;
}
//车次操作接口方法
bool MyStation::OnMenuCheCiClicked(QString menuName, int gdCode, Train *pTrainTemp)
{
    QByteArray dataArray;
    int nCount = 10;
    //添加nCount个字节并全部置零
    dataArray.append(nCount, char(0));
    //标志位-车次信息
    dataArray[nCount-1] = 0xAC;

    if(menuName.contains("加车次号"))
    {
        //车次操作类型
        nCount++;
        dataArray.append(1, char(0));
        dataArray[nCount-1] = 0x01;//添加
        //车次号
        QByteArray byteArray = pTrainTemp->m_strCheCi.toLocal8Bit();
        int ccLen = byteArray.count();
        //车次号长度
        nCount++;
        dataArray.append(1, char(0));
        dataArray[nCount-1] = ccLen;
        //车次号内容
        nCount+=ccLen;
        dataArray.append(ccLen, char(0));
        for(int u=0; u<ccLen; u++)
        {
            dataArray[nCount-ccLen+u] = byteArray[u];
        }
        //单双数对应上下行
        if(dataArray[nCount-1]%2==0)
        {
            pTrainTemp->m_bRight=true;
        }
        else
        {
            pTrainTemp->m_bRight=false;
        }
        //所在区段设备号
        nCount+=2;
        dataArray.append(2, char(0));
        dataArray[nCount-2] = pTrainTemp->m_nPosCode&0xFF;
        dataArray[nCount-1] = (pTrainTemp->m_nPosCode>>8)&0xFF;
        //车次类型,列车/调车
        nCount++;
        dataArray.append(1, char(0));
        dataArray[nCount-1] = pTrainTemp->m_nType==LCMN?0x02:0x03;
        //方向
        nCount++;
        dataArray.append(1, char(0));
        dataArray[nCount-1] = pTrainTemp->m_bRight?0x5A:0xA5;
        //速度（预设）
        nCount+=2;
        dataArray.append(2, char(0));
        dataArray[nCount-2] = pTrainTemp->m_nSpeed&0xFF;
        dataArray[nCount-1] = (pTrainTemp->m_nSpeed<<8)&0xFF;
        //电力机车
        nCount++;
        dataArray.append(1, char(0));
        dataArray[nCount-1] = pTrainTemp->m_bElectric?0x11:0x00;
        //超限
        nCount++;
        dataArray.append(1, char(0));
        dataArray[nCount-1] = pTrainTemp->m_nLevelCX;
        //编组长度
        nCount++;
        dataArray.append(1, char(0));
        dataArray[nCount-1] = pTrainTemp->m_nLengh;
        //列车类型（客车/货车）
        nCount++;
        dataArray.append(1, char(0));
        dataArray[nCount-1] = pTrainTemp->m_nKHType==LCTYPE_KC?0x01:0x00;
        //是否模拟行车（是则向联锁加车）
        nCount++;
        dataArray.append(1, char(0));
        dataArray[nCount-1] = pTrainTemp->m_bManType?0x00:0x01;
    }
    else if(menuName.contains("删车次号"))
    {
        //车次操作类型
        nCount++;
        dataArray.append(1, char(0));
        dataArray[nCount-1] = 0x02;//添加
        //车次号
        QByteArray byteArray = pTrainTemp->m_strCheCi.toLocal8Bit();
        int ccLen = byteArray.count();
        //车次号长度
        nCount++;
        dataArray.append(1, char(0));
        dataArray[nCount-1] = ccLen;
        //车次号内容
        nCount+=ccLen;
        dataArray.append(ccLen, char(0));
        for(int u=0; u<ccLen; u++)
        {
            dataArray[nCount-ccLen+u] = byteArray[u];
        }
        //所在区段设备号
        nCount+=2;
        dataArray.append(2, char(0));
        dataArray[nCount-2] = pTrainTemp->m_nPosCode&0xFF;
        dataArray[nCount-1] = (pTrainTemp->m_nPosCode>>8)&0xFF;
    }
    else if(menuName.contains("车次号确认") || menuName.contains("修正车次号")|| menuName.contains("变更车次号"))
    {
        //车次操作类型
        nCount++;
        dataArray.append(1, char(0));
        dataArray[nCount-1] = 0x03;//添加
        //原车次号
        QByteArray byteArray = pTrainTemp->m_strCheCiOld.toLocal8Bit();
        int ccLen = byteArray.count();
        //原车次号长度
        nCount++;
        dataArray.append(1, char(0));
        dataArray[nCount-1] = ccLen;
        //原车次号内容
        nCount+=ccLen;
        dataArray.append(ccLen, char(0));
        for(int u=0; u<ccLen; u++)
        {
            dataArray[nCount-ccLen+u] = byteArray[u];
        }
        //所在区段设备号
        nCount+=2;
        dataArray.append(2, char(0));
        dataArray[nCount-2] = pTrainTemp->m_nPosCode&0xFF;
        dataArray[nCount-1] = (pTrainTemp->m_nPosCode>>8)&0xFF;
        //新车次号
        byteArray = pTrainTemp->m_strCheCi.toLocal8Bit();
        ccLen = byteArray.count();
        //新车次号长度
        nCount++;
        dataArray.append(1, char(0));
        dataArray[nCount-1] = ccLen;
        //新车次号内容
        nCount+=ccLen;
        dataArray.append(ccLen, char(0));
        for(int u=0; u<ccLen; u++)
        {
            dataArray[nCount-ccLen+u] = byteArray[u];
        }
    }
    else if(menuName.contains("改车次属性"))
    {
        //车次操作类型
        nCount++;
        dataArray.append(1, char(0));
        dataArray[nCount-1] = 0x04;//
        //车次号
        QByteArray byteArray = pTrainTemp->m_strCheCi.toLocal8Bit();
        int ccLen = byteArray.count();
        //车次号长度
        nCount++;
        dataArray.append(1, char(0));
        dataArray[nCount-1] = ccLen;
        //车次号内容
        nCount+=ccLen;
        dataArray.append(ccLen, char(0));
        for(int u=0; u<ccLen; u++)
        {
            dataArray[nCount-ccLen+u] = byteArray[u];
        }
        //所在区段设备号
        nCount+=2;
        dataArray.append(2, char(0));
        dataArray[nCount-2] = pTrainTemp->m_nPosCode&0xFF;
        dataArray[nCount-1] = (pTrainTemp->m_nPosCode>>8)&0xFF;
        //速度
        nCount+=2;
        dataArray.append(2, char(0));
        dataArray[nCount-2] = pTrainTemp->m_nSpeed&0xFF;
        dataArray[nCount-1] = (pTrainTemp->m_nSpeed<<8)&0xFF;
        //电力机车
        nCount++;
        dataArray.append(1, char(0));
        dataArray[nCount-1] = pTrainTemp->m_bElectric?0x11:0x00;
    }
    else if(menuName.contains("设置车次启动"))
    {
        //车次操作类型
        nCount++;
        dataArray.append(1, char(0));
        dataArray[nCount-1] = 0x05;//
        //车次号
        QByteArray byteArray = pTrainTemp->m_strCheCi.toLocal8Bit();
        int ccLen = byteArray.count();
        //车次号长度
        nCount++;
        dataArray.append(1, char(0));
        dataArray[nCount-1] = ccLen;
        //车次号内容
        nCount+=ccLen;
        dataArray.append(ccLen, char(0));
        for(int u=0; u<ccLen; u++)
        {
            dataArray[nCount-ccLen+u] = byteArray[u];
        }
        //所在区段设备号
        nCount+=2;
        dataArray.append(2, char(0));
        dataArray[nCount-2] = pTrainTemp->m_nPosCode&0xFF;
        dataArray[nCount-1] = (pTrainTemp->m_nPosCode>>8)&0xFF;
    }
    else if(menuName.contains("设置车次停稳"))
    {
        //车次操作类型
        nCount++;
        dataArray.append(1, char(0));
        dataArray[nCount-1] = 0x06;//
        //车次号
        QByteArray byteArray = pTrainTemp->m_strCheCi.toLocal8Bit();
        int ccLen = byteArray.count();
        //车次号长度
        nCount++;
        dataArray.append(1, char(0));
        dataArray[nCount-1] = ccLen;
        //车次号内容
        nCount+=ccLen;
        dataArray.append(ccLen, char(0));
        for(int u=0; u<ccLen; u++)
        {
            dataArray[nCount-ccLen+u] = byteArray[u];
        }
        //所在区段设备号
        nCount+=2;
        dataArray.append(2, char(0));
        dataArray[nCount-2] = pTrainTemp->m_nPosCode&0xFF;
        dataArray[nCount-1] = (pTrainTemp->m_nPosCode>>8)&0xFF;
    }

    //帧尾4+空白4
    nCount+=8;
    dataArray.append(8, char(0));
    //打包数据
    packHeadAndTail(&dataArray, nCount);
    //信号-发送数据给Server
    emit sendDataToCTCServerSignal(this, dataArray, nCount);

    return true;
}
//更新车次框的显示
void MyStation::UpdateTrainNumWnd()
{
    Train *pTrain;
    TrainNumWnd *pTrainNumWnd;
    for(int i=0; i<m_ArrayTrainNumWnd.size(); i++)
    {
        pTrainNumWnd = m_ArrayTrainNumWnd[i];
        bool bMatch = false;
        for(int j=0; j<m_ArrayTrain.size(); j++)
        {
            pTrain = m_ArrayTrain[j];
            //位置相同
            if(pTrainNumWnd->m_nPosCode == pTrain->m_nPosCode)
            {
                bMatch = true;
                pTrainNumWnd->m_bVisible = false;
                pTrainNumWnd->m_strCheCi = pTrain->m_strCheCi;
            }
        }
        if(!bMatch)
        {
            pTrainNumWnd->m_bVisible = true;
            pTrainNumWnd->m_strCheCi = "";
        }
    }
}
//根据车次获取列车信息
Train *MyStation::GetTrainByCheCi(QString _strCheci)
{
    Train *pTrain = nullptr;
    for(int i=0; i<m_ArrayTrain.size(); i++)
    {
        if(_strCheci == m_ArrayTrain[i]->m_strCheCi)
        {
            pTrain = m_ArrayTrain[i];
            break;
        }
    }
    return pTrain;
}
//设置车次窗显示隐藏
void MyStation::SetTrainNumWndShow(bool b)
{
    //车次框
    for(int i=0;i<m_ArrayTrainNumWnd.size();i++)
    {
        m_ArrayTrainNumWnd[i]->m_bShow=b;
    }
}
void MyStation::SetRoutePreWndShow(bool b)
{
    for(int i=0;i<vectRoutePreWnd.size();i++)
    {
        vectRoutePreWnd[i]->m_bVisible=b;
    }
}
void MyStation::SetRoutePreWndShow_m(bool b)
{
    for(int i=0;i<vectRoutePreWnd.size();i++)
    {
        vectRoutePreWnd[i]->m_bVisible_m=b;
    }
}
void MyStation::ResetState()
{
    CGDDC *pCGDDC=nullptr;
    CXHD *pXHD=nullptr;
    CGD *pGD=nullptr;
    FUNCBUTTON *FUN_Temp=nullptr;
    CZDBS *pZDBS=nullptr;
    CBZDBS *pBZDBS=nullptr;
    CJZ *pJZ=nullptr;
    CDcBtn *DCBUTTON=nullptr;

    for(int j=0;j<DevArray.size();j++)
    {
        if(DevArray[j]->getDevType() == Dev_XH)
        {
            pXHD=(CXHD*)(DevArray[j]);
            pXHD->setIsLCDown(0);
            pXHD->setIsDCDown(0);
            pXHD->m_nTgIsDown = 0;
            pXHD->setIsYDDown(0);
            pXHD->isLCANFlash = false;//终端闪烁
            pXHD->isDCANFlash = false;
            pXHD->isBTANFlash = false;
           // pXHD->bZDFlash = false;

        }
        if(DevArray[j]->getDevType() == Dev_GD)
        {
            pGD=(CGD*)(DevArray[j]);
            pGD->m_RangeVisible_XZ = false;

        }
    }
    //功能按钮
    for(int j=0;j<FuncBtnArray.size();j++)
    {
        if(FuncBtnArray[j]->getDevType() == Dev_FUNCB)
        {
            FUN_Temp = (FUNCBUTTON *)(FuncBtnArray[j]);
            if(FUN_Temp->getIsDown_Button()==1)
            {
                FUN_Temp->setIsDown_Button(0);
            }
        }
    }
    //自动闭塞
    for(int j=0;j<this->ZDBSArray.size();j++)
    {
        if(this->ZDBSArray[j]->getDevType() == Dev_ZDBS)
        {
            pZDBS = (CZDBS *)(this->ZDBSArray[j]);
            if(pZDBS->getIsDown_ZFZ()==1)
            {
                pZDBS->setIsDown_ZFZ(0);
            }
            if(pZDBS->getIsDown_FCFZ()==1)
            {
                pZDBS->setIsDown_FCFZ(0);
            }
            if(pZDBS->getIsDown_JCFZ()==1)
            {
                pZDBS->setIsDown_JCFZ(0);
            }
            if(pZDBS->getIsDown_GF()==1)
            {
                pZDBS->setIsDown_GF(0);
            }
        }
    }
    //半自动闭塞
    for(int j=0;j<this->BZDBSArray.size();j++)
    {
        if(this->BZDBSArray[j]->getDevType() == Dev_BZDBS)
        {
            pBZDBS = (CBZDBS *)(this->BZDBSArray[j]);
            if(pBZDBS->getIsDown_BS()==1)
            {
                pBZDBS->setIsDown_BS(0);
            }
            if(pBZDBS->getIsDown_SG()==1)
            {
                pBZDBS->setIsDown_SG(0);
            }
            if(pBZDBS->getIsDown_FY()==1)
            {
                pBZDBS->setIsDown_FY(0);
            }
            if(pBZDBS->getIsDown_BSQH()==1)
            {
                pBZDBS->setIsDown_BSQH(0);
            }
        }
    }
    //计轴
    for(int j=0;j<this->JZArray.size();j++)
    {
        if(this->JZArray[j]->getDevType() == Dev_JZ)
        {
            pJZ = (CJZ *)(this->JZArray[j]);
            if(pJZ->getIsDown_JZSY()==1)
            {
                pJZ->setIsDown_JZSY(0);
            }
            if(pJZ->getIsDown_JZTY()==1)
            {
                pJZ->setIsDown_JZTY(0);
            }
            if(pJZ->getIsDown_JZFL()==1)
            {
                pJZ->setIsDown_JZFL(0);
            }
            if(pJZ->getIsDown_BSQH()==1)
            {
                pJZ->setIsDown_BSQH(0);
            }
        }
    }
    //功能按钮 尖轨心轨
    for(int j=0;j<this->DCBtnArray.size();j++)
    {
        if(this->DCBtnArray[j]->getDevType() == Dev_DCBTN)
        {
            DCBUTTON=(CDcBtn*)(this->DCBtnArray[j]);
            if(DCBUTTON->getIsDown_Button() == 1)
            {
                DCBUTTON->setIsDown_Button(0);
            }


        }
    }
}
void MyStation::SetXHJANShow(int ntype,bool bshow)
{
    CXHD *pXHD=nullptr;
    CText *pText=nullptr;
    CGD *pGD=nullptr;
    CGDDC *pCGDDC=nullptr;
    CZDBS *pZDBS=nullptr;
    CBZDBS *pBZDBS=nullptr;
    lamp *plamp=nullptr;
    CTG *ptg=nullptr;
    FUNCBUTTON *pFUNCBUTTON=nullptr;
    CDcBtn *pDCBtnArray=nullptr;
    CJZ *pCJZ=nullptr;
    for(int j=0;j<DevArray.size();j++)
    {
        if(DevArray[j]->getDevType() == Dev_XH&&
                (ntype==0||ntype==1||ntype==2||ntype==3||ntype==4||ntype==5||ntype==6||ntype==18))
        {
            pXHD=(CXHD*)(DevArray[j]);
            if(ntype==0)//列车列车按钮
            {
                pXHD->m_LCXHDButtonShowFlag_BTN=bshow;
                pXHD->m_TGXHDButtonShowFlag_BTN=bshow;
                pXHD->m_YDXHDButtonShowFlag_BTN=bshow;

            }
            else if(ntype==1)//调车按钮
            {
                pXHD->m_DCXHDButtonShowFlag_BTN=bshow;
            }
            else if(ntype==2)//终端闪烁
            {
                pXHD->bZDFlash=bshow;
            }
            else if(ntype==3)//进站信号机文字显示
            {
                if(pXHD->getXHDType()==JZ_XHJ)
                {
                    pXHD->bXHJTextShow=bshow;
                }
            }
            else if(ntype==4)//出站信号机文字显示
            {
                if(pXHD->getXHDType()==CZ_XHJ)
                {
                    pXHD->bXHJTextShow=bshow;
                }
            }
            else if(ntype==5)//调车信号机文字显示
            {
                if(pXHD->getXHDType()==DC_XHJ)
                {
                    pXHD->bXHJTextShow=bshow;
                }
            }
            else if(ntype==6)//区间信号机文字显示
            {
                //TODO:暂时未找到区间信号机定义
//                if(pXHD->getXHDType()==CZ_XHJ)
//                {
//                    pXHD->bXHJTextShow=bshow;
//                }
            }
            else if(ntype==18)//调车信号机文字显示
            {
                pXHD->bTxtTGButtonshow=bshow;
            }
        }
         if(DevArray[j]->getDevType() == Dev_TEXT&&(ntype==7||ntype==8||ntype==14||ntype==18))
        {
            pText=(CText*)(DevArray[j]);
            if(ntype==7)//站名
            {
                pText->bStationNameShow=bshow;
              //  return;
            }
            if(ntype==8)//方向
            {
                pText->bdirectionShow=bshow;
            }
            else if(ntype==14)//道岔区段
            {
                pText->bDCQDTextShow=bshow;
            }
            else if(ntype==18)//一般文字
            {
                pText->bgeneralShow=bshow;
            }
        }
         if(DevArray[j]->getDevType() == Dev_GD&&(ntype==10||ntype==11||ntype==12))
        {
            pGD=(CGD*)(DevArray[j]);
            if(ntype==10) //股道
            {
                pGD->bTextShowGD=bshow;
            }
            else if(ntype==11) //无岔区段
            {
                pGD->bTextShowWCQD=bshow;
            }
            else if(ntype==12) //区间区段
            {
                pGD->bTextShowJJQD=bshow;
            }
        }
         if(DevArray[j]->getDevType() == Dev_DC&&(ntype==13))
        {
            pCGDDC=(CGDDC*)(DevArray[j]);
            if(ntype==13) //道岔
            {
                pCGDDC->bDCTextShow=bshow;
            }
        }
    }
    for(int j=0;j<ZDBSArray.size();j++)
    {
        if(ZDBSArray[j]->getDevType() == Dev_ZDBS)
        {
            pZDBS = (CZDBS *)(ZDBSArray[j]);
            if(ntype==16) //报警灯
            {
                pZDBS->bShowhave=bshow;
            }
            if(ntype==8) //方向
            {
                pZDBS->haveWord=bshow;
            }
            if(ntype==19) //按钮
            {
                pZDBS->bBtnNameShow=bshow;
            }
            if(ntype==20) //计数器
            {
                pZDBS->bShowQFJS=bshow;
            }
        }
    }
    for(int j=0;j<BZDBSArray.size();j++)
    {
        if(this->BZDBSArray[j]->getDevType() == Dev_BZDBS)
        {
            pBZDBS = (CBZDBS *)(BZDBSArray[j]);
            if(ntype==19) //按钮
            {
                pBZDBS->bBtnNameShow=bshow;
            }
            if(ntype==8) //方向
            {
                pBZDBS->haveWord=bshow;
            }
            if(ntype==20) //计数器
            {
                pBZDBS->bShowQFJS=bshow;
            }
        }
    }
    for(int j=0;j<JZArray.size();j++)
    {
        if(this->JZArray[j]->getDevType() == Dev_JZ)
        {
            pCJZ = (CJZ *)(JZArray[j]);
            if(ntype==19) //按钮
            {
                pCJZ->bBtnNameShow=bshow;
            }
            if(ntype==20) //计数器
            {
                pCJZ->bShowQFJS=bshow;
            }
        }
    }
    for(int j=0;j<LampArray.size();j++)
    {
        plamp=(lamp *)(LampArray[j]);
        if(ntype==16) //报警灯
        {
            plamp->blampshowbjtext=bshow;
        }
    }
    //FuncBtnArray
    for(int j=0;j<FuncBtnArray.size();j++)
    {
        pFUNCBUTTON=(FUNCBUTTON *)(FuncBtnArray[j]);
        if(ntype==19) //报警灯
        {
            pFUNCBUTTON->bBtnNameShow=bshow;
        }
        if(ntype==20) //计数器
        {
            pFUNCBUTTON->bShowQFJS=bshow;
        }
    }
    for(int j=0;j<DCBtnArray.size();j++)
    {
        pDCBtnArray=(CDcBtn *)(DCBtnArray[j]);
        if(ntype==19) //报警灯
        {
            pDCBtnArray->bBtnNameShow=bshow;
        }
        if(ntype==20) //计数器
        {
            pDCBtnArray->bShowQFJS=bshow;
        }
    }
    //
    //进路预告窗
    if(ntype==9)
    {
       //SetRoutePreWndShow(bshow);
       SetRoutePreWndShow_m(bshow);
    }

}
void MyStation::SetDevMouseMoveShow(QPoint mcoursePoint,CBaseData *mCBaseDataOld)
{
    CGDDC *pCGDDC=nullptr;
    CXHD *pXHD=nullptr;
    CGD *pGD=nullptr;
    lamp *pLamp=nullptr;
    for(int j=0;j<DevArray.size();j++)
    {
        if(DevArray[j]->getDevType() == Dev_DC)
        {
            pCGDDC=(CGDDC*)(DevArray[j]);
            if(pCGDDC->GetMousePoint(mcoursePoint))
            {
                if(mCBaseDataOld!=nullptr)
                {
                    mCBaseDataOld->m_RangeVisible_XHD=false;
                    mCBaseDataOld->m_RangeVisible_LCAN=false;
                    mCBaseDataOld->m_RangeVisible_TGAN=false;
                    mCBaseDataOld->m_RangeVisible_DCAN=false;
                    mCBaseDataOld->m_RangeVisible_Name=false;
                    mCBaseDataOld->m_RangeVisible_XHD_DDMD=false;
                    mCBaseDataOld->m_RangeVisible_XHD_YDAN=false;
                    mCBaseDataOld->m_RangeVisible=false;
                }
                mCBaseDataOld=DevArray[j];
                pCGDDC->m_RangeVisible = true;
                return;
            }
            else
            {
                pCGDDC->m_RangeVisible = false;
            }
        }
        else if(DevArray[j]->getDevType() == Dev_XH)
        {
            pXHD=(CXHD*)(DevArray[j]);
//            if(pXHD->GetMousePoint(mcoursePoint))
//            {
//                if(mCBaseDataOld!=nullptr)
//                {
//                    mCBaseDataOld->m_RangeVisible=false;
//                }
//                mCBaseDataOld=DevArray[j];
//                pXHD->m_RangeVisible = true;
//                return;
//            }
//            else
//            {
//                pXHD->m_RangeVisible = false;
//            }

            if(pXHD->GetMousePoint_XHD(mcoursePoint))
            {
                if(mCBaseDataOld!=nullptr)
                {
                    mCBaseDataOld->m_RangeVisible_XHD=false;
                    mCBaseDataOld->m_RangeVisible_LCAN=false;
                    mCBaseDataOld->m_RangeVisible_TGAN=false;
                    mCBaseDataOld->m_RangeVisible_DCAN=false;
                    mCBaseDataOld->m_RangeVisible_Name=false;
                    mCBaseDataOld->m_RangeVisible_XHD_DDMD=false;
                    mCBaseDataOld->m_RangeVisible_XHD_YDAN=false;
                    mCBaseDataOld->m_RangeVisible=false;
                }
                mCBaseDataOld=DevArray[j];
                pXHD->m_RangeVisible_XHD = true;
                return;
            }
            else
            {
                pXHD->m_RangeVisible_XHD = false;
            }
            if(pXHD->GetMousePoint_DCAN(mcoursePoint))
            {
                if(mCBaseDataOld!=nullptr)
                {
                    mCBaseDataOld->m_RangeVisible_XHD=false;
                    mCBaseDataOld->m_RangeVisible_LCAN=false;
                    mCBaseDataOld->m_RangeVisible_TGAN=false;
                    mCBaseDataOld->m_RangeVisible_DCAN=false;
                    mCBaseDataOld->m_RangeVisible_Name=false;
                    mCBaseDataOld->m_RangeVisible_XHD_DDMD=false;
                    mCBaseDataOld->m_RangeVisible_XHD_YDAN=false;
                    mCBaseDataOld->m_RangeVisible=false;
                }
                mCBaseDataOld=DevArray[j];
                pXHD->m_RangeVisible_DCAN = true;
                return;
            }
            else
            {
                pXHD->m_RangeVisible_DCAN = false;
            }
            if(pXHD->GetMousePoint_LCAN(mcoursePoint))
            {
                if(mCBaseDataOld!=nullptr)
                {
                    mCBaseDataOld->m_RangeVisible_XHD=false;
                    mCBaseDataOld->m_RangeVisible_LCAN=false;
                    mCBaseDataOld->m_RangeVisible_TGAN=false;
                    mCBaseDataOld->m_RangeVisible_DCAN=false;
                    mCBaseDataOld->m_RangeVisible_Name=false;
                    mCBaseDataOld->m_RangeVisible_XHD_DDMD=false;
                    mCBaseDataOld->m_RangeVisible_XHD_YDAN=false;
                    mCBaseDataOld->m_RangeVisible=false;
                }
                mCBaseDataOld=DevArray[j];
                pXHD->m_RangeVisible_LCAN = true;
                return;
            }
            else
            {
                pXHD->m_RangeVisible_LCAN = false;
            }
            if(pXHD->GetMousePoint_TGAN(mcoursePoint))
            {
                if(mCBaseDataOld!=nullptr)
                {
                    mCBaseDataOld->m_RangeVisible_XHD=false;
                    mCBaseDataOld->m_RangeVisible_LCAN=false;
                    mCBaseDataOld->m_RangeVisible_TGAN=false;
                    mCBaseDataOld->m_RangeVisible_DCAN=false;
                    mCBaseDataOld->m_RangeVisible_Name=false;
                    mCBaseDataOld->m_RangeVisible_XHD_DDMD=false;
                    mCBaseDataOld->m_RangeVisible_XHD_YDAN=false;
                    mCBaseDataOld->m_RangeVisible=false;
                }
                mCBaseDataOld=DevArray[j];
                pXHD->m_RangeVisible_TGAN = true;
                return;
            }
            else
            {
                pXHD->m_RangeVisible_TGAN = false;
            }
            if(pXHD->GetMousePoint_Name(mcoursePoint))
            {
                if(mCBaseDataOld!=nullptr)
                {
                    mCBaseDataOld->m_RangeVisible_XHD=false;
                    mCBaseDataOld->m_RangeVisible_LCAN=false;
                    mCBaseDataOld->m_RangeVisible_TGAN=false;
                    mCBaseDataOld->m_RangeVisible_DCAN=false;
                    mCBaseDataOld->m_RangeVisible_Name=false;
                    mCBaseDataOld->m_RangeVisible_XHD_DDMD=false;
                    mCBaseDataOld->m_RangeVisible_XHD_YDAN=false;
                    mCBaseDataOld->m_RangeVisible=false;
                }
                mCBaseDataOld=DevArray[j];
                pXHD->m_RangeVisible_Name = true;
                return;
            }
            else
            {
                pXHD->m_RangeVisible_Name = false;
            }

        }
        else if(DevArray[j]->getDevType() == Dev_GD)
        {
            pGD=(CGD*)(DevArray[j]);
            if(pGD->GetMousePoint(mcoursePoint))
            {
                if(mCBaseDataOld!=nullptr)
                {
                    mCBaseDataOld->m_RangeVisible_XHD=false;
                    mCBaseDataOld->m_RangeVisible_LCAN=false;
                    mCBaseDataOld->m_RangeVisible_TGAN=false;
                    mCBaseDataOld->m_RangeVisible_DCAN=false;
                    mCBaseDataOld->m_RangeVisible_Name=false;
                    mCBaseDataOld->m_RangeVisible_XHD_DDMD=false;
                    mCBaseDataOld->m_RangeVisible_XHD_YDAN=false;
                    mCBaseDataOld->m_RangeVisible=false;
                }
                mCBaseDataOld=DevArray[j];
                pGD->m_RangeVisible = true;
                return;
            }
            else
            {
                pGD->m_RangeVisible = false;
            }
        }
    }
    for(int i=0;i<LampArray.size();i++)
    {
        if(LampArray[i]->getDevType() == Dev_LAMP)
        {
            pLamp = (lamp *)LampArray[i];
            if(pLamp->GetMousePoint(mcoursePoint))
            {
                if(mCBaseDataOld!=nullptr)
                {
                    mCBaseDataOld->m_RangeVisible_XHD=false;
                    mCBaseDataOld->m_RangeVisible_LCAN=false;
                    mCBaseDataOld->m_RangeVisible_TGAN=false;
                    mCBaseDataOld->m_RangeVisible_DCAN=false;
                    mCBaseDataOld->m_RangeVisible_Name=false;
                    mCBaseDataOld->m_RangeVisible_XHD_DDMD=false;
                    mCBaseDataOld->m_RangeVisible_XHD_YDAN=false;
                    mCBaseDataOld->m_RangeVisible=false;
                }
                mCBaseDataOld=LampArray[i];
                pLamp->m_RangeVisible = true;
                return;
            }
            else
            {
                pLamp->m_RangeVisible = false;
            }
        }
    }

}
//制作数据帧头帧尾和关键信息
void MyStation::packHeadAndTail(QByteArray *pDataArray, int length)
{
    //帧头4+分类码6+帧尾4
    if(length < 14)
        return;
    for(int i=0; i<4; i++)
    {
        //帧头
        pDataArray->data()[i] = 0xEF;
        //帧尾
        pDataArray->data()[length-i-1] = 0xFE;
    }
    //帧长度
    memcpy(&pDataArray->data()[4], &length, 2);
    //车站id
    pDataArray->data()[6] = this->getStationID();
    //岗位码
    pDataArray->data()[7] = this->ABNUM;
    //数据来源标记
    pDataArray->data()[8] = DATATYPE_BOARD;
}
//站场重置
void MyStation::ResetStationInfo()
{
    this->m_ArrayRouteOrder.clear();
    this->m_ArrayTrafficLog.clear();
    this->m_ArrayStagePlan.clear();
    //this->m_ArrayStagePlanChg.clear();
    this->m_ArrayTrain.clear();
    this->m_ArrayDisOrderRecv.clear();
    this->m_ArrayDisOrderDisp.clear();
    this->m_ArrayDisOrderLocomot.clear();
    this->vectTextAnnotation.clear();
    this->ClearAllGDAntiSlip();
    for(int i=0;i<m_ArrayGDData.size();i++)
    {
        m_ArrayGDData[i]->Init();
    }
    emit updateDisOrderSignal();
    this->ResetState();
    ResetStationDevStatus();
    ResetZoneData();
}
DispatchOrderStation* MyStation::GetDisOrderRecvByShow()
{
    DispatchOrderStation* pArrayDisOrderRecv=nullptr;
    int n=m_ArrayDisOrderRecv.size();
    if(n>0)
    {
        pArrayDisOrderRecv=m_ArrayDisOrderRecv[n-1];
    }
    return pArrayDisOrderRecv;
}
//更新进路预告窗信息
void MyStation::UpdateRoutePreWndInfo()
{
    for(int i=0; i<vectRoutePreWnd.size(); i++)
    {
        RoutePreWnd *pRoutePreWnd = vectRoutePreWnd[i];
        for(int j=0; j<vectRoutePreWndTemp.size(); j++)
        {
            RoutePreWnd *pRoutePreWndTemp = vectRoutePreWndTemp[j];
            //同一个进路窗
            if(pRoutePreWnd->juncXHDCode == pRoutePreWndTemp->juncXHDCode)
            {
                pRoutePreWnd->vectRouteInfo.clear();
                int count2 = pRoutePreWndTemp->vectRouteInfo.size();
                for(int k=0;k<count2;k++)
                {
                    RoutePreWnd::RouteInfo routeInfo;
                    routeInfo.route_id = pRoutePreWndTemp->vectRouteInfo[k].route_id;
                    routeInfo.CheCi = pRoutePreWndTemp->vectRouteInfo[k].CheCi;
                    routeInfo.GDName = pRoutePreWndTemp->vectRouteInfo[k].GDName;
                    routeInfo.m_nKHType = pRoutePreWndTemp->vectRouteInfo[k].m_nKHType;
                    routeInfo.routeType = pRoutePreWndTemp->vectRouteInfo[k].routeType;
                    routeInfo.routeState = pRoutePreWndTemp->vectRouteInfo[k].routeState;
                    pRoutePreWnd->vectRouteInfo.append(routeInfo);
                }
                break;
            }
        }
    }
    //清空临时数据
    vectRoutePreWndTemp.clear();
}

//根据id查找列车进路序列
TrainRouteOrder* MyStation::GetTrainRouteOrderById(int routeId)
{
    TrainRouteOrder* routeOrder = nullptr;
    for(int i=0; i<m_ArrayRouteOrder.size(); i++)
    {
        if(routeId == m_ArrayRouteOrder[i]->route_id)
        {
            routeOrder = m_ArrayRouteOrder[i];
            break;
        }
        if(m_ArrayRouteOrder[i]->m_bZHJL == 1)
        {
            for(int j=0; j<m_ArrayRouteOrder.at(i)->m_vecSonTrainRouteOrder.size(); j++)
            {
                if(routeId == m_ArrayRouteOrder.at(i)->m_vecSonTrainRouteOrder[j]->route_id)
                {
                    routeOrder = m_ArrayRouteOrder.at(i)->m_vecSonTrainRouteOrder[j];
                    break;
                }
            }
        }
    }
    return routeOrder;
}

//自动闭塞管理信号机是否是上行
bool MyStation::ZDBSisSX(QString xhdname)
{
    bool bresult = false;
    QString className;
    CXHD *pXHD;
    for(auto ement:DevArray)
    {
        if(ement->getDevType() == Dev_XH)
        {
            pXHD=(CXHD *)ement;
            if(xhdname == pXHD->m_strName)
            {
                if(pXHD->m_nSX)
                {
                    bresult = true;
                }
            }
        }
    }
    return bresult;
}
void MyStation::SetFCZKState(bool b)
{
    CGDDC *pCGDDC=nullptr;
    CXHD *pXHD=nullptr;
    CGD *pGD=nullptr;
    FUNCBUTTON *FUN_Temp=nullptr;
    CZDBS *pZDBS=nullptr;
    CBZDBS *pBZDBS=nullptr;
    for(int j=0;j<DevArray.size();j++)
    {
        if(DevArray[j]->getDevType() == Dev_XH)
        {
             pXHD=(CXHD*)(DevArray[j]);
             pXHD->bFCZKState=b;
        }
    }
    //功能按钮
    for(int j=0;j<FuncBtnArray.size();j++)
    {
        if(FuncBtnArray[j]->getDevType() == Dev_FUNCB)
        {
            FUN_Temp = (FUNCBUTTON *)(FuncBtnArray[j]);
            FUN_Temp->bFCZKState=b;
        }
    }
    //自动闭塞
    for(int j=0;j<this->ZDBSArray.size();j++)
    {
        if(this->ZDBSArray[j]->getDevType() == Dev_ZDBS)
        {
            pZDBS = (CZDBS *)(ZDBSArray[j]);
            pZDBS->bFCZKState=b;
        }
    }
    //半自动闭塞
    for(int j=0;j<this->BZDBSArray.size();j++)
    {
        if(this->BZDBSArray[j]->getDevType() == Dev_BZDBS)
        {
            pBZDBS = (CBZDBS *)(BZDBSArray[j]);
            pBZDBS->bFCZKState=b;
        }
    }
}
//void MyStation::setQD_Color(CQD *qd,QColor color)
//{
//    if(qd==NULL)
//    {
//        return;
//    }
//    for(int i=0;i<qd->getChildNum();i++)
//    {
//        for(int j=0;j<DevArray.size();j++)
//        {
//            if(qd->getChild(i) == (int)DevArray[j]->getCode())
//            {
//                if(DevArray[j]->getDevType() == Dev_DC)
//                {
//                    CGDDC *gddc;
//                    gddc=(CGDDC*)(DevArray[j]);
//                    gddc->setQDColor(color);
//                    if(qd->getState(QDZY))
//                    {
//                       gddc->setQDState(QDZY);
//                    }
//                    else if(qd->getState(QDSB))
//                    {
//                       gddc->setQDState(QDSB);
//                    }
//                    else if(qd->getState(QDKX))
//                    {
//                       gddc->setQDState(QDKX);
//                    }
//                    break;
//                }
//                else if(DevArray[j]->getDevType() == Dev_GD)
//                {
//                    CGD *gd;
//                    gd=(CGD*)(DevArray[j]);
//                    gd->setQDColor(color);
//                    if(qd->getState(QDZY))
//                    {
//                       gd->setState(QDZY);
//                    }
//                    else if(qd->getState(QDSB))
//                    {
//                       gd->setState(QDSB);
//                    }
//                    else if(qd->getState(QDKX))
//                    {
//                       gd->setState(QDKX);
//                    }
//                    break;
//                }
//            }
//        }
//    }
//}
//通过信号机名称获取信号机编号
int MyStation::GetXHCodeByName(QString strName)
{
    CXHD *pXHD=nullptr;
    for(auto ement:DevArray)
    {
        if(ement->getDevType() == Dev_XH)
        {
            pXHD=(CXHD *)ement;
            if(strName == pXHD->m_strName)
            {
                return pXHD->getCode();
            }
        }
    }
    return -1;
}
//通过股道code获取股道名称
QString MyStation::GetGDNameByCode(int ncode)
{
    CGD *pGD=nullptr;
    for(auto ement:DevArray)
    {
        if(ement->getDevType() == Dev_GD)
        {
            pGD=(CGD *)ement;
            if(ncode == pGD->getCode())
            {
                return pGD->m_strName;
            }
        }
    }
    return "";
}
//获取非常站控状态
int MyStation::GetFCZKMode()
{
    if(m_nFCZKMode)//非常站控
    {
        return 0x22;
    }
    else
    {
      return m_nModeState;
    }
}


//根据索引索取列车类型（管内动车组、通勤列车等几十种）
QString MyStation::GetTrainType(int index)
{
    int count = v_TrainNumType.count();
    if(index >=0 && index<count)
    {
        return v_TrainNumType[index]->strTypeName;
    }
    return "";
}
//根据名称获取列车类型索引
int MyStation::GetTrainTypeIndex(QString _strType)
{
    for(int i=0; i<v_TrainNumType.count(); i++)
    {
        if(_strType == v_TrainNumType[i]->strTypeName)
        {
            return i;
        }
    }
    return -1;
}
//根据索引索取列车运行类型（动车组、快速旅客列车等几种）
QString MyStation::GetTrainRunType(int index)
{
    int count = v_TrainRunType.count();
    if(index >=0 && index<count)
    {
        return v_TrainRunType[index];
    }
    return "";
}
//根据名称获取列车运行类型索引
int MyStation::GetTrainRunTypeIndex(QString _strRunType)
{
    for(int i=0; i<v_TrainRunType.count(); i++)
    {
        if(_strRunType == v_TrainRunType[i])
        {
            return i;
        }
    }
    return -1;
}
//根据索引索取超限级别
QString MyStation::GetChaoXianLevel(int index)
{
    QString strLevel = "";
    if(index == 0)
    {
        strLevel = CHAOXIAN_0;
    }
    else if(index == 1)
    {
        strLevel = CHAOXIAN_1;
    }
    else if(index == 2)
    {
        strLevel = CHAOXIAN_2;
    }
    else if(index == 3)
    {
        strLevel = CHAOXIAN_3;
    }
    else if(index == 4)
    {
        strLevel = CHAOXIAN_4;
    }
    return strLevel;
}
//根据名称获取超限级别索引
int MyStation::GetChaoXianLevelIndex(QString _strChaoXian)
{
    int id = -1;
    if(_strChaoXian == CHAOXIAN_0)
    {
        id = 0;
    }
    else if(_strChaoXian == CHAOXIAN_1)
    {
        id = 1;
    }
    else if(_strChaoXian == CHAOXIAN_2)
    {
        id = 2;
    }
    else if(_strChaoXian == CHAOXIAN_3)
    {
        id = 3;
    }
    else if(_strChaoXian == CHAOXIAN_4)
    {
        id = 4;
    }
    return id;
}
//获取铅封计数名称
void MyStation::GetQJNameList()
{
    QianFeng mQianFeng;
    QString name="";
    CXHD *pXHD;
    CZDBS *pZDBS=nullptr;
    CBZDBS *pBZDBS=nullptr;
     CDcBtn *DCBUTTON=nullptr;
    for(auto ement:DevArray)
    {
        if(ement->getDevType() == Dev_XH)
        {
            CXHD *xhd=(CXHD *)ement;
            if(((xhd->getXHDType()==CZ_XHJ||xhd->getXHDType()==JZ_XHJ)&&xhd->m_YDXHDButtonShowFlag)&&!xhd->getSignalType())
            {
                mQianFeng.Init();
                name = xhd->m_strName + "引导";//"-YDA")
                mQianFeng.JucDevName = xhd->m_strName;
                mQianFeng.Type = QF_XHDJLYD;
                mQianFeng.StrName = name;
                mQianFengVect.append(mQianFeng);
            }
        }
    }

    mQianFeng.Init();
    mQianFeng.Type = QF_SZRJ;
    mQianFeng.StrName = "S总人解";
    mQianFengVect.append(mQianFeng);

    mQianFeng.Init();

    mQianFeng.Type = QF_SYDZS;
    mQianFeng.StrName = "S引导总锁";
    mQianFengVect.append(mQianFeng);

     mQianFeng.Init();
    mQianFeng.Type = QF_XZRJ;
    mQianFeng.StrName = "X总人解";
    mQianFengVect.append(mQianFeng);

    mQianFeng.Init();
    mQianFeng.Type = QF_XYDZS;
    mQianFeng.StrName = "X引导总锁";
    mQianFengVect.append(mQianFeng);
    //自动闭塞
    for(int j=0;j<ZDBSArray.size();j++)
    {
        QString strZDBS ="";
        if(ZDBSArray[j]->getDevType() == Dev_ZDBS)
        {
            pZDBS = (CZDBS *)(ZDBSArray[j]);
            strZDBS = pZDBS->m_strName;
            mQianFeng.Init();
            mQianFeng.Type = QF_ZDBS_ZFZ;
            mQianFeng.JucDevName = strZDBS;
            mQianFeng.StrName = strZDBS + pZDBS->ZFZAN_Name;
            mQianFengVect.append(mQianFeng);

            mQianFeng.Init();
            mQianFeng.Type = QF_ZDBS_JFZ;
            mQianFeng.JucDevName = strZDBS;
            mQianFeng.StrName = strZDBS + pZDBS->JCFZAN_Name;//接车辅助
            mQianFengVect.append(mQianFeng);

            mQianFeng.Init();
            mQianFeng.Type = QF_ZDBS_FFZ;
            mQianFeng.JucDevName = strZDBS;
            mQianFeng.StrName = strZDBS + pZDBS->FCFZAN_Name;//发车辅助
            mQianFengVect.append(mQianFeng);
            if ((strZDBS=="S")||(strZDBS=="X"))
            {
                mQianFeng.Init();
                mQianFeng.Type = QF_ZDBS_GF;
                mQianFeng.JucDevName = strZDBS;
                mQianFeng.StrName = strZDBS + pZDBS->GFAN_Name;
                mQianFengVect.append(mQianFeng);
            }
        }

    }
    //半自动闭塞
    for(int j=0;j<BZDBSArray.size();j++)
    {
        QString strBZDBS;
        if(BZDBSArray[j]->getDevType() == Dev_BZDBS)
        {
            pBZDBS = (CBZDBS *)(BZDBSArray[j]);
            //strBZDBS = pZDBS->m_strName;
            strBZDBS = pBZDBS->getName();
            mQianFeng.Init();
            mQianFeng.Type = QF_BZDBS_SG;
            mQianFeng.JucDevName = strBZDBS;
            mQianFeng.StrName = strBZDBS + "事故";
            mQianFengVect.append(mQianFeng);
        }
    }
    //尖轨 心轨
    for(int j=0;j<DCBtnArray.size();j++)
    {
        if(DCBtnArray[j]->getDevType() == Dev_DCBTN)
        {
            DCBUTTON=(CDcBtn*)(DCBtnArray[j]);
            if(DCBUTTON->m_tType == DCGZ_JG)
            {
                mQianFeng.Init();
                mQianFeng.Type = DCGZ_JG;
                mQianFeng.StrName = DCBUTTON->Button_Name;
                mQianFengVect.append(mQianFeng);
            }
            else if(DCBUTTON->m_tType == DCGZ_XG)
            {
                mQianFeng.Init();
                mQianFeng.Type = DCGZ_XG;
                mQianFeng.StrName = DCBUTTON->Button_Name;
                mQianFengVect.append(mQianFeng);
            }

        }
    }
    //if(pFrame->StaConfigInfo.bSXddmd == TRUE)
    {
        mQianFeng.Init();
        mQianFeng.Type = QF_SMD;
        mQianFeng.StrName = "S灭灯";
        mQianFengVect.append(mQianFeng);
    }
    // if(pFrame->StaConfigInfo.bXXddmd == TRUE)
    {
        mQianFeng.Init();
        mQianFeng.Type = QF_XMD;
        mQianFeng.StrName = "X灭灯";
        mQianFengVect.append(mQianFeng);
    }

    mQianFeng.Init();
    mQianFeng.JucDevName = "";
    mQianFeng.Type = QF_PDJS;
    mQianFeng.StrName = "坡道解锁";
    mQianFengVect.append(mQianFeng);

    mQianFeng.Init();
    mQianFeng.JucDevName = "";
    mQianFeng.Type = QF_SSGJS;
    mQianFeng.StrName = "S区故解";//事故解锁
    mQianFengVect.append(mQianFeng);

    mQianFeng.Init();
    mQianFeng.JucDevName = "";
    mQianFeng.Type = QF_XSGJS;
    mQianFeng.StrName = "X区故解";//事故解锁
    mQianFengVect.append(mQianFeng);

    mQianFeng.Init();
    mQianFeng.Type = QF_GZTZ;
    mQianFeng.StrName = "故障通知";
    mQianFengVect.append(mQianFeng);

    mQianFeng.Init();
    mQianFeng.Type = QF_SDJS;
    mQianFeng.StrName = "上电解锁";
    mQianFengVect.append(mQianFeng);
}
//语音播报
void MyStation::SpeechText(QString str)
{
    if(this->StaConfigInfo.bSpeechFlag)
    {
        if(tts)
        {
            //tts = new QTextToSpeech(this);
            tts->setLocale(QLocale::Chinese);//设置语言环境
            tts->setRate(0.0);//设置语速-1.0到1.0
            tts->setPitch(0.0);//设置音高-1.0到1.0
            tts->setVolume(1.0);//设置音量0.0-1.0
            if(tts->state()==QTextToSpeech::Ready)
            {
                tts->say(str);
            }
        }
    }
}
//接收服务端数据-语音播报信息
void MyStation::recvServerData_Speach(QByteArray recvArray, int recvlen)
{
    int nCount = 10;
    //播报次数
    int count = (int)(recvArray[nCount]&0xFF);
    nCount += 1;
    //播报内容
    int lenTxt = (int)(recvArray[nCount]&0xFF);
    nCount += 1;
    QString strText;
    strText = ByteArrayToUnicode(recvArray.mid(nCount,lenTxt));
    nCount += lenTxt;

    if(strText != "")
    {
        //this->SpeechText(strText);
    }
}
//系统报警
void MyStation::SetServerAlarm(QByteArray recvArray, int recvlen)
{
    TableData mTableData;
    bool blevel1=false;
    if((int)recvArray[9]==0x40)
    {
        //系统消息框内容
        if((int)recvArray[10]==0x01)
        {
            mTableData.Init();
            //信息级别
            if((int)recvArray[11]==0x01)
            {
                mTableData.strLevel="错误";
                blevel1=true;
            }
            else if((int)recvArray[11]==0x02)
            {
                mTableData.strLevel="警告";
            }
            else if((int)recvArray[11]==0x03)
            {
                mTableData.strLevel="信息";
            }
            else if((int)recvArray[11]==0x04)
            {
                mTableData.strLevel="防错办返回信息";
            }
            //信息类型
            if((int)recvArray[12]==0x01)
            {
                mTableData.strType="系统";
            }
            else if((int)recvArray[12]==0x02)
            {
                mTableData.strType="行车";
            }
            //信息时间 7
            {
                int year;
                int a =((int)recvArray[13])&0xff;
                int b=((int)recvArray[14])<<8;
                year=a+b;
                int month=(int)recvArray[15];
                int data=(int)recvArray[16];
                int hour=(int)recvArray[17];
                int minute=(int)recvArray[18];
                int second=(int)recvArray[19];
                mTableData.strData=/*QString::number(year)+"-"+*/QString::number(month)+"-"
                        +QString::number(data)+" "+QString::number(hour)+":"
                        +QString::number(minute)+":"+QString::number(second);
                QDateTime datetime=QDateTime::fromString(mTableData.strData,"M-d h:m:s");
                mTableData.strData=datetime.toString("MM-dd hh:mm:ss");

            }
            //信息描述长度 1
            int nlen1=0;
            nlen1=(int)recvArray[20];
            //信息描述内容
            QByteArray Data;
            Data=recvArray.mid(21,nlen1);
            mTableData.strDescribe = QString::fromLocal8Bit(Data);
            //信息来源长度 1
            int nlen2=0;
            nlen2=(int)recvArray[20+nlen1+1];
            //信息来源内容
            Data=recvArray.mid(20+nlen1+2,nlen2);
            mTableData.strSource = QString::fromLocal8Bit(Data);
//            mVectTableDate.append(mTableData);
//            mSystemalarm->InitData(mVectTableDate);
        }
        else if((int)recvArray[10]==0x02)
        {
            //类型
            int ntype =(int)recvArray[11];
            //信息描述长度 1
            int nlen1=0;
            nlen1=(int)recvArray[12];
            //信息描述内容
            QByteArray Data;
            Data=recvArray.mid(13,nlen1);
            QString strDescribe = QString::fromLocal8Bit(Data);
        }
        //一级报警信息弹出弹窗
        if(blevel1)
        {
            QMessageBox::warning(NULL, "提示", mTableData.strDescribe);
           // ShowSystemAlarmWidget(true);
        }
    }

}
//设置该站所有的股道防溜
void MyStation::SetAllGDAntiSlip()
{
    for(int i=0; i<m_ArrayGDAntiSlip.count(); i++)
    {
        CGD* pGDTemp = (CGD*)m_ArrayGDAntiSlip[i];
        int devIndex = GetIndexByStrName(pGDTemp->m_strName);
        if(devIndex <= -1)
            break;
        CGD* pGD = (CGD*)DevArray[devIndex];
        pGD->m_nLAntiSlipType = pGDTemp->m_nLAntiSlipType;
        pGD->m_nLTxNum = pGDTemp->m_nLTxNum;
        pGD->m_nLJgqNum = pGDTemp->m_nLJgqNum;
        pGD->m_nLJnMeters =pGDTemp->m_nLJnMeters;
        pGD->m_nRAntiSlipType = pGDTemp->m_nRAntiSlipType;
        pGD->m_nRTxNum = pGDTemp->m_nRTxNum;
        pGD->m_nRJgqNum = pGDTemp->m_nRJgqNum;
        pGD->m_nRJnMeters = pGDTemp->m_nRJnMeters;
        pGD->m_nTrainNums = pGDTemp->m_nTrainNums;
        //存车信息
//        pGD->m_sTrainInfoShow = pGDTemp->m_sTrainInfoShow;

//        pGD->nManualBrakeS=pGD->m_nLAntiSlipType;
//        pGD->nManualBrakeX=pGD->m_nRAntiSlipType;
        //铁鞋设置 东部
        if((pGD->m_nLAntiSlipType&0x01)==0x01)
        {
           pGD->bIronShoesSetS =true;
        }
        else
        {
            pGD->bIronShoesSetS =false;
        }
        //人力制动机 人力紧固器 东部
        if((pGD->m_nLAntiSlipType&0x02)==0x02)
        {
           pGD->nManualBrakeS =0x01;
        }
        else if((pGD->m_nLAntiSlipType&0x04)==0x04)
        {
            pGD->nManualBrakeS =0x02;
        }
        else
        {
            pGD->nManualBrakeS =0x00;
        }

        //铁鞋设置 西部
        if((pGD->m_nRAntiSlipType&0x01)==0x01)
        {
           pGD->bIronShoesSetX =true;
        }
        else
        {
            pGD->bIronShoesSetX =false;
        }
        //人力制动机 人力紧固器 西部
        if((pGD->m_nRAntiSlipType&0x02)==0x02)
        {
            pGD->nManualBrakeX =0x01;
        }
        else if((pGD->m_nRAntiSlipType&0x04)==0x04)
        {
            pGD->nManualBrakeX =0x02;
        }
        else
        {
            pGD->nManualBrakeX =0x00;
        }
    }
}
//清除该站所有的股道防溜
void MyStation::ClearAllGDAntiSlip()
{
    for(int i=0; i<m_ArrayGDAntiSlip.count(); i++)
    {
        CGD* pGD = (CGD*)m_ArrayGDAntiSlip[i];
        pGD->m_nLAntiSlipType = 0;
        pGD->m_nLTxNum = 0;
        pGD->m_nLJgqNum = 0;
        pGD->m_nLJnMeters = 0;
        pGD->m_nRAntiSlipType = 0;
        pGD->m_nRTxNum = 0;
        pGD->m_nRJgqNum = 0;
        pGD->m_nRJnMeters = 0;
        pGD->m_nTrainNums = 0;
       //pGD->isSpeedLimit=false;
        pGD->speedLimitStatus=0;
    }
    this->SetAllGDAntiSlip();
}

//初始化占线版所有股道数据
void MyStation::InitZXBGDData()
{
    CGD *pgd=nullptr;
    GDData* pGDData=nullptr;
    for(int i=0;i<DevArray.size();i++)
    {
        if(DevArray[i]->getDevType() == Dev_GD)
        {
            pgd=(CGD*)(DevArray[i]);
            if(pgd->getGDType()==GD_QD)
            {
                pGDData = new GDData();
                pGDData->pGD=pgd;
                m_ArrayGDData.append(pGDData);
            }
        }
    }
}
//初始化时道岔获取对应的道岔区段名称的方框
void MyStation::getDGNameFromText()
{
    for(int i=0;i<DevArray.size();i++)
    {
        if(DevArray[i]->getDevType() == Dev_TEXT)
        {
           CText *text;
           text=(CText *)(DevArray[i]);
           if(text->getTextType() == DCQDNAMETEXT)
           {
               for(int j=0;j<DevArray.size();j++)
               {
                   if(DevArray[j]->getDevType() == Dev_DC)
                   {
                       CGDDC *gddc;
                       gddc=(CGDDC*)(DevArray[j]);
                       if(text->m_strName.right(2) == "DG")
                       {
                           QString DCText = text->m_strName.left(text->m_strName.length()-2);
                           QString ch = "-";
                           QStringList strArr;
                           int c = StringSplit(DCText, ch, strArr);
                           if (2 == c)
                           {
                               if(gddc->m_strName == strArr[0])
                               {
                                   gddc->setDGTextRect(text->m_textRect);
                                   gddc->setDGName(text->m_strName);
                               }
                               if(gddc->m_strName == strArr[1])
                               {
                                   gddc->setDGTextRect(text->m_textRect);
                                   gddc->setDGName(text->m_strName);
                               }
                           }
                           else
                           {
                               if(gddc->m_strName == DCText)
                               {
                                   gddc->setDGTextRect(text->m_textRect);
                                   gddc->setDGName(text->m_strName);
                               }
                           }
                       }
                   }
               }
           }
        }
    }
}
//接收服务端数据-防错办信息
CheckResult* MyStation::recvServerData_RouteCheck(QByteArray recvArray)
{
    CheckResult* checkResult = new CheckResult;
    checkResult->stationId = (int)(recvArray[6]&0xFF);
    int nCount = 10;
    //子分类码
    int type = (int)(recvArray[nCount]&0xFF);
    nCount += 1;
    //逻辑反馈
    if(0x01 == type)
    {
        //计划号
        checkResult->id = ByteArrayToUInt(recvArray.mid(nCount,4));
        nCount+=4;
        //是否可以强制执行
        checkResult->bEnforced = ((int)recvArray[nCount] != 0x00) ? true : false;
        nCount += 1;
        //检查信息内容
        int lenTxt = (int)(recvArray[nCount]&0xFF);
        nCount += 1;
        QString strText;
        strText = ByteArrayToUnicode(recvArray.mid(nCount,lenTxt));
        nCount += lenTxt;
        checkResult->checkMsg = strText;
        qDebug() << "recvServerData_RouteCheck" << strText;
        return checkResult;
    }
    return checkResult;
}
//重置站场设备状态
void MyStation::ResetStationDevStatus()
{
    CXHD *pXHD;
    CGD *pGD;
    CGDDC *pGDDC;
    int m = (int)DevArray.size();
    for(int i = 0; i < (int)DevArray.size(); i++)
    {
        if(DevArray[i]->getDevType() == Dev_XH)
        {
            pXHD = (CXHD*)DevArray[i];
            pXHD->m_nFuncLockState = false;
        }
        else if(DevArray[i]->getDevType() == Dev_GD)
        {
            pGD = (CGD*)DevArray[i];
            pGD->m_nCheciLost = false;
            pGD->m_strCheCiNumCTC = "";
            pGD->isLock = false;
            pGD->isGDFLBL = false;
            pGD->m_nGDFLBLKX = false;
            pGD->isPowerCut = false;
            pGD->isSpeedLimit = false;
            //pGD->LimitSpeed = 0;
            pGD->isGDFLBL_xjFlag = false;
            //pGD->FLBLAlarmFlag = true;
            //pGD->FLBLCheCiLostFlag = true;
            //pGD->FLBL_xjFlagTime = 0;
        }
        else if(DevArray[i]->getDevType() == Dev_DC)
        {
            pGDDC = (CGDDC*)DevArray[i];
//            pGDDC->m_nDS = FALSE;
//            pGDDC->m_nFS = FALSE;
//            pGDDC->isFLBL[0] = FALSE;
//            pGDDC->isFLBL[1] = FALSE;
//            pGDDC->isFLBL[2] = FALSE;
//            pGDDC->m_nCQFLBLKX = FALSE;
            pGDDC->isPowerCutDW = false;
            pGDDC->isPowerCutFW = false;
            pGDDC->isJGGZ = false;
            pGDDC->isXGGZ = false;
            pGDDC->setIsDS(false);

//            pGDDC->isSpeedLimit = FALSE;
//            pGDDC->LimitSpeed = 0;
        }
    }
//    CXHAN *pXHAN;
//    for(int j=0;j<m_XHANArray.GetSize();j++)
//    {
//        pXHAN = (CXHAN*)m_XHANArray[j];
//        pXHAN->m_nFuncLockState = FALSE;
//    }
    //TODO:
    //功能按钮 尖轨心轨
//     CGDDC *pCGDDC=nullptr;
//    CDcBtn *DCBUTTON=nullptr;
//    for(int j=0;j<DCBtnArray.size();j++)
//    {
//        if(DCBtnArray[j]->getDevType() == Dev_DCBTN)
//        {
//            DCBUTTON=(CDcBtn*)(DCBtnArray[j]);
//            {
//                DCBUTTON->setIsDown_Button(0);
//                if(DCBUTTON->m_tType == DCGZ_JG)
//                {
//                    for(int k=0;k<DevArray.size();k++)
//                    {
//                        if(DevArray[k]->getDevType() == Dev_DC)
//                        {
//                            pCGDDC=(CGDDC*)(DevArray[k]);
//                            if (pCGDDC->m_nCode == DCBUTTON->m_nDcCode)
//                            {
//                            }
//                        }
//                    }
//                }
//                else if(DCBUTTON->m_tType == DCGZ_XG)
//                {
//                    for(int k=0;k<DevArray.size();k++)
//                    {
//                        if(DevArray[k]->getDevType() == Dev_DC)
//                        {
//                            pCGDDC=(CGDDC*)(DevArray[k]);
//                            //if (pCGDDC->m_strName == DCBUTTON->m_strDcName)
//                            if (pCGDDC->m_nCode == DCBUTTON->m_nDcCode)
//                            {
//                            }
//                        }
//                    }
//                }
//            }
//        }
//    }

}

//判断进路是否有组合进路
QString MyStation::GetZHJL(QString ZHJL)
{
    QString sdXH_temp;
    QString zdXH_temp;
    QStringList str;
    QString ch = ",";
    QString RouteDescrip_ZTJL;
    int m=StringSplit(ZHJL, ch, str);
    if(m==2)
    {
        sdXH_temp=str[0];
        zdXH_temp=str[1];
    }

    //int n=pFrame->vectZHJL.size();
    for(int i=0;i<vectZHJL.size();i++)
    {
        if((sdXH_temp==vectZHJL[i].ZHJL_SDXH)&&(zdXH_temp==vectZHJL[i].ZHJL_ZDXH))
        {
            RouteDescrip_ZTJL=vectZHJL[i].ZHJL_RouteDescrip_ZTJL;
            RouteDescrip_ZTJL.replace(","," ");
            return RouteDescrip_ZTJL;
        }
    }
    return ZHJL;
}

MyStation::ZHJL* MyStation::GetZHJLByDescrip(QString ZHJL)
{
    for(int i = 0; i < vectZHJL.size(); i++)
    {
        if(ZHJL == vectZHJL.at(i).ZHJL_RouteDescrip)
        {
            return &vectZHJL[i];
        }
    }
    return nullptr;
}

bool MyStation::HaveZHJL(QString ZHJL)
{
    QString sdXH_temp;
    QString zdXH_temp;
    QStringList str;
    QString ch = "-";
    QString RouteDescrip_ZTJL;
    int m=StringSplit(ZHJL, ch, str);
    if(m==2)
    {
        sdXH_temp=str[0];
        zdXH_temp=str[1];
    }

    //int n=pFrame->vectZHJL.size();
    for(int i=0;i<vectZHJL.size();i++)
    {
        if((sdXH_temp==vectZHJL[i].ZHJL_SDXH)&&(zdXH_temp==vectZHJL[i].ZHJL_ZDXH))
        {
            return true;
        }
    }
    return false;
}

bool MyStation::HaveZHJL_JL(QString ZHJL)//是否有组合进路的分支进路存在
{
    for(int j=0;j<vectZHJL.size();j++)
    {
        for(int k=0;k<2;k++)
        {
            if(ZHJL==vectZHJL[j].ZHJL_RouteDescrip_JL[k])
            {
                return true;
            }
        }
    }

    return false;
}
QString MyStation::GetZHJL_JL1(QString ZHJL,int j)
{
    QString sdXH_temp;
    QString zdXH_temp;
    QStringList str;
    QString ch = "-";
    QString RouteDescrip_ZTJL_Temp;
    int m=StringSplit(ZHJL, ch, str);
    if(m==2)
    {
        sdXH_temp=str[0];
        zdXH_temp=str[1];
    }

    //int n=pFrame->vectZHJL.size();
    for(int i=0;i<vectZHJL.size();i++)
    {
        if((sdXH_temp==vectZHJL[i].ZHJL_SDXH)&&(zdXH_temp==vectZHJL[i].ZHJL_ZDXH))
        {
            RouteDescrip_ZTJL_Temp=vectZHJL[i].ZHJL_RouteDescrip_JL[j];
            return RouteDescrip_ZTJL_Temp;
        }
    }

    return ZHJL;
}
int MyStation::GetNumberZHJL_ZTJL(QString ZHJL)
{
        int nSize = m_ArrayRouteOrder.size();
        TrainRouteOrder  *pTrainRouteOrder;

        for (int i = 0; i < nSize; i++)
        {
            pTrainRouteOrder = (TrainRouteOrder*)m_ArrayRouteOrder[i];
            if(pTrainRouteOrder->m_strRouteDescrip==ZHJL)
            {
                return i;
            }
        }

    return -1;
}

//组合进路增加两条进路序列
void MyStation::StagePlanToRouteOrder_ZHJLAdd(TrainRouteOrder* _pTrainRouteOrder)
{
    if(HaveZHJL(_pTrainRouteOrder->m_strRouteDescrip))
    {
        for(int i=0;i<vectZHJL.size();i++)
        {
            if(vectZHJL[i].ZHJL_RouteDescrip == _pTrainRouteOrder->m_strRouteDescrip)
            {
                for(int j=0;j<vectZHJL[i].ZHJL_SonRouteNum;j++)
                {
                    TrainRouteOrder* pRecvTrainRouteOrder = new TrainRouteOrder;
                    pRecvTrainRouteOrder->m_btRecvOrDepart = _pTrainRouteOrder->m_btRecvOrDepart;
                    pRecvTrainRouteOrder->m_btBeginOrEndFlg = _pTrainRouteOrder->m_btBeginOrEndFlg;//所属阶段计划标志
                    pRecvTrainRouteOrder->m_strTrainNum = _pTrainRouteOrder->m_strTrainNum;
                    pRecvTrainRouteOrder->m_nTrackCode = _pTrainRouteOrder->m_nTrackCode;
                    pRecvTrainRouteOrder->m_strTrack = _pTrainRouteOrder->m_strTrack;
                    pRecvTrainRouteOrder->m_nGDPos = _pTrainRouteOrder->m_nGDPos;
                    pRecvTrainRouteOrder->m_nCodeReachStaEquip = _pTrainRouteOrder->m_nCodeReachStaEquip;
                    pRecvTrainRouteOrder->m_bElectric = _pTrainRouteOrder->m_bElectric;
                    pRecvTrainRouteOrder->m_nAutoTouch = _pTrainRouteOrder->m_nAutoTouch;
                    //获取进路方向
                    pRecvTrainRouteOrder->m_strDirection = _pTrainRouteOrder->m_strDirection;
                    pRecvTrainRouteOrder->m_timBegin = _pTrainRouteOrder->m_timBegin;
                    pRecvTrainRouteOrder->m_timPlanned = _pTrainRouteOrder->m_timPlanned;
                    pRecvTrainRouteOrder->m_bSx = _pTrainRouteOrder->m_bSx;//NEW

                    //获取进路描述
                    pRecvTrainRouteOrder->m_strRouteDescrip = GetZHJL_JL1(_pTrainRouteOrder->m_strRouteDescrip,j);
                    QStringList str;
                    int c = StringSplit(pRecvTrainRouteOrder->m_strRouteDescrip, "-", str);
                    pRecvTrainRouteOrder->m_strXHDBegin = str[0];
                    pRecvTrainRouteOrder->m_strXHDBegin.replace("DA","");
                    pRecvTrainRouteOrder->m_strXHDBegin.replace("LA","");
                    pRecvTrainRouteOrder->m_strXHDBegin.replace("A","");
                    pRecvTrainRouteOrder->m_strXHDEnd = str[1];
                    pRecvTrainRouteOrder->m_strXHDEnd.replace("DA","");
                    pRecvTrainRouteOrder->m_strXHDEnd.replace("LA","");
                    pRecvTrainRouteOrder->m_strXHDEnd.replace("A","");
                    pRecvTrainRouteOrder->m_strRouteDescripReal = pRecvTrainRouteOrder->m_strXHDBegin + "," + pRecvTrainRouteOrder->m_strXHDEnd;
//                    _pTrainRouteOrder->vectRouteSon.push_back(pRecvTrainRouteOrder);

                    //判断延续进路
//                    InitRouteYXJL(pRecvTrainRouteOrder);
                    //初始化变通进路
//                    InitRouteBtjl(pRecvTrainRouteOrder);

                    int l = GetNumberZHJL_ZTJL(_pTrainRouteOrder->m_strRouteDescrip);
//                    m_ArrayRouteOrder.InsertAt(l+1+j,pRecvTrainRouteOrder);

                }
            }
        }
    }
}

//组合进路增加两条进路序列
void MyStation::StagePlanToRouteOrder_ZHJLRemove(TrainRouteOrder* _pTrainRouteOrder)
{
    //CTrainRouteOrder* pRecvTrainRouteOrder = new CTrainRouteOrder;
    //CTrainRouteOrder* pDepartTrainRouteOrder = new CTrainRouteOrder;

    if(HaveZHJL(_pTrainRouteOrder->m_strRouteDescrip))
    {
        int l = GetNumberZHJL_ZTJL(_pTrainRouteOrder->m_strRouteDescrip);
        //for(int i=0;i<_pTrainRouteOrder->number_ZHJL_Flag;i++)
        for(int i=0;i<2;i++)
        {
            if(GetZHJL_JL1(_pTrainRouteOrder->m_strRouteDescrip,i) == GetJLXLFromIndex(l+1))
            {
                //                    m_ArrayRouteOrder.RemoveAt(l+1);
            }
        }
        //m_ArrayRouteOrderGrid.RemoveAt(l+1);
        //m_ArrayRouteOrderGrid.RemoveAt(l+1);
    }
}
//处理组合进路和分支进路的状态
void MyStation::DealZHJLstate()
{
    int nSize = m_ArrayRouteOrder.size();
    TrainRouteOrder  *pTrainRouteOrder;

    for (int i = 0; i < nSize; i++)
    {
        pTrainRouteOrder = (TrainRouteOrder*)m_ArrayRouteOrder[i];
        for(int j=0;j<vectZHJL.size();j++)
        {
            if(pTrainRouteOrder->m_strRouteDescrip==vectZHJL[j].ZHJL_RouteDescrip)
            {
                if((pTrainRouteOrder->m_nOldRouteState_ZHJL_0==0)&&(pTrainRouteOrder->m_nOldRouteState_ZHJL_1==0))
                {
                    pTrainRouteOrder->SetState(0);
                }
                else if((pTrainRouteOrder->m_nOldRouteState_ZHJL_0==1)||(pTrainRouteOrder->m_nOldRouteState_ZHJL_1==1))
                {
                    pTrainRouteOrder->SetState(1);
                }
                else if((pTrainRouteOrder->m_nOldRouteState_ZHJL_0==2)&&(pTrainRouteOrder->m_nOldRouteState_ZHJL_1==2))
                {
                    pTrainRouteOrder->SetState(2);
                }
                else if((pTrainRouteOrder->m_nOldRouteState_ZHJL_0==3)||(pTrainRouteOrder->m_nOldRouteState_ZHJL_1==3))
                {
                    pTrainRouteOrder->SetState(3);
                }
                else if((pTrainRouteOrder->m_nOldRouteState_ZHJL_0==4)&&(pTrainRouteOrder->m_nOldRouteState_ZHJL_1==4))
                {
                    pTrainRouteOrder->SetState(4);
                }
                else if((pTrainRouteOrder->m_nOldRouteState_ZHJL_0==5)||(pTrainRouteOrder->m_nOldRouteState_ZHJL_1==5))
                {
                    pTrainRouteOrder->SetState(5);
                }
                else if((pTrainRouteOrder->m_nOldRouteState_ZHJL_0==6)||(pTrainRouteOrder->m_nOldRouteState_ZHJL_1==6))
                {
                    pTrainRouteOrder->SetState(6);
                }
            }
        }
    }
}
void MyStation::DealZHJLstate_JL0()
{
    int nSize = m_ArrayRouteOrder.size();
    TrainRouteOrder  *pTrainRouteOrder;
    TrainRouteOrder  *pTrainRouteOrder_TEMP;

    for (int i = 0; i < nSize; i++)
    {
        pTrainRouteOrder = (TrainRouteOrder*)m_ArrayRouteOrder[i];
        for(int j=0;j<vectZHJL.size();j++)
        {
            if(pTrainRouteOrder->m_strRouteDescrip==vectZHJL[j].ZHJL_RouteDescrip_JL[0])
            {
                for(int k = 0; k < nSize; k++)
                {
                    pTrainRouteOrder_TEMP = (TrainRouteOrder*)m_ArrayRouteOrder[k];
                    if(pTrainRouteOrder_TEMP->m_strRouteDescrip==vectZHJL[j].ZHJL_RouteDescrip)
                    {
                        pTrainRouteOrder_TEMP->m_nOldRouteState_ZHJL_0=pTrainRouteOrder->m_nOldRouteState;
                    }
                }
            }
        }
    }

}
void MyStation::DealZHJLstate_JL1()
{
    int nSize = m_ArrayRouteOrder.size();
    TrainRouteOrder  *pTrainRouteOrder;
    TrainRouteOrder  *pTrainRouteOrder_TEMP;

    for (int i = 0; i < nSize; i++)
    {
        pTrainRouteOrder = (TrainRouteOrder*)m_ArrayRouteOrder[i];
        for(int j=0;j<vectZHJL.size();j++)
        {
            if(pTrainRouteOrder->m_strRouteDescrip==vectZHJL[j].ZHJL_RouteDescrip_JL[1])
            {
                for(int k = 0; k < nSize; k++)
                {
                    pTrainRouteOrder_TEMP = (TrainRouteOrder*)m_ArrayRouteOrder[k];
                    if(pTrainRouteOrder_TEMP->m_strRouteDescrip==vectZHJL[j].ZHJL_RouteDescrip)
                    {
                        pTrainRouteOrder_TEMP->m_nOldRouteState_ZHJL_1=pTrainRouteOrder->m_nOldRouteState;
                    }
                }
            }
        }
    }
}

//处理组合进路和分支进路的状态存储
void MyStation::DealZHJLstate_JL0_SAVE()
{
    int nSize = m_ArrayRouteOrder.size();
    TrainRouteOrder  *pTrainRouteOrder;
    TrainRouteOrder  *pTrainRouteOrder_TEMP;

    for (int i = 0; i < nSize; i++)
    {
        pTrainRouteOrder = (TrainRouteOrder*)m_ArrayRouteOrder[i];
        for(int j=0;j<vectZHJL.size();j++)
        {
            if(pTrainRouteOrder->m_strRouteDescrip==vectZHJL[j].ZHJL_RouteDescrip)
            {
                if(pTrainRouteOrder->RouteDescrip_ZHJL_Flag==true)
                {
                    for(int k = 0; k < nSize; k++)
                    {
                        pTrainRouteOrder_TEMP = (TrainRouteOrder*)m_ArrayRouteOrder[k];
                        if(pTrainRouteOrder_TEMP->m_strRouteDescrip==vectZHJL[j].ZHJL_RouteDescrip_JL[0])
                        {
                            if(pTrainRouteOrder->m_nOldRouteState_ZHJL_0==0)
                            {
                                pTrainRouteOrder_TEMP->SetState(0);
                            }
                            else if(pTrainRouteOrder->m_nOldRouteState_ZHJL_0==1)
                            {
                                pTrainRouteOrder_TEMP->SetState(1);
                            }
                            else if(pTrainRouteOrder->m_nOldRouteState_ZHJL_0==2)
                            {
                                pTrainRouteOrder_TEMP->SetState(2);
                            }
                            else if(pTrainRouteOrder->m_nOldRouteState_ZHJL_0==3)
                            {
                                pTrainRouteOrder_TEMP->SetState(3);
                            }
                            else if(pTrainRouteOrder->m_nOldRouteState_ZHJL_0==4)
                            {
                                pTrainRouteOrder_TEMP->SetState(4);
                            }
                            else if(pTrainRouteOrder->m_nOldRouteState_ZHJL_0==5)
                            {
                                pTrainRouteOrder_TEMP->SetState(5);
                            }
                            else if(pTrainRouteOrder->m_nOldRouteState_ZHJL_0==6)
                            {
                                pTrainRouteOrder_TEMP->SetState(6);
                            }
                        }
                    }
                }
            }
        }
    }
}
void MyStation::DealZHJLstate_JL1_SAVE()
{
    int nSize = m_ArrayRouteOrder.size();
    TrainRouteOrder  *pTrainRouteOrder;
    TrainRouteOrder  *pTrainRouteOrder_TEMP;

    for (int i = 0; i < nSize; i++)
    {
        pTrainRouteOrder = (TrainRouteOrder*)m_ArrayRouteOrder[i];
        for(int j=0;j<vectZHJL.size();j++)
        {
            if(pTrainRouteOrder->m_strRouteDescrip==vectZHJL[j].ZHJL_RouteDescrip)
            {
                if(pTrainRouteOrder->RouteDescrip_ZHJL_Flag==true)
                {
                    for(int k = 0; k < nSize; k++)
                    {
                        pTrainRouteOrder_TEMP = (TrainRouteOrder*)m_ArrayRouteOrder[k];
                        if(pTrainRouteOrder_TEMP->m_strRouteDescrip==vectZHJL[j].ZHJL_RouteDescrip_JL[1])
                        {
                            if(pTrainRouteOrder->m_nOldRouteState_ZHJL_1==0)
                            {
                                pTrainRouteOrder_TEMP->SetState(0);
                            }
                            else if(pTrainRouteOrder->m_nOldRouteState_ZHJL_1==1)
                            {
                                pTrainRouteOrder_TEMP->SetState(1);
                            }
                            else if(pTrainRouteOrder->m_nOldRouteState_ZHJL_1==2)
                            {
                                pTrainRouteOrder_TEMP->SetState(2);
                            }
                            else if(pTrainRouteOrder->m_nOldRouteState_ZHJL_1==3)
                            {
                                pTrainRouteOrder_TEMP->SetState(3);
                            }
                            else if(pTrainRouteOrder->m_nOldRouteState_ZHJL_1==4)
                            {
                                pTrainRouteOrder_TEMP->SetState(4);
                            }
                            else if(pTrainRouteOrder->m_nOldRouteState_ZHJL_1==5)
                            {
                                pTrainRouteOrder_TEMP->SetState(5);
                            }
                            else if(pTrainRouteOrder->m_nOldRouteState_ZHJL_1==6)
                            {
                                pTrainRouteOrder_TEMP->SetState(6);
                            }
                        }
                    }
                }
            }
        }
    }

}


//处理组合进路和分支进路的自触状态
void MyStation::DealZHJLZCstate()
{
    int nSize = m_ArrayRouteOrder.size();
    TrainRouteOrder  *pTrainRouteOrder;

    for (int i = 0; i < nSize; i++)
    {
        pTrainRouteOrder = (TrainRouteOrder*)m_ArrayRouteOrder[i];
        for(int j=0;j<vectZHJL.size();j++)
        {
            if(pTrainRouteOrder->m_strRouteDescrip==vectZHJL[j].ZHJL_RouteDescrip)
            {
                if(pTrainRouteOrder->RouteDescrip_ZHJL_Flag==true)
                {
                    if((pTrainRouteOrder->m_nAutoTouch_ZHJL_0==false)&&(pTrainRouteOrder->m_nAutoTouch_ZHJL_1==false))
                    {
                        pTrainRouteOrder->m_nAutoTouch = false;
                    }
                    else if((pTrainRouteOrder->m_nAutoTouch_ZHJL_0==false)&&(pTrainRouteOrder->m_nAutoTouch_ZHJL_1==true))
                    {
                        pTrainRouteOrder->m_nAutoTouch = false;
                    }
                    else if((pTrainRouteOrder->m_nAutoTouch_ZHJL_0==true)&&(pTrainRouteOrder->m_nAutoTouch_ZHJL_1==false))
                    {
                        pTrainRouteOrder->m_nAutoTouch = false;
                    }
                    else if((pTrainRouteOrder->m_nAutoTouch_ZHJL_0==true)&&(pTrainRouteOrder->m_nAutoTouch_ZHJL_1==true))
                    {
                        pTrainRouteOrder->m_nAutoTouch = true;
                    }
                }
            }
        }
    }

}
void MyStation::DealZHJLZCstate_JL0()
{
    int nSize = m_ArrayRouteOrder.size();
    TrainRouteOrder  *pTrainRouteOrder;
    TrainRouteOrder  *pTrainRouteOrder_TEMP;

    for (int i = 0; i < nSize; i++)
    {
        pTrainRouteOrder = (TrainRouteOrder*)m_ArrayRouteOrder[i];
        for(int j=0;j<vectZHJL.size();j++)
        {
            if(pTrainRouteOrder->m_strRouteDescrip==vectZHJL[j].ZHJL_RouteDescrip_JL[0])
            {
                for(int k = 0; k < nSize; k++)
                {
                    pTrainRouteOrder_TEMP = (TrainRouteOrder*)m_ArrayRouteOrder[k];
                    if(pTrainRouteOrder_TEMP->m_strRouteDescrip==vectZHJL[j].ZHJL_RouteDescrip)
                    {
                        pTrainRouteOrder_TEMP->m_nAutoTouch_ZHJL_0=pTrainRouteOrder->m_nAutoTouch;
                    }
                }
            }
        }
    }

}
void MyStation::DealZHJLZCstate_JL1()
{
    int nSize = m_ArrayRouteOrder.size();
    TrainRouteOrder  *pTrainRouteOrder;
    TrainRouteOrder  *pTrainRouteOrder_TEMP;

    for (int i = 0; i < nSize; i++)
    {
        pTrainRouteOrder = (TrainRouteOrder*)m_ArrayRouteOrder[i];
        for(int j=0;j<vectZHJL.size();j++)
        {
            if(pTrainRouteOrder->m_strRouteDescrip==vectZHJL[j].ZHJL_RouteDescrip_JL[1])
            {
                for(int k = 0; k < nSize; k++)
                {
                    pTrainRouteOrder_TEMP = (TrainRouteOrder*)m_ArrayRouteOrder[k];
                    if(pTrainRouteOrder_TEMP->m_strRouteDescrip==vectZHJL[j].ZHJL_RouteDescrip)
                    {
                        pTrainRouteOrder_TEMP->m_nAutoTouch_ZHJL_1=pTrainRouteOrder->m_nAutoTouch;
                    }
                }
            }
        }
    }

}

//处理组合进路和分支进路的状态存储
void MyStation::DealZHJLZCstate_JL0_SAVE()
{
    int nSize = m_ArrayRouteOrder.size();
    TrainRouteOrder  *pTrainRouteOrder;
    TrainRouteOrder  *pTrainRouteOrder_TEMP;

    for (int i = 0; i < nSize; i++)
    {
        pTrainRouteOrder = (TrainRouteOrder*)m_ArrayRouteOrder[i];
        for(int j=0;j<vectZHJL.size();j++)
        {
            if(pTrainRouteOrder->m_strRouteDescrip==vectZHJL[j].ZHJL_RouteDescrip)
            {
                if(pTrainRouteOrder->RouteDescrip_ZHJL_Flag==true)
                {
                    for(int k = 0; k < nSize; k++)
                    {
                        pTrainRouteOrder_TEMP = (TrainRouteOrder*)m_ArrayRouteOrder[k];
                        if(pTrainRouteOrder_TEMP->m_strRouteDescrip==vectZHJL[j].ZHJL_RouteDescrip_JL[0])
                        {
                            if(pTrainRouteOrder->m_nAutoTouch_ZHJL_0==false)
                            {
                                pTrainRouteOrder_TEMP->m_nAutoTouch = false;
                            }
                            else if(pTrainRouteOrder->m_nOldRouteState_ZHJL_0==true)
                            {
                                pTrainRouteOrder_TEMP->m_nAutoTouch = true;
                            }
                        }
                    }
                }
            }
        }
    }

}
void MyStation::DealZHJLZCstate_JL1_SAVE()
{
    int nSize = m_ArrayRouteOrder.size();
    TrainRouteOrder  *pTrainRouteOrder;
    TrainRouteOrder  *pTrainRouteOrder_TEMP;

    for (int i = 0; i < nSize; i++)
    {
        pTrainRouteOrder = (TrainRouteOrder*)m_ArrayRouteOrder[i];
        for(int j=0;j<vectZHJL.size();j++)
        {
            if(pTrainRouteOrder->m_strRouteDescrip==vectZHJL[j].ZHJL_RouteDescrip)
            {
                if(pTrainRouteOrder->RouteDescrip_ZHJL_Flag==true)
                {
                    for(int k = 0; k < nSize; k++)
                    {
                        pTrainRouteOrder_TEMP = (TrainRouteOrder*)m_ArrayRouteOrder[k];
                        if(pTrainRouteOrder_TEMP->m_strRouteDescrip==vectZHJL[j].ZHJL_RouteDescrip_JL[1])
                        {
                            if(pTrainRouteOrder->m_nOldRouteState_ZHJL_1==false)
                            {
                                pTrainRouteOrder_TEMP->m_nAutoTouch = false;
                            }
                            else if(pTrainRouteOrder->m_nOldRouteState_ZHJL_1==true)
                            {
                                pTrainRouteOrder_TEMP->m_nAutoTouch = true;
                            }
                        }
                    }
                }
            }
        }
    }

}
//处理组合进路和分支进路的自触状态
void MyStation::DealZHJLZC_JL0(TrainRouteOrder* _pTrainRouteOrder)
{
    int nSize = m_ArrayRouteOrder.size();
    TrainRouteOrder  *pTrainRouteOrder_TEMP;
    for(int j=0;j<vectZHJL.size();j++)
    {
        if(_pTrainRouteOrder->m_strRouteDescrip==vectZHJL[j].ZHJL_RouteDescrip)
        {
            //if(_pTrainRouteOrder->RouteDescrip_ZHJL_Flag==TRUE)
            {
                for(int k = 0; k < nSize; k++)
                {
                    pTrainRouteOrder_TEMP = (TrainRouteOrder*)m_ArrayRouteOrder[k];
                    if(pTrainRouteOrder_TEMP->m_strRouteDescrip==vectZHJL[j].ZHJL_RouteDescrip_JL[0])
                    {
                        if(_pTrainRouteOrder->m_nAutoTouch == true)
                        {
                            pTrainRouteOrder_TEMP->m_nAutoTouch = true;
                            pTrainRouteOrder_TEMP->m_nAutoTouch_ZHJL_0 = true;
                        }
                        else
                        {
                            pTrainRouteOrder_TEMP->m_nAutoTouch = false;
                            pTrainRouteOrder_TEMP->m_nAutoTouch_ZHJL_0 = false;
                        }
                    }
                    if(pTrainRouteOrder_TEMP->m_strRouteDescrip==vectZHJL[j].ZHJL_RouteDescrip_JL[1])
                    {
                        if(_pTrainRouteOrder->m_nAutoTouch == true)
                        {
                            pTrainRouteOrder_TEMP->m_nAutoTouch = true;
                            pTrainRouteOrder_TEMP->m_nAutoTouch_ZHJL_1 = true;
                        }
                        else
                        {
                            pTrainRouteOrder_TEMP->m_nAutoTouch = false;
                            pTrainRouteOrder_TEMP->m_nAutoTouch_ZHJL_0 = false;
                        }
                    }
                }
            }
        }
    }
}

//处理组合进路和分支进路的点开时的进路状态显示
void MyStation::DealZHJLstateTo_JL0(TrainRouteOrder* _pTrainRouteOrder)
{

    int nSize = m_ArrayRouteOrder.size();
    TrainRouteOrder  *pTrainRouteOrder_TEMP;
    for(int j=0;j<vectZHJL.size();j++)
    {
        if(_pTrainRouteOrder->m_strRouteDescrip==vectZHJL[j].ZHJL_RouteDescrip)
        {
            if(_pTrainRouteOrder->RouteDescrip_ZHJL_Flag==true)
            {
                for(int k = 0; k < nSize; k++)
                {
                    pTrainRouteOrder_TEMP = (TrainRouteOrder*)m_ArrayRouteOrder[k];
                    if(pTrainRouteOrder_TEMP->m_strRouteDescrip==vectZHJL[j].ZHJL_RouteDescrip_JL[0])
                    {
                        if(_pTrainRouteOrder->m_nOldRouteState == 0)
                        {
                            pTrainRouteOrder_TEMP->SetState(0);
                            pTrainRouteOrder_TEMP->m_nOldRouteState_ZHJL_0 = 0;
                        }
                        else if(_pTrainRouteOrder->m_nOldRouteState == 1)
                        {
                            pTrainRouteOrder_TEMP->SetState(1);
                            pTrainRouteOrder_TEMP->m_nOldRouteState_ZHJL_0 = 1;
                        }
                        else if(_pTrainRouteOrder->m_nOldRouteState == 2)
                        {
                            pTrainRouteOrder_TEMP->SetState(2);
                            pTrainRouteOrder_TEMP->m_nOldRouteState_ZHJL_0 = 2;
                        }
                        else if(_pTrainRouteOrder->m_nOldRouteState == 3)
                        {
                            pTrainRouteOrder_TEMP->SetState(3);
                            pTrainRouteOrder_TEMP->m_nOldRouteState_ZHJL_0 = 3;
                        }
                        else if(_pTrainRouteOrder->m_nOldRouteState == 4)
                        {
                            pTrainRouteOrder_TEMP->SetState(4);
                            pTrainRouteOrder_TEMP->m_nOldRouteState_ZHJL_0 = 4;
                        }
                        else if(_pTrainRouteOrder->m_nOldRouteState == 5)
                        {
                            pTrainRouteOrder_TEMP->SetState(5);
                            pTrainRouteOrder_TEMP->m_nOldRouteState_ZHJL_0 = 5;
                        }
                        else if(_pTrainRouteOrder->m_nOldRouteState == 6)
                        {
                            pTrainRouteOrder_TEMP->SetState(6);
                            pTrainRouteOrder_TEMP->m_nOldRouteState_ZHJL_0 = 6;
                        }
                    }
                    if(pTrainRouteOrder_TEMP->m_strRouteDescrip==vectZHJL[j].ZHJL_RouteDescrip_JL[1])
                    {
                        if(_pTrainRouteOrder->m_nOldRouteState == 0)
                        {
                            pTrainRouteOrder_TEMP->SetState(0);
                            pTrainRouteOrder_TEMP->m_nOldRouteState_ZHJL_1 = 0;
                        }
                        else if(_pTrainRouteOrder->m_nOldRouteState == 1)
                        {
                            pTrainRouteOrder_TEMP->SetState(1);
                            pTrainRouteOrder_TEMP->m_nOldRouteState_ZHJL_1 = 1;
                        }
                        else if(_pTrainRouteOrder->m_nOldRouteState == 2)
                        {
                            pTrainRouteOrder_TEMP->SetState(2);
                            pTrainRouteOrder_TEMP->m_nOldRouteState_ZHJL_1 = 2;
                        }
                        else if(_pTrainRouteOrder->m_nOldRouteState == 3)
                        {
                            pTrainRouteOrder_TEMP->SetState(3);
                            pTrainRouteOrder_TEMP->m_nOldRouteState_ZHJL_1 = 3;
                        }
                        else if(_pTrainRouteOrder->m_nOldRouteState == 4)
                        {
                            pTrainRouteOrder_TEMP->SetState(4);
                            pTrainRouteOrder_TEMP->m_nOldRouteState_ZHJL_1 = 4;
                        }
                        else if(_pTrainRouteOrder->m_nOldRouteState == 5)
                        {
                            pTrainRouteOrder_TEMP->SetState(5);
                            pTrainRouteOrder_TEMP->m_nOldRouteState_ZHJL_1 = 5;
                        }
                        else if(_pTrainRouteOrder->m_nOldRouteState == 6)
                        {
                            pTrainRouteOrder_TEMP->SetState(6);
                            pTrainRouteOrder_TEMP->m_nOldRouteState_ZHJL_1 = 6;
                        }
                    }
                }
            }
        }
    }

}

//通过组合进路子进路的序号获取子进路的进路序列
QString MyStation::GetJLXLFromIndex(int l)
{

    int nSize = m_ArrayRouteOrder.size();
    TrainRouteOrder  *pTrainRouteOrder;

    for (int i = 0; i < nSize; i++)
    {
        pTrainRouteOrder = (TrainRouteOrder*)m_ArrayRouteOrder[i];
        if(i==l)
        {
            return pTrainRouteOrder->m_strRouteDescrip;
        }
    }

    return "";
}

void MyStation::ZHJLUnfoldState()//组合进路展开
{
    for(int i=0;i<(m_ArrayRouteOrder.size());i++)
    {
        TrainRouteOrder* pTrainRouteOrder = (TrainRouteOrder*)m_ArrayRouteOrder[i];
        if(HaveZHJL(pTrainRouteOrder->m_strRouteDescrip))
        {
            if(ZHJL_UNFLOD == true)
            {
                if(pTrainRouteOrder->RouteDescrip_ZHJL_Flag==false)
                {
                    pTrainRouteOrder->RouteDescrip_ZHJL_Flag=true;
                    pTrainRouteOrder->number_ZHJL_Flag=2;
                    StagePlanToRouteOrder_ZHJLAdd(pTrainRouteOrder);

                    DealZHJLstate_JL0_SAVE();
                    DealZHJLstate_JL1_SAVE();
                }

            }
        }
    }
}
// nCode接车、发车设备号, nFlg出发1 到达0 //阶段计划传来该设备号是否是上行
bool MyStation::GetSXByCode(int nCode, int nFlg)
{
    bool bvalue = false;
    for(auto ement:DevArray)
    {
        if(ement->getDevType() == Dev_XH)
        {
            CXHD *xhd=(CXHD *)ement;
            if(nCode == (int)xhd->getCode())
            {
                if(xhd->m_nSX == 1)	//如果是上行口
                {
                    if(nFlg == 0)
                        bvalue = true;
                    else
                        bvalue = false;
                }
                else //if(pXHD->m_nSX == 0) //如果下行口
                {
                    if(nFlg == 0)
                        bvalue = false;
                    else
                        bvalue = true;
                }
                break;
            }
        }
        else if(ement->getDevType() == Dev_GD)
        {
            CGD *gd=(CGD*)ement;
            if(nCode == (int)gd->getCode())
            {
                return gd->m_nSX;
            }
        }
        else if(ement->getDevType() == Dev_DC)
        {
            CGDDC *gddc=(CGDDC *)ement;
            if(((-2==nFlg) && (nCode == (int)gddc->getQDCode()))
                || (nCode == (int)gddc->getCode()))
            {
                return gddc->m_nSX;
            }
        }
    }
    return bvalue;
}
