#include "central_client.hpp"
#include "pares_client.hpp"
#include <cstdint>
#include <functional>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/security/credentials.h>
#include <iostream>
#include <map>
#include <string>
#include <string>
#include <utility>
#include <vector>


std::map<std::string, std::function<int(std::string, std::string)>> commands;


int main(int /*argc*/, char **argv)
{
  auto central_client = CentralClient(grpc::CreateChannel(argv[1], grpc::InsecureChannelCredentials()), argv[1]);

#pragma unroll
  while (true) {
    std::string input;
    std::cin >> input;

    if (input == "T") {
      std::cout << central_client.Terminate() << "\n";
      continue;
    }

    if (input == "C") {
      std::uint32_t key = 0;

      std::cin >> key;

      std::string addr = central_client.Map(key);
      if (addr.empty()) {
        continue;
      }
      std::cout << addr << ":";

      auto pares_client = ParesClient(grpc::CreateChannel(addr, grpc::InsecureChannelCredentials()), addr);
      std::cout << pares_client.Consult(key) << "\n";
    }
  }
}
