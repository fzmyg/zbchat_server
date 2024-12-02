/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-11-01 10:33:54
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-11-02 19:27:19
 * @FilePath: /coding/Server/gateServer/StatusGrpcClient.h
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
class StatusPool{
public:
    StatusPool(std::size_t pool_size,std::string host,std::string port);
    ~StatusPool();
    std::unique_ptr<message::StatusService::Stub> getConnection();
    void returnConnection(std::unique_ptr<message::StatusService::Stub> ret_ptr);
    void close();
private:
    std::queue<std::unique_ptr<message::StatusService::Stub>> _connections;
    std::size_t _size;
    std::string _host;
    std::string _port;
    std::mutex _mux;
    std::atomic<bool> _stop;
    std::condition_variable _cv;
};


class StatusGrpcClient:public Singleton<StatusGrpcClient>{
    friend class Singleton<StatusGrpcClient>;
public:
    ~StatusGrpcClient();
    message::LoginRsp login(int uid,std::string token);
private:
    StatusGrpcClient();
private:
    std::unique_ptr<StatusPool> _pool;
};