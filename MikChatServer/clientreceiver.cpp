#include <muduo/base/Logging.h>
#include "clientreceiver.h"
#include "clientsender.h"
#include "mikchatserver.h"

using namespace muduo;
using namespace muduo::net;
using namespace Json;
ClientReceiver::ClientReceiver(const TcpConnectionPtr &conn)
    : ClientHandler(conn), m_connSender(new ClientSender(conn)), m_conn(conn)
{

}

Buffer ClientReceiver::toStream(const int32_t cmdType, Value &jsonItem)
{
    Buffer msg;
    msg.appendInt32(cmdType);
    msg.append(StringPiece(jsonItem.toStyledString()));
    return msg;
}

void ClientReceiver::onReceive(const TcpConnectionPtr &conn,
                               Buffer *buffer,
                               Timestamp time)
{
    int32_t cmdType = buffer->readInt32();
    LOG_INFO << cmdType;

    switch (cmdType)
    {
    case ACCOUNTLOGIN:
        loginProc(conn, buffer);
        break;
    case ACCOUNTREGISTER:
        registerProc(conn, buffer);
        break;
    case CHATMESSAGE:
        chatMessageProc(conn, buffer);
        break;
    case UPDATEPERSONALINFOS:
        updatePersonalInfos(conn, buffer);
        break;
    default:
        break;
    }
}

void ClientReceiver::loginProc(const TcpConnectionPtr &conn, Buffer *buffer)
{
    /// 不带std的string是muduo库定义的类型
    string receiveMsg;
    Reader jsonReader;
    Value loginRequest;
    receiveMsg = buffer->retrieveAllAsString();
    LOG_INFO << receiveMsg;
    /// 验证密码是否正确
    std::string loginAccountNum, loginAccountPassword;
    std::string recvMsg(receiveMsg.c_str());
    if (jsonReader.parse(recvMsg, loginRequest))
    {
        loginAccountNum = loginRequest["account_num"].asString();
        loginAccountPassword = loginRequest["account_password"].asString();
    }
    LOG_INFO << loginAccountNum << " num & pwd " << loginAccountPassword;

    /// 响应客户端登录请求
    responseLogin(loginAccountNum, loginAccountPassword);

    /// 通知好友上线
    notifyFriendsOnline(conn);
}

void ClientReceiver::responseLogin(const std::string &name, const std::string &pwd)
{
    ClientLoginRetStatus status = UsersManager::Instance().checkAccountExist(name, pwd);
    Value jsonItem;
    /// 如果登录成功，则把账户信息发送到客户端
    if (ClientLoginRetStatus::LOGINOK == status)
    {
        jsonItem["status"] = "loginok";
        m_ui = UsersManager::Instance().getUserInfosByAccount(name);
        Value jsonInfos;

        jsonInfos["u_user_sex"] = m_ui.u_user_sex;
        jsonInfos["u_user_id"] = m_ui.u_user_id;
        jsonInfos["u_user_name"] = m_ui.u_user_name;
        jsonInfos["u_user_nickname"] = m_ui.u_user_nickname;
        jsonInfos["u_user_password"] = m_ui.u_user_password;
        jsonInfos["u_user_facetype"] = m_ui.u_user_facetype;
        jsonInfos["u_user_customface"] = m_ui.u_user_customface;
        jsonInfos["u_user_birthday"] = m_ui.u_user_birthday;
        jsonInfos["u_user_email"] = m_ui.u_user_email;
        jsonInfos["u_user_remark"] = m_ui.u_user_remark;
        jsonInfos["u_user_last_login_time"] = m_ui.u_user_last_login_time;
        std::set<int> friendShip = UsersManager::Instance().getFriendShipById(m_ui.u_user_id);
        Value jsonShipArray;
        for (const int id : friendShip)
        {
            Value jsonShip;
            std::string user_name = UsersManager::Instance().getUserNameById(id);
            jsonShip["friend"] = user_name;
            jsonShipArray.append(jsonShip);
        }
        jsonInfos["friends"] = jsonShipArray;
        jsonItem["user_infos"] = jsonInfos;
    }
    else if (ClientLoginRetStatus::PASSWORDERROR == status)
    {
        jsonItem["status"] = "passworderror";
    }
    else if (ClientLoginRetStatus::ACCOUNTNOTEXIST == status)
    {
        jsonItem["status"] = "accountnotexist";
    }
    else if (ClientLoginRetStatus::LOGINSERVERISSUE == status)
    {
        jsonItem["status"] = "loginserverissue";
    }
    else if (ClientLoginRetStatus::LOGINNOTDEFINEERROR == status)
    {
        jsonItem["status"] = "loginnotdefineerror";
    }
    Buffer msg = toStream(int32_t(LOGINVERIFYACCOUNT), jsonItem);
    m_connSender->sendMsg(&msg);
}

bool ClientReceiver::checkUserNameExist(const std::string &userName)
{
    for (int friendId : m_ui.f_friendship)
    {
        std::string name  = UsersManager::Instance().getUserNameById(friendId);
        if (name == userName)
        {
            return true;
        }
    }
    return false;
}

void ClientReceiver::notifyFriendsOnline(const TcpConnectionPtr &conn)
{
    /// TODO 这里待优化，因为这里获取到的是全部在线用户，需要从全部在线用户中筛选出来好友
    std::list<std::shared_ptr<ClientReceiver>> allList = MikChatServer::Instance().getAllClients();
    for (const std::shared_ptr<ClientReceiver> f : allList)
    {
        if (checkUserNameExist(f->getUserInfos().u_user_name))
        {
            Value jsonItem;
            std::string notifyMsg = m_ui.u_user_name + " online now!";
            jsonItem["notify_friend_online"] = notifyMsg;
            Buffer msg = toStream(int32_t(NOTIFYFRIENDSONLINE), jsonItem);
            f->getConnSender()->sendMsg(&msg);
        }
    }
}

void ClientReceiver::registerProc(const TcpConnectionPtr &conn, Buffer *buffer)
{
    string receiveMsg = buffer->retrieveAllAsString();
    LOG_INFO << receiveMsg;
    std::string registerAccountNUm, registerAccountPassword, registerAccountNickName, registerTime;
    std::string recvMsg(receiveMsg.c_str());
    Reader jsonReader;
    Value registerRequest;
    if (jsonReader.parse(recvMsg, registerRequest))
    {
        registerAccountNUm = registerRequest["account_num"].asString();
        registerAccountPassword = registerRequest["account_password"].asString();
        registerAccountNickName = registerRequest["account_nickname"].asString();
        registerTime = registerRequest["register_time"].asString();
    }
    LOG_INFO << registerAccountNickName << " " << registerAccountNUm << " " << registerAccountPassword;

    /// 处理注册
    UserInfomations ui;
    ui.u_user_name = registerAccountNUm;
    ui.u_user_nickname = registerAccountNickName;
    ui.u_user_password = registerAccountPassword;
    ui.u_user_register_time = registerTime;
    ClientRegisterStatus status = UsersManager::Instance().AddNewUserToDatabase(ui);
    Value jsonItem;
    if (ClientRegisterStatus::REGISTEROK == status)
    {
        jsonItem["status"] = "registerok";
    }
    else if (ClientRegisterStatus::ACCOUNTALREADYEXIST == status)
    {
        jsonItem["status"] = "accountalreadyexist";
    }
    else if (ClientRegisterStatus::REGISTERSERVERISSUE == status)
    {
        jsonItem["status"] = "registerserverissue";
    }
    else if (ClientRegisterStatus::REGISTERNOTDEFINEERROR == status)
    {
        jsonItem["status"] = "registernotdefineerror";
    }
    Buffer msg = toStream(int32_t(ACCOUNTREGISTER), jsonItem);
    m_connSender->sendMsg(&msg);
    /// 响应客户端注册帐号请求
}

void ClientReceiver::chatMessageProc(const TcpConnectionPtr &conn, Buffer *buffer)
{
    /// 解析命令：发送方用户id,接收方id,chat内容,这三项都以json格式化
    string receiveMsg;
    Reader jsonReader;
    Value chatRequest;
    receiveMsg = buffer->retrieveAllAsString();
    LOG_INFO << receiveMsg;
    std::string recvMsg(receiveMsg.c_str());
    std::string sendUserName, recvUserName, chatMsg;
    if (jsonReader.parse(recvMsg, chatRequest))
    {
        sendUserName = chatRequest["send_user_name"].asString();
        recvUserName = chatRequest["recv_user_name"].asString();
        chatMsg = chatRequest["chat_message"].asString();
    }
    assert(sendUserName == m_ui.u_user_name);
    std::list<std::shared_ptr<ClientReceiver> > allClients = MikChatServer::Instance().getAllClients();
    for (std::shared_ptr<ClientReceiver> cr : allClients)
    {
        if (cr->getUserInfos().u_user_name == recvUserName)
        {
            transmitChatMsg(cr, chatMsg);
            break;
        }
    }

    /// 写入数据库
    int send_id = UsersManager::Instance().getUserInfosByAccount(sendUserName).u_user_id;
    int recv_id = UsersManager::Instance().getUserInfosByAccount(recvUserName).u_user_id;
    if (!UsersManager::Instance().saveChatRecordToDatabase(send_id, recv_id, chatMsg))
    {
        LOG_ERROR << "save chat message to databse failed!";
    }
}

void ClientReceiver::transmitChatMsg(std::shared_ptr<ClientReceiver> receiver, const std::string &chatMsg)
{
    Value jsonItem;
    jsonItem["chat_message"] = chatMsg;
    Buffer msg = toStream(int32_t(TRANSMITCHATMSG), jsonItem);
    receiver->getConnSender()->sendMsg(&msg);
}

void ClientReceiver::updatePersonalInfos(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buffer)
{
    /// 解析命令：UserInfomations各项
    string receiveMsg;
    Reader jsonReader;
    Value infosRequest;
    receiveMsg = buffer->retrieveAllAsString();
    LOG_INFO << receiveMsg;
    std::string recvMsg(receiveMsg.c_str());
    UserInfomations ui;
    if (jsonReader.parse(recvMsg, infosRequest))
    {
        ui.u_user_sex = infosRequest["u_user_sex"].asInt();
        ui.u_user_id = infosRequest["u_user_id"].asInt();
        ui.u_user_facetype = infosRequest["u_user_facetype"].asInt();
        ui.u_user_name = infosRequest["u_user_name"].asString();
        ui.u_user_nickname = infosRequest["u_user_nickname"].asString();
        ui.u_user_password = infosRequest["u_user_password"].asString();
        ui.u_user_customface = infosRequest["u_user_customface"].asString();
        ui.u_user_birthday = infosRequest["u_user_birthday"].asString();
        ui.u_user_email = infosRequest["u_user_email"].asString();
        ui.u_user_remark = infosRequest["u_user_remark"].asString();
        ui.u_user_last_login_time = infosRequest["u_user_last_login_time"].asString();
    }
    if (!UsersManager::Instance().updatePersonalInfos(ui))
    {
        LOG_ERROR << "update personal info failed!";
    }
}
