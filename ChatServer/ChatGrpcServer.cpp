/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-11-22 22:47:35
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-11-29 21:45:21
 * @FilePath: /coding/Server/ChatServer/ChatGrpcServer.cpp
 */
#include "ChatGrpcServer.h"
#include "ChatGrpcImpl.h"
#include "ConfigMgr.h"
#include "global.h"
ChatGrpcServer::ChatGrpcServer(const std::string& host,const std::string& port)
{
    //加载传输证书和密钥
    std::string server_key = readFile("./ssl/server.key");
    std::string server_cert = readFile("./ssl/server.crt");
    std::string ca_cert = readFile("./ssl/ca.crt");
    // 配置 SSL/TLS
    grpc::SslServerCredentialsOptions::PemKeyCertPair key_cert = {server_key, server_cert};
    grpc::SslServerCredentialsOptions ssl_opts;
    ssl_opts.pem_key_cert_pairs.push_back(key_cert);
    ssl_opts.pem_root_certs = ca_cert;
    std::shared_ptr<grpc::ServerCredentials> server_creds = grpc::SslServerCredentials(ssl_opts);

    grpc::ServerBuilder builder;
    ChatServiceImpl *service = new ChatServiceImpl;
    std::string address_url = host+":"+port;
    //builder.AddListeningPort(address_url,server_creds);
    builder.AddListeningPort(address_url,grpc::InsecureServerCredentials());
    builder.RegisterService(service);
    _server = std::move(builder.BuildAndStart());
    //boost::asio::signal_set signals(_ioc,SIGINT,SIGTERM); //键盘信号 和 终止信号
    //signals.async_wait([&](boost::system::error_code ec,int sig_num)mutable{
        //ChatGrpc服务器关闭
        //_server->Shutdown();
        //_ioc.stop();
    //});
    _server_thread.reset(new std::thread([&]()mutable{
        //_ioc.run();
        _server->Wait();
    }));
    _server_thread->detach();
    std::cerr<<"ChatGrpcServer启动成功,监听地址:"<<address_url<<std::endl;
    //_server->Wait();
}
ChatGrpcServer::~ChatGrpcServer()
{
    close();
    std::cerr<<"ChatGrpcServer 正常关闭"<<std::endl;
}
void ChatGrpcServer::close()
{
    if(_stop==true)
        return;
    _stop = true;
    _server->Shutdown();
}