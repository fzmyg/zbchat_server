/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-11-26 08:35:31
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-11-26 12:33:03
 * @FilePath: /coding/Server/ResouceServer/main.cpp
 */
#include "ResourceServer.h"
#include "log.h"
#include "ConfigMgr.h"
#include <boost/asio.hpp>
#include <csignal>
int main()
{
    try{
        auto config_mgr = ConfigMgr::getInstance();
        //std::cerr<<(*config_mgr)["ResourceServer"]["Host"];
        ResourceServer server((*config_mgr)["ResourceServer"]["Host"],(*config_mgr)["ResourceServer"]["Port"]);
        boost::asio::io_context ioc;
        boost::asio::signal_set sig_set(ioc,SIGTERM,SIGINT);
        sig_set.async_wait([&](boost::system::error_code ec,int sig)mutable{
            server.stop();
            consoleLog("收到信号:",sig);
        });
        ioc.run();
    }catch(std::exception& e){
        consoleLog(e.what());
    }
}