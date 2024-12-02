/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-11-26 08:35:31
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-11-26 09:09:13
 * @FilePath: /coding/Server/ResouceServer/ResourceServer.cpp
 */
#include "ResourceServer.h"
#include "log.h"
ResourceServer::ResourceServer(const std::string &ip, const std::string &port)
{
    grpc::ServerBuilder builder;
    ResourceServiceImpl * service = new ResourceServiceImpl;
    builder.AddListeningPort(ip+":"+port,grpc::InsecureServerCredentials());
    builder.RegisterService(service);
    _server = std::move(builder.BuildAndStart());
    consoleLog("资源服务器启动成功，监听ip:",ip+":"+port);
}
ResourceServer::~ResourceServer()
{
    if(_stop.load()==false)
        this->stop();
    consoleLog("资源服务器关闭");
}

void ResourceServer::stop()
{
    if(_stop.load()==true)
        return;
    _stop = true;
    _server->Shutdown();
}