#ifndef MIK_CLIENTSENDER_H
#define MIK_CLIENTSENDER_H

/*
 * ClientSender是客户端消息发送类，负责响应客户端请求。ClientSender与ClientReceiver持有共同的connection.
 */
#include <muduo/net/TcpConnection.h>
#include <muduo/net/Buffer.h>

#include "clienthandler.h"

class ClientSender
{
public:
    ClientSender(const muduo::net::TcpConnectionPtr &conn);

    void sendMsg(const char *msg);
    void sendMsg(muduo::net::Buffer *msg);

private:
    muduo::net::TcpConnectionPtr m_conn;
};

#endif // MIK_CLIENTSENDER_H
