/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-11-03 16:45:16
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-11-30 09:38:52
 * @FilePath: /coding/Server/ChatServer/ChatSession.cpp
 */
#include "ChatSession.h"
#include "LogicSystem.h"
#include "ChatServer.h"
#include "RedisMgr.h"
#include "UserMgr.h"
#include "log.h"
#include "global.h"
ChatSession::ChatSession(boost::asio::io_context& ioc,ChatServer* chat_server,std::string uuid)
:_sock(ioc),_chat_server(chat_server),_id(uuid),_stop(false)
{
    
    
}

ChatSession::~ChatSession()
{
    if(_stop==false){
        close();
    }
    _stop = true;
}


void ChatSession::start()
{
    boost::asio::ip::tcp::endpoint ep = _sock.remote_endpoint();
    consoleLog("用户 ",ep.address().to_string(),":",ep.port()," 连接成功");
    this->_user_data._ip = ep.address().to_string();
    this->_user_data._port = std::to_string(ep.port());
    auto self = shared_from_this();
    boost::asio::async_read(_sock,boost::asio::buffer<char,HEAD_LEN>(_recv_data._head,HEAD_LEN),boost::asio::transfer_exactly(HEAD_LEN),std::bind(&ChatSession::readHeadCallBack,this,self,std::placeholders::_1,std::placeholders::_2));
}

void ChatSession::close()
{
    if(_stop == true){
        return;
    }   
    _stop = true;
    if(_sock.is_open()){
        this->_sock.cancel();
        this->_sock.close();
    }
    this->_chat_server->erase(this->_id);
    UserMgr::getInstance()->rmUserSession(this->_user_data._uid);
    RedisConnectionMgr::ErrorCode ec;
    do{
        ec=RedisConnectionMgr::getInstance()->del(login_server+std::to_string(this->_user_data._uid));
    }while(ec!=RedisConnectionMgr::ErrorCode::SUCCESS); //删除redis链接
    do{
        ec=RedisConnectionMgr::getInstance()->srem(login_set,this->_user_data._email);
    }while(ec!=RedisConnectionMgr::ErrorCode::SUCCESS); //删除redis链接
    consoleLog("用户 ",this->_user_data._ip,":",this->_user_data._port," 断开连接");
}   

void ChatSession::readHeadCallBack(std::shared_ptr<ChatSession> self,boost::system::error_code ec,std::size_t transfer_size)
{
    if(transfer_size == 0){
        close();
        return;
    }
    if(ec){
        consoleLog("Exception in readHeadCallBack, error message is ",ec.message());
        close();
        return;
    }
    unsigned short int msg_id = 0;
    memcpy(&msg_id,_recv_data._head,HEAD_ID_LEN);
    msg_id = boost::asio::detail::socket_ops::network_to_host_short(msg_id);
    _recv_data._msg_id = msg_id;
    unsigned short int msg_len = 0;
    memcpy(&msg_len,_recv_data._head+HEAD_ID_LEN,HEAD_DATA_LEN);
    msg_len = boost::asio::detail::socket_ops::network_to_host_short(msg_len);
    _recv_data._data.resize(msg_len+1);
    boost::asio::async_read(_sock,boost::asio::buffer(_recv_data._data),boost::asio::transfer_exactly(msg_len),std::bind(&ChatSession::readBodyCallBack,this,self,std::placeholders::_1,std::placeholders::_2));
}

void ChatSession::readBodyCallBack(std::shared_ptr<ChatSession> self,boost::system::error_code ec,std::size_t transfer_size)
{
    if(ec){
        consoleLog("Exception in readBodyCallBack, error message is ",ec.message());
        close();
        return;
    }
    LogicSystem::getInstance()->postProcessNode(self,_recv_data._msg_id,_recv_data._data);
    _recv_data.clear();
    boost::asio::async_read(_sock,boost::asio::buffer(_recv_data._head,HEAD_LEN),boost::asio::transfer_exactly(HEAD_LEN),std::bind(&ChatSession::readHeadCallBack,this,self,std::placeholders::_1,std::placeholders::_2)); //继续读取该链接
}

void ChatSession::send(unsigned short int msg_id,std::string data)
{
    std::shared_ptr<DataNode> node = std::make_shared<DataNode>(msg_id,data);
    std::unique_lock<std::mutex> lock(_send_data_mux);
    if(_send_datas.size() == 0){
        auto self = shared_from_this();
        boost::asio::async_write(_sock,boost::asio::buffer(node->_head,HEAD_LEN),boost::asio::transfer_exactly(HEAD_LEN),std::bind(&ChatSession::writeHeadCallBack,this,self,node,std::placeholders::_1,std::placeholders::_2));
    }else{
        _send_datas.push(node);
    }
}

void ChatSession::writeHeadCallBack(std::shared_ptr<ChatSession> self,std::shared_ptr<DataNode> node,boost::system::error_code ec,std::size_t transfer_size)
{
    if(ec){
        consoleLog("Exception in writeHeadCallBack, error message is ",ec.message());
        close();
        return;
    }
    boost::asio::async_write(_sock,boost::asio::buffer(node->_data),boost::asio::transfer_exactly(node->_data.size()),std::bind(&ChatSession::writeBodyCallBack,this,self,node,std::placeholders::_1,std::placeholders::_2));
}

void ChatSession::writeBodyCallBack(std::shared_ptr<ChatSession> self,std::shared_ptr<DataNode> node,boost::system::error_code ec,std::size_t transfer_size)
{
    if(ec){
        consoleLog("Exception in writeBodyCallBack, error message is ",ec.message());
        close();
        return;
    }
    std::unique_lock<std::mutex> lock(_send_data_mux);
    if(_send_datas.empty()==false){
        std::shared_ptr<DataNode> node = _send_datas.front();
        _send_datas.pop();
        boost::asio::async_write(_sock,boost::asio::buffer(node->_head,HEAD_LEN),boost::asio::transfer_exactly(HEAD_LEN),std::bind(&ChatSession::writeHeadCallBack,this,self,node,std::placeholders::_1,std::placeholders::_2));
    }
}

void ChatSession::regCallBack(int reqid,std::string uuid,std::function<void()> func)
{
    std::unique_lock<std::mutex> lock(this->_callback_mux);
    auto map_it = this->_callback_handlers.find(reqid);
    if(map_it == this->_callback_handlers.end()){
        std::map<std::string,std::function<void()>> map;
        map.insert(std::pair<std::string,std::function<void()>>(uuid,func));
        this->_callback_handlers.insert(std::pair<int,std::map<std::string,std::function<void()>>>(reqid,std::move(map)));
    }else{
        std::map<std::string,std::function<void()>>& map = map_it->second;
        map.insert(std::pair<std::string,std::function<void()>>(uuid,func));
    }
}

int ChatSession::executeCallBack(int reqid,const std::string&uuid)
{
    std::unique_lock<std::mutex> lock(this->_callback_mux);
    auto map_it = this->_callback_handlers.find(reqid);
    if(map_it == this->_callback_handlers.end()){
        return -1;
    }else{
        std::map<std::string,std::function<void()>>& map = map_it->second;
        auto it = map.find(uuid);
        if(it==map.end()){
            return -1;
        }
        it->second(); //执行唤醒函数
        map.erase(it); //call_back中删除
        return 0;
    }
}

int ChatSession::rmCallBack(int reqid,const std::string&uuid)
{
    std::unique_lock<std::mutex> lock(this->_callback_mux);
    auto map_it = this->_callback_handlers.find(reqid);
    if(map_it == this->_callback_handlers.end()){
        return -1;
    }else{
        std::map<std::string,std::function<void()>>& map = map_it->second;
        auto it = map.find(uuid);
        if(it==map.end()){
            return -1;
        }
        map.erase(it); //call_back中删除
        return 0;
    }
}