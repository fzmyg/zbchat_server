/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-11-22 19:46:14
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-11-28 12:56:00
 * @FilePath: /coding/Server/ChatServer/ChatGrpcClient.h
 */
#pragma once
#include "message.grpc.pb.h"
#include "message.pb.h"
#include "Singleton.hpp"
#include <mutex>
#include <thread>
#include <queue>
#include <memory>
#include <condition_variable>
#include <grpcpp/grpcpp.h>
#include <atomic>
#include <string>
#include <map>
class ChatGrpcPool{
public:
    ChatGrpcPool(std::string host,std::string port,std::size_t size = 5);
    ~ChatGrpcPool();
    std::unique_ptr<message::ChatService::Stub> getConnection();
    void returnConnection(std::unique_ptr<message::ChatService::Stub> ret_ptr);
    void close();
private:
    std::mutex _mux;
    std::condition_variable _cv;
    std::queue<std::unique_ptr<message::ChatService::Stub>> _connections;
    std::atomic<bool> _stop;
};

class ChatSingleGrpcClient
{
public:
    message::AddFriendRsp addFriend(const message::AddFriendReq&req);
    message::ReplyAddFriendRsp replyAddFriend(const message::ReplyAddFriendReq&req);
    ChatSingleGrpcClient(std::string host,std::string port,std::size_t pool_size);
private:
    ChatGrpcPool _connection_pool;
};

class ChatGrpcClient
:public Singleton<ChatGrpcClient>
{
    friend class Singleton<ChatGrpcClient>;
public:
    int addFriend(const std::string& address,const message::AddFriendReq&req,message::AddFriendRsp & rsp);
    int replyAddFriend(const std::string& address,const message::ReplyAddFriendReq&req,message::ReplyAddFriendRsp& rsp);
private:
    ChatGrpcClient(const char*const* address,int n);
    ChatGrpcClient(){};
private:
    std::map<std::string,std::shared_ptr<ChatSingleGrpcClient>> _connection_map;
};