/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-10-10 21:47:09
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-10-31 09:19:27
 * @FilePath: /coding/Server/gateServer/VarifyGrpcClient.cpp
 */
#include "VarifyGrpcClient.h"
#include "Error.h"
#include "ConfigMgr.h"
VarifyGrpcClient::VarifyGrpcClient()
{
    std::shared_ptr<ConfigMgr> cfg = ConfigMgr::getInstance();
    _rpc_pool.reset(new RPCPool(5,(*cfg)["VerifyServer"]["Host"],(*cfg)["VerifyServer"]["Port"]));
}

message::GetVarifyRsp VarifyGrpcClient::getVarifyCode(int key,std::string email)
{
    grpc::ClientContext client_context;
    message::GetVarifyReq request;
    message::GetVarifyRsp response;
    request.set_email(email);
    request.set_key(key);
    auto stub = _rpc_pool->getConnetion();
    grpc::Status status = stub->GetVarifyCode(&client_context,request,&response);
    if(status.ok()==false){
        _rpc_pool->returnConnection(std::move(stub));
        response.set_error(ERR_NETWORK);
        return response;
    }
    _rpc_pool->returnConnection(std::move(stub));
    response.set_error(SUCCESS);
    return response;
}