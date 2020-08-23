#pragma once

#include <unorder_map>

namespace shrek {

// Glues the network listening with the request dispatching
template <typename network_layer,
          template <typename...> typename route_handlerT, typename request,
          typename response>
struct server {
    using maybe_response = std::optional<responseT<session_data>>;
    using route_handler  = route_handlerT<request, response>;
    using client_id      = typename network_layer::client_id;
    using connection     = typename network_layer::connection;

    server(const network_layer& network, route_handler&&);

    struct client {
        client(server*, connection*);
        void send(const response&);

      public: // interface expected by network layer
        void on_data(const uint8_t* data, uint32_t len);

      private:
        connection* const connection_;
        server* const     parent_;
    };

    void on_connection(const client_id&, connection*);
    void on_disconnection(const client_id&) {}

  private:
    network_layer* const                                   network_;
    route_handler                                          route_handler_;
    std::unordered_map<client_id, std::unique_ptr<client>> clients_;
};

#define SERVER_TEMPLATE_ARGS                                                   \
    template <typename session_data, typename network_layer,                   \
              template <typename...> typename route_handlerT,                  \
              typename requestT, typename responseT>

#define SERVER_T                                                               \
    server<session_data, network_layer, route_handlerT, requestT, responseT>

SERVER_TEMPLATE_ARGS
inline SERVER_T::server(const network_layer& l, route_handler&& r)
    : network_{l}, route_handler_{std::move(r)} {
    l.listen(this);
}

inline void SERVER_T::on_connection(const client_id& id, connection* con) {
    auto r =
        clients_.emplace(std::piecewise_construct, std::forward_as_tuple{id},
                         std::forward_as_tuple{this, con});
    if (r.second == false) {
        std::cout << " > 1 connection with the same client_id=" << client_id << std::endl;
    }
}

SERVER_TEMPLATE_ARGS
inline SERVER_T::client::client(SERVER_T* s, connection* c)
    : connection_{c}, parent_{s} {}

SERVER_TEMPLATE_ARGS
inline void SERVER_T::client::on_data(const uint8_t* data, uint32_t len) {
    auto&& req = request::servialize(data, len);
    route_handler_.dispatch(req);
}

SERVER_TEMPLATE_ARGS
inline void SERVER_T::client::send(const response& s) {
    auto&& [data, len] = response::serialize(s);
    connection_->send(data, len);
}

#undefine SERVER_TEMPLATE_ARGS
#undefine SERVER_T

} // namespace shrek
