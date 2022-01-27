#include "pares_client.hpp"
#include <cstdint>

using namespace TP2;
ParesClient::ParesClient(const std::shared_ptr<grpc::Channel> &channel,
  std::string id) : stub_(Pares::NewStub(channel)), id(std::move(id))
{
}

[[nodiscard]] auto ParesClient::Insert(const uint32_t &key, const std::string &value) const -> int
{
  InsertRequest request;
  request.set_key(key);
  request.set_value(value);

  InsertReply reply;
  grpc::ClientContext context;

  auto status = stub_->Insert(&context, request, &reply);

  if (status.ok()) {
    return reply.result();
  }

  return -1;
}

[[nodiscard]] auto ParesClient::Consult(const uint32_t &key) const -> std::string
{
  grpc::ClientContext context;
  ConsultRequest request;
  request.set_key(key);

  ConsultReply reply;

  auto status = stub_->Consult(&context, request, &reply);

  if (status.ok()) {
    return reply.value();
  }

  return {};
}

[[nodiscard]] auto ParesClient::Terminate() const -> uint32_t
{
  grpc::ClientContext context;
  ParesTerminateRequest request;

  ParesTerminateReply reply;

  auto status = stub_->Terminate(&context, request, &reply);

  if (status.ok()) {
    return reply.ret();
  }

  return static_cast<uint32_t>(1);
}

[[nodiscard]] auto ParesClient::Activate(const std::string &id) const -> uint32_t
{
  grpc::ClientContext context;
  ActivateRequest request;
  request.set_id(id);

  ActivateReply reply;

  auto status = stub_->Activate(&context, request, &reply);

  if (status.ok()) {
    return reply.ret();
  }

  return static_cast<uint32_t>(1);
}
