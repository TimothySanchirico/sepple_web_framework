#include <arpa/inet.h>
#include <cstdint>
#include <sys/types.h>

#include <functional>

namespace shrek {

struct tcp_server {
    tcp_server(uint16_t port);

    using client_id = uint64_t;
    struct connection {

        using data_callback = std::function<void(const uint8_t*, uint32_t)>;
        connection(int fd) : fd_{fd} {}

        void listen(auto* listener);
        void send(const uint8_t* data, uint32_t len);

        void receive(const uint8_t* data, uint32_t len);

        void close();

      private:
        data_callback on_data_;
        int           fd_;
    };
    using connection_callback =
        std::function<void(const client_id&, connection*)>;
    using disconnection_callback =
        std::function<void(const client_id&)>;

    void listen(auto* listener);

    // blocking
    void run();

  private:
    sockaddr_in            addr_;
    int                    sockfd_;
    connection_callback    on_connection_;
    disconnection_callback on_disconnection_;
};

void tcp_server::listen(auto* listener) {
    on_connection_ = [listener](const client_id& id, connection* c) {
        listener->on_connection(id, c);
    };
    on_disconnection_ = [listener](const client_id& id) {
        listener->on_disconnection(id);
    };
}

void tcp_server::connection::listen(auto* listener) {
    on_data_ = [listener](const uint8_t* data, uint32_t len) {
        listener->on_data(data, len);
    };
}

} // namespace shrek
