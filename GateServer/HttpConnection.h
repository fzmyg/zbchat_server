/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-10-07 16:04:20
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-10-27 18:43:07
 * @FilePath: /coding/gateServer/HttpConnection.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <unordered_map>
#include <iostream>
#include <string>
#include <memory>
#include <thread>
class LogicSystem;

class HttpConnection
:public std::enable_shared_from_this<HttpConnection>
{
    friend class LogicSystem;
public: //FUNCTION 
    HttpConnection(boost::asio::io_context&ioc)
    :_sock(ioc)
    ,_timer(ioc,boost::asio::chrono::seconds(60))
    {
        ;
    }
    HttpConnection(boost::asio::ip::tcp::socket &&sock)
    :_sock(std::forward<boost::asio::ip::tcp::socket>(sock))
    ,_timer(_sock.get_executor(),boost::asio::chrono::seconds(60))
    ,_remote_host(_sock.remote_endpoint().address().to_string())
    ,_remote_port(_sock.remote_endpoint().port())
    {
        std::cerr<<"host:"<<_remote_host<<":"<<_remote_port<<" 链接成功"<<std::endl;
    }
    ~HttpConnection(){
        std::cerr<<"host:"<<_remote_host<<":"<<_remote_port<<" 断开链接"<<std::endl;
    }

    void start();
    boost::asio::ip::tcp::socket & getSocket();
    void setRemoteHost(std::string address,boost::asio::ip::port_type port);
private: //FUNCTION
    void checkDeadLine();
    void recvRequest();
    void recvRequestCallBack(std::shared_ptr<HttpConnection>self,boost::beast::error_code ec,std::size_t size);
    void parseRequest();
    void sendResponse();
    void sendResponseCallBack(std::shared_ptr<HttpConnection>self,boost::beast::error_code ec,std::size_t size);
    void disConnect();
    bool preParseUrl();

private: //VARIABLE
    boost::beast::http::request<boost::beast::http::dynamic_body> _request;
    boost::beast::http::response<boost::beast::http::dynamic_body> _response;
    boost::beast::flat_buffer _buf{8192};
    boost::asio::ip::tcp::socket _sock; //
    std::string _remote_host;
    boost::asio::ip::port_type _remote_port;
    boost::asio::steady_timer _timer;
    std::string _url_path; //url 请求目录
    std::unordered_map<std::string,std::string> _url_params;//url 参数
};