#include "usersmanager.h"
#include "mikdatabase.h"
#include "muduo/base/Logging.h"
#include <QDebug>

//ui.u_id
//ui.u_user_id
//ui.u_user_name
//ui.u_user_nickname
//ui.u_user_password
//ui.u_user_facetype
//ui.u_user_customface
//ui.u_user_sex
//ui.u_user_birthday
//ui.u_user_email
//ui.u_user_register_time
//ui.u_user_remark
//ui.u_user_last_login_time

UsersManager::UsersManager()
{

}

UsersManager::~UsersManager()
{

}

UsersManager &UsersManager::Instance()
{
    static UsersManager singleInstance;
    return singleInstance;
}

void UsersManager::Initialize(const char *host,
                const char *user,
                const char *password,
                const char *databaseName)
{
    if (!MikDatabaseManager::Instance().Initialize(host, user, password, databaseName))
    {
        return;
    }

    LoadUsersFromDatabase();

    LoadFriendshipFromDatabase();

    LOG_INFO << "Load data from database finished!";
}

ClientRegisterStatus UsersManager::AddNewUserToDatabase(const UserInfomations &ui)
{
    for (auto &item : m_setAllInfos)
    {
        if (item.u_user_name == ui.u_user_name)
        {
            return ClientRegisterStatus::ACCOUNTALREADYEXIST;
        }
    }
    char sql[1024] = {0};
    std::snprintf(sql, 1024, "insert into UserInfoTable values ("
                             "%d, %d, '%s', '%s', '%s', %d, '%s', %d, '%s', '%s', '%s', '%s', '%s')",
                  1,
                  ui.u_user_id,
                  ui.u_user_name.c_str(),
                  ui.u_user_nickname.c_str(),
                  ui.u_user_password.c_str(),
                  ui.u_user_facetype,
                  ui.u_user_customface.c_str(),
                  ui.u_user_sex,
                  ui.u_user_birthday.c_str(),
                  ui.u_user_email.c_str(),
                  ui.u_user_register_time.c_str(),
                  ui.u_user_remark.c_str(),
                  ui.u_user_last_login_time.c_str());
    LOG_INFO << "AddNewUser SQL:" << sql;
    if (!MikDatabaseManager::Instance().GetDatabaseConn()->ExecuteSQL(sql))
    {
        LOG_ERROR << "AddNewUserToDatabase fail";
        return ClientRegisterStatus::REGISTERSERVERISSUE;
    }

    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::pair<std::set<UserInfomations>::iterator, bool> p;
        p = m_setAllInfos.insert(ui);
        m_mapAllInfos[ui.u_user_id] = ui;
    }
    return ClientRegisterStatus::REGISTEROK;
}

bool UsersManager::saveChatRecordToDatabase(int send_id, int recv_id, const std::string &msg)
{
    char sql[1024] = {0};
    std::snprintf(sql, 1024, "insert into ChatRecordTable values ("
                             "%d, %d, %d, '%s')",
                  1,
                  send_id,
                  recv_id,
                  msg.c_str());
    LOG_INFO << "SaveChatRecord SQL:" << sql;
    if (!MikDatabaseManager::Instance().GetDatabaseConn()->ExecuteSQL(sql))
    {
        LOG_ERROR << "SaveChatRecord fail";
        return false;
    }
    return true;
}

bool UsersManager::updatePersonalInfos(const UserInfomations &ui)
{
    char sql[1024] = {0};
    std::snprintf(sql, 1024, "update UserInfoTable set "
                  "u_user_sex=%d, u_user_facetype=%d, u_user_nickname='%s', u_user_customface='%s', "
                  "u_user_birthday='%s', u_user_email='%s', u_user_remark='%s' where u_user_name='%s'",
                  ui.u_user_sex,
                  ui.u_user_facetype,
                  ui.u_user_nickname.c_str(),
                  ui.u_user_customface.c_str(),
                  ui.u_user_birthday.c_str(),
                  ui.u_user_email.c_str(),
                  ui.u_user_remark.c_str(),
                  ui.u_user_name.c_str()
                  );
    LOG_INFO << "update personal infomations SQL:" << sql;
    if (!MikDatabaseManager::Instance().GetDatabaseConn()->ExecuteSQL(sql))
    {
        LOG_ERROR << "update personal infomations fail";
        return false;
    }
    return true;
}

void UsersManager::LoadUsersFromDatabase()
{
    const char *sql = "select * from UserInfoTable ORDER BY u_user_id DESC";
    std::shared_ptr<DatabaseResultInfo> resultInfo(MikDatabaseManager::Instance().GetDatabaseConn()->QuerySQL(sql));
    if (resultInfo == nullptr)
    {
        return;
    }

    MYSQL_ROW rowInfo;
    for (uint i = 0; i < resultInfo->rowCount; ++i)
    {
        rowInfo = mysql_fetch_row(resultInfo->result);
        UserInfomations ui;
        ui.u_id = atoi(rowInfo[0]);
        ui.u_user_id = atoi(rowInfo[1]);
        ui.u_user_name = rowInfo[2];
        ui.u_user_nickname = rowInfo[3];
        ui.u_user_password = rowInfo[4];
        ui.u_user_facetype = atoi(rowInfo[5]);
        ui.u_user_customface = rowInfo[6];
        ui.u_user_sex = atoi(rowInfo[7]);
        ui.u_user_birthday = rowInfo[8];
        ui.u_user_email = rowInfo[9];
        ui.u_user_register_time = rowInfo[10];
        ui.u_user_remark = rowInfo[11];
        ui.u_user_last_login_time = rowInfo[12];

        std::pair<std::set<UserInfomations>::iterator, bool> p;
        p = m_setAllInfos.insert(ui);
        m_mapAllInfos[ui.u_user_id] = ui;
    }

    MikDatabaseManager::Instance().GetDatabaseConn()->EndQuerySQL(resultInfo->result);
}

void UsersManager::LoadFriendshipFromDatabase()
{
    char sql[1024] = {0};
    std::map<int, UserInfomations>::iterator itr = m_mapAllInfos.begin();
    while (itr != m_mapAllInfos.end())
    {
        std::snprintf(sql, 1024, "select * from FriendshipTable WHERE f_user_id1 = %d OR f_user_id2 = %d", itr->first, itr->first);
        std::shared_ptr<DatabaseResultInfo> resultInfo(MikDatabaseManager::Instance().GetDatabaseConn()->QuerySQL(sql));
        if (resultInfo == nullptr)
        {
            return;
        }

        MYSQL_ROW rowInfo;
        for (uint i = 0; i < resultInfo->rowCount; ++i)
        {
            rowInfo = mysql_fetch_row(resultInfo->result);
            if (atoi(rowInfo[1]) == itr->first)
            {
                itr->second.f_friendship.insert(atoi(rowInfo[2]));
            }
            else
            {
                itr->second.f_friendship.insert(atoi(rowInfo[1]));
            }
        }
        MikDatabaseManager::Instance().GetDatabaseConn()->EndQuerySQL(resultInfo->result);

        ++ itr;
    }
}

ClientLoginRetStatus UsersManager::checkAccountExist(const std::string &accountNum, const std::string &accountPassword)
{
    if (m_setAllInfos.empty())
    {
        /// 如果账户不存在m_setAllInfos，有可能是刚注册用户，这时候需动态查询一次数据库
        LoadUsersFromDatabase();
        if (m_setAllInfos.empty())
        {
            return ClientLoginRetStatus::LOGINSERVERISSUE;
        }
    }

    for (auto &item : m_setAllInfos)
    {
        if (item.u_user_name == accountNum)
        {
            if (item.u_user_password == accountPassword)
            {
                return ClientLoginRetStatus::LOGINOK;
            }
            else
            {
                return ClientLoginRetStatus::PASSWORDERROR;
            }
        }
    }
    return ClientLoginRetStatus::ACCOUNTNOTEXIST;
}

const UserInfomations UsersManager::getUserInfosByAccount(const std::string accountNum)
{
    for (auto item : m_setAllInfos)
    {
        if (item.u_user_name == accountNum)
            return item;
    }
}
