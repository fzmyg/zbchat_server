/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-10-07 16:04:12
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-11-06 14:38:45
 * @FilePath: /coding/gateServer/HttpConnection.cpp
 */
#include "HttpConnection.h"
#include "LogicSystem.h"
#include <iostream>

//启动会话
void HttpConnection::start()
{
    checkDeadLine(); //启动计时
    recvRequest();   //接收数据
}

boost::asio::ip::tcp::socket& HttpConnection::getSocket()
{   
    return this->_sock;
}

//检查连接时间
void HttpConnection::checkDeadLine()
{
    auto self = shared_from_this();
    self->_timer.async_wait([self](boost::system::error_code ec){
        if(ec&&ec!=boost::asio::error::operation_aborted){//若没有被手动取消
            std::cerr<<"Time out:"<<ec.message()<<std::endl;
            self->disConnect();
            return;
        }
    });
}

//断开连接
void HttpConnection::disConnect()
{
    if(this->_sock.is_open()){
        this->_sock.close();
        this->_timer.cancel();
    }
}

//接收客户端请求
void HttpConnection::recvRequest()
{
    auto self = shared_from_this();
    boost::beast::http::async_read(_sock,_buf,_request,std::bind(&HttpConnection::recvRequestCallBack,this,self,std::placeholders::_1,std::placeholders::_2));
}

//接收请求回调
void HttpConnection::recvRequestCallBack(std::shared_ptr<HttpConnection>self,boost::beast::error_code ec,std::size_t size)
{
    if(ec){
        std::cerr<<ec.message()<<std::endl;
        disConnect();
        return;
    }
    parseRequest();
    sendResponse();
}


//解析请求
void HttpConnection::parseRequest()
{
    _response.version(_request.version()); //设置http版本号与请求相同
    _response.keep_alive(false);           //不保持长连接
    switch(_request.method()){
        case boost::beast::http::verb::get: //get请求
            if(preParseUrl()==false){ // //解析url参数错误
                std::cerr<<"解析url参数错误"<<std::endl;
                _response.result(boost::beast::http::status::not_found);
                _response.set(boost::beast::http::field::content_type,"text/plain");
                boost::beast::ostream(_response.body())<<"Parse url error\r\n";
                return;
            } 
            if(LogicSystem::getInstance()->handleHttpGet(_url_path,shared_from_this())==true){ //处理get请求
                _response.result(boost::beast::http::status::ok);
                _response.set(boost::beast::http::field::server,"GateServer");
                return;
            }else{//出现出错误需重新处理
                std::cerr<<"处理get请求出错"<<std::endl;
                _response.result(boost::beast::http::status::not_found);
                _response.set(boost::beast::http::field::content_type,"text/plain");
                boost::beast::ostream(_response.body())<<"url not found!\r\n";
            }
            break;
        case boost::beast::http::verb::post://post请求
            if(LogicSystem::getInstance()->handleHttpPost(_request.target(),shared_from_this())==true){
                _response.result(boost::beast::http::status::ok);
                _response.set(boost::beast::http::field::server,"GateServer");
                return;
            }else{ //请求错误
                std::cerr<<"处理post请求出错"<<std::endl;
                _response.result(boost::beast::http::status::not_found);
                _response.set(boost::beast::http::field::content_type,"text/plain");
                boost::beast::ostream(_response.body())<<"url not found!\r\n";
            }
            break;
        default: //未匹配到http method
            _response.result(boost::beast::http::status::not_found);
            _response.set(boost::beast::http::field::content_type,"text/plain");
            boost::beast::ostream(_response.body())<<"url not found!\r\n";
            break;
    }
}

//发送回复报文
void HttpConnection::sendResponse()
{
    auto self = shared_from_this();
    _response.content_length(_response.body().size());
    boost::beast::http::async_write(_sock,_response,std::bind(&HttpConnection::sendResponseCallBack,this,self,std::placeholders::_1,std::placeholders::_2));
}


void HttpConnection::sendResponseCallBack(std::shared_ptr<HttpConnection> self,boost::beast::error_code ec,std::size_t size)
{
    if(ec){
        std::cerr<<ec.message()<<std::endl;
        disConnect();
        return;
    }
    this->_timer.cancel();
    //self->_sock.shutdown(boost::asio::ip::tcp::socket::shutdown_send);
    self->_sock.close();
}


bool HttpConnection::preParseUrl()
{
    // /path?key1=val1&key2=val2
    std::string target = _request.target();
    int pos = target.find('?');
    if(pos == std::string::npos || pos==target.size()-1){ // 没有？也可正确解析
        _url_path = target.substr(0,pos);
        return true;
    }
    _url_path = target.substr(0,pos);
    // key1=val1&key2=val2
    std::string params = target.substr(pos+1);
    while((pos = params.find('&'))!=std::string::npos){
        std::string pair = params.substr(0,pos);
        int equal_pos = pair.find('=');
        if(equal_pos==std::string::npos){
            return false; //一个键值对里未找到=则判断为错误
        }
        _url_params.insert(std::pair<std::string,std::string>(decodeUrl(pair.substr(0,equal_pos)),decodeUrl(pair.substr(equal_pos+1))));
        params.erase(0,pos+1);
    }
    std::string pair = params;
    int equal_pos = pair.find('=');
    if(equal_pos==std::string::npos){
        return false; //一个键值对里未找到=则判断为错误
    }
    _url_params.insert(std::pair<std::string,std::string>(decodeUrl(pair.substr(0,equal_pos)),decodeUrl(pair.substr(equal_pos+1))));
    
    return true;
}

void HttpConnection::setRemoteHost(std::string address,boost::asio::ip::port_type port)
{
    this->_remote_host = address;
    this->_remote_port = port;
    std::cerr<<"host:"<<_remote_host<<":"<<_remote_port<<"链接成功"<<std::endl;
}