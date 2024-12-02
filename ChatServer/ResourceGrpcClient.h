/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-11-26 09:27:01
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-11-26 17:11:26
 * @FilePath: /coding/Server/ChatServer/ResourceGrpcClient.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once

#include "Singleton.hpp"
#include "message.pb.h"
#include "message.grpc.pb.h"
#include <grpcpp/grpcpp.h>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <queue>
class ResourcePool{
public:
    ResourcePool(std::size_t pool_size,std::string host,std::string port);
    ~ResourcePool();
    std::unique_ptr<message::ResourceService::Stub> getConnection();
    void returnConnection(std::unique_ptr<message::ResourceService::Stub> ret_ptr);
    void close();
private:
    std::queue<std::unique_ptr<message::ResourceService::Stub>> _connections;
    std::size_t _size;
    std::string _host;
    std::string _port;
    std::mutex _mux;
    std::atomic<bool> _stop;
    std::condition_variable _cv;
};


class ResourceGrpcClient:public Singleton<ResourceGrpcClient>{
    friend class Singleton<ResourceGrpcClient>;
public:
    ~ResourceGrpcClient();
    message::AddIconRsp addIcon(int uid,std::string icon_pixmap);
    message::GetIconRsp getIcon(std::string url);
private:
    ResourceGrpcClient();
private:
    std::unique_ptr<ResourcePool> _pool;
};
