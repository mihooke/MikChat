#include <muduo/net/Buffer.h>
#include "clientsender.h"

ClientSender::ClientSender(const muduo::net::TcpConnectionPtr &conn)
    : m_conn(conn)
{

}

void ClientSender::sendMsg(const char *msg)
{
    if (msg == nullptr)
    {
        return;
    }
    if (strlen(msg) <= 0)
    {
        return;
    }

    m_conn->send(msg);
}

void ClientSender::sendMsg(muduo::net::Buffer *msg)
{
    if (msg == nullptr)
    {
        return;
    }

    m_conn->send(msg);
}
