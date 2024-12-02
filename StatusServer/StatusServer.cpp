/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-11-01 18:29:49
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-11-03 11:21:16
 * @FilePath: /coding/Server/StatusServer/StatusServer.cpp
 */
#include "StatusServer.h"

StatusServer::StatusServer()
{
    
    grpc::ServerBuilder builder;
    StatusServiceImpl *service = new StatusServiceImpl;
    std::shared_ptr<ConfigMgr> conf = ConfigMgr::getInstance();
    std::string address_url = (*conf)["StatusServer"]["ListenAddress"]+":"+(*conf)["StatusServer"]["ListenPort"];
    builder.AddListeningPort(address_url,grpc::InsecureServerCredentials());
    builder.RegisterService(service);
    _server = std::move(builder.BuildAndStart());
    boost::asio::signal_set signals(_ioc,SIGINT,SIGTERM); //键盘信号 和 终止信号
    signals.async_wait([&](boost::system::error_code ec,int sig_num)mutable{
        //状态服务器关闭
        _server->Shutdown();
        _ioc.stop();
    });
    _sig_thread.reset(new std::thread([&]()mutable{
        _ioc.run();
    }));
    _sig_thread->detach();
    std::cerr<<"StatusServer启动成功,监听地址:"<<address_url<<std::endl;
    _server->Wait();
}
StatusServer::~StatusServer()
{
    std::cerr<<"StatusServer 正常关闭"<<std::endl;
}