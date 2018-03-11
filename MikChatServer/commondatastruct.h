#ifndef MIK_COMMONDATASTRUCT_H
#define MIK_COMMONDATASTRUCT_H

/*
 * 这里定义了一些struct，工程中所有用到的struct集中起来，便于处理。
 */
#include <string>
#include <set>
#include "mysql/mysql.h"

/*
 * DatabaseInfo用来保存数据库账户信息
 */
typedef struct _tag_database_info
{
    std::string host;
    std::string user;
    std::string password;
    std::string databaseName;
} DatabaseInfo;

/*
 * DatabaseResultInfo用来保存每一次执行SQL语句从数据库fetch的结果
 */
typedef struct _tag_database_result_info
{
    _tag_database_result_info()
    {
        result = nullptr;
        fieldCount = 0;
        rowCount = 0;
    }
    MYSQL_RES *result;
    uint fieldCount;
    my_ulonglong rowCount;
} DatabaseResultInfo;

/*
 * UserInfomations用来保存用户个人信息
 */
typedef struct _tag_user_info
{
    short u_user_sex;
    int u_id;
    int u_user_id;
    int u_user_facetype;
    std::string u_user_name;
    std::string u_user_nickname;
    std::string u_user_password;
    std::string u_user_customface;
    std::string u_user_birthday;
    std::string u_user_email;
    std::string u_user_register_time;
    std::string u_user_remark;
    std::string u_user_last_login_time;
    std::set<int> f_friendship;

    _tag_user_info()
    {
        u_user_sex = 1;
        u_id = 0;
        u_user_id = 100;
        u_user_name = "";
        u_user_nickname = "";
        u_user_password = "";
        u_user_facetype = 0;
        u_user_customface = "";
        u_user_birthday = "2017-01-01";
        u_user_email = "";
        u_user_register_time = "2017-01-01 12:13:14";
        u_user_remark = "";
        u_user_last_login_time = "2017-01-01 23:59:59";
    }

    /// UserInfomations会作为set map的元素，有insert操作，故必须定义operator<
    bool operator<(const _tag_user_info &r) const
    {
        if (u_user_id < r.u_user_id) {return true;}
        else  return false;
    }
} UserInfomations;

/*
 * ClientLoginRetStatus用来表示客户端登录请求，服务端返回值
 */
enum ClientLoginRetStatus
{
    LOGINOK = 1,
    PASSWORDERROR = 2,
    ACCOUNTNOTEXIST = 3,
    LOGINSERVERISSUE = 4,
    LOGINNOTDEFINEERROR = 100
};

/*
 * ClientRegisterStatus用来表示客户端注册请求，服务端返回值
 */
enum ClientRegisterStatus
{
    REGISTEROK = 1,
    ACCOUNTALREADYEXIST = 2,
    REGISTERSERVERISSUE = 3,
    REGISTERNOTDEFINEERROR = 100
};

#endif // MIK_COMMONDATASTRUCT_H
