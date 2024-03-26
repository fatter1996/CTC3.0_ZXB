#include "mydoc.h"
#include <QtDebug>
#include <QSettings>
#include <QMessageBox>
#include "GlobalHeaders/GlobalFuntion.h"

//发送数据给CTC逻辑服务端
void MyDoc::sendDataToCTCLogic2(QByteArray pSendDate, int nLength)
{
   emit sendDataToMainSignal2(pSendDate,nLength);
}
//槽-发送数据给CTC后台服务逻辑-TCP通道
void MyDoc::sendDataToCTCServerSlot2(MyStation *pMyStation, QByteArray dataArray, int nLength)
{
    sendDataToCTCLogic2(dataArray,nLength);
}
//TCP连接槽
void MyDoc::tcpClientConnectedSlot(QString clientAdd, int clientPort)
{
    QLOG_INFO()<<QString("TCP连接成功Server[%1:%2]").arg(clientAdd).arg(clientPort);
    //重新请求数据，后续定时器会请求数据
    this->bRequstData = true;
}
//TCP断开槽
void MyDoc::tcpClientDisconnectedSlot(QString clientAdd, int clientPort)
{
    QLOG_WARN()<<QString("TCP连接断开Server[%1:%2]").arg(clientAdd).arg(clientPort);
    //清空所有数据
    for (int i = 0; i < vectMyStation.size(); i++)
    {
        MyStation* pStation = vectMyStation.at(i);
        //debug
        //站场重置
        pStation->commToServer=5;
        pStation->ResetStationInfo();
        emit pStation->TrainRouteOrderUpData(SYNC_FLAG_DELALL);
        emit pStation->TrafficLogUpData(SYNC_FLAG_DELALL);
//        emit ResetStationSignal();
//        emit StagePlanUpData();
    }
    //QMessageBox::warning(NULL, "提示", COMMUNICATION_LOST_MSG);
}
//解析服务端socket通道数据-TCP
void MyDoc::receiveCTCServerDataSlot2(QByteArray dataArray,QString clientAdd, int clientPort)
{
    int recvLength=dataArray.size();
    if(recvLength < 14)
    {
        return;   //无效数据
    }
    //校验和分割收到的数据
    QVector<QByteArray> recvSpitArr = SplitReceiveData_SameHeadTail(dataArray);
    //遍历数据
    for(int i=0; i<recvSpitArr.size(); i++)
    {
        QByteArray dataArray1 = recvSpitArr[i];
        //车站id
        int nStaid = (int)(dataArray1[6]&0xFF);
        //判断id是否已配置
        MyStation* pStation = getMyStationByStaIDInStaArray(nStaid);
        //功能分类码
        int dataType = (int)(dataArray1[9]&0xFF);
        //数据同步
        if(dataType == FUNCTYPE_SYNC)
        {
            //同步类型
            int syncType = (int)(dataArray1[10]&0xFF);
            //同步标志
            int syncFlag = (int)(dataArray1[11]&0xFF);
            //总包数
            int totalPk = (int)((dataArray1[12]&0xFF)|((dataArray1[13]&0xFF)<<8));
            //当前包号
            int currPkNum = (int)((dataArray1[14]&0xFF)|((dataArray1[15]&0xFF)<<8));
            //阶段计划
            if(syncType == SYNC_JDJH)
            {
                if (pStation)
                {
                    pStation->updateStagePlan(dataArray1, syncFlag);
                    qDebug() << "阶段计划" << dataArray1.toHex();
                    //emit StagePlanUpData();
                }
            }
            //列车进路序列
            else if(syncType == SYNC_JLXL)
            {
                if (pStation)
                {
                    pStation->updateTrainRouteOrder(dataArray1, syncFlag);
                    pStation->emit TrainRouteOrderUpData(syncFlag, ByteArrayToUInt(dataArray1.mid(16, 2)));
                    qDebug() << "列车进路序列" << dataArray1.toHex();
                }
            }
            //行车日志
            else if(syncType == SYNC_XCRZ)
            {
                if (pStation)
                {
                    qDebug() << "行车日志" << dataArray1.toHex();
                    pStation->updateTrafficLog(dataArray1, syncFlag);
                    emit pStation->TrafficLogUpData(syncFlag, ByteArrayToUInt(dataArray1.mid(16, 2)));
                }
            }
            //调度命令
            else if(syncType == SYNC_DDML)
            {
                if (pStation)
                {
                    pStation->updateDisOrder(dataArray1, syncFlag);
                    qDebug() << "调度命令" << dataArray1.toHex();
                }
            }
            //股道防溜
            else if(syncType == SYNC_GDFL)
            {
                if (pStation)
                {
                    pStation->updateGDAntiSlip(dataArray1, syncFlag);
                    emit pStation->StationStorageUpData();
                }
                qDebug() << "股道防溜1" << dataArray1.toHex();
            }
            //防错办
            else if(syncType == SYNC_FCB)
            {
                if (pStation)
                {
                    pStation->updateFCBAttr(dataArray1, syncFlag);
                    qDebug() << "防错办" << dataArray1.toHex();
                }
            }
        }
    }
}
//防溜设置槽
void MyDoc::SendAntiSlipSolt(GDData* pGDData,bool bSet,int nstationid)
{
    bool bSetLR=true;
    //设置
//   if(bSet)
//   {
//       //发送左侧
//       bSetLR=true;
//       sendAntiSlipDataToServer(pGDData,bSet,bSetLR,nstationid);
//       //发送右侧
//       bSetLR=false;
//       sendAntiSlipDataToServer(pGDData,bSet,bSetLR,nstationid);
//   }
//   else//撤销
//   {
//       sendAntiSlipDataToServer(pGDData,bSet,bSetLR,nstationid);
//   }
    //发送左侧
    sendAntiSlipDataToServer(pGDData,bSet,bSetLR,nstationid);
    //发送右侧
    bSetLR=false;
    sendAntiSlipDataToServer(pGDData,bSet,bSetLR,nstationid);
}
void MyDoc::sendAntiSlipDataToServer(GDData* pGDData,bool bSet,bool bSetLR,int nstationid)
{
    char SendArray[1000]={0};//设备状态数据数组
    unsigned char SendArray1[1000]={0};
    int frame_number;//帧长度
    SendArray1[0]=SendArray1[1]=SendArray1[2]=SendArray1[3]=0xEF;//帧头
    //本站地址码//本站ID
    SendArray1[6]=nstationid;
//    SendArray1[7]=TargetAddr;//目标地址码//低四位表示A/B机的序号，从1开始，1~10；高四位表示岗位，单机版值班员0，值班员1，信号员2；
    SendArray1[7]=getMyStationByStaIDInStaArray(nstationid)->ABNUM;
    SendArray1[8]=DATATYPE_BOARD;//数据标志//0xAA联锁，0xBB教师机，0xCC为CTC，0xDD为集控，0xCD占线板，0xAB培训软件
    SendArray1[9]=0x39;
    SendArray1[10]=0x01;//防溜操作
    memcpy(SendArray1+11,&pGDData->pGD->m_nCode,2);//股道设备号

    int FLSZ_TXNum=0;
    int FLSZ_JGQNum=0;
    int FLSZ_JNMS=0;
    int FLSZ_Type=0;
    //左侧或右侧防溜
    if(bSetLR)
    {
        SendArray1[13]=0xAA;//0xAA左侧，0xBB右侧
        FLSZ_TXNum=pGDData->pGD->m_nLTxNum;
        FLSZ_JGQNum=pGDData->pGD->m_nLJgqNum;
        FLSZ_JNMS=pGDData->pGD->m_nLJnMeters;
        //铁鞋
        if(pGDData->pGD->bIronShoesSetS)
        {
            FLSZ_Type=FLSZ_Type|0x01;
        }
        //人力制动机
        if(pGDData->pGD->nManualBrakeS==1)
        {
            FLSZ_Type=FLSZ_Type|0x02;
        }
        else if(pGDData->pGD->nManualBrakeS==2)
        {
            FLSZ_Type=FLSZ_Type|0x04;
        }
    }
    else
    {
        SendArray1[13]=0xBB;//0xAA左侧，0xBB右侧
        FLSZ_TXNum=pGDData->pGD->m_nRTxNum;
        FLSZ_JGQNum=pGDData->pGD->m_nRJgqNum;
        FLSZ_JNMS=pGDData->pGD->m_nRJnMeters;
        //铁鞋
        if(pGDData->pGD->bIronShoesSetX)
        {
            FLSZ_Type=FLSZ_Type|0x01;
        }
        //人力制动机
        if(pGDData->pGD->nManualBrakeX==1)
        {
            FLSZ_Type=FLSZ_Type|0x02;
        }
        else if(pGDData->pGD->nManualBrakeX==2)
        {
            FLSZ_Type=FLSZ_Type|0x04;
        }
    }

    //设置或取消防溜
    SendArray1[14]=bSet? 0xAA : 0xBB;;//0xAA设置防溜，0xBB取消防溜
    //防溜设备类型
    SendArray1[15]=FLSZ_Type;//0x01铁鞋，0x02人力制动机，0x04人力制动机紧固器，0x12铁鞋+人力制动机（即0x01|0x02），0x14铁鞋+人力制动机紧固器（即0x01|0x04）
    //铁鞋号
    memcpy(SendArray1+16,&FLSZ_TXNum,1);
    //紧固器号
    memcpy(SendArray1+17,&FLSZ_JGQNum,1);
    //警内米数
    memcpy(SendArray1+18,&FLSZ_JNMS,2);
    //存车信息 旧版本存车信息为int类型 占2个字节
    memcpy(SendArray1+20,&pGDData->pGD->m_nTrainNums,2);
    int nindex=22;

    //存车信息 新版本 字符串格式 协议新增的
    QByteArray strTrainInfoZCC = pGDData->pGD->m_sTrainInfoShow.toLocal8Bit();//toLatin1();
    int nTrainInfoZCClen = strTrainInfoZCC.count();
     memcpy(SendArray1+nindex,&nTrainInfoZCClen,2);
    nindex++;
    nindex++;
    for(int u=0; u<nTrainInfoZCClen; u++)
    {
        nindex++;
        SendArray1[nindex] = strTrainInfoZCC[u];
    }
    SendArray1[nindex]=SendArray1[nindex+1]=SendArray1[nindex+2]=SendArray1[nindex+3]=0xFE;
    frame_number=nindex+4;
    memcpy(SendArray1+4,&frame_number,2);//帧长度
    memcpy(SendArray,SendArray1,frame_number);
    QByteArray dataArray;
    dataArray.resize(frame_number);
    memcpy(dataArray.data(),SendArray,frame_number);  //转换数据类型
    sendDataToCTCLogic(dataArray,frame_number);
}
//占线版向服务端发送供电臂设置信息
void MyDoc::SendPowerSupplyArmDataToSeverData(int nStaIndex, int nID, int bset)
{
    char SendArray[1000]={0};//设备状态数据数组
    unsigned char SendArray1[1000]={0};
    int frame_number=18;//帧长度
    SendArray1[0]=SendArray1[1]=SendArray1[2]=SendArray1[3]=0xEF;//帧头
    memcpy(SendArray1+4,&frame_number,2);//帧长度

    //本站地址码//本站ID
    SendArray1[6]=nStaIndex;
//    SendArray1[7]=TargetAddr;//目标地址码//低四位表示A/B机的序号，从1开始，1~10；高四位表示岗位，单机版值班员0，值班员1，信号员2；
    SendArray1[7]=getMyStationByStaIDInStaArray(nStaIndex)->ABNUM;
    SendArray1[8]=DATATYPE_BOARD;//数据标志//0xAA联锁，0xBB教师机，0xCC为CTC，0xDD为集控，0xCD占线板，0xAB培训软件
    SendArray1[9]=0x88;
    SendArray1[10]=bset;//功能类型_供电臂设置
    memcpy(SendArray1+11,&nID,2);//设备号
    SendArray1[13]=0x00;
    SendArray1[14]=SendArray1[15]=SendArray1[16]=SendArray1[17]=0xFE;
    memcpy(SendArray,SendArray1,18);
    QByteArray dataArray;
    dataArray.resize(frame_number);
    memcpy(dataArray.data(),SendArray,frame_number);  //转换数据类型
    sendDataToCTCLogic(dataArray,frame_number);
}
//占线版向服务端发送封锁设备设置信息
void MyDoc::SendBlockingDevDataToSeverData(int nStaIndex,int nID,bool bset)
{
    char SendArray[1000]={0};//设备状态数据数组
    unsigned char SendArray1[1000]={0};
    int frame_number=18;//帧长度
    SendArray1[0]=SendArray1[1]=SendArray1[2]=SendArray1[3]=0xEF;//帧头
    memcpy(SendArray1+4,&frame_number,2);//帧长度

    //本站地址码//本站ID
    SendArray1[6]=nStaIndex;
//    SendArray1[7]=TargetAddr;//目标地址码//低四位表示A/B机的序号，从1开始，1~10；高四位表示岗位，单机版值班员0，值班员1，信号员2；
    SendArray1[7]=getMyStationByStaIDInStaArray(nStaIndex)->ABNUM;
    SendArray1[8]=DATATYPE_BOARD;//数据标志//0xAA联锁，0xBB教师机，0xCC为CTC，0xDD为集控，0xCD占线板，0xAB培训软件
    SendArray1[9]=0x88;
    SendArray1[10]=bset==true?0x10:0x11;//功能类型_供电臂设置
    memcpy(SendArray1+11,&nID,2);//设备号
    SendArray1[13]=0x00;
    SendArray1[14]=SendArray1[15]=SendArray1[16]=SendArray1[17]=0xFE;
    memcpy(SendArray,SendArray1,18);
    QByteArray dataArray;
    dataArray.resize(frame_number);
    memcpy(dataArray.data(),SendArray,frame_number);  //转换数据类型
    sendDataToCTCLogic(dataArray,frame_number);
}
