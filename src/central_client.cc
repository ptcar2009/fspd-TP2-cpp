#include "central_client.hpp"
#include <cstdint>


using namespace TP2;
CentralClient::CentralClient(const std::shared_ptr<grpc::Channel> &channel,
  std::string id) : stub_(Central::NewStub(channel)), id(std::move(id))
{
}

[[nodiscard]] auto CentralClient::Map(const uint32_t key) const -> std::string
{
  grpc::ClientContext context;
  MapRequest request;

  request.set_key(key);

  MapReply reply;

  auto status = stub_->Map(&context, request, &reply);

  if (status.ok()) {
    return reply.id();
  }

  return {};
}

[[nodiscard]] auto CentralClient::Register(const std::vector<uint32_t> &keys, const std::string& id) const -> uint32_t
{
  grpc::ClientContext context;
  RegisterRequest request;

  for (const auto &key : keys) {
    request.add_keys(key);
  }
  request.set_id(id);

  RegisterReply reply;

  auto status = stub_->Register(&context, request, &reply);

  if (status.ok()) {
    return reply.result();
  }

  return -1;
}


[[nodiscard]] auto CentralClient::Terminate() const -> uint32_t
{
  grpc::ClientContext context;
  CentralTerminateRequest request;

  CentralTerminateReply reply;

  auto status = stub_->Terminate(&context, request, &reply);

  if (status.ok()) {
    return reply.ret();
  }

  return 1;
}
