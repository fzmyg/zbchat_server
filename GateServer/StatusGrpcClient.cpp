/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-11-01 10:34:09
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-11-03 11:15:27
 * @FilePath: /coding/Server/gateServer/StatusGrpcClient.cpp
 */
#include "StatusGrpcClient.h"
#include "ConfigMgr.h"
#include "Error.h"
StatusPool::StatusPool(std::size_t size,std::string host,std::string port)
:_size(size),_host(host),_port(port),_stop(false)
{
    for(int i = 0;i < size ;i++){
        std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel(host+":"+port,grpc::InsecureChannelCredentials());
        if(channel==nullptr){
            i--;
            continue;
        }
        std::unique_ptr<message::StatusService::Stub> stub (message::StatusService::NewStub(channel));
        if(stub==nullptr){
            i--;
            continue;
        }
        std::cerr<<"创建StautsServer::Stub"<<i+1<<"成功"<<std::endl;
        _connections.emplace(std::move(stub));
    }
}

StatusPool::~StatusPool()
{
   close();
   std::unique_lock<std::mutex> lock(_mux);
   std::size_t size = _connections.size();
   for(int i = 0;i<size;i++){
        _connections.pop();
   }
}

std::unique_ptr<message::StatusService::Stub> StatusPool::getConnection()
{
    if(_stop){
        return nullptr;
    }
    std::unique_ptr<message::StatusService::Stub> ret_ptr;
    {
        std::unique_lock<std::mutex> lock(_mux);
        _cv.wait(lock,[this](){
            return _connections.empty()==false || _stop;
        });
        if(_stop)
            return nullptr;
        ret_ptr = std::move(_connections.front());
        _connections.pop();
    }
    return std::move(ret_ptr);
}

void StatusPool::returnConnection(std::unique_ptr<message::StatusService::Stub> ret_ptr)
{
    if(_stop){
        return;
    }
    {
        std::unique_lock<std::mutex> lock(_mux);
        _connections.emplace(std::move(ret_ptr));
        _cv.notify_one();
    }
}

void StatusPool::close()
{
    if(_stop == false){
        _stop=true;
        _cv.notify_all();
    }
}


StatusGrpcClient::~StatusGrpcClient()
{
    std::cerr<<"状态服务退出"<<std::endl;
}

StatusGrpcClient::StatusGrpcClient()
{
    std::cerr<<"状态服务启动"<<std::endl;
    std::shared_ptr<ConfigMgr> cfg = ConfigMgr::getInstance();
    std::string host = (*cfg)["StatusServer"]["Host"];
    std::string port = (*cfg)["StatusServer"]["Port"];
    _pool.reset(new StatusPool(5,host,port));
}

message::GetChatServerRsp StatusGrpcClient::getChatServer(int uid)
{
    grpc::ClientContext cont;
    message::GetChatServerReq req;
    message::GetChatServerRsp rsp;
    req.set_uid(uid);
    std::unique_ptr<message::StatusService::Stub> stub =  _pool->getConnection();
    if(stub==nullptr){
        rsp.set_error(ERR_NETWORK);
        rsp.set_host("0.0.0.0");
        rsp.set_port("0xffff");
        rsp.set_token("");
        return rsp;
    }
    grpc::Status stat = stub->GetChatServer(&cont,req,&rsp);
    if(!stat.ok()){
        rsp.set_error(ERR_NETWORK);
        rsp.set_host("0.0.0.0");
        rsp.set_port("0xffff");
        rsp.set_token("");
    }
    _pool->returnConnection(std::move(stub));
    return rsp;
}