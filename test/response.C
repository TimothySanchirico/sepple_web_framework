#include "http_response.H"
#include <gtest/gtest.h>

namespace shrek {

TEST(http_response_test, OK_Basic) {
    http_response r{"random body"};
    EXPECT_EQ(r.version(), http::http_version::HTTP_1_1);
    EXPECT_EQ(r.code(), http::status_code::OK);

    const std::string expected =
        "HTTP/1.1 200 OK\r\n\r\n"\
        "random body";
    auto buf = serialize(r);
    ASSERT_EQ(buf, expected);
}

TEST(http_response_test, With_Cookie) {
    http_response r{"random body"};
    r.set_cookie("test_cookie", "test_value");
    EXPECT_EQ(r.version(), http::http_version::HTTP_1_1);
    EXPECT_EQ(r.code(), http::status_code::OK);

    const std::string expected = "HTTP/1.1 200 OK\r\n"
                                 "Set-Cookie: test_cookie=test_value\r\n"
                                 "\r\n"
                                 "random body";
    auto buf = serialize(r);
    ASSERT_EQ(buf, expected);
}

} // namespace shrek
