#include <functional>
#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include "mikchatserver.h"
#include "clienthandler.h"
#include "clientreceiver.h"

using namespace muduo::net;

MikChatServer::MikChatServer()
{

}

MikChatServer::~MikChatServer()
{

}

MikChatServer &MikChatServer::Instance()
{
    static MikChatServer singleInstance;
    return singleInstance;
}

bool MikChatServer::Initialize(EventLoop *loop, const InetAddress &address)
{
    m_server.reset(new TcpServer(loop, address, "MikChatServer"));
    m_server->setConnectionCallback(bind(&MikChatServer::onConnection, this, std::placeholders::_1));
    return true;
}

void MikChatServer::start()
{
    m_server->start();
}

void MikChatServer::onConnection(const TcpConnectionPtr &conn)
{
    if (conn->connected())
    {
        std::shared_ptr<ClientReceiver> clientHandler(new ClientReceiver(conn));
        conn->setMessageCallback(bind(&ClientReceiver::onReceive, clientHandler.get(), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        m_clients.push_back(clientHandler);
    }
    else
    {
        closeConnection(conn);
    }
}

void MikChatServer::closeConnection(const muduo::net::TcpConnectionPtr &conn)
{
    ;
}
