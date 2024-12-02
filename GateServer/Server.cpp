/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-10-07 15:21:12
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-10-18 11:05:17
 * @FilePath: /coding/gateServer/Server.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "IOContextPool.h"
#include "Server.h"
#include "HttpConnection.h"
#include <iostream>
void Server::start()
{
    auto self = shared_from_this();
    std::shared_ptr<HttpConnection> session=std::make_shared<HttpConnection>(IOContextPool::getInstance()->getIOService());
    _actor.async_accept(session->getSocket(),std::bind(&Server::acceptCallBack,this,self,session,std::placeholders::_1));
}

void Server::acceptCallBack(std::shared_ptr<Server> self,std::shared_ptr<HttpConnection> session,boost::system::error_code ec)
{
    if(ec){
        std::cerr<<"Error in accept"<<ec.message()<<std::endl;
        return ;
    }
    session->setRemoteHost(session->getSocket().remote_endpoint().address().to_string(),session->getSocket().remote_endpoint().port());
    session->start();
    this->start();
}
