// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: message.proto

#include "message.pb.h"
#include "message.grpc.pb.h"

#include <functional>
#include <grpcpp/support/async_stream.h>
#include <grpcpp/support/async_unary_call.h>
#include <grpcpp/impl/channel_interface.h>
#include <grpcpp/impl/client_unary_call.h>
#include <grpcpp/support/client_callback.h>
#include <grpcpp/support/message_allocator.h>
#include <grpcpp/support/method_handler.h>
#include <grpcpp/impl/rpc_service_method.h>
#include <grpcpp/support/server_callback.h>
#include <grpcpp/impl/server_callback_handlers.h>
#include <grpcpp/server_context.h>
#include <grpcpp/impl/service_type.h>
#include <grpcpp/support/sync_stream.h>
namespace message {

static const char* VarifyService_method_names[] = {
  "/message.VarifyService/GetVarifyCode",
};

std::unique_ptr< VarifyService::Stub> VarifyService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< VarifyService::Stub> stub(new VarifyService::Stub(channel, options));
  return stub;
}

VarifyService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_GetVarifyCode_(VarifyService_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status VarifyService::Stub::GetVarifyCode(::grpc::ClientContext* context, const ::message::GetVarifyReq& request, ::message::GetVarifyRsp* response) {
  return ::grpc::internal::BlockingUnaryCall< ::message::GetVarifyReq, ::message::GetVarifyRsp, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetVarifyCode_, context, request, response);
}

void VarifyService::Stub::async::GetVarifyCode(::grpc::ClientContext* context, const ::message::GetVarifyReq* request, ::message::GetVarifyRsp* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::message::GetVarifyReq, ::message::GetVarifyRsp, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetVarifyCode_, context, request, response, std::move(f));
}

void VarifyService::Stub::async::GetVarifyCode(::grpc::ClientContext* context, const ::message::GetVarifyReq* request, ::message::GetVarifyRsp* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetVarifyCode_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::message::GetVarifyRsp>* VarifyService::Stub::PrepareAsyncGetVarifyCodeRaw(::grpc::ClientContext* context, const ::message::GetVarifyReq& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::message::GetVarifyRsp, ::message::GetVarifyReq, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetVarifyCode_, context, request);
}

::grpc::ClientAsyncResponseReader< ::message::GetVarifyRsp>* VarifyService::Stub::AsyncGetVarifyCodeRaw(::grpc::ClientContext* context, const ::message::GetVarifyReq& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetVarifyCodeRaw(context, request, cq);
  result->StartCall();
  return result;
}

VarifyService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      VarifyService_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< VarifyService::Service, ::message::GetVarifyReq, ::message::GetVarifyRsp, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](VarifyService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::message::GetVarifyReq* req,
             ::message::GetVarifyRsp* resp) {
               return service->GetVarifyCode(ctx, req, resp);
             }, this)));
}

VarifyService::Service::~Service() {
}

::grpc::Status VarifyService::Service::GetVarifyCode(::grpc::ServerContext* context, const ::message::GetVarifyReq* request, ::message::GetVarifyRsp* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


static const char* StatusService_method_names[] = {
  "/message.StatusService/GetChatServer",
};

std::unique_ptr< StatusService::Stub> StatusService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< StatusService::Stub> stub(new StatusService::Stub(channel, options));
  return stub;
}

StatusService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_GetChatServer_(StatusService_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status StatusService::Stub::GetChatServer(::grpc::ClientContext* context, const ::message::GetChatServerReq& request, ::message::GetChatServerRsp* response) {
  return ::grpc::internal::BlockingUnaryCall< ::message::GetChatServerReq, ::message::GetChatServerRsp, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetChatServer_, context, request, response);
}

void StatusService::Stub::async::GetChatServer(::grpc::ClientContext* context, const ::message::GetChatServerReq* request, ::message::GetChatServerRsp* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::message::GetChatServerReq, ::message::GetChatServerRsp, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetChatServer_, context, request, response, std::move(f));
}

void StatusService::Stub::async::GetChatServer(::grpc::ClientContext* context, const ::message::GetChatServerReq* request, ::message::GetChatServerRsp* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetChatServer_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::message::GetChatServerRsp>* StatusService::Stub::PrepareAsyncGetChatServerRaw(::grpc::ClientContext* context, const ::message::GetChatServerReq& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::message::GetChatServerRsp, ::message::GetChatServerReq, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetChatServer_, context, request);
}

::grpc::ClientAsyncResponseReader< ::message::GetChatServerRsp>* StatusService::Stub::AsyncGetChatServerRaw(::grpc::ClientContext* context, const ::message::GetChatServerReq& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetChatServerRaw(context, request, cq);
  result->StartCall();
  return result;
}

StatusService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      StatusService_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< StatusService::Service, ::message::GetChatServerReq, ::message::GetChatServerRsp, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](StatusService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::message::GetChatServerReq* req,
             ::message::GetChatServerRsp* resp) {
               return service->GetChatServer(ctx, req, resp);
             }, this)));
}

StatusService::Service::~Service() {
}

::grpc::Status StatusService::Service::GetChatServer(::grpc::ServerContext* context, const ::message::GetChatServerReq* request, ::message::GetChatServerRsp* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace message

