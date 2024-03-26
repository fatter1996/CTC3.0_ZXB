#include "socketudp.h"
#include "GlobalHeaders/GlobalFuntion.h"

SocketUDP::SocketUDP(QObject *parent) : QObject(parent)
{

}

//初始化固定IP的端口
bool SocketUDP::initByIP(QString ipAdress, int port)
{
    socketUDP = new QUdpSocket();
    //绑定本地接收UDP socket通道
    if(true == socketUDP->bind(QHostAddress(ipAdress),port))
    {
        connect(socketUDP,SIGNAL(readyRead()),this,SLOT(recvDataSlot()));
        return true;
    }
    else
    {
        qCritical()<<QString("init UDP socket %1:%2 failed!").arg(ipAdress).arg(port);
        return false;
    }
}
//初始化所有本机IP的端口
bool SocketUDP::initAnyIP(int port)
{
    socketUDP = new QUdpSocket();
    //绑定本地接收UDP socket通道
    if(true == socketUDP->bind(QHostAddress::Any,port))
    {
        connect(socketUDP,SIGNAL(readyRead()),this,SLOT(recvDataSlot()));
        return true;
    }
    else
    {
        qCritical()<<QString("init UDP socket Any:%1 failed!").arg(port);
        return false;
    }
}

//打包帧头帧尾标志
void SocketUDP::packHeadAndEnd(QByteArray &dataArray, int len)
{
    for(int i=0;i<4;i++)
    {
        dataArray[i]=0xEF;
    }
    for(int i=1;i<5;i++)
    {
        dataArray[len-i]=0xFE;
    }
    dataArray[4]=(len);
    dataArray[5]=(len>>8);
}

//接收数据槽
void SocketUDP::recvDataSlot()
{
    QByteArray recvData;
    //client ip addr
    QHostAddress clientAddress;
    QString strClientAddress;
    //client port
    quint16 clientPort;
    recvData.clear();
    while(socketUDP->hasPendingDatagrams()/*&&socketUDP->pendingDatagramSize()>0*/)
    {
        recvData.resize(socketUDP->pendingDatagramSize());
        socketUDP->readDatagram(recvData.data(), recvData.size(), &clientAddress, &clientPort);
        QString a=clientAddress.toString();
        strClientAddress = clientAddress.toString();
        int aq=socketUDP->pendingDatagramSize();
        //发送信号
        emit recvDataSignal(recvData, strClientAddress, clientPort);
    }
}

//发送数据槽
void SocketUDP::sendDataSlot(QByteArray dataArray, QString oppAdress, int oppProt, int len)
{
    packHeadAndEnd(dataArray, len);
    socketUDP->writeDatagram(dataArray, QHostAddress(oppAdress), oppProt);
}
