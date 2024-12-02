/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-11-01 18:21:03
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-11-02 20:02:23
 * @FilePath: /coding/Server/StatusServer/StatusServer.h
 */
#pragma once
#include "message.grpc.pb.h"
#include "message.pb.h"
#include "ConfigMgr.h"
#include "StatusServiceImpl.h"
#include <grpcpp/grpcpp.h>
#include <iostream>
#include <boost/asio.hpp>
#include <thread>
class StatusServer final
{
public:
    StatusServer();
    ~StatusServer();
private:
    std::unique_ptr<grpc::Server> _server;
    std::unique_ptr<std::thread> _sig_thread;//负责检测退出
    boost::asio::io_context _ioc; //负责检测退出
};