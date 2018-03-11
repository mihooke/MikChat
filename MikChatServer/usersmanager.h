#ifndef MIK_USERSMANAGER_H
#define MIK_USERSMANAGER_H

/*
 * UsersManager是单例类，负责响应用户界面的一些操作，比如用户注册，更改信息，加好友等和数据库交互的工作。
 */
#include <map>
#include <set>
#include <memory>
#include <mutex>
#include "commondatastruct.h"
#include "mikdatabasemanager.h"

class UsersManager final
{
public:
    UsersManager();
    ~UsersManager();
    UsersManager(const UsersManager &) = delete;
    UsersManager &operator=(const UsersManager &) = delete;

    static UsersManager &Instance();
    void Initialize(const char *host,
                    const char *user,
                    const char *password,
                    const char *databaseName);
    ClientRegisterStatus AddNewUserToDatabase(const UserInfomations &ui);
    bool saveChatRecordToDatabase(int send_id, int recv_id, const std::string &msg);
    bool updatePersonalInfos(const UserInfomations &ui);

    std::map<int, UserInfomations> getUserInfos() const {return m_mapAllInfos;}
    ClientLoginRetStatus checkAccountExist(const std::string &accountNum, const std::string &accountPassword);

    const UserInfomations getUserInfosById(const int userId) { return m_mapAllInfos[userId]; }
    const UserInfomations getUserInfosByAccount(const std::string accountNum);
    const std::set<int> getFriendShipById(const int userId) { return m_mapAllInfos[userId].f_friendship; }
    const std::string getUserNameById(const int userId) { return m_mapAllInfos[userId].u_user_name; }
private:
    void LoadUsersFromDatabase();
    void LoadFriendshipFromDatabase();

private:
    std::set<UserInfomations> m_setAllInfos;
    std::map<int, UserInfomations> m_mapAllInfos;
    std::mutex m_mutex;
};

#endif // MIK_USERSMANAGER_H
