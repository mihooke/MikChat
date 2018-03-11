#ifndef MIK_MIKDATABASEMANAGER_H
#define MIK_MIKDATABASEMANAGER_H

/*
 * MikDatabaseManager是单例类，负责对数据库的创建，检查工作
 */
#include <string>
#include <iostream>
#include <memory>
#include <vector>

class MikDatabase;
class MikDatabaseManager final
{
public:
    MikDatabaseManager();
    ~MikDatabaseManager();
    MikDatabaseManager(const MikDatabaseManager &) = delete;
    MikDatabaseManager &operator =(const MikDatabaseManager &) = delete;

    static MikDatabaseManager &Instance();
    bool Initialize(const char *host,
                    const char *user,
                    const char *password,
                    const char *databaseName);

    std::shared_ptr<MikDatabase> GetDatabaseConn() const { return m_databaseConnect; }
    std::string GetHost() const;
    std::string GetUser() const;
    std::string GetPassword() const;
    std::string GetDatabaseName() const;

private:
    /* @ 创建MySQL连接
     */
    bool CreateMySQLConnection();
    /* @ 检查数据库是否存在
     */
    bool CheckDatabaseExist(const char *databaseName);
    bool CreateDatabase();
    /* @ 检查数据表是否存在
     */
    bool CheckTableExist();
    bool CreateTable();

    bool IsTableMatched(const std::vector<char *> &tables);

private:
    std::shared_ptr<MikDatabase> m_databaseConnect;
    std::string m_host;
    std::string m_user;
    std::string m_password;
    std::string m_databaseName;
};

#endif // MIK_MIKDATABASEMANAGER_H
