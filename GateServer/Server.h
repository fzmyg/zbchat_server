/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-10-07 15:21:07
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-10-27 18:30:29
 * @FilePath: /coding/gateServer/Server.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once
#include <memory>
#include <boost/asio.hpp>
#include <iostream>
#include <thread>
class HttpConnection;
class Server:
public std::enable_shared_from_this<Server>
{
public:
    Server(boost::asio::io_context&ioc,boost::asio::ip::port_type port_number):
    _ioc(ioc),
    _actor(ioc,boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4::any(),port_number))
    {
        std::cout<<"gate服务器成功启动 线程id="<<std::this_thread::get_id()<<std::endl;
    }
    void start();
private:
    void acceptCallBack(std::shared_ptr<Server> self,std::shared_ptr<HttpConnection> session,boost::system::error_code ec);
private:
    boost::asio::io_context & _ioc;
    boost::asio::ip::tcp::acceptor _actor;
};