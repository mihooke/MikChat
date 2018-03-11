#ifndef MIK_CLIENTRECEIVER_H
#define MIK_CLIENTRECEIVER_H

/*
 * ClientReceiver是处理客户端发送的消息，对于服务器来说，基本上所有的消息都是命令式消息，
 * 即：客户端发来一个消息，要求服务器处理，并返回给客户端一个结果。
 *
 * 客户端发送消息格式是：
 *     @命令类型（int） + 消息（json字符串）
 * 服务端发送消息格式是：
 *     @命令类型（int） + 消息（json字符串）
 */
#include <memory>
#include <muduo/net/TcpConnection.h>
#include <muduo/base/Timestamp.h>
#include <muduo/net/Buffer.h>
#include <jsoncpp/json/json.h>

#include "usersmanager.h"
#include "clienthandler.h"

enum CmdType
{
    ACCOUNTREGISTER = 1,
    ACCOUNTLOGIN    = 2,
    LOGINVERIFYACCOUNT = 3,
    CHATMESSAGE     = 4,
    TRANSMITCHATMSG = 5,
    NOTIFYFRIENDSONLINE = 6,
    UPDATEPERSONALINFOS = 7,
};

class ClientSender;
class ClientReceiver : public ClientHandler
{
public:
    ClientReceiver(const muduo::net::TcpConnectionPtr &conn);
    muduo::net::Buffer toStream(const int32_t cmdType, Json::Value &jsonItem);

    void onReceive(const muduo::net::TcpConnectionPtr &conn,
                   muduo::net::Buffer *buffer,
                   muduo::Timestamp time);
    muduo::net::TcpConnectionPtr getReceiverConn() const { return m_conn; }
    std::shared_ptr<ClientSender> getConnSender() const { return m_connSender; }
    UserInfomations  getUserInfos() const { return m_ui; }

private:
    void loginProc(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buffer);
    void registerProc(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buffer);
    void chatMessageProc(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buffer);
    void updatePersonalInfos(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buffer);

    void responseLogin(const std::string &name, const std::string &pwd);
    bool checkUserNameExist(const std::string &userName);
    void notifyFriendsOnline(const muduo::net::TcpConnectionPtr &conn);
    void transmitChatMsg(std::shared_ptr<ClientReceiver> receiver, const std::string &chatMsg);
private:
    muduo::net::TcpConnectionPtr m_conn;
    std::shared_ptr<ClientSender> m_connSender;
    UserInfomations m_ui;
};

#endif // MIK_CLIENTRECEIVER_H
