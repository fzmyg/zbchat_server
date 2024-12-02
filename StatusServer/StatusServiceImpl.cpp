/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-11-01 18:01:16
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-11-25 21:36:02
 * @FilePath: /coding/Server/StatusServer/StatusServiceImpl.cpp
 */
#include "StatusServiceImpl.h"
#include "TransferKits.h"
#include "ConfigMgr.h"
#include "RedisMgr.h"
#include "Error.h"
#include <boost/uuid.hpp>
#include <memory>

static std::string chat_server_token = "chat_server_token_";
StatusServiceImpl::StatusServiceImpl()
{
    std::shared_ptr<ConfigMgr> conf = ConfigMgr::getInstance();
    ChatServerInfo server1("ChatServer1",(*conf)["ChatServer1"]["Host"],(*conf)["ChatServer1"]["Port"],0);
    ChatServerInfo server2("ChatServer2",(*conf)["ChatServer2"]["Host"],(*conf)["ChatServer2"]["Port"],0);
    _servers.insert(std::pair<std::string,ChatServerInfo>(server1._name,server1));
    _servers.insert(std::pair<std::string,ChatServerInfo>(server2._name,server2));
}

grpc::Status StatusServiceImpl::GetChatServer(grpc::ServerContext*context,const message::GetChatServerReq*request,message::GetChatServerRsp*response)
{
    ChatServerInfo chat_server = getChatServerInfo();
    response->set_host(chat_server._host);
    response->set_port(chat_server._port);
    std::string token = generateUniqueString();
    response->set_token(token);   
    
    auto redis_mgr = RedisConnectionMgr::getInstance();
    std::string s_uid = std::to_string(request->uid());
    redis_mgr->hset(chat_server_token + s_uid,"token",token);
    redis_mgr->hset(chat_server_token + s_uid,"ip",chat_server._host);
    redis_mgr->hset(chat_server_token + s_uid,"port",chat_server._port);
    
    response->set_error(ErrorCode::SUCCESS);
    std::cerr<<"rpc调用GetChatServer成功\n"<<"ChatServer Info:"<<"url="<<chat_server._host<<":"<<chat_server._port<<" token="<<token<<std::endl;
    return grpc::Status::OK;
}

grpc::Status StatusServiceImpl::Login(grpc::ServerContext*context,const message::LoginReq*request,message::LoginRsp*response)
{
    int uid = request->uid();
    std::string token = request->token();
    token = xorEncryptPassword(token);
    std::string true_token ;
    std::map<int,std::string>::iterator it;
    {
        RedisConnectionMgr::ErrorCode ec = RedisConnectionMgr::getInstance()->hget(chat_server_token + std::to_string(uid),"token",true_token);
        if(ec == RedisConnectionMgr::ErrorCode::FAILURE){
            response->set_error(TOKEN_NOT_FIND);
            return grpc::Status::OK;
        }   
        if(true_token != token){
            response->set_error(TOKEN_NOT_MATCH);
            return grpc::Status::OK;
        }
    }
    response->set_error(SUCCESS);
    std::string chat_server_ip = context->peer();
    chat_server_ip = chat_server_ip.substr(chat_server_ip.find(':')+1);
    std::string chat_server_port = chat_server_ip.substr(chat_server_ip.find(':')+1);
    std::cerr<<chat_server_ip<<" "<<chat_server_port<<std::endl;
    std::cerr<<"rpc调用Login成功\n";
    return grpc::Status::OK;
}

ChatServerInfo StatusServiceImpl::getChatServerInfo()
{
    std::lock_guard<std::mutex> lock(_servers_mux);
    ChatServerInfo min_connect_cnt_server = _servers.begin()->second;
    for(auto & it:_servers){
        if(min_connect_cnt_server._connect_cnt > it.second._connect_cnt){
            min_connect_cnt_server = it.second;
        }
    }
    _servers.find(min_connect_cnt_server._name)->second._connect_cnt ++;
    return min_connect_cnt_server;
}

std::string StatusServiceImpl::generateUniqueString()
{
    boost::uuids::uuid token = boost::uuids::random_generator()();
    return boost::uuids::to_string(token);
}