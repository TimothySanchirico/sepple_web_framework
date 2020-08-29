#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>

#include <cstdint>

#include "http.H"
#include "util.H"

namespace shrek {

struct http_request {

    using request_type = http::request_type;
    using http_version = http::http_version;

    static http_request serialize(const uint8_t* data, uint32_t len,
                                  auto&& metadata);

    const std::string endpoint() const { return endpoint_; }
    const auto&       type() const { return type_; }
    const auto&       version() const { return version_; }
    const auto&       meta() const { return meta_; }
    const auto&       cookies() const { return cookies_; }
    const auto&       data() const { return data_; }

    friend std::ostream& operator<<(std::ostream&, const http_request&);

  private:
    http_request() = default;

    request_type                                 type_;
    http_version                                 version_;
    std::string                                  endpoint_;
    std::unordered_map<std::string, std::string> meta_;
    std::unordered_map<std::string, std::string> cookies_;
    std::string                                  data_;
};

// where the work is
inline http_request http_request::serialize(const uint8_t* data, uint32_t len,
                                            auto&& metadata) {
    http_request req = {};

    std::stringstream stream(std::string(reinterpret_cast<const char*>(data), len));

    std::string t;
    stream >> t;
    req.type_ = http::make_type(t);

    std::string r;
    stream >> r;
    req.endpoint_ = r;

    std::string v;
    stream >> v;
    req.version_ = http::make_version(v);

    // TODO fill in the other fields

    // get the type
    return req;
}

} // namespace shrek
