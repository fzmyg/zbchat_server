/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-11-22 21:15:48
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-12-01 19:03:32
 * @FilePath: /coding/Server/ChatServer/UserMgr.h
 */
#pragma once
#include "Singleton.hpp"
#include "ChatSession.h"
#include <memory>
#include <unordered_map>
#include <map>
#include <string>


class ChatSession;
class UserMgr:public Singleton<UserMgr>
{
    friend class Singleton<UserMgr>;
    UserMgr();
public:
    ~UserMgr();
    std::shared_ptr<ChatSession> getSession(int uid);
    void setUserSession(int uid,std::shared_ptr<ChatSession> session);
    void rmUserSession(int uid);
private:
    std::mutex _mux;
    std::unordered_map<int,std::shared_ptr<ChatSession>> _sessions;
};

enum SEX{
    MALE,
    FEMALE
};

//add_friend 表中存储的申请状态
enum AddFriendStatus{
    NOT_SEND = 0, //用户未登录 消息未发送过
    SEND, //用户已登录 消息已发送但未被处理
    APPLYACCESS, //申请已添加 消息已被处理
    APPLYREJECT  //申请已拒绝
};
enum ApplyStatusToUser{
    WAITTING_TO_ADD = 0, //等待自己确认
    WAITING_FOR_OTHER = 1, //等待别人确认
    ADD_SUCCESS = 2, //添加成功
    ACTIVE_ADD_REJECT = 3, //主动拒接
    PASSIVE_ADD_REJECT = 4, //被拒绝
    //PASSIVE_ADD_REJECT,//被拒绝
    //ACTIVE_ADD_REJECT, //拒接添加
    //ADD_SUCCESS,       //添加成功
    //WAITING_FOR_OTHER, //等待别人确认
    //WAITTING_TO_ADD    //等待自己确认
};

struct ApplyData{
    ApplyData(){}
    ApplyData(const ApplyData& data)
    :_have_icon(data._have_icon),_icon(data._icon),
    _uid(data._uid),_name(data._name),
    _nick(data._nick),_msg(data._msg)
    ,_stat(data._stat){

    }
    ApplyData(ApplyData&& data)
        :_have_icon(data._have_icon),_icon(std::move(data._icon)),
        _uid(std::move(data._uid)),_name(std::move(data._name)),
        _nick(std::move(data._nick)),_msg(std::move(data._msg))
        ,_stat(std::move(data._stat))
    {

    }
    ApplyData(bool have_icon,std::string icon,int uid,std::string name,std::string nick,std::string msg,ApplyStatusToUser stat)
    :_have_icon(have_icon),_icon(icon),_uid(uid),_name(name),_nick(nick),_msg(msg),_stat(stat){}
    bool _have_icon;
    std::string _icon;
    int _uid;
    std::string _name;
    std::string _nick;
    std::string _msg;
    ApplyStatusToUser _stat;
};
