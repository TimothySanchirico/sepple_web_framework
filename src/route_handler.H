#pragma once

#include <functional>
#include <unordered_map>
#include <utility>

namespace shrek {

template <typename request_t, typename response_t> struct route_handler {
    using response_generator =
        std::function<std::optional<response_t>(const request_t&)>;
    using endpoint =
        std::string; // xxx should be a function of the request type

    void dispatch(auto* client, const request_t&);

    template <typename... F> void add_endpoint(const endpoint&, F&&...);
    template <typename... F> void set_default_handler(F&&... f);

  private:
    std::unordered_map<endpoint, response_generator> response_generators_;
    response_generator default_ = [](auto&&...) { return std::nullopt; };
};

#define ROUTE_HANDLER_TEMPLATE                                                 \
    template <typename request_t, typename response_t>
#define ROUTE_HANDLER_T route_handler<request_t, response_t>

ROUTE_HANDLER_TEMPLATE
inline void ROUTE_HANDLER_T::dispatch(auto* client, const request_t& req) {

    const auto& key = req.endpoint();
    auto        it  = response_generators_.find(key);
    const auto  resp =
        it != std::end(response_generators_) ? it->second(req) : default_(req);

    if (resp) {
        client->send(*resp);
    }
}

ROUTE_HANDLER_TEMPLATE
template <typename... F>
inline void ROUTE_HANDLER_T::add_endpoint(const ROUTE_HANDLER_T::endpoint& dest,
                                          F&&... f) {
    auto r = response_generators_.emplace(
        std::piecewise_construct, std::forward_as_tuple(dest),
        std::forward_as_tuple(std::forward<F>(f)...));
    if (r.second == false) {
        std::cout << "Error: endpoint=" << dest << "could not be added"
                  << std::endl;
    }
}

ROUTE_HANDLER_TEMPLATE
template <typename... F>
inline void ROUTE_HANDLER_T::set_default_handler(F&&... f) {
    default_ = response_generator{std::forward<F>(f)...};
}

#undef ROUTE_HANDLER_TEMPLATE
#undef ROUTE_HANDLER_T

} // namespace shrek
