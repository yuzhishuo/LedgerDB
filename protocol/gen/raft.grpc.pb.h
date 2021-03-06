// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: raft.proto
#ifndef GRPC_raft_2eproto__INCLUDED
#define GRPC_raft_2eproto__INCLUDED

#include "raft.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_generic_service.h>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/client_context.h>
#include <grpcpp/impl/codegen/completion_queue.h>
#include <grpcpp/impl/codegen/message_allocator.h>
#include <grpcpp/impl/codegen/method_handler.h>
#include <grpcpp/impl/codegen/proto_utils.h>
#include <grpcpp/impl/codegen/rpc_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/status.h>
#include <grpcpp/impl/codegen/stub_options.h>
#include <grpcpp/impl/codegen/sync_stream.h>

namespace raft {

class Raft final {
 public:
  static constexpr char const* service_full_name() {
    return "raft.Raft";
  }
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    virtual ::grpc::Status RRequestVote(::grpc::ClientContext* context, const ::raft::RequestVote& request, ::raft::RequestVoteResponse* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::raft::RequestVoteResponse>> AsyncRRequestVote(::grpc::ClientContext* context, const ::raft::RequestVote& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::raft::RequestVoteResponse>>(AsyncRRequestVoteRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::raft::RequestVoteResponse>> PrepareAsyncRRequestVote(::grpc::ClientContext* context, const ::raft::RequestVote& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::raft::RequestVoteResponse>>(PrepareAsyncRRequestVoteRaw(context, request, cq));
    }
    virtual ::grpc::Status RAppendEntries(::grpc::ClientContext* context, const ::raft::AppendEntries& request, ::raft::AppendEntriesResponse* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::raft::AppendEntriesResponse>> AsyncRAppendEntries(::grpc::ClientContext* context, const ::raft::AppendEntries& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::raft::AppendEntriesResponse>>(AsyncRAppendEntriesRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::raft::AppendEntriesResponse>> PrepareAsyncRAppendEntries(::grpc::ClientContext* context, const ::raft::AppendEntries& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::raft::AppendEntriesResponse>>(PrepareAsyncRAppendEntriesRaw(context, request, cq));
    }
    class async_interface {
     public:
      virtual ~async_interface() {}
      virtual void RRequestVote(::grpc::ClientContext* context, const ::raft::RequestVote* request, ::raft::RequestVoteResponse* response, std::function<void(::grpc::Status)>) = 0;
      virtual void RRequestVote(::grpc::ClientContext* context, const ::raft::RequestVote* request, ::raft::RequestVoteResponse* response, ::grpc::ClientUnaryReactor* reactor) = 0;
      virtual void RAppendEntries(::grpc::ClientContext* context, const ::raft::AppendEntries* request, ::raft::AppendEntriesResponse* response, std::function<void(::grpc::Status)>) = 0;
      virtual void RAppendEntries(::grpc::ClientContext* context, const ::raft::AppendEntries* request, ::raft::AppendEntriesResponse* response, ::grpc::ClientUnaryReactor* reactor) = 0;
    };
    typedef class async_interface experimental_async_interface;
    virtual class async_interface* async() { return nullptr; }
    class async_interface* experimental_async() { return async(); }
   private:
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::raft::RequestVoteResponse>* AsyncRRequestVoteRaw(::grpc::ClientContext* context, const ::raft::RequestVote& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::raft::RequestVoteResponse>* PrepareAsyncRRequestVoteRaw(::grpc::ClientContext* context, const ::raft::RequestVote& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::raft::AppendEntriesResponse>* AsyncRAppendEntriesRaw(::grpc::ClientContext* context, const ::raft::AppendEntries& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::raft::AppendEntriesResponse>* PrepareAsyncRAppendEntriesRaw(::grpc::ClientContext* context, const ::raft::AppendEntries& request, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());
    ::grpc::Status RRequestVote(::grpc::ClientContext* context, const ::raft::RequestVote& request, ::raft::RequestVoteResponse* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::raft::RequestVoteResponse>> AsyncRRequestVote(::grpc::ClientContext* context, const ::raft::RequestVote& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::raft::RequestVoteResponse>>(AsyncRRequestVoteRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::raft::RequestVoteResponse>> PrepareAsyncRRequestVote(::grpc::ClientContext* context, const ::raft::RequestVote& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::raft::RequestVoteResponse>>(PrepareAsyncRRequestVoteRaw(context, request, cq));
    }
    ::grpc::Status RAppendEntries(::grpc::ClientContext* context, const ::raft::AppendEntries& request, ::raft::AppendEntriesResponse* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::raft::AppendEntriesResponse>> AsyncRAppendEntries(::grpc::ClientContext* context, const ::raft::AppendEntries& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::raft::AppendEntriesResponse>>(AsyncRAppendEntriesRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::raft::AppendEntriesResponse>> PrepareAsyncRAppendEntries(::grpc::ClientContext* context, const ::raft::AppendEntries& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::raft::AppendEntriesResponse>>(PrepareAsyncRAppendEntriesRaw(context, request, cq));
    }
    class async final :
      public StubInterface::async_interface {
     public:
      void RRequestVote(::grpc::ClientContext* context, const ::raft::RequestVote* request, ::raft::RequestVoteResponse* response, std::function<void(::grpc::Status)>) override;
      void RRequestVote(::grpc::ClientContext* context, const ::raft::RequestVote* request, ::raft::RequestVoteResponse* response, ::grpc::ClientUnaryReactor* reactor) override;
      void RAppendEntries(::grpc::ClientContext* context, const ::raft::AppendEntries* request, ::raft::AppendEntriesResponse* response, std::function<void(::grpc::Status)>) override;
      void RAppendEntries(::grpc::ClientContext* context, const ::raft::AppendEntries* request, ::raft::AppendEntriesResponse* response, ::grpc::ClientUnaryReactor* reactor) override;
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
    ::grpc::ClientAsyncResponseReader< ::raft::RequestVoteResponse>* AsyncRRequestVoteRaw(::grpc::ClientContext* context, const ::raft::RequestVote& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::raft::RequestVoteResponse>* PrepareAsyncRRequestVoteRaw(::grpc::ClientContext* context, const ::raft::RequestVote& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::raft::AppendEntriesResponse>* AsyncRAppendEntriesRaw(::grpc::ClientContext* context, const ::raft::AppendEntries& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::raft::AppendEntriesResponse>* PrepareAsyncRAppendEntriesRaw(::grpc::ClientContext* context, const ::raft::AppendEntries& request, ::grpc::CompletionQueue* cq) override;
    const ::grpc::internal::RpcMethod rpcmethod_RRequestVote_;
    const ::grpc::internal::RpcMethod rpcmethod_RAppendEntries_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    virtual ::grpc::Status RRequestVote(::grpc::ServerContext* context, const ::raft::RequestVote* request, ::raft::RequestVoteResponse* response);
    virtual ::grpc::Status RAppendEntries(::grpc::ServerContext* context, const ::raft::AppendEntries* request, ::raft::AppendEntriesResponse* response);
  };
  template <class BaseClass>
  class WithAsyncMethod_RRequestVote : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_RRequestVote() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_RRequestVote() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status RRequestVote(::grpc::ServerContext* /*context*/, const ::raft::RequestVote* /*request*/, ::raft::RequestVoteResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestRRequestVote(::grpc::ServerContext* context, ::raft::RequestVote* request, ::grpc::ServerAsyncResponseWriter< ::raft::RequestVoteResponse>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_RAppendEntries : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_RAppendEntries() {
      ::grpc::Service::MarkMethodAsync(1);
    }
    ~WithAsyncMethod_RAppendEntries() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status RAppendEntries(::grpc::ServerContext* /*context*/, const ::raft::AppendEntries* /*request*/, ::raft::AppendEntriesResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestRAppendEntries(::grpc::ServerContext* context, ::raft::AppendEntries* request, ::grpc::ServerAsyncResponseWriter< ::raft::AppendEntriesResponse>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(1, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_RRequestVote<WithAsyncMethod_RAppendEntries<Service > > AsyncService;
  template <class BaseClass>
  class WithCallbackMethod_RRequestVote : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithCallbackMethod_RRequestVote() {
      ::grpc::Service::MarkMethodCallback(0,
          new ::grpc::internal::CallbackUnaryHandler< ::raft::RequestVote, ::raft::RequestVoteResponse>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::raft::RequestVote* request, ::raft::RequestVoteResponse* response) { return this->RRequestVote(context, request, response); }));}
    void SetMessageAllocatorFor_RRequestVote(
        ::grpc::MessageAllocator< ::raft::RequestVote, ::raft::RequestVoteResponse>* allocator) {
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::GetHandler(0);
      static_cast<::grpc::internal::CallbackUnaryHandler< ::raft::RequestVote, ::raft::RequestVoteResponse>*>(handler)
              ->SetMessageAllocator(allocator);
    }
    ~WithCallbackMethod_RRequestVote() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status RRequestVote(::grpc::ServerContext* /*context*/, const ::raft::RequestVote* /*request*/, ::raft::RequestVoteResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* RRequestVote(
      ::grpc::CallbackServerContext* /*context*/, const ::raft::RequestVote* /*request*/, ::raft::RequestVoteResponse* /*response*/)  { return nullptr; }
  };
  template <class BaseClass>
  class WithCallbackMethod_RAppendEntries : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithCallbackMethod_RAppendEntries() {
      ::grpc::Service::MarkMethodCallback(1,
          new ::grpc::internal::CallbackUnaryHandler< ::raft::AppendEntries, ::raft::AppendEntriesResponse>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::raft::AppendEntries* request, ::raft::AppendEntriesResponse* response) { return this->RAppendEntries(context, request, response); }));}
    void SetMessageAllocatorFor_RAppendEntries(
        ::grpc::MessageAllocator< ::raft::AppendEntries, ::raft::AppendEntriesResponse>* allocator) {
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::GetHandler(1);
      static_cast<::grpc::internal::CallbackUnaryHandler< ::raft::AppendEntries, ::raft::AppendEntriesResponse>*>(handler)
              ->SetMessageAllocator(allocator);
    }
    ~WithCallbackMethod_RAppendEntries() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status RAppendEntries(::grpc::ServerContext* /*context*/, const ::raft::AppendEntries* /*request*/, ::raft::AppendEntriesResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* RAppendEntries(
      ::grpc::CallbackServerContext* /*context*/, const ::raft::AppendEntries* /*request*/, ::raft::AppendEntriesResponse* /*response*/)  { return nullptr; }
  };
  typedef WithCallbackMethod_RRequestVote<WithCallbackMethod_RAppendEntries<Service > > CallbackService;
  typedef CallbackService ExperimentalCallbackService;
  template <class BaseClass>
  class WithGenericMethod_RRequestVote : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_RRequestVote() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_RRequestVote() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status RRequestVote(::grpc::ServerContext* /*context*/, const ::raft::RequestVote* /*request*/, ::raft::RequestVoteResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_RAppendEntries : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_RAppendEntries() {
      ::grpc::Service::MarkMethodGeneric(1);
    }
    ~WithGenericMethod_RAppendEntries() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status RAppendEntries(::grpc::ServerContext* /*context*/, const ::raft::AppendEntries* /*request*/, ::raft::AppendEntriesResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithRawMethod_RRequestVote : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_RRequestVote() {
      ::grpc::Service::MarkMethodRaw(0);
    }
    ~WithRawMethod_RRequestVote() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status RRequestVote(::grpc::ServerContext* /*context*/, const ::raft::RequestVote* /*request*/, ::raft::RequestVoteResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestRRequestVote(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithRawMethod_RAppendEntries : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_RAppendEntries() {
      ::grpc::Service::MarkMethodRaw(1);
    }
    ~WithRawMethod_RAppendEntries() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status RAppendEntries(::grpc::ServerContext* /*context*/, const ::raft::AppendEntries* /*request*/, ::raft::AppendEntriesResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestRAppendEntries(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(1, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithRawCallbackMethod_RRequestVote : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawCallbackMethod_RRequestVote() {
      ::grpc::Service::MarkMethodRawCallback(0,
          new ::grpc::internal::CallbackUnaryHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::grpc::ByteBuffer* request, ::grpc::ByteBuffer* response) { return this->RRequestVote(context, request, response); }));
    }
    ~WithRawCallbackMethod_RRequestVote() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status RRequestVote(::grpc::ServerContext* /*context*/, const ::raft::RequestVote* /*request*/, ::raft::RequestVoteResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* RRequestVote(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)  { return nullptr; }
  };
  template <class BaseClass>
  class WithRawCallbackMethod_RAppendEntries : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawCallbackMethod_RAppendEntries() {
      ::grpc::Service::MarkMethodRawCallback(1,
          new ::grpc::internal::CallbackUnaryHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::grpc::ByteBuffer* request, ::grpc::ByteBuffer* response) { return this->RAppendEntries(context, request, response); }));
    }
    ~WithRawCallbackMethod_RAppendEntries() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status RAppendEntries(::grpc::ServerContext* /*context*/, const ::raft::AppendEntries* /*request*/, ::raft::AppendEntriesResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* RAppendEntries(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)  { return nullptr; }
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_RRequestVote : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithStreamedUnaryMethod_RRequestVote() {
      ::grpc::Service::MarkMethodStreamed(0,
        new ::grpc::internal::StreamedUnaryHandler<
          ::raft::RequestVote, ::raft::RequestVoteResponse>(
            [this](::grpc::ServerContext* context,
                   ::grpc::ServerUnaryStreamer<
                     ::raft::RequestVote, ::raft::RequestVoteResponse>* streamer) {
                       return this->StreamedRRequestVote(context,
                         streamer);
                  }));
    }
    ~WithStreamedUnaryMethod_RRequestVote() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status RRequestVote(::grpc::ServerContext* /*context*/, const ::raft::RequestVote* /*request*/, ::raft::RequestVoteResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedRRequestVote(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::raft::RequestVote,::raft::RequestVoteResponse>* server_unary_streamer) = 0;
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_RAppendEntries : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithStreamedUnaryMethod_RAppendEntries() {
      ::grpc::Service::MarkMethodStreamed(1,
        new ::grpc::internal::StreamedUnaryHandler<
          ::raft::AppendEntries, ::raft::AppendEntriesResponse>(
            [this](::grpc::ServerContext* context,
                   ::grpc::ServerUnaryStreamer<
                     ::raft::AppendEntries, ::raft::AppendEntriesResponse>* streamer) {
                       return this->StreamedRAppendEntries(context,
                         streamer);
                  }));
    }
    ~WithStreamedUnaryMethod_RAppendEntries() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status RAppendEntries(::grpc::ServerContext* /*context*/, const ::raft::AppendEntries* /*request*/, ::raft::AppendEntriesResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedRAppendEntries(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::raft::AppendEntries,::raft::AppendEntriesResponse>* server_unary_streamer) = 0;
  };
  typedef WithStreamedUnaryMethod_RRequestVote<WithStreamedUnaryMethod_RAppendEntries<Service > > StreamedUnaryService;
  typedef Service SplitStreamedService;
  typedef WithStreamedUnaryMethod_RRequestVote<WithStreamedUnaryMethod_RAppendEntries<Service > > StreamedService;
};

}  // namespace raft


#endif  // GRPC_raft_2eproto__INCLUDED
