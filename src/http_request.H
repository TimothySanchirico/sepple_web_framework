#pragma once

#include <climits>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <unordered_map>

#include <cstdint>

#include "http.H"
#include "util.H"

namespace shrek {

struct http_request {

    using request_type = http::request_type;
    using http_version = http::http_version;

    static std::optional<http_request> deserialize(const uint8_t* data,
                                                   uint32_t       len);

    const std::string endpoint() const { return endpoint_; }
    const auto&       type() const { return type_; }
    const auto&       version() const { return version_; }
    const auto&       meta() const { return meta_; }
    const auto&       cookies() const { return cookies_; }
    const auto&       data() const { return data_; }

    std::optional<std::string> meta(const std::string& f) const;

    friend std::ostream& operator<<(std::ostream&, const http_request&);

  private:
    http_request() = default;

    enum class field {
        Accept,
        Accept_Charset,
        Accept_Encoding,
        Accept_Language,
        Authorization,
        Expect,
        From,
        Host,
        If_Match,
        If_Modified_Since,
        If_None_Match,
        If_Range,
        If_Unmodified_Since,
        Max_Forwards,
        Proxy_Authorization,
        Range,
        Refer,
        TE,
        User_Agent
    };

    request_type                                 type_;
    http_version                                 version_;
    std::string                                  endpoint_;
    std::unordered_map<std::string, std::string> meta_;
    std::unordered_map<std::string, std::string> cookies_;
    std::string                                  data_;
};

// where the work is
inline std::optional<http_request>
http_request::deserialize(const uint8_t* data, uint32_t len) {
    http_request req = {};
    auto         s   = std::string(reinterpret_cast<const char*>(data), len);
    auto header_end = s.find("\r\n\r\n");
    if (header_end == std::string::npos) {
        return std::nullopt;
    }

    std::stringstream stream(s.substr(0, header_end));

    std::string t;
    stream >> t;
    req.type_ = http::make_type(t);

    std::string r;
    stream >> r;
    req.endpoint_ = r;

    std::string v;
    stream >> v;
    req.version_ = http::make_version(v);

    // XXX this is shit
    for (std::string line; std::getline(stream, line);) {
        if (line.size() == 0) continue;
        auto e     = line.find_first_of(':');
        auto field = line.substr(0, e);
        if (field == "Cookie") {
            auto split = line.find_first_of('=');
            auto name  = line.substr(e + 2, split - (e + 2));
            auto value = line.substr(split + 1, line.size() - (split + 1));
            req.cookies_[name] = value;
        } else {
            auto value       = line.substr(e + 2, line.size() - (e +  2));
            req.meta_[field] = value;
        }
    }

    req.data_ = s.substr(header_end + 4, s.size() - (header_end + 4));
    return req;
}

} // namespace shrek
