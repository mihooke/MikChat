#ifndef MIK_COMMON_H
#define MIK_COMMON_H

#include <QString>
#include <QSet>

/*
 * 保存用户个人信息
 */
typedef struct _tag_user_info
{
    short u_user_sex;
    int u_id;
    int u_user_id;
    int u_user_facetype;
    QString u_user_name;
    QString u_user_nickname;
    QString u_user_password;
    QString u_user_customface;
    QString u_user_birthday;
    QString u_user_email;
    QString u_user_register_time;
    QString u_user_remark;
    QString u_user_last_login_time;
    QSet<QString> f_friendship;

    _tag_user_info()
    {
        u_user_sex = 1;
        u_id = 0;
        u_user_id = 100;
        u_user_facetype = 0;
        u_user_name = "";
        u_user_nickname = "";
        u_user_password = "";
        u_user_customface = "";
        u_user_birthday = "";
        u_user_email = "";
        u_user_register_time = "";
        u_user_remark = "";
        u_user_last_login_time = "";
    }

    _tag_user_info(const _tag_user_info &other)
    {
        u_user_sex = other.u_user_sex;
        u_id = other.u_id;
        u_user_id = other.u_user_id;
        u_user_facetype = other.u_user_facetype;
        u_user_name = other.u_user_name;
        u_user_nickname = other.u_user_nickname;
        u_user_password = other.u_user_password;
        u_user_customface = other.u_user_customface;
        u_user_birthday = other.u_user_birthday;
        u_user_email = other.u_user_email;
        u_user_register_time = other.u_user_register_time;
        u_user_remark = other.u_user_remark;
        u_user_last_login_time = other.u_user_last_login_time;
    }
} UserInformations;

#endif // MIK_COMMON_H
