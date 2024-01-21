// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: greet.proto

#include "greet.pb.h"
#include "greet.grpc.pb.h"

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

static const char* GreetService_method_names[] = {
  "/GreetService/SayHello",
};

std::unique_ptr< GreetService::Stub> GreetService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< GreetService::Stub> stub(new GreetService::Stub(channel, options));
  return stub;
}

GreetService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_SayHello_(GreetService_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status GreetService::Stub::SayHello(::grpc::ClientContext* context, const ::HelloRequest& request, ::HelloResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::HelloRequest, ::HelloResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_SayHello_, context, request, response);
}

void GreetService::Stub::async::SayHello(::grpc::ClientContext* context, const ::HelloRequest* request, ::HelloResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::HelloRequest, ::HelloResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_SayHello_, context, request, response, std::move(f));
}

void GreetService::Stub::async::SayHello(::grpc::ClientContext* context, const ::HelloRequest* request, ::HelloResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_SayHello_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::HelloResponse>* GreetService::Stub::PrepareAsyncSayHelloRaw(::grpc::ClientContext* context, const ::HelloRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::HelloResponse, ::HelloRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_SayHello_, context, request);
}

::grpc::ClientAsyncResponseReader< ::HelloResponse>* GreetService::Stub::AsyncSayHelloRaw(::grpc::ClientContext* context, const ::HelloRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncSayHelloRaw(context, request, cq);
  result->StartCall();
  return result;
}

GreetService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      GreetService_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< GreetService::Service, ::HelloRequest, ::HelloResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](GreetService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::HelloRequest* req,
             ::HelloResponse* resp) {
               return service->SayHello(ctx, req, resp);
             }, this)));
}

GreetService::Service::~Service() {
}

::grpc::Status GreetService::Service::SayHello(::grpc::ServerContext* context, const ::HelloRequest* request, ::HelloResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

