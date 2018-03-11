#ifndef MIK_MIKDATABASE_H
#define MIK_MIKDATABASE_H

/*
 * MikDatabase封装MySQL API
 * 主要包括查询，执行SQL语句操作
 */
#include <iostream>
#include <string>
#include "mysql/mysql.h"
#include "muduo/base/Logging.h"

typedef struct _tag_database_result_info DatabaseResultInfo;
class MikDatabase
{
public:
    MikDatabase(std::string host,
                std::string user,
                std::string password,
                std::string databaseName);
    ~MikDatabase();

    bool ConnectToMySQL();
    bool ExecuteSQL(const char *sql);
    /// QuerySQL()和EndQuerySQL()是配对使用的查询函数，在MySQL的查询操作中，如果调用mysql_store_result，
    /// 必须调用mysql_free_result对查询结果进行内存释放
    DatabaseResultInfo *QuerySQL(const char *sql);
    void EndQuerySQL(MYSQL_RES *result);

    bool HandleQuryResult(MYSQL_RES *result, uint fieldCount, my_ulonglong rowCount);
private:
    MYSQL *m_mysql;
    std::string m_host;
    std::string m_user;
    std::string m_password;
    std::string m_databaseName;
};

#endif // MIK_MIKDATABASE_H
