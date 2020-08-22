#pragma once

namespace shrek {
    

// Glues the network listening with the request dispatching
template <typename network_layer,
          template <typename...> typename route_handlerT, typename request,
          typename response>
struct server {
    using maybe_response  = std::optional<responseT<session_data>>;
    using request_handler = std::function<response(const request&)>;
    using route_handler   = route_handlerT<request, response>;

    server(const network_layer& network, route_handler&&);

  public: // interface expected by network layer
    void onData(const uint8_t* data, uint32_t len);

  private:
    route_handler route_handler_;
};

#define SERVER_TEMPLATE_ARGS                                                   \
    template <typename session_data, typename network_layer,                   \
              template <typename...> typename route_handlerT,                  \
              typename requestT, typename responseT>

#define SERVERT                                                                \
    server<session_data, network_layer, route_handlerT, requestT, responseT>

SERVER_TEMPLATE_ARGS
inline SERVERT::server(const network_layer& l, route_handler&& r):
    route_handler_{std::move(r)} { l.listen(this); }

SERVER_TEMPLATE_ARGS
inline void SERVERT::onData(const uint8_t* data, uint32_t len) {
    auto&& req = request::servialize(data, len);
    route_handler_.dispatch(req);
}

} // namespace shrek
