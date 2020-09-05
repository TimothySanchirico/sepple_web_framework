
#include "mock_network_bridge.H"
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int socket(int domain, int type, int protocol) { return shrek::get_next_fd(); }

int bind(int sockfd, const sockaddr* addr, socklen_t addrlen) {
    auto& state = shrek::get_socket_state(sockfd);
    if (state.state() == shrek::socket_state::connection_state::created) {
        state.bind();
        return 0;
    }
    return -1;
}

int listen(int sockfd, int backlog) {
    auto& state = shrek::get_socket_state(sockfd);
    if (state.state() == shrek::socket_state::connection_state::bound) {
        state.listen();
        return 0;
    }
    return -1;
}

int accept(int sockfd, sockaddr* addr, socklen_t addrlen) {
    auto& state = shrek::get_socket_state(sockfd);
    if (state.state() == shrek::socket_state::connection_state::listening) {
        auto  fd           = shrek::get_next_fd();
        auto& client_state = shrek::create_socket_state(
            fd, shrek::socket_state::connection_state::connected);
        return client_state.state() ==
               shrek::socket_state::connection_state::connected;
    }
    return -1;
}

ssize_t send(int sockfd, const void* buf, size_t len, int flags) {
    std::vector<uint8_t> data;
    const auto*          b     = reinterpret_cast<const uint8_t*>(buf);
    auto&                state = shrek::get_socket_state(sockfd);
    if (state.state() == shrek::socket_state::connection_state::connected) {
        std::copy(b, b + len, std::back_inserter(data));
        state.send(data);
        return len;
    }
    return -1;
}

ssize_t recv(int sockfd, void* buf, size_t len, int flags) {
    auto* b     = reinterpret_cast<uint8_t*>(buf);
    auto& state = shrek::get_socket_state(sockfd);
    if (state.state() == shrek::socket_state::connection_state::connected) {
        auto data = state.read();
        std::copy(std::begin(data), std::end(data), b);
        return data.size();
    }

    return -1;
}

int close(int sockfd) {
    auto& state = shrek::get_socket_state(sockfd);
    if (state.state() == shrek::socket_state::connection_state::connected) {
        state.close();
        return 0;
    }
    return -1;
}
