#include <http_request.H>
#include <route_handler.H>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using testing::_;

struct route_handler_test : public testing::Test {
    using request         = shrek::http_request;
    using response        = bool;
    using route_handler_t = shrek::route_handler<request, response>;
    virtual void SetUp() override;
    virtual void TearDown() override {}

    struct mock_client {
        MOCK_METHOD1(send, void(const response&));
    };

    std::string create_request(const std::string& endpoint);

  protected:
    std::unique_ptr<route_handler_t> route_handler;
    mock_client                      client;
};

void route_handler_test::SetUp() {
    route_handler = std::make_unique<route_handler_t>();
}

std::string
route_handler_test::create_request(const std::string& endpoint) {

    const std::string sample_request = "GET " + endpoint +
                                       " HTTP/1.1\n"
                                       "Host: wttr.in\n"
                                       "User-Agent: curl/7.58.0\n"
                                       "Accept: */*\r\n"
                                       "\r\n";
    return sample_request;
}

TEST_F(route_handler_test, basic_dispatch_with_response) {
    const std::string somewhere = "/somewhere";
    auto              req       = create_request(somewhere);
    bool              flag      = false;
    auto handler = [&flag](const request& req) -> std::optional<response> {
        flag = true;
        return {false};
    };
    route_handler->add_endpoint(somewhere, handler);

    EXPECT_CALL(client, send(false)).Times(1);

    route_handler->dispatch(std::addressof(client), reinterpret_cast<const uint8_t*>(req.data()), req.size());

    ASSERT_EQ(flag, true);
}

TEST_F(route_handler_test, basic_dispatch_with_no_response) {
    const std::string somewhere = "/somewhere";
    auto              req       = create_request(somewhere);
    bool              flag      = false;
    auto handler = [&flag](const request& req) -> std::optional<response> {
        flag = true;
        return std::nullopt;
    };
    route_handler->add_endpoint(somewhere, handler);

    EXPECT_CALL(client, send(_)).Times(0);
    route_handler->dispatch(std::addressof(client), reinterpret_cast<const uint8_t*>(req.data()), req.size());

    ASSERT_EQ(flag, true);
}

