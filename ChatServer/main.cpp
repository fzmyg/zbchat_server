/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-11-01 23:00:04
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-11-30 09:46:40
 * @FilePath: /coding/Server/ChatServer/main.cpp
 */
#include "ConfigMgr.h"
#include "AsioIOServicePool.h"
#include "ChatServer.h"
#include "log.h"
#include "LogicSystem.h"
#include "StatusGrpcClient.h"
#include "ChatGrpcServer.h"
#include "ChatGrpcClient.h"
#include "ResourceGrpcClient.h"
#include "RedisMgr.h"
#include "MySQLMgr.h"
#include <iostream>
#include <exception>
#include <boost/asio.hpp>
#include <csignal>
#include <string>

// 接收多个参数 参数数不少于2个(argc>=3) 第一个参数为用于客户端连接的chat_server_port,第二个参数为用于其他chat_server连接的grpc端口，其余参数为其他服务器grpc端口
int main(int argc,char**argv)
{
    if(argc<=3){
        std::cerr<<"right format : ./a.out [chat_port_number] [chat_rpc_port_number] [peer_chat_rpc_address]..."<<std::endl;
        return EXIT_FAILURE;
    }
    try{
        boost::asio::io_context ioc; //负责接收连接
        RedisConnectionMgr::getInstance();
        MySQLMgr::getInstance();
        auto server = ChatServer::getInstance(ioc,atoi(argv[1])); //初始化聊天服务器
        SelfChatServerInfo::getInstance("101.200.14.216",argv[1],argv[2]); //初始化服务器连接信息
        boost::asio::signal_set signal_set(ioc,SIGTERM,SIGINT); //接收关闭信号
        auto io_pool = AsioIOServicePool::getInstance();    //实例化io_c 池，池中io_context负责与socket通信
        auto logic_system = LogicSystem::getInstance();     //实例化逻辑处理系统
        StatusGrpcClient::getInstance();                    //实例化与StatusServer通信的客户端
        ResourceGrpcClient::getInstance();
        auto chat_grpc_server = ChatGrpcServer::getInstance("0.0.0.0",std::string(argv[2]));
        ChatGrpcClient::getInstance(argv+3,argc-3);
        signal_set.async_wait([&](boost::system::error_code ec,int sig_num)mutable{
            server->stop(); //不再接收链接请求
            logic_system->stop(); //处理完待处理数据加入ioc中
            io_pool->stop(); //清理占位work，不调用ioc.stop(),等待所有异步操作处理完后退出
            chat_grpc_server->close();
        });
        ioc.run();
    }catch(std::exception&e){
        consoleLog("Exception",e.what());
    }
    return EXIT_SUCCESS;
}