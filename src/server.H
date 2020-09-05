#pragma once

#include "http_response.H"
#include "route_handler.H"

#include <iostream>
#include <memory>
#include <optional>
#include <unordered_map>

namespace shrek {

auto serialize(const auto& m) { return m.serialize(m); }

// Glues the network listening with the request dispatching
template <typename network_layer,
          template <typename...> typename route_handlerT, typename request,
          typename response>
struct server {
    using route_handler = route_handlerT<request, response>;
    using client_id     = typename network_layer::client_id;
    using connection    = typename network_layer::connection;

    server(network_layer& network, auto&& rh);

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
    void on_disconnection(const client_id&);

  private:
    route_handler                                          route_handler_;
    std::unordered_map<client_id, std::unique_ptr<client>> clients_;
};

#define SERVER_TEMPLATE_ARGS                                                   \
    template <typename network_layer,                                          \
              template <typename...> typename route_handlerT,                  \
              typename request, typename response>

#define SERVER_T server<network_layer, route_handlerT, request, response>

SERVER_TEMPLATE_ARGS
inline SERVER_T::server(network_layer& l, auto&& r)
    : route_handler_{std::forward<decltype(r)>(r)} {
    l.listen(this);
}

SERVER_TEMPLATE_ARGS
inline void SERVER_T::on_connection(const client_id& id, connection* con) {
    auto r =
        clients_.emplace(std::piecewise_construct, std::forward_as_tuple(id),
                         std::forward_as_tuple(new client{this, con}));
    if (r.second == false) {
        std::cout << " > 1 connection with the same client_id=" << id
                  << std::endl;
    }
}

SERVER_TEMPLATE_ARGS
inline void SERVER_T::on_disconnection(const client_id& id) {
    auto it = clients_.find(id);
    if (it != std::end(clients_)) {
        clients_.erase(it);
    }
}

SERVER_TEMPLATE_ARGS
inline SERVER_T::client::client(SERVER_T* s, connection* c)
    : connection_{c}, parent_{s} {
    connection_->listen(this);
}

SERVER_TEMPLATE_ARGS
inline void SERVER_T::client::on_data(const uint8_t* data, uint32_t len) {
    auto   empty_metadata = 0;
    auto&& req            = request::deserialize(data, len, empty_metadata);
    parent_->route_handler_.dispatch(this, req);
}

SERVER_TEMPLATE_ARGS
inline void SERVER_T::client::send(const response& s) {
    auto&&         buffer = serialize(s);
    const uint8_t* data   = reinterpret_cast<const uint8_t*>(buffer.data());
    connection_->send(data, buffer.size());
}

template <typename N, template <typename...> class RH, typename Req,
          typename Resp>
auto make_server(N& n, RH<Req, Resp>& rh) {
    return server<N, RH, Req, Resp>(n, rh);
}

#undef SERVER_TEMPLATE_ARGS
#undef SERVER_T

} // namespace shrek
