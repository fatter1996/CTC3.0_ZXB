#include "servertcp.h"

ServerTCP::ServerTCP(QObject *parent) : QObject(parent)
{
    tcpServer = new QTcpServer(this);
}

ServerTCP::~ServerTCP()
{
    //清除
    vectTcpSocket.clear();
    if(tcpServer)
    {
        //关闭服务
        tcpServer->close();
        tcpServer = nullptr;
    }
}
//初始化固定IP的端口
bool ServerTCP::initByIP(QString ipAdress, int port)
{
    //绑定本地IP和端口
    bool res = tcpServer->listen(QHostAddress(ipAdress), port);
    if(res)
    {
        connect(tcpServer, &QTcpServer::newConnection, this, &ServerTCP::connectToClientSlot);
    }
    else
    {
        qCritical()<<QString("init TCP listen %1:%2 failed!").arg(ipAdress).arg(port);
    }
    return res;
}
//初始化所有本机IP的端口
bool ServerTCP::initAnyIP(int port)
{
    //绑定本地IP和端口
    bool res = tcpServer->listen(QHostAddress::Any, port);
    if(res)
    {
        connect(tcpServer, &QTcpServer::newConnection, this, &ServerTCP::connectToClientSlot);
    }
    else
    {
        qCritical()<<QString("init TCP listen Any:%1 failed!").arg(port);
    }
    return res;
}
//和客户端建立连接的槽
void ServerTCP::connectToClientSlot()
{
    //取出连接好的套接字
    QTcpSocket* tcpSocketClient = tcpServer->nextPendingConnection();
    //获得通信套接字的控制信息
    QString ipAdress = tcpSocketClient->peerAddress().toString().split("::ffff:")[1];//获取连接的 ip地址
    quint16 port = tcpSocketClient->peerPort();//获取连接的 端口号
    //连接信号槽
    connect(tcpSocketClient, &QTcpSocket::readyRead, this, &ServerTCP::recvDataSlot);
    connect(tcpSocketClient, &QTcpSocket::disconnected, this, &ServerTCP::onDisconnectedSlot);
    //增加该链接
    vectTcpSocket.append(tcpSocketClient);
}
//和客户端断开连接的槽
void ServerTCP::onDisconnectedSlot()
{
    //获取信号发送者
    QTcpSocket *tcpSocketClient = qobject_cast<QTcpSocket*>(sender());
    for(int i=0; i<vectTcpSocket.size(); i++)
    {
        QTcpSocket* tcpSocket = vectTcpSocket.at(i);
        if(tcpSocket->socketDescriptor() == tcpSocketClient->socketDescriptor())
        {
            //移除客户端链接
            vectTcpSocket.removeAt(i);
            return;
        }
    }
}
//接收数据槽
void ServerTCP::recvDataSlot()
{
    //获取信号发送者
    QTcpSocket *tcpSocketClient = qobject_cast<QTcpSocket*>(sender());
    //获得通信套接字的控制信息
    QString ipAdress = tcpSocketClient->peerAddress().toString().split("::ffff:")[1];//获取连接的 ip地址
    quint16 port = tcpSocketClient->peerPort();//获取连接的 端口号
    QString clientInfo = QString("Recv from %1:%2").arg(ipAdress).arg(QString::number(port));
    QByteArray buffer;
    //获取套接字中的内容
    buffer = tcpSocketClient->readAll();
    QString recvMsg = QString::fromLocal8Bit(buffer);
    //发送信号（供外部使用）
    emit recvDataSignal(buffer, ipAdress, port);
}
//发送数据槽
void ServerTCP::sendDataSlot(QByteArray dataArray, int len)
{
    //TODO ?? 如何发给指定的客户端

    //发给所有（暂时）
    for(int i=0; i<vectTcpSocket.size(); i++)
    {
        QTcpSocket* tcpSocket = vectTcpSocket.at(i);
        if(tcpSocket->write(dataArray) != len)
        {
            continue;
        }
    }
}


