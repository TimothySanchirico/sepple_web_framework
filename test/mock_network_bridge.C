#include "mock_network_bridge.H"

std::map<int, std::vector<uint8_t>>& get_socket_map() {
    static std::map<int, std::vector<uint8_t>> map;
    return map;
}
