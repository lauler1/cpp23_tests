#include <string>
#include <grpcpp/grpcpp.h>
#include "greet.grpc.pb.h"

int main(int argc, char** argv) {
  std::string target_str;
  std::string greet_str;
  target_str = "localhost:50051";
  greet_str = "world";

  auto channel = grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials());
  auto stub = GreetService::NewStub(channel);

  HelloRequest request;
  request.set_greeting(greet_str);

  HelloResponse response;

  grpc::ClientContext context;

  grpc::Status status = stub->SayHello(&context, request, &response);

  if (status.ok()) {
    std::cout << "Greet received: " << response.reply() << std::endl;
  } else {
    std::cout << "RPC failed" << std::endl;
  }

  return 0;
}
