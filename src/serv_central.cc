#include "central.pb.h"
#include "central.grpc.pb.h"
#include "server.hpp"

#include <cstdint>
#include <functional>
#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/server_builder.h>
#include <iostream>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

#include <map>
#include <sstream>
#include <string>


using grpc::ServerContext;
using grpc::Status;
using namespace TP2;


using TP2::Central;


class CentralImpl final : public Central::Service
{

public:
  CentralImpl() = default;
  Status Register(ServerContext * /*context*/, const RegisterRequest *request, RegisterReply *response) override
  {
      std::cout << "receive register request from " << request->id() << "\n";
    for (const auto &key : request->keys()) {
        std::cout << key << "\n";

      values[key] = request->id();
    }

    response->set_result(0);

    return Status::OK;
  }

  Status Map(ServerContext * /*context*/, const MapRequest *request, MapReply *response) override
  {

    if (values.find(request->key()) != values.end()) {
      response->set_id(values[request->key()]);
    }

    return Status::OK;
  }

  Status Terminate(ServerContext * /*context*/, const CentralTerminateRequest * /*request*/, CentralTerminateReply *response) override
  {

    response->set_ret(0);
    return Status::OK;
  }

private:
  std::map<uint32_t, std::string> values;
};
void usage()
{
  std::cout << "Usage:\n";
  std::cout << "central <port>\n";
}
int main(int argc, char **argv)
{

  if (argc != 2) {
    usage();
    exit(1);
  }
  std::stringstream s;
  s << "0.0.0.0:" << argv[1];
  RunServer(s.str(), CentralImpl());
}
