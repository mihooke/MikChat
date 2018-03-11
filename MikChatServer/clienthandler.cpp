#include "clienthandler.h"

ClientHandler::ClientHandler(const muduo::net::TcpConnectionPtr &conn)
    : m_connection(conn)
{

}

ClientHandler::~ClientHandler()
{

}
