#include "mock_network_bridge.H"

#include <tuple>
#include <utility>

namespace shrek {

socket_state::socket_state() : state_{connection_state::unitialized} {}

socket_state::socket_state(int fd, connection_state s) : fd_{fd}, state_{s} {}

std::vector<uint8_t> socket_state::read() {
    auto r = read_buffer_;
    read_buffer_.clear();
    return r;
}

void socket_state::send(const std::vector<uint8_t> data) {
    send_buffer_ = data;
}

void socket_state::mock_send(const std::vector<uint8_t> data) {
    read_buffer_ = data;
}

std::vector<uint8_t> socket_state::mock_read() {
    auto r = send_buffer_;
    send_buffer_.clear();
    return r;
}

void socket_state::close() { state_ = connection_state::disconnected; }
void socket_state::bind() { state_ = connection_state::bound; }
void socket_state::listen() { state_ = connection_state::listening; }

static std::map<int, socket_state> map;

socket_state& get_socket_state(int fd) { return map[fd]; }
socket_state& create_socket_state(int fd, socket_state::connection_state s) {
    auto r = map.emplace(std::piecewise_construct, std::forward_as_tuple(fd),
                         std::forward_as_tuple(fd, s));
    return map[fd];
}
std::map<int, socket_state>& get_socket_state_map() { return map; }

static int fd_counter = 4;
int        get_next_fd() { return fd_counter++; }

} // namespace shrek
