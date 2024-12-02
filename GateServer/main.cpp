/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-10-07 15:20:00
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-11-06 21:57:57
 * @FilePath: /coding/gateServer/main.cpp
 */
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <boost/signals2.hpp>
#include <iostream>
#include <jsoncpp/json/json.h>
#include <cassert>
#include <csignal>
#include "Server.h"
#include "ConfigMgr.h"
#include "RedisMgr.h"
#include "IOContextPool.h"
#include "MySQLMgr.h"
#include "StatusGrpcClient.h"
#include "VarifyGrpcClient.h"
int main()
{
    try{
        boost::asio::io_context ioc;
        std::shared_ptr<Server> server=std::make_shared<Server>(ioc,8080);
        server->start();
        ConfigMgr::getInstance();
        RedisConnectionMgr::getInstance();
        MySQLMgr::getInstance();
        StatusGrpcClient::getInstance();
        VarifyGrpcClient::getInstance();
        boost::asio::signal_set sig_set(ioc,SIGINT,SIGTERM);
        std::shared_ptr<IOContextPool> pool = IOContextPool::getInstance();
        sig_set.async_wait([&ioc,pool](boost::system::error_code ec,int signal){
            if(ec){
                return;
            }
            ioc.stop();
            pool->stop();
            std::cerr<<"程序正常终止"<<std::endl;
        });
        ioc.run();
    }catch(std::exception&e){
        std::cerr<<"Error:"<<e.what()<<std::endl;
        return EXIT_FAILURE;
    }
    return 0;
}