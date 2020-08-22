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
                                       "Accept: */*\n"
                                       "\n";
    auto empty_meta_data = 0;
    auto req = shrek::http_request::serialize(reinterpret_cast<const uint8_t*>(sample_request.data()), sample_request.size(), empty_meta_data);
    ASSERT_EQ(req.type(), shrek::http_request::request_type::GET);
    ASSERT_EQ(req.version(), shrek::http_request::http_version::HTTP_1_1);
    ASSERT_EQ(req.endpoint(), "/");
}
