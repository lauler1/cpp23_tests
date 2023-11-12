// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: math_service.proto

#include "math_service.pb.h"
#include "math_service.grpc.pb.h"

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
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/server_context.h>
#include <grpcpp/impl/service_type.h>
#include <grpcpp/support/sync_stream.h>
namespace mathservice {

static const char* MathService_method_names[] = {
  "/mathservice.MathService/Add",
  "/mathservice.MathService/Multiply",
};

std::unique_ptr< MathService::Stub> MathService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< MathService::Stub> stub(new MathService::Stub(channel, options));
  return stub;
}

MathService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_Add_(MathService_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_Multiply_(MathService_method_names[1], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status MathService::Stub::Add(::grpc::ClientContext* context, const ::mathservice::AddRequest& request, ::mathservice::AddResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::mathservice::AddRequest, ::mathservice::AddResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_Add_, context, request, response);
}

void MathService::Stub::async::Add(::grpc::ClientContext* context, const ::mathservice::AddRequest* request, ::mathservice::AddResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::mathservice::AddRequest, ::mathservice::AddResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Add_, context, request, response, std::move(f));
}

void MathService::Stub::async::Add(::grpc::ClientContext* context, const ::mathservice::AddRequest* request, ::mathservice::AddResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Add_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::mathservice::AddResponse>* MathService::Stub::PrepareAsyncAddRaw(::grpc::ClientContext* context, const ::mathservice::AddRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::mathservice::AddResponse, ::mathservice::AddRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_Add_, context, request);
}

::grpc::ClientAsyncResponseReader< ::mathservice::AddResponse>* MathService::Stub::AsyncAddRaw(::grpc::ClientContext* context, const ::mathservice::AddRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncAddRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status MathService::Stub::Multiply(::grpc::ClientContext* context, const ::mathservice::MultiplyRequest& request, ::mathservice::MultiplyResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::mathservice::MultiplyRequest, ::mathservice::MultiplyResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_Multiply_, context, request, response);
}

void MathService::Stub::async::Multiply(::grpc::ClientContext* context, const ::mathservice::MultiplyRequest* request, ::mathservice::MultiplyResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::mathservice::MultiplyRequest, ::mathservice::MultiplyResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Multiply_, context, request, response, std::move(f));
}

void MathService::Stub::async::Multiply(::grpc::ClientContext* context, const ::mathservice::MultiplyRequest* request, ::mathservice::MultiplyResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Multiply_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::mathservice::MultiplyResponse>* MathService::Stub::PrepareAsyncMultiplyRaw(::grpc::ClientContext* context, const ::mathservice::MultiplyRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::mathservice::MultiplyResponse, ::mathservice::MultiplyRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_Multiply_, context, request);
}

::grpc::ClientAsyncResponseReader< ::mathservice::MultiplyResponse>* MathService::Stub::AsyncMultiplyRaw(::grpc::ClientContext* context, const ::mathservice::MultiplyRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncMultiplyRaw(context, request, cq);
  result->StartCall();
  return result;
}

MathService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      MathService_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< MathService::Service, ::mathservice::AddRequest, ::mathservice::AddResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](MathService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::mathservice::AddRequest* req,
             ::mathservice::AddResponse* resp) {
               return service->Add(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      MathService_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< MathService::Service, ::mathservice::MultiplyRequest, ::mathservice::MultiplyResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](MathService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::mathservice::MultiplyRequest* req,
             ::mathservice::MultiplyResponse* resp) {
               return service->Multiply(ctx, req, resp);
             }, this)));
}

MathService::Service::~Service() {
}

::grpc::Status MathService::Service::Add(::grpc::ServerContext* context, const ::mathservice::AddRequest* request, ::mathservice::AddResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status MathService::Service::Multiply(::grpc::ServerContext* context, const ::mathservice::MultiplyRequest* request, ::mathservice::MultiplyResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace mathservice

