/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-11-22 22:47:26
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-11-27 18:34:30
 * @FilePath: /coding/Server/ChatServer/ChatGrpcServer.h
 */
#pragma once
#include "Singleton.hpp"
#include <thread>
#include <grpcpp/grpcpp.h>
#include <memory>
#include <boost/asio.hpp>
#include <atomic>
class ChatGrpcServer
:public Singleton<ChatGrpcServer>
{
    friend class Singleton<ChatGrpcServer>;
public:
    ChatGrpcServer(const std::string& host,const std::string& port);
    ~ChatGrpcServer();
    void close();
private:
    std::unique_ptr<grpc::Server> _server;
    std::unique_ptr<std::thread> _server_thread;//负责检测退出
    std::atomic<bool> _stop;
    std::string _host;
    std::string _port;
    //boost::asio::io_context _ioc; //负责检测退出
};