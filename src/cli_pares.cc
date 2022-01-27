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

using namespace TP2;


std::map<std::string, std::function<int(std::string, std::string)>> commands;


int main(int /*argc*/, char **argv)
{
  auto client = ParesClient(grpc::CreateChannel(argv[1], grpc::InsecureChannelCredentials()), argv[1]);

  #pragma unroll
  while (true) {
    std::string input;
    std::cin >> input;

    if (input == "T") {
      std::cout << client.Terminate() << "\n";
      continue;
    }

    if (input == "A") {
      std::string id;
      std::cin >> id;
      std::cout << client.Activate(id) << "\n";
    }

    if (input == "C") {
      std::uint32_t key = 0;

      std::cin >> key;

      std::cout << client.Consult(key) << "\n";
    }

    if (input == "I") {
      std::uint32_t key = 0;
      std::string value;

      std::cin >> key >> value;

      std::cout << client.Insert(key, value) << "\n";
    }

  }
}
