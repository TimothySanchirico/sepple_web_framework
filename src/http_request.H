#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>

#include <cstdint>

#include "util.H"

namespace shrek {

struct http_request {
    static http_request serialize(const uint8_t* data, uint32_t len,
                                  auto&& metadata);

    const std::string endpoint() const { return endpoint_; }
    const auto&       type() const { return type_; }
    const auto&       version() const { return version_; }
    const auto&       meta() const { return meta_; }
    const auto&       cookies() const { return cookies_; }
    const auto&       data() const { return data_; }

    enum class request_type { INVALID, GET, POST, UNHANDLED };
    enum class http_version { INVALID, HTTP_1, HTTP_1_1, UNHANDLED };

    static std::string type_string(request_type);
    static std::string version_string(http_version);

    static request_type make_type(const std::string&);
    static http_version make_version(const std::string&);

    friend std::ostream& operator<<(std::ostream&, const http_request&);
    friend std::ostream& operator<<(std::ostream&,
                                    http_request::request_type);
    friend std::ostream& operator<<(std::ostream&,
                                    http_request::http_version);

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
    req.type_ = make_type(t);

    std::string r;
    stream >> r;
    req.endpoint_ = r;

    std::string v;
    stream >> v;
    req.version_ = make_version(v);

    // TODO fill in the other fields

    // get the type
    return req;
}

} // namespace shrek
