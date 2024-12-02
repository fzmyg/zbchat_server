/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-10-09 22:44:48
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-10-31 09:19:04
 * @FilePath: /coding/gateServer/VarifyGrpcClient.h
 */
#include <grpcpp/grpcpp.h>
#include <string>
#include <thread>
#include <mutex>
#include <atomic>
#include <queue>
#include <condition_variable>
#include <cstddef>
#include "Singleton.hpp"
#include "message.grpc.pb.h"
#include "message.pb.h"

class RPCPool{
public:
    RPCPool(){ }
    RPCPool(std::size_t pool_size,const std::string& host,const std::string&port):_pool_size(pool_size),_stop(false),_host(host),_port(port){
        for(int i = 0;i<pool_size;i++){
            std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel(host+":"+port,grpc::InsecureChannelCredentials());
            std::unique_ptr<message::VarifyService::Stub> stub= message::VarifyService::NewStub(channel);
            _stubs.push(std::move(stub));
        }
    }
    ~RPCPool(){
        std::lock_guard<std::mutex> lock(_mux);
        while(_stubs.empty()==false){
            _stubs.pop();
        }
    }

    void close(){
        _stop.store(true);
        _cond.notify_all();
    }
    //取stub
    std::unique_ptr<message::VarifyService::Stub> getConnetion(){
        std::unique_lock<std::mutex> lock(_mux);
        _cond.wait(lock,[=](){
            if(_stop.load()){
                return true;
            }
            return !_stubs.empty();
        });
        if(_stop.load()){
            return nullptr;
        }
        std::unique_ptr<message::VarifyService::Stub>  context = std::move(_stubs.front());
        _stubs.pop();
        return std::move(context);
    }
    //放stub
    void returnConnection(std::unique_ptr<message::VarifyService::Stub> context){
        std::lock_guard<std::mutex> lock(_mux);
        if(_stop.load()){
            return;
        }
        _stubs.push(std::move(context));
        _cond.notify_one();
    }
private:
    std::size_t _pool_size = 0;
    std::atomic<bool> _stop;
    std::string _host;
    std::string _port;
    std::queue<std::unique_ptr<message::VarifyService::Stub>>  _stubs;
    std::condition_variable _cond;
    std::mutex _mux;
};

class VarifyGrpcClient
:public Singleton<VarifyGrpcClient>
{
    friend class Singleton<VarifyGrpcClient>;
public:
    message::GetVarifyRsp  getVarifyCode(int key,std::string email);
protected:
    VarifyGrpcClient();
private:
    std::unique_ptr<RPCPool> _rpc_pool;
};