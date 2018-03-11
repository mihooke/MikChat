#ifndef MIK_MIKCHATSERVER_H
#define MIK_MIKCHATSERVER_H

/*
 * MikChatServer是主服务类，负责调度从客户端接收的TCP连接。
 */
#include <list>
#include <memory>
#include <muduo/net/TcpServer.h>
#include <muduo/net/TcpConnection.h>

class ClientReceiver;
class MikChatServer final
{
public:
    MikChatServer();
    ~MikChatServer();
    MikChatServer(const MikChatServer &) = delete;
    MikChatServer &operator=(const MikChatServer &) = delete;

    static MikChatServer &Instance();
    bool Initialize(muduo::net::EventLoop *loop,
                    const muduo::net::InetAddress &address);

    void start();

    const std::list<std::shared_ptr<ClientReceiver> > getAllClients() const { return m_clients; }
private:
    // 转发client的连接，在ClientReceiver中解析请求
    void onConnection(const muduo::net::TcpConnectionPtr &conn);

    void closeConnection(const muduo::net::TcpConnectionPtr &conn);

private:
    std::shared_ptr<muduo::net::TcpServer> m_server;
    // m_clients保存了所有在线client连接
    std::list<std::shared_ptr<ClientReceiver> > m_clients;
};

#endif // MIK_MIKCHATSERVER_H
