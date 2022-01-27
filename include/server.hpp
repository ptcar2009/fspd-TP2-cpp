#ifndef SERVER_HPP_TP2

#include <grpcpp/grpcpp.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

template <typename ServerImpl>
void RunServer(std::string&& server_address, ServerImpl service) {

  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  grpc::ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();

}

#endif
