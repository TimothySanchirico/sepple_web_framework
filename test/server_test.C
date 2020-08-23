#include "http_request.H"
#include "route_handler.H"
#include "server.H"
#include <functional>
#include <gmock/gmock-generated-function-mockers.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <tuple>
#include <unordered_map>
#include <utility>

using testing::_;

struct mock_network_layer {
    using client_id = uint64_t;
    struct connection {
        void listen(auto*);

        MOCK_METHOD2(send, void(const uint8_t*, uint32_t));

        std::function<void(const uint8_t*, uint32_t)> listener;
    };

    void listen(auto* listener);

    std::pair<client_id, connection*> create_connection();

    std::function<void(client_id, connection*)> connection_callback_ = {};
    std::unordered_map<client_id, std::unique_ptr<connection>> connections_;
};

inline void mock_network_layer::listen(auto* listener) {
    connection_callback_ = [listener](client_id id, connection* c) {
        listener->on_connection(id, c);
    };
}

inline std::pair<typename mock_network_layer::client_id,
                 typename mock_network_layer::connection*>
mock_network_layer::create_connection() {
    const auto id = connections_.size();
    auto*      c  = new connection;
    auto       r  = connections_.emplace(id, c);
    if (r.second == false) {
        std::cout << "Failed to insert client_id=" << id << std::endl;
    }

    connection_callback_(id, c);
    return {id, c};
}

inline void mock_network_layer::connection::listen(auto* l) {
    listener = [l](const uint8_t* data, uint32_t len) {
        l->on_data(data, len);
    };
}

struct mock_response {
    mock_response(const std::string&);

    static std::pair<const uint8_t*, uint32_t> serialize(const mock_response&);
    std::string                                data_;
};

mock_response::mock_response(const std::string& s) : data_{s} {}

std::pair<const uint8_t*, uint32_t>
mock_response::serialize(const mock_response& r) {
    return {reinterpret_cast<const uint8_t*>(r.data_.data()), r.data_.size()};
}

struct server_test : public testing::Test {
    virtual void SetUp() override;
    virtual void TearDown() override {}

    using request         = shrek::http_request;
    using route_handler_t = shrek::route_handler<request, mock_response>;

    // toda
    request create_request(const std::string& endpoint);

    using server_t = shrek::server<mock_network_layer, shrek::route_handler,
                                   request, mock_response>;

    using response_generator = typename route_handler_t::response_generator;

    std::unique_ptr<server_t> server;

    // hack to get around the thing where the server owns and does not expose
    // the route handler
    response_generator responder;
    mock_network_layer network_layer;
};

void server_test::SetUp() {
    route_handler_t rh{};
    rh.add_endpoint("/test_endpoint", [this](const request& req) {
        return this->responder(req);
    });
    server.reset(
        new server_t(network_layer, std::forward<route_handler_t&&>(rh)));
}

TEST_F(server_test, basic_connection_and_response) {

    const std::string req = "GET /test_endpoint HTTP/1.1\n"
                            "Host: wttr.in\n"
                            "User-Agent: curl/7.58.0\n"
                            "Accept: */*\n"
                            "\n";
    auto p       = network_layer.create_connection();
    auto [id, c] = p;
    responder    = [](const request&) { return mock_response{"hello world"}; };
    // todo actually match the hello world
    EXPECT_CALL(*c, send(_, _)).Times(1);
    c->listener(reinterpret_cast<const uint8_t*>(req.data()), req.size());
}
