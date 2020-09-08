#pragma once

#include "http.H"
#include "gzip.H"

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
    const auto& meta() const& { return meta_; }
    const auto& data() const& { return data_; }
    const auto& cookies() const& { return cookies_; }

    void set_cookie(const std::string& k, const std::string& v);
    void set_meta(const std::string& k, const std::string& v);

    struct header {
        enum class fields {};
    };

  private:
    std::unordered_map<std::string, std::string> meta_;
    std::unordered_map<std::string, std::string> cookies_;
    http::http_version                           version_;
    http::status_code                            code_;
    std::string                                  data_;
};

inline std::string serialize(const http_response& resp) {
    std::ostringstream os;
    os << resp.version() << " " << static_cast<uint32_t>(resp.code()) << " "
       << http::reason_phrase(resp.code()) << http::crlf;

    for (auto&& [name, value] : resp.meta()) {
        os << name << ": " << value << http::crlf;
    }
    for (auto&& [cookie, value] : resp.cookies()) {
        os << "Set-Cookie: " << cookie << "=" << value << http::crlf;
    }

    os << http::crlf;

    auto encoding = resp.meta().find("Content-Encoding");
    if (encoding != std::end(resp.meta())) {
        if (encoding->second == "gzip") {
            auto compressed = gzip::compress(resp.data());
            os.write(reinterpret_cast<const char*>(compressed.data()),
                     compressed.size());
        } else {
            os << resp.data();
        }
    } else {
        os << resp.data();
    }
    return os.str();
}

std::ostream& operator<<(std::ostream& os, const http_response&);

std::string response_from_template(const std::string& filename);

} // namespace shrek
