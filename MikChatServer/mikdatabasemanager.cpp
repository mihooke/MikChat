#include "commondatastruct.h"
#include "mikdatabasemanager.h"
#include "mikdatabase.h"
#include "globals.h"

MikDatabaseManager::MikDatabaseManager()
{

}

MikDatabaseManager::~MikDatabaseManager()
{

}

MikDatabaseManager &MikDatabaseManager::Instance()
{
    static MikDatabaseManager singleInstance;
    return singleInstance;
}

bool MikDatabaseManager::Initialize(const char *host,
                const char *user,
                const char *password,
                const char *databaseName)
{
    m_host = host;
    m_user = user;
    m_password = password;
    m_databaseName = databaseName;
    /// 初始化MySQL，并创建MySQL的连接
    if (!CreateMySQLConnection())
    {
        LOG_ERROR << "MikDatabaseManager connect to MySQL " << m_host << "fail";
        return false;
    }

    /// 检查MikDatabase数据库是否存在，不存在则创建之
    if (!CheckDatabaseExist(databaseName))
    {
        if (CreateDatabase())
        {
            LOG_ERROR << "Initialize mikDatabase fail";
            return false;
        }
    }

    /// 检查所需表是否存在，不存在则创建之
    if (!CheckTableExist())
    {
        if (!CreateTable())
        {
            LOG_ERROR << "Initialize mikDatabaseTables fail";
            return false;
        }
    }
    return true;
}

std::string MikDatabaseManager::GetHost() const
{
    return m_host;
}

std::string MikDatabaseManager::GetUser() const
{
    return m_user;
}

std::string MikDatabaseManager::GetPassword() const
{
    return m_password;
}

std::string MikDatabaseManager::GetDatabaseName() const
{
    return m_databaseName;
}

bool MikDatabaseManager::CreateMySQLConnection()
{
    m_databaseConnect.reset(new MikDatabase(m_host,
                                            m_user,
                                            m_password,
                                            m_databaseName));
    return m_databaseConnect->ConnectToMySQL();
}

bool MikDatabaseManager::CheckDatabaseExist(const char *databaseName)
{
    const char *sql = "show databases";
    std::shared_ptr<DatabaseResultInfo> resultInfo(m_databaseConnect->QuerySQL(sql));
    if (resultInfo == nullptr)
    {
        return false;
    }

    MYSQL_ROW rowInfo;
    for (uint i = 0; i < resultInfo->rowCount; ++i)
    {
        rowInfo = mysql_fetch_row(resultInfo->result);
        for (uint j = 0; j < resultInfo->fieldCount; ++j)
        {
            if (!strcmp(rowInfo[0], databaseName))
            {
                m_databaseConnect->EndQuerySQL(resultInfo->result);
                return true;
            }
        }
    }
    m_databaseConnect->EndQuerySQL(resultInfo->result);
    return false;
}

bool MikDatabaseManager::CreateDatabase()
{
    std::string sql = "create database " + m_databaseName;
    if (m_databaseConnect->ExecuteSQL(sql.c_str()))
    {
        return true;
    }
    return false;
}

bool MikDatabaseManager::CheckTableExist()
{
    std::string sqlUseDb = "use " + m_databaseName;
    if (!m_databaseConnect->ExecuteSQL(sqlUseDb.c_str()))
    {
        LOG_ERROR << "Use database from MySQL " << m_host << "fail";
        return false;
    }

    const char *sqlShowTables = "show tables";
    std::shared_ptr<DatabaseResultInfo> resultInfo(m_databaseConnect->QuerySQL(sqlShowTables));
    if (resultInfo == nullptr)
    {
        return false;
    }

    MYSQL_ROW rowInfo;
    std::vector<char *> queryTables;
    for (uint i = 0; i < resultInfo->rowCount; ++i)
    {
        rowInfo = mysql_fetch_row(resultInfo->result);

        for (uint j = 0; j < resultInfo->fieldCount; ++j)
        {
            queryTables.push_back(rowInfo[0]);
        }
    }
    if (!IsTableMatched(queryTables))
    {
        m_databaseConnect->EndQuerySQL(resultInfo->result);
        return false;
    }
    m_databaseConnect->EndQuerySQL(resultInfo->result);
    return true;
}

bool MikDatabaseManager::IsTableMatched(const std::vector<char *> &tables)
{
    bool firstTableFound = false;
    bool secondTableFound = false;
    bool thirdTableFOund = false;
    for (auto tableName : tables)
    {
        if (!firstTableFound)
        {
            if (!mikDatabaseTables[0].compare(tableName))
            {
                firstTableFound = true;
            }
        }
        if (!secondTableFound)
        {
            if (!mikDatabaseTables[1].compare(tableName))
            {
                secondTableFound = true;
            }
        }
        if (!thirdTableFOund)
        {
            if (!mikDatabaseTables[2].compare(tableName))
            {
                thirdTableFOund = true;
            }
        }
    }
    if (firstTableFound && secondTableFound && thirdTableFOund)
    {
        return true;
    }

    return false;
}

bool MikDatabaseManager::CreateTable()
{
    std::string sql = "create table ";
    sql += mikDatabaseTables[0];
    sql += " ( ";
    sql += "u_id bigint(20) NOT NULL COMMENT '',\n";
    sql += "u_user_id bigint(20) NOT NULL COMMENT '',\n";
    sql += "u_user_name varchar(50) NOT NULL COMMENT '',\n";
    sql += "u_user_nickname varchar(50) NOT NULL COMMENT '',\n";
    sql += "u_user_password varchar(50) NOT NULL COMMENT '',\n";
    sql += "u_user_facetype int(10) COMMENT '',\n";
    sql += "u_user_customface varchar(50) COMMENT '',\n";
    sql += "u_user_sex tinyint(10) COMMENT '',\n";
    sql += "u_user_birthday date COMMENT '',\n";
    sql += "u_user_email varchar(50) COMMENT '',\n";
    sql += "u_user_register_time datetime COMMENT '',\n";
    sql += "u_user_remark varchar(50) COMMENT '',\n";
    sql += "u_user_last_login_time datetime COMMENT ''\n";
    //sql += "PRIMARY KEY (u_user_id)\n";
    sql += " )";
    if (!m_databaseConnect->ExecuteSQL(sql.c_str()))
    {
        LOG_ERROR << "Create table UserInfoTable fail";
        return false;
    }

    sql = "create table ";
    sql += mikDatabaseTables[1];
    sql += "(";
    sql += "f_id bigint(20) NOT NULL COMMENT 'auto increment id',\n";
    sql += "f_user_id1 bigint(20) NOT NULL COMMENT '',\n";
    sql += "f_user_id2 bigint(20) NOT NULL COMMENT '',\n";
    sql += "PRIMARY KEY (f_id)\n";
    sql += ")";
    if (!m_databaseConnect->ExecuteSQL(sql.c_str()))
    {
        LOG_ERROR << "Create table FriendshipTable fail";
        return false;
    }

    sql = "create table ";
    sql += mikDatabaseTables[2];
    sql += "(";
    sql += "c_id bigint(20) NOT NULL COMMENT 'auto increment id',\n";
    sql += "c_send_user_id bigint(20) NOT NULL COMMENT '',\n";
    sql += "c_recv_user_id bigint(20) NOT NULL COMMENT '',\n";
    sql += "c_chat_msg blob NOT NULL COMMENT '',\n";
    sql += "PRIMARY KEY (c_id)\n";
    sql += ")";
    if (!m_databaseConnect->ExecuteSQL(sql.c_str()))
    {
        LOG_ERROR << "Create table ChatRecordTable fail";
        return false;
    }
    return true;
}
