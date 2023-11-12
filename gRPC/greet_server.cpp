#include <string>
#include <grpcpp/grpcpp.h>
#include "greet.grpc.pb.h"

class GreetServiceImpl final : public GreetService::Service {
  grpc::Status SayHello(grpc::ServerContext* context, const HelloRequest* request, HelloResponse* response) override {
    std::string reply = "Hello " + request->greeting();
    response->set_reply(reply);
    return grpc::Status::OK;
  }
};

int main(int argc, char** argv) {
  std::string server_address("0.0.0.0:50051");
  GreetServiceImpl service;

  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  server->Wait();
}
