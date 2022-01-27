#include <cstdint>
#include <grpcpp/grpcpp.h>
#include "pares.grpc.pb.h"
#include "pares.pb.h"

#ifndef PARES_CLIENT_HPP
#define PARES_CLIENT_HPP

class ParesClient
{
public:
  explicit ParesClient(const std::shared_ptr<grpc::Channel> &channel,
    std::string id);

  [[nodiscard]] auto Insert(const uint32_t &key, const std::string &value) const -> int;

  [[nodiscard]] auto Consult(const uint32_t &key) const -> std::string;

  [[nodiscard]] auto Terminate() const -> uint32_t;

  [[nodiscard]] auto Activate(const std::string &id) const -> uint32_t;


private:
  std::unique_ptr<TP2::Pares::Stub> stub_;
  const std::string id;
};


#endif// !PARES_CLIENT_HPP
