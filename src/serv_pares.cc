#include "pares.grpc.pb.h"
#include "pares.pb.h"
#include "central_client.hpp"
#include "server.hpp"
#include <cstdint>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include <map>
#include <string>
#include <sstream>
#include <utility>

using namespace TP2;
using grpc::Status;
using ::TP2::ConsultRequest;
using ::TP2::ConsultReply;
using ::TP2::ActivateRequest;
using ::TP2::ActivateReply;
using grpc::ServerContext;

class ParesImpl final : public Pares::Service
{
public:
  ParesImpl() = default;
  explicit ParesImpl(std::string id) : id(std::move(id)) {}
  Status Insert(ServerContext * /*context*/, const InsertRequest *request, InsertReply *response) override
  {
    if (values.find(request->key()) != values.end()) {
      response->set_result(-1);
      return Status::OK;
    }

    values[request->key()] = request->value();

    response->set_result(0);

    return Status::OK;
  }

  Status Consult(ServerContext * /*context*/, const ConsultRequest *request, ConsultReply *response) override
  {
    if (values.find(request->key()) != values.end()) {
      response->set_value(values[request->key()]);
    } else {
      response->set_value("");
    }

    return Status::OK;
  }

  Status Activate(ServerContext *context, const ActivateRequest *request, ActivateReply *response) override
  {
    auto client = CentralClient(grpc::CreateChannel(request->id(), grpc::InsecureChannelCredentials()), request->id());

    std::vector<uint32_t> keys;

    for (auto &key : values) {
      keys.push_back(key.first);
    }


    auto ret = client.Register(keys, id);


    response->set_ret(ret);
    return Status::OK;
  }

  Status Terminate(ServerContext * /*context*/, const ParesTerminateRequest * /*request*/, ParesTerminateReply *response) override
  {
    response->set_ret(0);
    return Status::OK;
  }


private:
  std::map<uint32_t, std::string> values;
    std::string id;
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
  RunServer(s.str(), ParesImpl(s.str()));
}
