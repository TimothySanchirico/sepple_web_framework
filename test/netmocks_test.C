#include <cstring>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "mock_network_bridge.H"

int main() {
    const auto sockfd = 4;
    auto&      state  = shrek::create_socket_state(
        sockfd, shrek::socket_state::connection_state::connected);

    auto msg = std::string{"hello world"};
    auto r =
        send(sockfd, reinterpret_cast<const void*>(msg.data()), msg.size(), 0);

    auto data = state.mock_read();
    if (memcmp(data.data(), msg.data(), msg.size()) == 0) {
        std::cout << "MockNetworkTest: PASS!" << std::endl;
        return 0;
    } else {
        std::cout << "MockNetworkTest: FAILED!" << std::endl;
        return -1;
    }
}
