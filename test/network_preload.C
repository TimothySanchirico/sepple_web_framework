
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "mock_network_bridge.H"

ssize_t send(int sockfd, const void* buf, size_t len, int flags) {
    std::vector<uint8_t> data;
    const auto* b = reinterpret_cast<const uint8_t*>(buf);
    auto& sending_map = get_socket_map();
    std::copy(b, b + len, std::back_inserter(data));
    sending_map[sockfd] = data;
    return 0;
}
