/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-11-22 22:00:58
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-11-28 09:08:00
 * @FilePath: /coding/Server/ChatServer/ChatGrpcImpl.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once
#include "message.pb.h"
#include "message.grpc.pb.h"
#include <grpcpp/grpcpp.h>

class ChatServiceImpl final
:public message::ChatService::Service
{
public:
    ChatServiceImpl();
private:
    virtual ::grpc::Status AddFriend(::grpc::ServerContext* context, const ::message::AddFriendReq* request, ::message::AddFriendRsp* response) override;
    virtual ::grpc::Status ReplyAddFriend(::grpc::ServerContext* context, const ::message::ReplyAddFriendReq* request, ::message::ReplyAddFriendRsp* response) override;
};