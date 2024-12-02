/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-10-07 21:12:05
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-10-26 16:04:02
 * @FilePath: /coding/gateServer/LogicSystem.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma  once
#include "Singleton.hpp"
#include <functional>
#include <map>

class HttpConnection;

class LogicSystem
:public Singleton<LogicSystem>
{
    friend Singleton<LogicSystem>;
    typedef std::function<bool(std::shared_ptr<HttpConnection>)> HttpHandler;
private:
    LogicSystem();
    void  registerHttpGet(std::string ,HttpHandler);
    void  registerHttpPost(std::string,HttpHandler);
public: 
    bool handleHttpGet(std::string url,std::shared_ptr<HttpConnection> session);
    bool handleHttpPost(std::string path,std::shared_ptr<HttpConnection> session);
    ~LogicSystem(){};
private:
    std::map<std::string,HttpHandler> _post_handlers; //post请求处理函数管理器
    std::map<std::string,HttpHandler> _get_handlers;  //get请求处理函数管理器
};


extern std::string encodeUrl(const std::string & str);
extern std::string decodeUrl(const std::string & str);
extern std::string xorEncryptPassword(const std::string &str);