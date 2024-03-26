#include "sockettcp.h"
#include <QHostAddress>
#include <QTimerEvent>
#include "GlobalHeaders/GlobalFuntion.h"
#include "Log/log.h"

SocketTCP::SocketTCP(QObject *parent) : QObject(parent)
{
    ipAdress = "";
    port = 0;
    tcpSocket = new QTcpSocket(this);
}

SocketTCP::~SocketTCP()
{

}

//初始化固定IP的端口(和服务端建立连接)
bool SocketTCP::initByIP(QString ipAdress, int port)
{
    this->ipAdress = ipAdress;
    this->port = port;
    connect(tcpSocket, &QTcpSocket::connected, this, [=](){
        QLOG_INFO()<<QString("TCP connect to %1:%2 succeeded!").arg(ipAdress).arg(port);
        if(timer >= 0)
        {
            //连接成功则kill定时器
            killTimer(timer);
            timer = -1;
        }
        //主动发送连接成功信号
        emit onConnectedSignal(ipAdress, port);
    });
    //连接
    tcpSocket->connectToHost((QHostAddress)ipAdress, port);
    //启动定时器去连接，若首次没连接则定时尝试去连接，连接成功则kill定时器
    timer = startTimer(1000);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &SocketTCP::recvDataSlot);
    connect(tcpSocket, &QTcpSocket::disconnected, this, &SocketTCP::disconnectedSlot);

    return true;
}

//断开链接槽
void SocketTCP::disconnectedSlot()
{
    QLOG_WARN()<<QString("TCP disconnected from %1:%2 !").arg(ipAdress).arg(port);
    //发送信号（供外部使用）
    emit onDisconnectedSignal(ipAdress, port);
    timer = startTimer(1000);
}

//接收数据槽
void SocketTCP::recvDataSlot()
{
    QString clientInfo = QString("Recv from %1:%2").arg(ipAdress).arg(QString::number(port));
    QByteArray buffer;
    //获取套接字中的内容
    buffer = tcpSocket->readAll();
    //发送信号（供外部使用）
    emit recvDataSignal(buffer, ipAdress, port);
}

//发送数据槽
void SocketTCP::sendDataSlot(QByteArray dataArray, int len)
{
    tcpSocket->write(dataArray);
}

void SocketTCP::timerEvent(QTimerEvent *event)
{
    if(timer == event->timerId())
    {
        tcpSocket->connectToHost((QHostAddress)ipAdress, port);
    }
}
