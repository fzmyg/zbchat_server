/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-11-01 23:00:19
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-12-01 11:03:11
 * @FilePath: /coding/Server/ChatServer/LogicSystem.h
 */
#pragma once
#include "Singleton.hpp"
#include <condition_variable>
#include <functional>
#include <memory>
#include <thread>
#include <mutex>
#include <queue>
#include <atomic>
#include <map>
class ChatSession;

//msg_id
enum ChatMsgId{
    LOGIN,
    SEARCH_USER,
    ADD_FRIEND_APPLY,
    ADD_FRIEND_REPLY,
    CLIENT_RECV_ADD_FRIEND_APPLY,
    CLIENT_RECV_ADD_FRIEND_REPLY,
    GET_FRIEND_APPLY_SOURCE
};

struct LogicNode{
    std::shared_ptr<ChatSession> _session;
    unsigned short int _msg_id;
    std::string _data;
    LogicNode(const std::shared_ptr<ChatSession>& session,unsigned short int msg_id,const std::string &data)
    :_session(std::forward<const std::shared_ptr<ChatSession>&>(session)),_msg_id(msg_id),_data(data){}
    LogicNode(const LogicNode& node):_session(node._session),_msg_id(node._msg_id),_data(node._data){}
    LogicNode(LogicNode&& node):_session(std::move(node._session)),_msg_id(node._msg_id),_data(std::move(node._data)){} 
};

//负责将接收到的数据进行处理
class LogicSystem
:public Singleton<LogicSystem>
{
    friend Singleton<LogicSystem>;
public:
    ~LogicSystem();
    void postProcessNode(const std::shared_ptr<ChatSession> session,unsigned short int msg_id,const std::string &data);
    void stop();
private:
    void processNode(const LogicNode& node);
    LogicSystem();
    void regHandler(ChatMsgId id,std::function<int(LogicNode)> handler);
    int LoginHandler(const LogicNode& node); //登录回调
    int SearchUserHandler(const LogicNode& node);
    bool isUid(std::string str);
    int AddFriendApplyHandler(const LogicNode& node);
    int AddFriendReplyHandler(const LogicNode& node);
    int ClientRecvAddFriendApplyHandler(const LogicNode& node);
    int GetFriendApplySourceHandler(const LogicNode& node);
    std::vector<std::unique_ptr<std::thread>> _workers;
    std::map<unsigned short int,std::function<int(const LogicNode&)>> _handlers;
    std::queue<LogicNode> _process_node;
    std::mutex _mux;
    std::condition_variable _cv;
    std::atomic<bool> _stop;
};