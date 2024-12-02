#include "ResourceGrpcClient.h"
#include "ConfigMgr.h"
#include "Error.h"
#include "log.h"
static std::string xorEncryptPassword(const std::string &str)
{
    char len = static_cast<char>(str.size());
    std::string ret (str);
    for(int i = 0;i<len;i++)
    {
        ret[i] = ret[i] ^ len;
    }
    return ret;
}

ResourcePool::ResourcePool(std::size_t size,std::string host,std::string port)
:_size(size),_host(host),_port(port),_stop(false)
{
    for(int i = 0;i < size ;i++){
        std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel(host+":"+port,grpc::InsecureChannelCredentials());
        if(channel==nullptr){
            i--;
            continue;
        }
        std::unique_ptr<message::ResourceService::Stub> stub (message::ResourceService::NewStub(channel));
        if(stub==nullptr){
            i--;
            continue;
        }
        std::cerr<<"创建StautsServer::Stub"<<i+1<<"成功"<<std::endl;
        _connections.emplace(std::move(stub));
    }
}

ResourcePool::~ResourcePool()
{
   close();
   std::unique_lock<std::mutex> lock(_mux);
   std::size_t size = _connections.size();
   for(int i = 0;i<size;i++){
        _connections.pop();
   }
}

std::unique_ptr<message::ResourceService::Stub> ResourcePool::getConnection()
{
    if(_stop){
        return nullptr;
    }
    std::unique_ptr<message::ResourceService::Stub> ret_ptr;
    {
        std::unique_lock<std::mutex> lock(_mux);
        _cv.wait(lock,[this](){
            return _connections.empty()==false || _stop;
        });
        if(_stop)
            return nullptr;
        ret_ptr = std::move(_connections.front());
        _connections.pop();
    }
    return std::move(ret_ptr);
}

void ResourcePool::returnConnection(std::unique_ptr<message::ResourceService::Stub> ret_ptr)
{
    if(_stop){
        return;
    }
    {
        std::unique_lock<std::mutex> lock(_mux);
        _connections.emplace(std::move(ret_ptr));
        _cv.notify_one();
    }
}

void ResourcePool::close()
{
    if(_stop == false){
        _stop=true;
        _cv.notify_all();
    }
}

ResourceGrpcClient::~ResourceGrpcClient()
{
    consoleLog("资源服务客户端退出");
}

ResourceGrpcClient::ResourceGrpcClient()
{
    consoleLog("资源服务启动");
    std::shared_ptr<ConfigMgr> cfg = ConfigMgr::getInstance();
    std::string host = (*cfg)["ResourceServer"]["Host"];
    std::string port = (*cfg)["ResourceServer"]["Port"];
    _pool.reset(new ResourcePool(5,host,port));
}

message::AddIconRsp ResourceGrpcClient::addIcon(int uid,std::string icon_pixmap)
{
    grpc::ClientContext cont;
    message::AddIconReq req;
    message::AddIconRsp rsp;
    req.set_uid(uid);
    req.set_contents(icon_pixmap);
    std::unique_ptr<message::ResourceService::Stub> stub =  _pool->getConnection();
    if(stub==nullptr){
        rsp.set_error(ERR_NETWORK);
        return rsp;
    }
    grpc::Status stat = stub->AddIcon(&cont,req,&rsp);
    if(stat.ok()==false){
        rsp.set_error(ERR_NETWORK);
    }
    _pool->returnConnection(std::move(stub));
    return rsp;
}

 message::GetIconRsp ResourceGrpcClient::getIcon(std::string url)
 {
    grpc::ClientContext cont;
    message::GetIconReq req;
    message::GetIconRsp rsp;
    req.set_url(url);
    std::unique_ptr<message::ResourceService::Stub> stub =  _pool->getConnection();
    if(stub==nullptr){
        rsp.set_error(ERR_NETWORK);
        return rsp;
    }
    grpc::Status stat = stub->GetIcon(&cont,req,&rsp);
    if(stat.ok()==false){
        rsp.set_error(ERR_NETWORK);
    }
    _pool->returnConnection(std::move(stub));
    return rsp;
 }

