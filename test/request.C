#include "http_request.H"
#include <gtest/gtest.h>

struct http_request_test : public testing::Test {
    virtual void SetUp() override {}
    virtual void TearDown() override {}
};

TEST_F(http_request_test, basic_serialize) {
    const std::string sample_request = "GET / HTTP/1.1\n"
                                       "Host: wttr.in\n"
                                       "User-Agent: curl/7.58.0\n"
                                       "Accept: */*\r\n"
                                       "\r\n";
    auto req = shrek::http_request::deserialize(reinterpret_cast<const uint8_t*>(sample_request.data()), sample_request.size());
    ASSERT_EQ(static_cast<bool>(req), true);
    ASSERT_EQ(req->type(), shrek::http::request_type::GET);
    ASSERT_EQ(req->version(), shrek::http::http_version::HTTP_1_1);
    ASSERT_EQ(req->endpoint(), "/");
    ASSERT_EQ(*(req->meta("Host")), std::string{"wttr.in"});
}
