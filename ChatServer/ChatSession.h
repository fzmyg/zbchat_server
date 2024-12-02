/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-11-03 16:44:41
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-11-30 09:37:25
 * @FilePath: /coding/Server/ChatServer/ChatSession.h
 */
#pragma once
#include <boost/asio.hpp>
#include <string>
#include <memory>
#include <queue>
#include <cstring>
#include <map>
#include <functional>
#include <atomic>
#define HEAD_LEN 4
#define HEAD_ID_LEN 2
#define HEAD_DATA_LEN 2
class ChatServer;

struct DataNode{
    char _head[4]={0};
    unsigned short int _msg_id = 0;
    std::string _data = "";
    DataNode()=default;
    DataNode(unsigned short int msg_id,std::string data):_msg_id(msg_id),_data(data){ 
        msg_id = boost::asio::detail::socket_ops::host_to_network_short(_msg_id);
        memcpy(_head,&msg_id,HEAD_ID_LEN);
        unsigned short int len =  boost::asio::detail::socket_ops::host_to_network_short(_data.size());
        memcpy(_head+HEAD_ID_LEN,&len,HEAD_DATA_LEN);
    }
    DataNode(const DataNode& node):_msg_id(node._msg_id),_data(node._data){
        unsigned short int msg_id = boost::asio::detail::socket_ops::host_to_network_short(_msg_id);
        memcpy(_head,&msg_id,HEAD_ID_LEN);
        unsigned short int len =  boost::asio::detail::socket_ops::host_to_network_short(_data.size());
        memcpy(_head+HEAD_ID_LEN,&len,HEAD_DATA_LEN);
    }
    DataNode(DataNode&& node):_msg_id(node._msg_id),_data(std::move(node._data)){ 
        unsigned short int msg_id = boost::asio::detail::socket_ops::host_to_network_short(_msg_id);
        memcpy(_head,&msg_id,HEAD_ID_LEN);
        unsigned short int len =  boost::asio::detail::socket_ops::host_to_network_short(_data.size());
        memcpy(_head+HEAD_ID_LEN,&len,HEAD_DATA_LEN);
    }
    void clear(){
        _msg_id = 0;
        memset(_head,0,4);
        _data.clear();
    }
};

struct ChatSessionUserData{
    ChatSessionUserData(){}
    ChatSessionUserData(int uid,int sex,int age,const std::string & email,const std::string&name,const std::string &nick
    ,const std::string& desc,const std::string& location,const std::string& icon):_uid(uid),_age(age),_sex(sex),_email(email),_name(name),_nick(nick),_desc(desc),_location(location),_icon(icon){};
    void setData(int uid,int sex,int age,const std::string & email,const std::string&name,const std::string &nick,const std::string& desc,const std::string& location,const std::string& icon){
        _uid = uid;
        _sex = sex;
        _age = age;
        _email = email;
        _name = name;
        _nick = nick;
        _desc = desc;
        _location = location;
        _icon = icon;
    }
    int _uid;
    int _sex;
    int _age;
    std::string _email;
    std::string _name;
    std::string _nick;
    std::string _desc;
    std::string _location;
    std::string _icon;
    std::string _ip;
    std::string _port;
};

class ChatSession
:public std::enable_shared_from_this<ChatSession>
{
public:
    ~ChatSession();
    ChatSession(boost::asio::io_context& ioc,ChatServer* chatserver,std::string uuid);
    inline boost::asio::ip::tcp::socket& getSocket(){ return _sock; }
    inline std::string& getId() { return _id;}
    inline void setUserData(int uid,int sex,int age,const std::string & email,const std::string&name,const std::string &nick,const std::string& desc,const std::string& location,const std::string& icon){_user_data.setData(uid,sex,age,email,name,nick,desc,location,icon);}
    void start();
    void close();
    void send(unsigned short int msg_id,std::string data);
    //inline void setUid(int uid){ this->_uid = uid; };
    void regCallBack(int reqid,std::string uuid,std::function<void()> func);
    int executeCallBack(int reqid,const std::string&uuid);
    int rmCallBack(int reqid,const std::string&uuid);
private:
    void readHeadCallBack(std::shared_ptr<ChatSession> self,boost::system::error_code ec,std::size_t transfer_size);
    void readBodyCallBack(std::shared_ptr<ChatSession> self,boost::system::error_code ec,std::size_t transfer_size);
    void writeHeadCallBack(std::shared_ptr<ChatSession> self,std::shared_ptr<DataNode> node,boost::system::error_code ec,std::size_t size);
    void writeBodyCallBack(std::shared_ptr<ChatSession> self,std::shared_ptr<DataNode> node,boost::system::error_code ec,std::size_t size);
private:
    std::atomic<bool> _stop;
    boost::asio::ip::tcp::socket _sock;
    ChatServer* _chat_server;
    DataNode _recv_data;
    std::string _id;
    //int _uid;
    std::queue<std::shared_ptr<DataNode>> _send_datas;
    std::mutex _send_data_mux;
    std::map<int,std::map<std::string,std::function<void()>>> _callback_handlers;
    std::mutex _callback_mux;
    ChatSessionUserData _user_data;
};