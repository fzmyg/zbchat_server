/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-11-01 23:02:04
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-11-27 18:58:43
 * @FilePath: /coding/Server/ChatServer/ChatServer.cpp
 */
#include "ChatServer.h"
#include "AsioIOServicePool.h"
#include "log.h"
#include <iostream>
#include <boost/uuid.hpp>

ChatServer::~ChatServer()
{
    if(_stop==false)
        stop();
}

void ChatServer::stop()
{
    _stop=true;
    _ioc.stop();
}

ChatServer::ChatServer(boost::asio::io_context& ioc,unsigned short port)
:_ator(ioc,boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("0.0.0.0"),port)),_ioc(ioc),_stop(false)
{
    std::cerr<<"Server listening on 0.0.0.0:"<<port<<std::endl;
    startAccept();
}

void ChatServer::startAccept()
{
    boost::asio::io_context& ioc = AsioIOServicePool::getInstance()->getIOService();
    std::shared_ptr<ChatSession> session = std::make_shared<ChatSession>(ioc,this,generateUuid());
    std::unique_lock<std::mutex> lock(_mux);
    _sessions.insert(std::pair<std::string,std::shared_ptr<ChatSession>>(session->getId(),session));
    _ator.async_accept(session->getSocket(),std::bind(&ChatServer::acceptCallBack,this,session,std::placeholders::_1));
}

void ChatServer::acceptCallBack(std::shared_ptr<ChatSession> session,boost::system::error_code ec)
{
    if(ec){
        _sessions.erase(session->getId());
        std::cerr<<"session accept failed , error is"<<ec.what()<<std::endl;
        return;
    }
    session->start();
    startAccept();
}


std::string ChatServer::generateUuid()
{
    return boost::uuids::to_string(boost::uuids::random_generator()());
}

void ChatServer::erase(std::string session_id)
{
    std::unique_lock<std::mutex> lock(_mux);
    _sessions.erase(session_id);
}