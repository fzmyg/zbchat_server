/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-11-02 20:16:01
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-11-23 12:24:40
 * @FilePath: /coding/Server/StatusServer/ChatServerInfo.hpp
 */
#pragma once
#include <string>
struct ChatServerInfo{
    std::string _name;
    std::string _host;
    std::string _port;
    std::size_t _connect_cnt;
    ChatServerInfo(std::string name,std::string host,std::string port,std::size_t connect_cnt):_name(name),_host(host),_port(port),_connect_cnt(connect_cnt){ }
    ChatServerInfo(const ChatServerInfo& cs):_name(cs._name),_host(cs._host),_port(cs._port),_connect_cnt(cs._connect_cnt){ }
    ChatServerInfo& operator=(const ChatServerInfo& cs){
        if(&cs==this){
            return *this;
        }
        this->_connect_cnt = cs._connect_cnt;
        this->_host = cs._host;
        this->_name = cs._name;
        this->_port = cs._port;
        return *this;
    }
    inline std::string toString(){
        return std::string("name \"")+_name+"\" "+std::string("host \"")+_host+"\" "+std::string("port \"")+_port+"\" "+std::string("connect_cnt ")+std::to_string(_connect_cnt); 
    }
};