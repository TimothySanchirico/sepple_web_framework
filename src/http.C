#include "http.H"

#include <iostream>
#include <unordered_map>

namespace shrek {
namespace http {

std::string type_string(request_type t) {
    switch (t) {
    case request_type::GET:
        return "GET";
    case request_type::POST:
        return "POST";
    default: // TODO exceptions
        return "Unhandled";
    }
}

std::string version_string(http_version v) {
    switch (v) {
    case http_version::HTTP_1:
        return "HTTP/1.0";
    case http_version::HTTP_1_1:
        return "HTTP/1.1";
    default:
        return "Unhandled";
    }
}

http_version make_version(const std::string& v) {
    const static std::unordered_map<std::string, http_version> map = {
        {"HTTP/1.0", http_version::HTTP_1},
        {"HTTP/1.1", http_version::HTTP_1_1}};
    const auto it = map.find(v);
    if (it != std::end(map)) {
        return it->second;
    } else {
        std::cout << "http_version=" << v << " not handled" << std::endl;
    }
    return {};
}

request_type make_type(const std::string& v) {
    const static std::unordered_map<std::string, request_type> map = {
        {"GET", request_type::GET}, {"POST", request_type::POST}};
    const auto it = map.find(v);
    if (it != std::end(map)) {
        return it->second;
    } else {
        std::cout << "http_request_type=" << v << " not handled" << std::endl;
    }
    return {};
}

std::ostream& operator<<(std::ostream& os, request_type t) {
    os << type_string(t);
    return os;
}

std::ostream& operator<<(std::ostream& os, http_version v) {
    os << version_string(v);
    return os;
}

} // namespace http
} // namespace shrek
