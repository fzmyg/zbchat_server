/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-11-22 19:43:48
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-11-29 21:44:25
 * @FilePath: /coding/Server/ChatServer/ChatGrpcClient.cpp
 */
#include "ChatGrpcClient.h"
#include "ConfigMgr.h"
#include "global.h"
ChatGrpcPool::ChatGrpcPool(std::string host,std::string port,std::size_t size):_stop(false)
{
    // 加载证书和密钥
    std::string client_key = readFile("./ssl/client.key");
    std::string client_cert = readFile("./ssl/client.crt");
    std::string ca_cert = readFile("./ssl/ca.crt");

    // 配置 SSL/TLS
    grpc::SslCredentialsOptions ssl_opts;
    ssl_opts.pem_private_key = client_key;
    ssl_opts.pem_cert_chain = client_cert;
    ssl_opts.pem_root_certs = ca_cert;

    auto channel_creds = grpc::SslCredentials(ssl_opts);
    for(int i = 0;i<size;i++){
        //std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel(host+":"+port,channel_creds);
        std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel(host+":"+port,grpc::InsecureChannelCredentials());
        if(channel==nullptr){
            i--;
            continue;
        }
        std::unique_ptr<message::ChatService::Stub> stub = message::ChatService::NewStub(channel);
        if(stub==nullptr){
            i--;
            continue;
        }
        this->_connections.emplace(std::move(stub));
        std::cerr<<"创建ChatServer::Stub :"<<host+":"+port<<"成功\n";
    }
}

ChatGrpcPool::~ChatGrpcPool()
{
    if(_stop==false){
        close();
    }
}

void ChatGrpcPool::close()
{
    _stop.store(true);
    _cv.notify_all();
    std::unique_ptr<std::mutex> lock;
    _stop.store(true);
    while(_connections.empty()==false){
        _connections.pop();
    }
}

std::unique_ptr<message::ChatService::Stub> ChatGrpcPool::getConnection()
{
    if(_stop){
        return nullptr;
    }
    std::unique_ptr<message::ChatService::Stub> ret_ptr;
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

void ChatGrpcPool::returnConnection(std::unique_ptr<message::ChatService::Stub> ret_ptr)
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


message::AddFriendRsp ChatSingleGrpcClient::addFriend(const message::AddFriendReq&req)
{
    grpc::ClientContext context;
    std::unique_ptr<message::ChatService::Stub> stub = _connection_pool.getConnection();
    message::AddFriendRsp rsp;
    grpc::Status stat = stub->AddFriend(&context,req,&rsp);
    if(stat.ok()){

    }else{

    }
    return rsp;
}

message::ReplyAddFriendRsp ChatSingleGrpcClient::replyAddFriend(const message::ReplyAddFriendReq&req)
{
    grpc::ClientContext context;
    std::unique_ptr<message::ChatService::Stub> stub = _connection_pool.getConnection();
    message::ReplyAddFriendRsp rsp;
    grpc::Status stat = stub->ReplyAddFriend(&context,req,&rsp);
    if(stat.ok()){

    }else{

    }
    return rsp;
}

ChatSingleGrpcClient::ChatSingleGrpcClient(std::string host,std::string port,std::size_t pool_size)
:_connection_pool(host,port,pool_size)
{

}


int ChatGrpcClient::addFriend(const std::string& address,const message::AddFriendReq&req,message::AddFriendRsp&rsp)
{
    auto it = this->_connection_map.find(address);
    if(it == _connection_map.end()){
        return -1;
    }
    rsp = it->second->addFriend(req);
    return 0;
}

int ChatGrpcClient::replyAddFriend(const std::string& address,const message::ReplyAddFriendReq&req,message::ReplyAddFriendRsp&rsp)
{
    auto it = this->_connection_map.find(address);
    if(it == _connection_map.end()){
        return -1;
    }
    rsp = it->second->replyAddFriend(req);
    return 0;
}

ChatGrpcClient::ChatGrpcClient(const char*const* address,int n)
{
    for(int i = 0;i<n;i++){
        std::string ip_address(*address);
        int opt_pos = ip_address.find(':');
        std::string host = ip_address.substr(0,opt_pos);
        std::string port = ip_address.substr(opt_pos+1);
        this->_connection_map.insert(std::pair<std::string,std::shared_ptr<ChatSingleGrpcClient>>(ip_address,std::make_shared<ChatSingleGrpcClient>(host,port,5)));
    }
}
