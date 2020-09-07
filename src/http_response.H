#pragma once

#include "http.H"

#include <fstream>
#include <iosfwd>
#include <sstream>
#include <string>
#include <unordered_map>

namespace shrek {

struct http_response {
    http_response(const std::string& body);

    auto        code() const& { return code_; }
    auto        version() const& { return version_; }
    const auto& data() const& { return data_; }
    const auto& cookies() const& { return cookies_; }

    void set_cookie(const std::string& k, const std::string& v);

    struct header {
        enum class fields {};
    };

  private:
    std::unordered_map<std::string, std::string> cookies_;
    http::http_version                           version_;
    http::status_code                            code_;
    std::string                                  data_;
};

inline std::string serialize(const http_response& resp) {
    std::ostringstream os;
    os << resp.version() << " " << static_cast<uint32_t>(resp.code()) << " "
       << http::reason_phrase(resp.code()) << http::crlf;

    for (auto&& [cookie, value] : resp.cookies()) {
        os << "Set-Cookie: " << cookie << "=" << value << http::crlf;
    }

    os << http::crlf;
    os << resp.data();
    return os.str();
}

std::ostream& operator<<(std::ostream& os, const http_response&);

std::string response_from_template(const std::string& filename);

} // namespace shrek
