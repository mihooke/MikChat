#include "mikdatabase.h"
#include "commondatastruct.h"

MikDatabase::MikDatabase(std::string host,
                         std::string user,
                         std::string password,
                         std::string databaseName)
    : m_mysql(nullptr), m_host(host), m_user(user), m_password(password), m_databaseName(databaseName)
{

}

MikDatabase::~MikDatabase()
{
    if (m_mysql != nullptr)
    {
        mysql_close(m_mysql);
    }
}

bool MikDatabase::ConnectToMySQL()
{
    mysql_close(m_mysql);
    m_mysql = mysql_init(m_mysql);
    if (m_mysql == nullptr)
    {
        LOG_ERROR << "Initialize MySQL " << m_host << "error" << mysql_error(m_mysql);
        mysql_close(m_mysql);
        return false;
    }
    m_mysql = mysql_real_connect(m_mysql,
                                 m_host.c_str(),
                                 m_user.c_str(),
                                 m_password.c_str(),
                                 m_databaseName.c_str(),
                                 0, nullptr, 0);
    if (m_mysql == nullptr)
    {
        LOG_ERROR << "Connect to MySQL " << m_host << "error" << mysql_error(m_mysql);
        mysql_close(m_mysql);
        return false;
    }

    mysql_set_character_set(m_mysql, "set names utf8");
    LOG_INFO << "Connect to MySQL successfully!";
    return true;
}

DatabaseResultInfo *MikDatabase::QuerySQL(const char *sql)
{
    if (m_mysql == nullptr)
    {
        m_mysql = mysql_init(m_mysql);
        if (m_mysql == nullptr)
        {
            LOG_ERROR << "ExecuteSQL from MySQL " << m_host << "error" << mysql_error(m_mysql);
            return nullptr;
        }
    }
    MYSQL_RES *result = nullptr;
    DatabaseResultInfo *resultInfo = new DatabaseResultInfo();

    /// 查询函数mysql_real_query，当sql字符串中包含二进制数据时（可能包含空字节），必须用这个函数；mqsql_query函数sql时以null结尾的string
    /// 所以我们用mysql_real_query，查询正确返回0，否则非0值
    int retQuery = mysql_real_query(m_mysql, sql, strlen(sql));
    if (retQuery != 0)
    {
        LOG_ERROR << "ExecuteSQL from MySQL " << m_host << "fail" << mysql_error((m_mysql));
        return nullptr;
    }

    /// mysql_store_result函数会返回执行sql结果，如果有返回值，则返回值保存在result中；没有返回值，则返回null指针（比如DELETE操作）
    result = mysql_store_result(m_mysql);
    uint fieldCount = mysql_field_count(m_mysql);
    if (fieldCount == 0)
    {
        LOG_ERROR << "Fetch result from MySQL " << m_host << "fail" << mysql_error(m_mysql);
        return nullptr;
    }

    /// fetch sql语句执行结果影响的行数
    my_ulonglong rowCount = mysql_affected_rows(m_mysql);
    if (rowCount == -1)
    {
        LOG_ERROR << "Fetch rows from MySQL " << m_host << "fail" << mysql_error(m_mysql);
        return nullptr;
    }

    resultInfo->result = result;
    resultInfo->fieldCount = fieldCount;
    resultInfo->rowCount = rowCount;
    //HandleQuryResult(result, fieldCount, rowCount); handle in another thread
    return resultInfo;
}

bool MikDatabase::ExecuteSQL(const char *sql)
{
    if (m_mysql == nullptr)
    {
        m_mysql = mysql_init(m_mysql);
        if (m_mysql == nullptr)
        {
            LOG_ERROR << "ExecuteSQL from MySQL " << m_host << "error" << mysql_error(m_mysql);
            return false;
        }
    }

    int retQuery = mysql_query(m_mysql, sql);
    if (retQuery)
    {
        LOG_ERROR << "ExecuteSQL from MySQL " << m_host << "error" << mysql_error(m_mysql) << retQuery;
        return false;
    }
    return true;
}

void MikDatabase::EndQuerySQL(MYSQL_RES *result)
{
    if (result)
    {
        mysql_free_result(result);
    }
    result = nullptr;
}

bool MikDatabase::HandleQuryResult(MYSQL_RES *result, uint fieldCount, my_ulonglong rowCount)
{
    /// TODO
    mysql_free_result(result);
    return true;
}
