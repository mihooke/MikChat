#ifndef MIK_CLIENTHANDLER_H
#define MIK_CLIENTHANDLER_H

/*
 * ClientHandler类是客户端处理基类，所有与客户端打交道的类都需继承此类。
 */
#include <memory>
#include "muduo/net/TcpConnection.h"

class ClientHandler
{
public:
    ClientHandler(const muduo::net::TcpConnectionPtr &conn);
    virtual ~ClientHandler();

protected:
    muduo::net::TcpConnectionPtr m_connection;
};

#endif // MIK_CLIENTHANDLER_H
