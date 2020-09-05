#include "network.H"

#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>
#include <sys/socket.h>

#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace shrek {
std::vector<int> fds() {
    static std::vector<int> cleanme;
    return cleanme;
}

void register_fd_for_cleanup(int fd) { fds().push_back(fd); }

void do_cleanup() {
    for (auto fd : fds()) {
        std::cout << "Closing fd=fd" << std::endl;
        close(fd);
    }
    fds().clear();
}

tcp_server::tcp_server(uint16_t port) {
    sockfd_ = socket(AF_INET, SOCK_STREAM, 0ul);


    if (sockfd_ < 0) {
        perror("Failed to create socket");
    }

    addr_.sin_family      = AF_INET;
    addr_.sin_addr.s_addr = htonl(INADDR_ANY);
    addr_.sin_port        = htons(port);
}

void tcp_server::run() {
    sockaddr* addr    = reinterpret_cast<sockaddr*>(std::addressof(addr_));
    auto      bind_ec = bind(sockfd_, addr, sizeof(addr_));
    if (bind_ec != 0) {
        perror("Failed to bind");
        return;
    }

    register_fd_for_cleanup(sockfd_);
    std::atexit(do_cleanup);

    auto listen_ec = ::listen(sockfd_, 10);
    if (listen_ec != 0) {
        perror("Failed to listen");
        return;
    }

    // infinite loop
    sockaddr_in client_addr;
    socklen_t   client_addr_len = sizeof(client_addr);

    while (auto fd = accept(sockfd_, (sockaddr*)std::addressof(client_addr),
                            std::addressof(client_addr_len))) {
        if (fd < 0)
            break;

        // lol
        // TODO
        auto* conn = new connection{fd};
        auto  id   = fd;
        on_connection_(id, conn);

        std::vector<uint8_t> data;
        data.resize(1000);
        auto n = recv(fd, data.data(), data.size(), 0);
        conn->receive(data.data(), n);

        // we generally have to close the connection for the webpage to render
        conn->close();
        on_disconnection_(id);

    }
    perror("Server died");
}

void tcp_server::connection::receive(const uint8_t* data, uint32_t len) {
    on_data_(data, len);
}

void tcp_server::connection::send(const uint8_t* data, uint32_t len) {
    auto r = ::send(fd_, data, len, 0);
    std::cout << "[OUTGOING]" << std::string((const char*)data, len)
              << std::endl;
    if (r < 0) {
        perror("Error sending");
    }
}

void tcp_server::connection::close() {
    auto r = ::close(fd_);
    if (r < 0) {
        perror("Error closing client connection");
    }
}

} // namespace shrek
