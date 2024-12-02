/*
 * @Author: zbccc zbccc.p@gmail.com
 * @Date: 2024-11-26 08:35:31
 * @LastEditors: zbccc zbccc.p@gmail.com
 * @LastEditTime: 2024-11-26 18:14:07
 * @FilePath: /coding/Server/ResouceServer/ResourceServiceImpl.h
 */
#pragma once
#include "message.grpc.pb.h"
#include "message.pb.h"
#include <grpcpp/grpcpp.h>

class ResourceServiceImpl
:public message::ResourceService::Service
{
public:
    grpc::Status AddIcon(grpc::ServerContext*context,message::AddIconReq*req,message::AddIconRsp*rsp);

    grpc::Status GetIcon(grpc::ServerContext*context,message::GetIconReq*req,message::GetIconRsp*rsp);
};