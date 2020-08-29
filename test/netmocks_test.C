#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <cstring>

#include "mock_network_bridge.H"

int main() {
    auto& socket_map = get_socket_map();
    auto msg = std::string{"hello world"};
    auto r = send(STDOUT_FILENO, reinterpret_cast<const void*>(msg.data()), msg.size(), 0);
    auto it = socket_map.find(STDOUT_FILENO);
    if (it != std::end(socket_map)) {
        std::cout << "MockNetworkTest: PASS!" << std::endl;
        return memcmp(it->second.data(), msg.data(), msg.size());
    }
    std::cout << "MockNetworkTest: FAILED!" << std::endl;
    return -1;
}
