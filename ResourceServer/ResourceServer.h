/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-11-26 08:35:31
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-11-26 08:58:53
 * @FilePath: /coding/Server/ResouceServer/ResourceServer.h
 */
#pragma once
#include "message.grpc.pb.h"
#include "message.pb.h"
#include "ResourceServiceImpl.h"
#include <grpcpp/grpcpp.h>
#include <atomic>

class ResourceServer{
public:
    ResourceServer(const std::string& ip,const std::string& port);
    ~ResourceServer();
    void stop();
private:
    std::unique_ptr<grpc::Server> _server;
    std::atomic<bool> _stop;
};