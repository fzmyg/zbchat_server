/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-11-01 23:00:12
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-12-01 21:57:33
 * @FilePath: /coding/Server/ChatServer/LogicSystem.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "LogicSystem.h"
#include "ChatSession.h"
#include "StatusGrpcClient.h"
#include "message.grpc.pb.h"
#include "message.pb.h"
#include "Error.h"
#include "UserMgr.h"
#include "log.h"
#include "MySQLMgr.h"
#include "RedisMgr.h"
#include "ChatServer.h"
#include "ChatGrpcClient.h"
#include "global.h"
#include <jsoncpp/json/json.h>
#include <cctype> 
#include <string>


LogicSystem::~LogicSystem()
{
    if(_stop==false)
        stop();
}

void LogicSystem::stop()
{
    _stop = true;
    _cv.notify_all();
    for(auto& it:_workers){
        it->join();
    }
}

LogicSystem::LogicSystem()
{
    int size = std::thread::hardware_concurrency();
    for(int i = 0 ;i < size ;i++){
        _workers.emplace_back(new std::thread([&]()mutable{
            consoleLog("Logic thread ",std::this_thread::get_id()," 启动成功");
            while(1){
                std::unique_lock<std::mutex> lock(_mux);
                _cv.wait(lock,[&]()mutable{
                    return _process_node.empty()==false || _stop.load();
                });
                if(_stop){
                    while(_process_node.empty()==false){
                        LogicNode node = std::move(_process_node.front());
                        _process_node.pop();
                        lock.unlock(); //唤醒其他线程
                        processNode(node);
                        lock.lock(); //继续处理未完成的任务
                    }
                    break;
                }
                LogicNode node = std::move(_process_node.front());
                _process_node.pop();
                lock.unlock();
                processNode(node);
            }
            consoleLog("Logic thread ",std::this_thread::get_id()," 退出成功");
        }));
    }
    regHandler(ChatMsgId::LOGIN, std::bind(&LogicSystem::LoginHandler, this, std::placeholders::_1));
    regHandler(ChatMsgId::SEARCH_USER,std::bind(&LogicSystem::SearchUserHandler,this,std::placeholders::_1));
    regHandler(ChatMsgId::ADD_FRIEND_APPLY,std::bind(&LogicSystem::AddFriendApplyHandler,this,std::placeholders::_1));
    regHandler(ChatMsgId::ADD_FRIEND_REPLY,std::bind(&LogicSystem::AddFriendApplyHandler,this,std::placeholders::_1));
    regHandler(ChatMsgId::CLIENT_RECV_ADD_FRIEND_APPLY,std::bind(&LogicSystem::ClientRecvAddFriendApplyHandler,this,std::placeholders::_1));
    regHandler(ChatMsgId::GET_FRIEND_APPLY_SOURCE,std::bind(&LogicSystem::GetFriendApplySourceHandler,this,std::placeholders::_1));
}

void LogicSystem::postProcessNode(const std::shared_ptr<ChatSession> session, unsigned short int msg_id, const std::string &data)
{
    if(_stop)
        return;
    std::unique_lock<std::mutex> lock(_mux);
    _process_node.emplace(session,msg_id,data);
    _cv.notify_one();
}

void LogicSystem::processNode(const LogicNode& node)
{
    auto it = _handlers.find(node._msg_id);
    if(it == _handlers.end()){
        consoleLog("Error in function processNode ,no function to handle msg_id",node._msg_id);
        return;
    }
    it->second(node);
}

void LogicSystem::regHandler(ChatMsgId id,std::function<int(LogicNode)> handler)
{
    _handlers.insert(std::pair<unsigned short int,std::function<int(LogicNode)>>(id,handler));
}

int LogicSystem::LoginHandler(const LogicNode& node)
{
    Json::Reader reader;
    Json::Value root;
    reader.parse(node._data,root);
    int uid = root["uid"].asInt();
    std::string token = root["token"].asString(); //此token已加密
    message::LoginRsp rsp = StatusGrpcClient::getInstance()->login(uid,token);

    if(rsp.error()!=ErrorCode::SUCCESS){ //服务器认证失败
        if(rsp.error()==ErrorCode::ERR_NETWORK){ //网络错误
            //恶意攻击
            //用户正常网络错误 , 回包让用户重新连接
            node._session->close();
            //以下错误都为恶意攻击，直接断开连接
        }else if(rsp.error()==ErrorCode::TOKEN_NOT_MATCH) { //状态服务器相应 token不匹配
            node._session->close();
        }else if(rsp.error()==ErrorCode::TOKEN_NOT_FIND){ //状态服务器没有相应uid对应token
            node._session->close();
        }
        return -1;    
    }
    Json::Value send_data;
    //send_data["error"]=rsp.error();
    //send_data["uid"]=uid;
    MySQLMgr::getInstance()->searchUser(uid,send_data);
    uid = send_data["uid"].asInt();
    int sex = send_data["sex"].asInt();
    int age = send_data["age"].asInt();
    std::string name = send_data["name"].asString();
    std::string nick = send_data["nick"].asString();
    std::string email = send_data["email"].asString();
    std::string location = send_data["location"].asString();
    std::string desc = send_data["desc"].asString();
    std::string icon_pixmap  = send_data["icon"].asString();
    node._session->setUserData(uid,sex,age,email,name,nick,desc,location,icon_pixmap);
    //node._session->setUid(uid);
    UserMgr::getInstance()->setUserSession(uid,node._session);
    RedisConnectionMgr::ErrorCode ec;
    do{
        ec=RedisConnectionMgr::getInstance()->hset(login_server+std::to_string(uid),"host",SelfChatServerInfo::getInstance()->_host);
    }while(ec!=RedisConnectionMgr::ErrorCode::SUCCESS);
    do{
        ec=RedisConnectionMgr::getInstance()->hset(login_server+std::to_string(uid),"port",SelfChatServerInfo::getInstance()->_port);
    }while(ec!=RedisConnectionMgr::ErrorCode::SUCCESS);
    do{
        ec=RedisConnectionMgr::getInstance()->hset(login_server+std::to_string(uid),"grpc_port",SelfChatServerInfo::getInstance()->_grpc_port);
    }while(ec!=RedisConnectionMgr::ErrorCode::SUCCESS);
    node._session->send(LOGIN,send_data.toStyledString());

    return 0;
}

bool LogicSystem::isUid(std::string str)
{
    int len = str.size();
    for(int i = 0;i<len;i++){
        if(isdigit(str[i])==false){
            return false;
        }
    }
    return true;
}

int LogicSystem::SearchUserHandler(const LogicNode& node)
{
    Json::Reader reader;
    Json::Value root;
    reader.parse(node._data,root);
    int self_uid = root["fromuid"].asInt();
    std::string search_name_or_uid = root["search_name_or_uid"].asString();
    Json::Value search_info;
    Defer send_search_info([&search_info,&node](){
        node._session->send(SEARCH_USER,search_info.toStyledString());
    });
    if(isUid(search_name_or_uid)){
        int uid = std::stoi(search_name_or_uid);
        //先从mysql中查取搜索信息
        int ret = MySQLMgr::getInstance()->searchUser(uid,search_info);
        if(ret == -1){
            search_info["error"] = USER_NOT_EXISTED;
            return 0;
        }else if(ret==MYSQL_CONNECT_ERROR){
            search_info["error"] = ErrorCode::ERR_NETWORK;
            return 0;
        }else if(ret == ICON_NOT_FIND){
            search_info["error"] = ErrorCode::PIXMAP_NOT_FIND;
            return 0;
        }else{
            search_info["error"] = ErrorCode::SUCCESS;
            return 0;
        }
    }else{
        std::string &name = search_name_or_uid;
        int ret = MySQLMgr::getInstance()->searchUser(name,search_info);
        if(ret == -1){
            search_info["error"] = USER_NOT_EXISTED;
            return 0;
        }else if(ret==MYSQL_CONNECT_ERROR){
            search_info["error"] = ErrorCode::ERR_NETWORK;
            return 0;
        }else if(ret == ICON_NOT_FIND){
            search_info["error"] = ErrorCode::PIXMAP_NOT_FIND;
            return 0;
        }else{
            search_info["error"] = ErrorCode::SUCCESS;
            return 0;
        }
    }
    return 0;
}

int LogicSystem::AddFriendApplyHandler(const LogicNode& node)
{
    Json::Reader reader;
    Json::Value root;
    reader.parse(node._data,root);
    int from_uid = root["fromuid"].asInt();
    int to_uid = root["touid"].asInt();
    std::string msg = root["apply_message"].asString();
    std::string name = root["fromname"].asString();
    std::string nick = root["fromnick"].asString();
    std::string icon = root["fromicon"].asString();
    Json::Value send_data;
    Defer send_search_info([&send_data,&node,&to_uid](){
        send_data["touid"]=to_uid;
        node._session->send(ADD_FRIEND_APPLY,send_data.toStyledString());
    });

    //mysql中添加好友申请记录
    int mysql_stat = MySQLMgr::getInstance()->addFriendApply(from_uid,to_uid,msg);
    if(mysql_stat==-1){
        send_data["error"] = ErrorCode::ERR_NETWORK;
        return 0;
    }else if(mysql_stat == 1){
        send_data["error"] = ErrorCode::REPEAT_APPLY;
        return 0;
    }

    //本服务器中查找用户是否在线
    std::shared_ptr<ChatSession> dst_session = UserMgr::getInstance()->getSession(to_uid);
    if(dst_session==nullptr){ //不在本服务器中 
        //查询用户登录服务器
        auto redis_mgr = RedisConnectionMgr::getInstance();
        std::string host;
        std::string grpc_port;
        if(RedisConnectionMgr::ErrorCode::FAILURE == redis_mgr->hget(login_server+std::to_string(to_uid),"host",host)){ 
            //用户未登录
            send_data["error"] = ErrorCode::SUCCESS;
            return 0;
        };
        //用户登录在其他服务器中
        RedisConnectionMgr::ErrorCode ec;
        do{
            ec = redis_mgr->hget(login_server+std::to_string(to_uid),"grpc_port",grpc_port);
        }while(ec!=RedisConnectionMgr::ErrorCode::SUCCESS);
        redis_mgr->hget(login_server+std::to_string(to_uid),"grpc_port",grpc_port);
        auto chat_grpc_client = ChatGrpcClient::getInstance();
        message::AddFriendReq req;
        message::AddFriendRsp rsp;
        req.set_fromuid(from_uid);
        req.set_destuid(to_uid);
        req.set_desc_msg(msg);
        req.set_fromname(name);
        req.set_fromnick(nick);
        req.set_fromicon(icon);
        int stat = chat_grpc_client->addFriend(host+":"+grpc_port,req,rsp); //同步rpc调用成功
        if(stat != 0){ // rpc调用失败
            send_data["error"] = ErrorCode::ERR_NETWORK;
            return 0;
        }else{ //rpc调用成功
            send_data["error"] = ErrorCode::SUCCESS;
            return 0;
        }

    }else{ //在本服务器中
        //直接发送相应请求
        Json::Value send_data2;
        send_data2["fromuid"] = from_uid;
        send_data2["fromname"] = name;
        send_data2["touid"] = to_uid;
        send_data2["apply_message"] = msg;
        std::string uuid = generateUUid();
        send_data2["id"] = uuid;
        send_data2["fromicon"] = icon;
        send_data2["fromnick"] = nick;
        dst_session->send(ChatMsgId::CLIENT_RECV_ADD_FRIEND_APPLY,send_data2.toStyledString());
        std::mutex mux;
        std::unique_lock<std::mutex> lock(mux);
        std::condition_variable cv;
        bool recv_tag;
        dst_session->regCallBack(ChatMsgId::CLIENT_RECV_ADD_FRIEND_APPLY,uuid,[&cv,&recv_tag](){
            recv_tag = true;
            cv.notify_one();
        });
        cv.wait_for(lock,std::chrono::seconds(5));
        if(recv_tag){
            send_data["error"] == ErrorCode::SUCCESS;
            return 0;
        }else{
            send_data["error"] == ErrorCode::ERR_NETWORK;
            return 0;
        }
    }
    send_data["error"] = ErrorCode::SUCCESS;
    return 0;
}

int LogicSystem::AddFriendReplyHandler(const LogicNode& node)
{

}

//客户端收到消息后回传给服务器
int LogicSystem::ClientRecvAddFriendApplyHandler(const LogicNode& node)
{
    Json::Value root;
    Json::Reader reader;
    reader.parse(node._data,root);
    int ec = root["error"].asInt();
    if(ec!= ErrorCode::SUCCESS){
        return -1;   
    }
    int from_uid = root["fromuid"].asInt();
    int to_uid = root["touid"].asInt();
    std::string uuid = root["id"].asString();
    int stat;
    auto mysql_mgr = MySQLMgr::getInstance();
    do{
        mysql_mgr->updateApplyInfo(from_uid,to_uid,AddFriendStatus::SEND);
    }while(stat != 0);
    node._session->executeCallBack(ChatMsgId::CLIENT_RECV_ADD_FRIEND_APPLY,uuid);//唤醒rpc服务线程
    return 0;
}

int LogicSystem::GetFriendApplySourceHandler(const LogicNode& node)
{
    Json::Value root;
    Json::Reader reader;
    reader.parse(node._data,root);
    int uid = root[uid].asInt();
    int offset = root["offset"].asInt();
    int count = root["count"].asInt();
    int stat = root["stat"].asInt();
    Json::Value send_data;
    Defer defer_func([&send_data,&node](){
        node._session->send(GET_FRIEND_APPLY_SOURCE,send_data.toStyledString());
    });
    std::vector<std::shared_ptr<ApplyData>> apply_info;
    int mysql_stat = MySQLMgr::getInstance()->getFriendApplySource(uid,offset,count,static_cast<ApplyStatusToUser>(stat),apply_info);
    if(mysql_stat == -1){
        send_data["error"] = ErrorCode::ERR_NETWORK;
        return -1;
    }
    count = apply_info.size();
    send_data["error"] = ErrorCode::SUCCESS;
    send_data["offset"] = offset;
    send_data["count"] = count;
    send_data["stat"] = stat;
    for(int i = 0;i<count;i++){
        std::string index = std::to_string(i);
        send_data["uid"+index] = apply_info[i]->_uid;
        send_data["name"+index] = apply_info[i]->_name;
        send_data["nick"+index] = apply_info[i]->_nick;
        send_data["message"+index] = apply_info[i]->_msg;
        send_data["icon"+index] = apply_info[i]->_icon;
    }
    return 0;
}
