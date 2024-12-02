/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-11-22 21:15:56
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-11-22 21:49:14
 * @FilePath: /coding/Server/ChatServer/UserMgr.cpp
 */
#include "UserMgr.h"

UserMgr::UserMgr()
{

}
UserMgr::~UserMgr()
{
    _sessions.clear();
}
std::shared_ptr<ChatSession> UserMgr::getSession(int uid)
{
    std::unique_lock<std::mutex> lock(_mux);
    auto it = _sessions.find(uid);
    if(it == _sessions.end()){
        return nullptr;
    }
    return it->second;
}
void UserMgr::setUserSession(int uid, std::shared_ptr<ChatSession> session)
{
    std::unique_lock<std::mutex> lock(_mux);
    _sessions[uid] = session;
}
void UserMgr::rmUserSession(int uid)
{
    std::unique_lock<std::mutex> lock(_mux);
    _sessions.erase(uid);
}