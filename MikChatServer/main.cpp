#include <iostream>
#include <muduo/net/EventLoop.h>
#include "mikdatabasemanager.h"
#include "usersmanager.h"
#include "mikchatserver.h"

using namespace std;

int main(int argc, char *argv[])
{
    //MikDatabaseManager::Instance().Initialize("127.0.0.1", "root", "lzali", "student");
    UsersManager::Instance().Initialize("127.0.0.1", "root", "lzali", "student");

    muduo::net::EventLoop loop;
    muduo::net::InetAddress address(9527);
    MikChatServer::Instance().Initialize(&loop, address);
    MikChatServer::Instance().start();
    loop.loop();

    return 0;
}
