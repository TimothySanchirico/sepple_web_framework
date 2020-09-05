#pragma once

#include <cstdint>
#include <map>
#include <vector>

namespace shrek {
struct socket_state {
    enum class connection_state {
        unitialized,
        created,
        bound,
        listening,
        connected,
        disconnected
    };
    socket_state(int fd, connection_state);
    socket_state();

    std::vector<uint8_t> read();
    void                 send(std::vector<uint8_t>);

    // send into the read buffer
    // read from the send buffer
    void                 mock_send(const std::vector<uint8_t>);
    std::vector<uint8_t> mock_read();

    auto state() const {return state_; }

    void close();
    void bind();
    void listen();

  private:
    connection_state     state_;
    int                  fd_;
    std::vector<uint8_t> read_buffer_;
    std::vector<uint8_t> send_buffer_;
};

socket_state& get_socket_state(int fd);
socket_state& create_socket_state(int fd, socket_state::connection_state);
std::map<int, socket_state>& get_socket_state_map();
int                          get_next_fd();

} // namespace shrek
