/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-11-01 23:02:11
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-11-29 19:32:10
 * @FilePath: /coding/Server/ChatServer/ChatServer.h
 */
#pragma once
#include "ChatSession.h"
#include "Singleton.hpp"
#include "log.h"
#include <boost/asio.hpp>
#include <mutex>
#include <memory>
#include <string>
#include <atomic>
#include <map>
class ChatServer:public Singleton<ChatServer>
{
    friend class Singleton<ChatServer>;
    ChatServer(boost::asio::io_context& ioc,unsigned short port);
public:
    ~ChatServer();
    void erase(std::string session_id);
    void stop();
    inline bool isStoped(){ return _stop.load();}
private:
    void startAccept();
    void acceptCallBack(std::shared_ptr<ChatSession> session,boost::system::error_code ec);
    std::string generateUuid();
private:
    boost::asio::io_context& _ioc;
    boost::asio::ip::tcp::acceptor _ator;
    std::map<std::string,std::shared_ptr<ChatSession>> _sessions;
    std::mutex _mux;
    std::atomic<bool> _stop;
};

class SelfChatServerInfo:public Singleton<SelfChatServerInfo>
{
    friend class Singleton<SelfChatServerInfo>;
    SelfChatServerInfo(const std::string& host,const std::string& port,std::string& grpc_port):_host(host),_port(port),_grpc_port(grpc_port){consoleLog("初始化Chat服务器信息成功:",_host,":",_port);};
    SelfChatServerInfo(const char*host,const char*port,const char* grpc_port):_host(host),_port(port),_grpc_port(grpc_port){consoleLog("初始化Chat服务器信息成功",_host,":",_port);};
    SelfChatServerInfo(){};
public:
    std::string _host;
    std::string _port;
    std::string _grpc_port;
};