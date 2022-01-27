#include <cstdint>
#include <grpcpp/grpcpp.h>
#include "central.grpc.pb.h"
#include "central.pb.h"

#ifndef CENTRAL_CLIENT_HPP
#define CENTRAL_CLIENT_HPP

using namespace TP2;
class CentralClient
{
public:
  explicit CentralClient(const std::shared_ptr<grpc::Channel> &channel,
    std::string id);
  [[nodiscard]] auto Map(uint32_t key) const -> std::string;

  [[nodiscard]] auto Register(const std::vector<uint32_t> &keys,const std::string& id) const -> uint32_t;


  [[nodiscard]] auto Terminate() const -> uint32_t;


private:
  std::unique_ptr<Central::Stub> stub_;
  const std::string id;
};

#endif// !CENTRAL_CLIENT_HPP
