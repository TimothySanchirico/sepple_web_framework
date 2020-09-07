#include "http_response.H"
#include "util.H"

namespace shrek {

http_response::http_response(const std::string& body)
    : version_{http::http_version::HTTP_1_1}, code_{http::status_code::OK},
      data_{body} {}

void http_response::set_cookie(const std::string& key,
                               const std::string& value) {
    cookies_[key] = value;
}

// TODO make a file server
std::string response_from_template(const std::string& filename) {
    std::ifstream      file{filename, std::ios_base::in};
    std::ostringstream s;
    s << file.rdbuf();
    return s.str();
}

std::ostream& operator<<(std::ostream& os, const http_response& resp) {
    os << "http_response={"
       << "type=" << resp.version() << ", code=" << resp.code()
       << ", cookies=" << resp.cookies() << ", data=" << resp.data() << "}";
    return os;
}

} // namespace shrek
