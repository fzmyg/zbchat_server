/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-11-01 18:01:07
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-11-25 21:37:27
 * @FilePath: /coding/Server/StatusServer/StatusServiceImpl.h
 */
#pragma once
#include "message.grpc.pb.h"
#include "message.pb.h"
#include "ChatServerInfo.hpp"
#include <grpcpp/grpcpp.h>
#include <string>
#include <map>
#include <mutex>
//获取聊天服务器状态并向聊天服务器实现登录验证服务
class StatusServiceImpl final:public message::StatusService::Service
{
public:
    StatusServiceImpl();
    grpc::Status GetChatServer(grpc::ServerContext*context,const message::GetChatServerReq*request,message::GetChatServerRsp*response) override;
    grpc::Status Login(grpc::ServerContext*context,const message::LoginReq*request,message::LoginRsp*response) override;
private:
    ChatServerInfo getChatServerInfo();
    std::string generateUniqueString();
private:
    std::map<std::string,ChatServerInfo> _servers;
    std::mutex _servers_mux;
};
