#include "mydoc.h"
#include <QtDebug>
#include <QSettings>
#include <QMessageBox>
#include <QTextCodec>
#include "GlobalHeaders/GlobalFuntion.h"

MyDoc::MyDoc(QObject *parent) : QObject(parent)
{
    //变量初始化
    SysLifeSeconds = 0;
    currStaIndex = 0;
    bUdpMode = true;
    socketUDP = nullptr;
    tcpClient = nullptr;
    databasePort = 0;

    wndType = 1;

    pTimer = new QTimer(this);
    connect(pTimer, SIGNAL(timeout()), this, SLOT(timerTime()));
    pTimer->start(1000);
    initAllData();

    qInfo()<<"init MyDoc";
}
MyDoc::~MyDoc()
{
    for (int i = 0; i < vectMyStation.size(); i++)
    {
        MyStation* pStation = vectMyStation.at(i);
        delete pStation;
    }

    for (int i = 0; i < vectThread.size(); i++)
    {
        QThread* pThread = vectThread.at(i);
        if(pThread->isRunning())
        {
            pThread->quit();
        }
        delete pThread;
    }
    pTimer->stop();
    delete pTimer;
}

//初始化所有数据
void MyDoc::initAllData()
{
    readGlobalData();
    //ReadTrainNumTypeTXT();
    ReadZXBWindowShowTxt();
    //ReadDDOrderTemplate();
    initNetCom();
    //最后初始化车站逻辑线程
    initGlobalLogic();
    InitAllWindow();
    //记录当前时间
    SysStartedDataTime = QDateTime::currentDateTime();
}
//读取全局数据
void MyDoc::readGlobalData()
{
    QString strFile="Data/Global.ini";
    QString ErrMsg = QString("%1配置数据格式错误：").arg(strFile);
    QFileInfo fileInfo(strFile);
    if (!fileInfo.isFile())
    {
        QLOG_ERROR()<<QString::fromLocal8Bit("File %1 not existed!").arg(strFile);
        return;
    }

    QSettings* settings = new QSettings(strFile, QSettings::IniFormat);
    // 指定为中文
    settings->setIniCodec("UTF-8");

    //读取配置-通信信息
    m_AddrCTCToService.nPortCTC = settings->value("Address/LocalPort").toInt();
    m_AddrCTCToService.addrService = settings->value("Address/ServerAddr").toString();
    m_AddrCTCToService.nPortService1 = settings->value("Address/ServerPort1").toInt();
    m_AddrCTCToService.nPortService2 = settings->value("Address/ServerPort2").toInt();

    //读取配置-车站信息
    //int num = settings->value("Stations/Num").toInt();
    //QString str = settings->value("Stations/Num").toString();
    //str = str.toLower();//转小写，防错
    //int num = StringToHex(str);
    //int num = str.contains("0x")?str.toInt(0,16):str.toInt(0,10);
    int num = StringToHex(settings->value("Stations/Num").toString());
    for(int i=0; i<num; i++)
    {
        QString strNum = QString("Stations/Num%1").arg(i+1);
        QString info = settings->value(strNum).toString();
        QStringList strArr;
        int count = StringSplit(info, "|", strArr);
        if (2 == count)
        {
            MyStation *pMyStation = new MyStation();
            //pMyStation->setStationID(strArr[0].toInt());
            strArr[0] = strArr[0].toLower();//转小写，防错
            pMyStation->setStationID(StringToHex(strArr[0]));
            pMyStation->setStationName(strArr[1]);
            QString staFile = QString("Data/data%1/Station.txt").arg(i+1);
            QFileInfo fileInfo2(staFile);
            if (!fileInfo2.isFile())
            {
                QLOG_ERROR()<<QString::fromLocal8Bit("File %1 not existed!").arg(staFile);
                break;
            }
            if(pMyStation->readStationDev(staFile, pMyStation))
            {
                //匹配通过按钮与对应信号机关系
                pMyStation->SetXHJTGBtnState();
                staFile = QString("Data/data%1/StationConfig.txt").arg(i+1);
                pMyStation->readStationConfig(staFile, pMyStation);
                pMyStation->SetXHJPDJSState();
                staFile = QString("Data/data%1/BSConfig.txt").arg(i+1);
                pMyStation->readBSConfig(staFile, pMyStation);
//                //车站指示灯
//                staFile = QString("Data/data%1/LampFuncBConfig.txt").arg(i+1);
//                pMyStation->readLampFuncBConfig(staFile, pMyStation);
                //读取联锁表
                staFile = QString("Data/data%1/InterlockTable.txt").arg(i+1);
                pMyStation->readInterlockTable(staFile, pMyStation);

                //车站供电臂设置配置数据
                staFile = QString("Data/data%1/PowerSupplyArm.txt").arg(i+1);
                pMyStation->readPowerSupplyArmData(staFile);
                //封锁设备数据读取
                //股道设备数据
                staFile = QString("Data/data%1/Blockoperation_GD.txt").arg(i+1);
                pMyStation->readBlockingDevDataGD(staFile);
                //区间设备数据
                staFile = QString("Data/data%1/Blockoperation_QJ.txt").arg(i+1);
                pMyStation->readBlockingDevDataQJ(staFile);

                //发送数据绑定
                connect(pMyStation, &MyStation::sendDataToCTCServerSignal, this, &MyDoc::sendDataToCTCServerSlot);
                //发送数据绑定
                connect(pMyStation, &MyStation::sendDataToCTCServerSignal2, this, &MyDoc::sendDataToCTCServerSlot2);

                //初始化
                pMyStation->InitData();
                vectMyStation.append(pMyStation);

//                //工作线程
//                QThread *thread = new QThread(this);
//                vectThread.append(thread);
//                pMyStation->isThreadStart = true;
//                pMyStation->moveToThread(thread);
//                connect(thread, &QThread::started, pMyStation, &MyStation::startWorkSlot);
//                connect(pMyStation, &MyStation::endWorkSignal, thread, &QThread::quit);
//                //thread->start();//此处不启动，初始化结束后统一启动

//                //发送数据绑定
//                connect(pMyStation, &MyStation::sendDataToCTCServerSignal, this, &MyDoc::sendDataToCTCServerSlot);
//                //发送数据绑定
//                connect(pMyStation, &MyStation::sendDataToCTCServerSignal2, this, &MyDoc::sendDataToCTCServerSlot2);

            }
        }
        else
        {
            QLOG_ERROR()<<ErrMsg<<strNum;
        }
    }
    bRouteOrderShow = settings->value("CONFIG/LCJLXLWNDSHOW").toBool();
    delete settings;
}

//初始化全局窗口初始化
void MyDoc::InitAllWindow()
{
    pPowerSupplyArmDialog =new PowerSupplyArmDialog();
    pBlockingDevDialog =new BlockingDevDialog();
    connect(pPowerSupplyArmDialog, &PowerSupplyArmDialog::SendTextAnalysisSignal,[=](int index, QPoint pt, QString dev){
        pTextAnalysis->sendTextAnalysisMsg(index, pZXBMainWindow->mapFromGlobal(pt), dev);
    });
    connect(pBlockingDevDialog, &BlockingDevDialog::SendTextAnalysisSignal,[=](int index, QPoint pt, QString dev){
        pTextAnalysis->sendTextAnalysisMsg(index, pZXBMainWindow->mapFromGlobal(pt), dev);
    });
}
//初始化通信
void MyDoc::initNetCom()
{
    //主通信方式UDP模式
    {
        socketUDP = new SocketUDP;
        if(socketUDP->initAnyIP(m_AddrCTCToService.nPortCTC))
        {
            connect(socketUDP,SIGNAL(recvDataSignal(QByteArray, QString, int)),this,SLOT(receiveCTCServerDataSlot(QByteArray, QString, int)));
            connect(this,SIGNAL(sendDataToMainSignal(QByteArray,QString,int,int)),socketUDP,SLOT(sendDataSlot(QByteArray,QString,int,int)));
            QLOG_INFO()<<QString("本地端口Any:%1初始化成功！").arg(m_AddrCTCToService.nPortCTC);
        }
    }
    QLOG_INFO()<<QString("与服务端主通信1-%1:%2读取成功！").arg(m_AddrCTCToService.addrService).arg(m_AddrCTCToService.nPortService1);
    //主通信方式TCP模式
    {
        tcpClient = new SocketTCP;
        if(tcpClient->initByIP(m_AddrCTCToService.addrService, m_AddrCTCToService.nPortService2))
        {
            connect(tcpClient,SIGNAL(onConnectedSignal(QString, int)),this,SLOT(tcpClientConnectedSlot(QString, int)));
            connect(tcpClient,SIGNAL(onDisconnectedSignal(QString, int)),this,SLOT(tcpClientDisconnectedSlot(QString, int)));
            connect(tcpClient,SIGNAL(recvDataSignal(QByteArray,QString, int)),this,SLOT(receiveCTCServerDataSlot2(QByteArray,QString,int)));
            connect(this,SIGNAL(sendDataToMainSignal2(QByteArray,int)),tcpClient,SLOT(sendDataSlot(QByteArray,int)));
            QLOG_INFO()<<QString("与服务端主通信2-%1:%2初始化成功！").arg(m_AddrCTCToService.addrService).arg(m_AddrCTCToService.nPortService2);
        }
    }
}
//初始化全局逻辑
void MyDoc::initGlobalLogic()
{
    for (int i = 0; i < vectMyStation.size(); i++)
    {
        MyStation* pStation = vectMyStation.at(i);
        //工作线程
        QThread *thread = new QThread(this);
        vectThread.append(thread);
        pStation->isThreadStart = true;
        pStation->moveToThread(thread);
        connect(thread, &QThread::started, pStation, &MyStation::startWorkSlot);
        connect(pStation, &MyStation::endWorkSignal, thread, &QThread::quit);
        //thread->start();//此处不启动，初始化结束后统一启动
    }
    //统一启动车站线程
    for (int i = 0; i < vectThread.size(); i++)
    {
        QThread* pThread = vectThread.at(i);
        pThread->start();
    }
}
//定时器-1秒定时器
void MyDoc::timerTime()
{
    //统计运行时间
    SysLifeSeconds = SysStartedDataTime.msecsTo(QDateTime::currentDateTime())/1000;

    //发送心跳
    SendHeartBeatToTCCService();

    //系统启动后4秒时发送请求数据
    //if(4 == SysLifeSeconds)
    if(bRequstData)
    {
        bRequstData = false;
        SysInitSeconds = SysLifeSeconds;
    }
    //再过1秒执行
    if((SysInitSeconds+1) == SysLifeSeconds)
    {
        for (int i = 0; i < vectMyStation.size(); i++)
        {
            MyStation* pStation = vectMyStation.at(i);
            //debug
            //请求所有数据
            pStation->RequestDataFromServer(SYNC_ALL);
        }
    }
    for(int i=0;i<vectMyStation.size();i++)
    {
        if(vectMyStation[i]->commToServer>0)
        {
            vectMyStation[i]->commToServer--;
        }
    }
    //获取与服务端通信状态
    pMainWindow->SetServiceCommFlag(SetServiceCommState());
}
//根据站名获取索引
int MyDoc::getIndexByStaNameInStaArray(QString strStation)
{
    for (int i = 0; i < vectMyStation.size(); i++)
    {
        MyStation* pStation = vectMyStation.at(i);
        if (strStation == pStation->getStationName())
        {
            return i;
        }
    }
    return -1;
}

//根据站名在车站数组中获取车站指针
MyStation* MyDoc::getMyStationByStaNameInStaArray(QString strStation)
{
    for (int i = 0; i < vectMyStation.size(); i++)
    {
        MyStation* pStation = vectMyStation.at(i);
        if (strStation == pStation->getStationName())
        {
            return pStation;
        }
    }
    return NULL;
}

//根据id在车站数组中获取车站指针
MyStation* MyDoc::getMyStationByStaIDInStaArray(int id)
{
    for (int i = 0; i < vectMyStation.size(); i++)
    {
        MyStation* pStation = vectMyStation.at(i);
        if (id == pStation->getStationID())
        {
            return pStation;
        }
    }
    return NULL;
}

//根据索引在车站数组中获取车站指针
MyStation* MyDoc::getMyStationByIndexInStaArray(int idx)
{
    if (0 <= idx && idx <vectMyStation.size())
    {
        MyStation* pStation = vectMyStation.at(idx);
        return pStation;
    }
    return NULL;
}

//在车站数组中获取当前车站指针
MyStation *MyDoc::getCurrMyStationInStaArray()
{
    MyStation* pStation = getMyStationByIndexInStaArray(currStaIndex);
    return pStation;
}
int MyDoc::setCurrIndexByStaName(QString strStation)
{
    //设置车站索引
    currStaIndex = getIndexByStaNameInStaArray(strStation);
    if (currStaIndex > -1)
    {
        //用于控制命令栏的状态更新
        //GetMyStationByStaNameInStaArray(strStation)->m_bModeChanged = TRUE;
    }
    return currStaIndex;
}
//读取列车运行类型配置文件
void MyDoc::ReadTrainNumTypeTXT()
{
    QString filePath = GetWorkDirRoot()+"Data/TrainNumType.txt";
    QFile file(filePath);
    // 指定为GBK
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QLOG_ERROR()<<"打开TrainNumType.txt数据文件失败!";
        return;
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
        else if(m_str.left(2) == "//")
        {
            continue;
        }
        else if(m_str.indexOf("|") >= 0)
        {
            QStringList strArr;
            int c = StringSplit(m_str, "|", strArr);
            if(c == 2)
            {
                TrainNumType *trainNum_type = new TrainNumType;
                trainNum_type->strType = strArr[0].trimmed();
                trainNum_type->strTypeName = strArr[1].trimmed();
                v_TrainNumType.append(trainNum_type);
            }
        }
        else
        {
            v_TrainRunType.push_back(m_str.trimmed());
        }
    }
    file.close();
    //各站同步
    InitStationTrainNumType();
}
//读取调度命令类型模板
void MyDoc::ReadDDOrderTemplate()
{
    DisOrderTemplate mDisOrderTemplate;
    //读取调度命令模板
    QString filePath = GetWorkDirRoot()+"Data/DispatchOrder.conf";
    QFile file(filePath);
    // 指定为GBK
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QLOG_ERROR()<<"DispatchOrder.conf数据文件失败!";
        return;
    }
    while(!file.atEnd())
    {
        QByteArray line = file.readLine();
        QString m_str = codec->toUnicode(line);
        QString getString;
        m_str=m_str.remove("\n");
        if(m_str == "####")
        {
            break;
        }
        else if(m_str=="##")	//开始读取模板
        {
            line = file.readLine();
            m_str = codec->toUnicode(line);
            m_str=m_str.remove("\n");
            mDisOrderTemplate.Init();
            if(m_str.left(18) == "strDisOrderHeader:")
            {
                line = file.readLine();
                m_str = codec->toUnicode(line);
                m_str=m_str.remove("\n");
                mDisOrderTemplate.strDisOrderHeader=m_str;
            }
            line = file.readLine();
            m_str = codec->toUnicode(line);
            m_str=m_str.remove("\n");
            if(m_str.left(19) == "strDisOrderContent:")
            {
                line = file.readLine();
                m_str = codec->toUnicode(line);
                m_str=m_str.remove("\n");
                mDisOrderTemplate.strDisOrderContent=m_str;
            }
            mVectDisOrderTemplate.append(mDisOrderTemplate);
        }
    }
    file.close();
}
//各站列车运行类型数据同步初始化
void MyDoc::InitStationTrainNumType()
{
    for (int s = 0; s < vectMyStation.count(); s++)
    {
        MyStation* myStation = vectMyStation[s];
        for(int a=0; a<v_TrainNumType.count(); a++)
        {
            myStation->v_TrainNumType.append(v_TrainNumType[a]);
        }
        for(int a=0; a<v_TrainRunType.count(); a++)
        {
            myStation->v_TrainRunType.append(v_TrainRunType[a]);
        }
    }
}
//读取占线版界面显示配置文件
void MyDoc::ReadZXBWindowShowTxt()
{
    QString filePath = GetWorkDirRoot()+"Data/ViewShowConfig.txt";
    QFile file(filePath);
    // 指定为GBK
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QLOG_ERROR()<<"ViewShowConfig.txt数据文件失败!";
        return;
    }
    while(!file.atEnd())
    {
        QByteArray line = file.readLine();
        QString m_str = codec->toUnicode(line);
        m_str=m_str.trimmed();
        QString getString;
        if(m_str.left(5) == "##END")
        {
            break;
        }
        else if(m_str.left(2) == "//")
        {
            continue;
        }
        else
        {
            if(m_str.left(7)=="GDView=")
            {
                getString = m_str.mid(7,m_str.indexOf(";")-m_str.indexOf("=")-1);
                QStringList strArr;
                int c = StringSplit(getString, "|", strArr);
                if(c == 4+strArr[0].toInt())
                {
                    mZXBWindowCon.nGDView=strArr[0].toInt();
                    for(int i=0;i<mZXBWindowCon.nGDView;i++)
                    {
                        mZXBWindowCon.nGDViewStationID.append(StringToHex(strArr[1+i]));
                    }

                    if(strArr[strArr.size()-3]=="v"||strArr[strArr.size()-3]=="V")
                    {
                        mZXBWindowCon.bGDHV=true;
                    }
                    else mZXBWindowCon.bGDHV=false;
                    mZXBWindowCon.nViewType=strArr[strArr.size()-2].toInt();
                    mZXBWindowCon.nMaxRow=strArr[strArr.size()-1].toInt();
                }
            }
            else if(m_str.left(7)=="JLView=")
            {
                getString = m_str.mid(7,m_str.indexOf(";")-m_str.indexOf("=")-1);
                QStringList strArr;
                int c = StringSplit(getString, "|", strArr);
                if(c == 2+strArr[0].toInt())
                {
                    mZXBWindowCon.nJLView=strArr[0].toInt();
                    for(int i=0;i<mZXBWindowCon.nJLView;i++)
                    {
                        mZXBWindowCon.nRouteViewStationID.append(StringToHex(strArr[1+i]));
                    }
                    if(strArr[strArr.size()-1]=="h"||strArr[strArr.size()-1]=="H")
                    {
                        mZXBWindowCon.bJLHV=true;
                    }
                    else if(strArr[strArr.size()-1]=="t"||strArr[strArr.size()-1]=="T")
                    {
                        mZXBWindowCon.bJLHV=false;
                    }
                }
            }
            else if(m_str.left(7)=="FLView=")
            {
                getString = m_str.mid(7,m_str.indexOf(";")-m_str.indexOf("=")-1);
                QStringList strArr;
                int c = StringSplit(getString, "|", strArr);
                if(c == 2 + strArr[0].toInt())
                {
                    mZXBWindowCon.nFLView=strArr[0].toInt();
                    for(int i=0;i<mZXBWindowCon.nFLView;i++)
                    {
                        mZXBWindowCon.nFLViewStationID.append(StringToHex(strArr[1+i]));
                    }
                    if(strArr[strArr.size()-1]=="h"||strArr[strArr.size()-1]=="H")
                    {
                        mZXBWindowCon.bFLHV=true;
                    }
                    else if(strArr[strArr.size()-1]=="t"||strArr[strArr.size()-1]=="T")
                    {
                        mZXBWindowCon.bFLHV=false;
                    }
                }
            }
            else if(m_str.left(7)=="DCView=")
            {
                getString = m_str.mid(7,m_str.indexOf(";")-m_str.indexOf("=")-1);
                QStringList strArr;
                int c = StringSplit(getString, "|", strArr);
                if(c == 2+strArr[0].toInt())
                {
                    mZXBWindowCon.nDCView=strArr[0].toInt();
                    for(int i=0;i<mZXBWindowCon.nDCView;i++)
                    {
                        mZXBWindowCon.nShuntViewStationID.append(StringToHex(strArr[1+i]));
                    }
                    if(strArr[strArr.size()-1]=="h"||strArr[strArr.size()-1]=="H")
                    {
                        mZXBWindowCon.bDCHV=true;
                    }
                    else if(strArr[strArr.size()-1]=="t"||strArr[strArr.size()-1]=="T")
                    {
                        mZXBWindowCon.bDCHV=false;
                    }
                }
            }
        }
    }
    file.close();
}

//将接收到的数据根据帧头帧位进行分割并返回分割后的二维数组
QVector<QByteArray> MyDoc::SplitReceiveData_SameHeadTail(QByteArray recvArray)
{
    QVector<QByteArray> dataArray;//用于存放分割后的数据
    QByteArray tempArray;//临时数组

    //长度不够则退出
    if(recvArray.size()<14)
    {
        return dataArray;
    }

    int head = 0xEF;//帧头
    int tail = 0xFE;//帧尾
    QVector<int> header;
    header.append(head);
    header.append(head);
    header.append(head);
    header.append(head);
    QVector<int> ender;
    ender.append(tail);
    ender.append(tail);
    ender.append(tail);
    ender.append(tail);
    int index_Header=0;
    int dataLen=0;//数据长度包含帧头和帧尾

    //从头开始遍历数据帧
    for(int i=0; i<recvArray.size(); i++)
    {
        //比对帧头
        bool headFlag=true;
        for(int j=0;j<header.size();j++)
        {
            if(i+j+ender.size() >= recvArray.size())//越界防护
            {
                headFlag=false;
                break;
            }
            if((int)header[j] != (int)(recvArray[i+j]&0xFF))
            {
                headFlag=false;
                break;
            }
        }
        if(!headFlag)
        {
            continue;
        }

        //记录帧头开始位置
        index_Header = i;
        //数据长度包含帧头和帧尾
        dataLen = (int)((recvArray[i+header.size()]&0xFF) | ((recvArray[i+header.size()+1]&0xFF)<<8));
        //帧头+内容+帧尾
        if(i+dataLen>recvArray.size())//越界防护
        {
            break;
        }
        //将i指向帧尾开始的位置，避免不必要的循环
        i+= (dataLen-ender.size());

        //比对帧尾
        bool endFlag=true;
        for(int j=0; j<ender.size(); j++)
        {
            if(i+j>=recvArray.size())  //简单防护
            {
                endFlag=false;
                break;
            }
            if(ender[j] != (int)(recvArray[i+j]&0xFF))
            {
                endFlag=false;
                break;
            }
        }
        if(endFlag == false)
        {
            continue;//帧尾校验失败则不作为帧尾处理
        }

        //加入整包数据
        tempArray.append(recvArray.data()+index_Header, dataLen);
        dataArray.append(tempArray);
        tempArray.clear();

        i+= (ender.size());//到帧尾结束的位置
        i--;
    }

    return dataArray;
}

//培训软件-根据设备编号获取设备名称
QString MyDoc::getNameOfDevNode(MyStation* pStation, unsigned int devnode)
{
    for(auto ement:pStation->DevArray)
    {
        if(ement->getDevType() == Dev_DC)
        {
            CGDDC *gddc=(CGDDC *)ement;
            if(devnode == (int)gddc->getCode())
            {
                return gddc->getName();
            }
        }
        else if(ement->getDevType() == Dev_XH)
        {
            CXHD *xhd=(CXHD *)ement;
            if(devnode == (int)xhd->getCode())
            {
                return xhd->getName();
            }
        }
        else if(ement->getDevType() == Dev_GD)
        {
            CGD *gd=(CGD*)ement;
            if(devnode == (int)gd->getCode())
            {
                return gd->getName();
            }
        }
    }
    return "";
}
//培训软件-根据设备名称获取设备编号
unsigned int MyDoc::getDevNodeOfName(MyStation* pStation, QString strName)
{
    for(auto ement:pStation->DevArray)
    {
        if(ement->getDevType() == Dev_DC)
        {
            CGDDC *gddc=(CGDDC *)ement;
            if(strName == gddc->getName())
            {
                return gddc->getCode();
            }
        }
        else if(ement->getDevType() == Dev_XH)
        {
            CXHD *xhd=(CXHD *)ement;
            if(strName == xhd->getName())
            {
                return xhd->getCode();
            }
        }
        else if(ement->getDevType() == Dev_GD)
        {
            CGD *gd=(CGD*)ement;
            if(strName == gd->getName())
            {
                return gd->getCode();
            }
        }
    }
    return 65535;
}
bool MyDoc::SetServiceCommState()
{
    bool b=false;
    bFlagCommToServer=false;
    for(int i=0;i<vectMyStation.size();i++)
    {
        if(vectMyStation[i]->commToServer>0)
        {
            b=true;
        }
        else
        {
            b=false;
        }
        bFlagCommToServer=bFlagCommToServer|b;
    }
    return bFlagCommToServer;
}
