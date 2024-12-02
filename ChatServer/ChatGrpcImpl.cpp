/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-11-22 22:00:47
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-12-01 21:59:10
 * @FilePath: /coding/Server/ChatServer/ChatGrpcImpl.cpp
 */
#include "ChatGrpcImpl.h"
#include "UserMgr.h"
#include "Error.h"
#include "LogicSystem.h"
#include "global.h"
#include "MySQLMgr.h"
#include <mutex>
#include <condition_variable>
#include <jsoncpp/json/json.h>

ChatServiceImpl::ChatServiceImpl()
{

}

::grpc::Status ChatServiceImpl::AddFriend(::grpc::ServerContext *context
,const ::message::AddFriendReq *request,
::message::AddFriendRsp *response)
{
    int from_uid = request->fromuid();
    int dest_uid = request->destuid();
    std::string from_name = request->fromname();
    std::string desc_msg = request->desc_msg();
    std::string from_nick = request->fromnick();
    std::string from_icon = request->fromicon();
    auto session = UserMgr::getInstance()->getSession(dest_uid);
    //用户未登录
    if(session == nullptr){
        response->set_fromuid(from_uid);
        response->set_destuid(dest_uid);
        response->set_error(ErrorCode::USER_NOT_LOGINED);
        return grpc::Status::OK;
    }
    //
    Json::Value send_data;
    send_data["fromuid"]=from_uid;
    send_data["fromname"]=from_name;
    send_data["touid"]=dest_uid;
    send_data["message"]=desc_msg;
    std::string uuid = generateUUid();
    send_data["id"] = uuid;
    send_data["fromicon"] = from_icon;
    send_data["fromnick"] = from_nick;
    session->send(ChatMsgId::CLIENT_RECV_ADD_FRIEND_APPLY,send_data.toStyledString());
    std::condition_variable cv;
    std::mutex mux;
    std::unique_lock<std::mutex> lock(mux);
    bool recv_tag = false;
    //注册回调函数
    session->regCallBack(ChatMsgId::CLIENT_RECV_ADD_FRIEND_APPLY,uuid,[&cv,&recv_tag](){
        recv_tag = true;
        cv.notify_one();
    });
    //阻塞等待客户端回包
    cv.wait_for(lock,std::chrono::seconds(5));

    if(recv_tag == true){
        return grpc::Status::OK;
    }
    if(recv_tag == false){
        session->rmCallBack(ChatMsgId::CLIENT_RECV_ADD_FRIEND_APPLY,uuid);
    }
    return grpc::Status(grpc::StatusCode::DEADLINE_EXCEEDED,"client send data timeout");
}

::grpc::Status ChatServiceImpl::ReplyAddFriend(::grpc::ServerContext *context, const ::message::ReplyAddFriendReq *request, ::message::ReplyAddFriendRsp *response)
{
    return grpc::Status::OK;
}