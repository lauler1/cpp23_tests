// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: math_service.proto
#ifndef GRPC_math_5fservice_2eproto__INCLUDED
#define GRPC_math_5fservice_2eproto__INCLUDED

#include "math_service.pb.h"

#include <functional>
#include <grpcpp/generic/async_generic_service.h>
#include <grpcpp/support/async_stream.h>
#include <grpcpp/support/async_unary_call.h>
#include <grpcpp/support/client_callback.h>
#include <grpcpp/client_context.h>
#include <grpcpp/completion_queue.h>
#include <grpcpp/support/message_allocator.h>
#include <grpcpp/support/method_handler.h>
#include <grpcpp/impl/codegen/proto_utils.h>
#include <grpcpp/impl/rpc_method.h>
#include <grpcpp/support/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/server_context.h>
#include <grpcpp/impl/service_type.h>
#include <grpcpp/impl/codegen/status.h>
#include <grpcpp/support/stub_options.h>
#include <grpcpp/support/sync_stream.h>

namespace mathservice {

class MathService final {
 public:
  static constexpr char const* service_full_name() {
    return "mathservice.MathService";
  }
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    virtual ::grpc::Status Add(::grpc::ClientContext* context, const ::mathservice::AddRequest& request, ::mathservice::AddResponse* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::mathservice::AddResponse>> AsyncAdd(::grpc::ClientContext* context, const ::mathservice::AddRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::mathservice::AddResponse>>(AsyncAddRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::mathservice::AddResponse>> PrepareAsyncAdd(::grpc::ClientContext* context, const ::mathservice::AddRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::mathservice::AddResponse>>(PrepareAsyncAddRaw(context, request, cq));
    }
    virtual ::grpc::Status Multiply(::grpc::ClientContext* context, const ::mathservice::MultiplyRequest& request, ::mathservice::MultiplyResponse* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::mathservice::MultiplyResponse>> AsyncMultiply(::grpc::ClientContext* context, const ::mathservice::MultiplyRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::mathservice::MultiplyResponse>>(AsyncMultiplyRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::mathservice::MultiplyResponse>> PrepareAsyncMultiply(::grpc::ClientContext* context, const ::mathservice::MultiplyRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::mathservice::MultiplyResponse>>(PrepareAsyncMultiplyRaw(context, request, cq));
    }
    class async_interface {
     public:
      virtual ~async_interface() {}
      virtual void Add(::grpc::ClientContext* context, const ::mathservice::AddRequest* request, ::mathservice::AddResponse* response, std::function<void(::grpc::Status)>) = 0;
      virtual void Add(::grpc::ClientContext* context, const ::mathservice::AddRequest* request, ::mathservice::AddResponse* response, ::grpc::ClientUnaryReactor* reactor) = 0;
      virtual void Multiply(::grpc::ClientContext* context, const ::mathservice::MultiplyRequest* request, ::mathservice::MultiplyResponse* response, std::function<void(::grpc::Status)>) = 0;
      virtual void Multiply(::grpc::ClientContext* context, const ::mathservice::MultiplyRequest* request, ::mathservice::MultiplyResponse* response, ::grpc::ClientUnaryReactor* reactor) = 0;
    };
    typedef class async_interface experimental_async_interface;
    virtual class async_interface* async() { return nullptr; }
    class async_interface* experimental_async() { return async(); }
   private:
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::mathservice::AddResponse>* AsyncAddRaw(::grpc::ClientContext* context, const ::mathservice::AddRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::mathservice::AddResponse>* PrepareAsyncAddRaw(::grpc::ClientContext* context, const ::mathservice::AddRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::mathservice::MultiplyResponse>* AsyncMultiplyRaw(::grpc::ClientContext* context, const ::mathservice::MultiplyRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::mathservice::MultiplyResponse>* PrepareAsyncMultiplyRaw(::grpc::ClientContext* context, const ::mathservice::MultiplyRequest& request, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());
    ::grpc::Status Add(::grpc::ClientContext* context, const ::mathservice::AddRequest& request, ::mathservice::AddResponse* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::mathservice::AddResponse>> AsyncAdd(::grpc::ClientContext* context, const ::mathservice::AddRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::mathservice::AddResponse>>(AsyncAddRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::mathservice::AddResponse>> PrepareAsyncAdd(::grpc::ClientContext* context, const ::mathservice::AddRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::mathservice::AddResponse>>(PrepareAsyncAddRaw(context, request, cq));
    }
    ::grpc::Status Multiply(::grpc::ClientContext* context, const ::mathservice::MultiplyRequest& request, ::mathservice::MultiplyResponse* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::mathservice::MultiplyResponse>> AsyncMultiply(::grpc::ClientContext* context, const ::mathservice::MultiplyRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::mathservice::MultiplyResponse>>(AsyncMultiplyRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::mathservice::MultiplyResponse>> PrepareAsyncMultiply(::grpc::ClientContext* context, const ::mathservice::MultiplyRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::mathservice::MultiplyResponse>>(PrepareAsyncMultiplyRaw(context, request, cq));
    }
    class async final :
      public StubInterface::async_interface {
     public:
      void Add(::grpc::ClientContext* context, const ::mathservice::AddRequest* request, ::mathservice::AddResponse* response, std::function<void(::grpc::Status)>) override;
      void Add(::grpc::ClientContext* context, const ::mathservice::AddRequest* request, ::mathservice::AddResponse* response, ::grpc::ClientUnaryReactor* reactor) override;
      void Multiply(::grpc::ClientContext* context, const ::mathservice::MultiplyRequest* request, ::mathservice::MultiplyResponse* response, std::function<void(::grpc::Status)>) override;
      void Multiply(::grpc::ClientContext* context, const ::mathservice::MultiplyRequest* request, ::mathservice::MultiplyResponse* response, ::grpc::ClientUnaryReactor* reactor) override;
     private:
      friend class Stub;
      explicit async(Stub* stub): stub_(stub) { }
      Stub* stub() { return stub_; }
      Stub* stub_;
    };
    class async* async() override { return &async_stub_; }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    class async async_stub_{this};
    ::grpc::ClientAsyncResponseReader< ::mathservice::AddResponse>* AsyncAddRaw(::grpc::ClientContext* context, const ::mathservice::AddRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::mathservice::AddResponse>* PrepareAsyncAddRaw(::grpc::ClientContext* context, const ::mathservice::AddRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::mathservice::MultiplyResponse>* AsyncMultiplyRaw(::grpc::ClientContext* context, const ::mathservice::MultiplyRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::mathservice::MultiplyResponse>* PrepareAsyncMultiplyRaw(::grpc::ClientContext* context, const ::mathservice::MultiplyRequest& request, ::grpc::CompletionQueue* cq) override;
    const ::grpc::internal::RpcMethod rpcmethod_Add_;
    const ::grpc::internal::RpcMethod rpcmethod_Multiply_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    virtual ::grpc::Status Add(::grpc::ServerContext* context, const ::mathservice::AddRequest* request, ::mathservice::AddResponse* response);
    virtual ::grpc::Status Multiply(::grpc::ServerContext* context, const ::mathservice::MultiplyRequest* request, ::mathservice::MultiplyResponse* response);
  };
  template <class BaseClass>
  class WithAsyncMethod_Add : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_Add() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_Add() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Add(::grpc::ServerContext* /*context*/, const ::mathservice::AddRequest* /*request*/, ::mathservice::AddResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestAdd(::grpc::ServerContext* context, ::mathservice::AddRequest* request, ::grpc::ServerAsyncResponseWriter< ::mathservice::AddResponse>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_Multiply : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_Multiply() {
      ::grpc::Service::MarkMethodAsync(1);
    }
    ~WithAsyncMethod_Multiply() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Multiply(::grpc::ServerContext* /*context*/, const ::mathservice::MultiplyRequest* /*request*/, ::mathservice::MultiplyResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestMultiply(::grpc::ServerContext* context, ::mathservice::MultiplyRequest* request, ::grpc::ServerAsyncResponseWriter< ::mathservice::MultiplyResponse>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(1, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_Add<WithAsyncMethod_Multiply<Service > > AsyncService;
  template <class BaseClass>
  class WithCallbackMethod_Add : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithCallbackMethod_Add() {
      ::grpc::Service::MarkMethodCallback(0,
          new ::grpc::internal::CallbackUnaryHandler< ::mathservice::AddRequest, ::mathservice::AddResponse>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::mathservice::AddRequest* request, ::mathservice::AddResponse* response) { return this->Add(context, request, response); }));}
    void SetMessageAllocatorFor_Add(
        ::grpc::MessageAllocator< ::mathservice::AddRequest, ::mathservice::AddResponse>* allocator) {
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::GetHandler(0);
      static_cast<::grpc::internal::CallbackUnaryHandler< ::mathservice::AddRequest, ::mathservice::AddResponse>*>(handler)
              ->SetMessageAllocator(allocator);
    }
    ~WithCallbackMethod_Add() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Add(::grpc::ServerContext* /*context*/, const ::mathservice::AddRequest* /*request*/, ::mathservice::AddResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* Add(
      ::grpc::CallbackServerContext* /*context*/, const ::mathservice::AddRequest* /*request*/, ::mathservice::AddResponse* /*response*/)  { return nullptr; }
  };
  template <class BaseClass>
  class WithCallbackMethod_Multiply : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithCallbackMethod_Multiply() {
      ::grpc::Service::MarkMethodCallback(1,
          new ::grpc::internal::CallbackUnaryHandler< ::mathservice::MultiplyRequest, ::mathservice::MultiplyResponse>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::mathservice::MultiplyRequest* request, ::mathservice::MultiplyResponse* response) { return this->Multiply(context, request, response); }));}
    void SetMessageAllocatorFor_Multiply(
        ::grpc::MessageAllocator< ::mathservice::MultiplyRequest, ::mathservice::MultiplyResponse>* allocator) {
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::GetHandler(1);
      static_cast<::grpc::internal::CallbackUnaryHandler< ::mathservice::MultiplyRequest, ::mathservice::MultiplyResponse>*>(handler)
              ->SetMessageAllocator(allocator);
    }
    ~WithCallbackMethod_Multiply() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Multiply(::grpc::ServerContext* /*context*/, const ::mathservice::MultiplyRequest* /*request*/, ::mathservice::MultiplyResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* Multiply(
      ::grpc::CallbackServerContext* /*context*/, const ::mathservice::MultiplyRequest* /*request*/, ::mathservice::MultiplyResponse* /*response*/)  { return nullptr; }
  };
  typedef WithCallbackMethod_Add<WithCallbackMethod_Multiply<Service > > CallbackService;
  typedef CallbackService ExperimentalCallbackService;
  template <class BaseClass>
  class WithGenericMethod_Add : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_Add() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_Add() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Add(::grpc::ServerContext* /*context*/, const ::mathservice::AddRequest* /*request*/, ::mathservice::AddResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_Multiply : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_Multiply() {
      ::grpc::Service::MarkMethodGeneric(1);
    }
    ~WithGenericMethod_Multiply() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Multiply(::grpc::ServerContext* /*context*/, const ::mathservice::MultiplyRequest* /*request*/, ::mathservice::MultiplyResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithRawMethod_Add : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_Add() {
      ::grpc::Service::MarkMethodRaw(0);
    }
    ~WithRawMethod_Add() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Add(::grpc::ServerContext* /*context*/, const ::mathservice::AddRequest* /*request*/, ::mathservice::AddResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestAdd(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithRawMethod_Multiply : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_Multiply() {
      ::grpc::Service::MarkMethodRaw(1);
    }
    ~WithRawMethod_Multiply() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Multiply(::grpc::ServerContext* /*context*/, const ::mathservice::MultiplyRequest* /*request*/, ::mathservice::MultiplyResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestMultiply(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(1, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithRawCallbackMethod_Add : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawCallbackMethod_Add() {
      ::grpc::Service::MarkMethodRawCallback(0,
          new ::grpc::internal::CallbackUnaryHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::grpc::ByteBuffer* request, ::grpc::ByteBuffer* response) { return this->Add(context, request, response); }));
    }
    ~WithRawCallbackMethod_Add() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Add(::grpc::ServerContext* /*context*/, const ::mathservice::AddRequest* /*request*/, ::mathservice::AddResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* Add(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)  { return nullptr; }
  };
  template <class BaseClass>
  class WithRawCallbackMethod_Multiply : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawCallbackMethod_Multiply() {
      ::grpc::Service::MarkMethodRawCallback(1,
          new ::grpc::internal::CallbackUnaryHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::grpc::ByteBuffer* request, ::grpc::ByteBuffer* response) { return this->Multiply(context, request, response); }));
    }
    ~WithRawCallbackMethod_Multiply() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Multiply(::grpc::ServerContext* /*context*/, const ::mathservice::MultiplyRequest* /*request*/, ::mathservice::MultiplyResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* Multiply(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)  { return nullptr; }
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_Add : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithStreamedUnaryMethod_Add() {
      ::grpc::Service::MarkMethodStreamed(0,
        new ::grpc::internal::StreamedUnaryHandler<
          ::mathservice::AddRequest, ::mathservice::AddResponse>(
            [this](::grpc::ServerContext* context,
                   ::grpc::ServerUnaryStreamer<
                     ::mathservice::AddRequest, ::mathservice::AddResponse>* streamer) {
                       return this->StreamedAdd(context,
                         streamer);
                  }));
    }
    ~WithStreamedUnaryMethod_Add() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status Add(::grpc::ServerContext* /*context*/, const ::mathservice::AddRequest* /*request*/, ::mathservice::AddResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedAdd(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::mathservice::AddRequest,::mathservice::AddResponse>* server_unary_streamer) = 0;
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_Multiply : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithStreamedUnaryMethod_Multiply() {
      ::grpc::Service::MarkMethodStreamed(1,
        new ::grpc::internal::StreamedUnaryHandler<
          ::mathservice::MultiplyRequest, ::mathservice::MultiplyResponse>(
            [this](::grpc::ServerContext* context,
                   ::grpc::ServerUnaryStreamer<
                     ::mathservice::MultiplyRequest, ::mathservice::MultiplyResponse>* streamer) {
                       return this->StreamedMultiply(context,
                         streamer);
                  }));
    }
    ~WithStreamedUnaryMethod_Multiply() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status Multiply(::grpc::ServerContext* /*context*/, const ::mathservice::MultiplyRequest* /*request*/, ::mathservice::MultiplyResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedMultiply(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::mathservice::MultiplyRequest,::mathservice::MultiplyResponse>* server_unary_streamer) = 0;
  };
  typedef WithStreamedUnaryMethod_Add<WithStreamedUnaryMethod_Multiply<Service > > StreamedUnaryService;
  typedef Service SplitStreamedService;
  typedef WithStreamedUnaryMethod_Add<WithStreamedUnaryMethod_Multiply<Service > > StreamedService;
};

}  // namespace mathservice


#endif  // GRPC_math_5fservice_2eproto__INCLUDED