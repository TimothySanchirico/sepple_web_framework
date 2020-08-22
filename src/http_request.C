#include "http_request.H"

namespace shrek {

std::string http_request::type_string(request_type t) {
    switch (t) {
    case request_type::GET:
        return "GET";
    case request_type::POST:
        return "POST";
    default: // TODO exceptions
        return "Unhandled";
    }
}

std::string http_request::version_string(http_version v) {
    switch (v) {
    case http_version::HTTP_1:
        return "HTTP/1.0";
    case http_version::HTTP_1_1:
        return "HTTP/1.1";
    default:
        return "Unhandled";
    }
}

http_request::http_version http_request::make_version(const std::string& v) {
    const static std::unordered_map<std::string, http_request::http_version>
        map = {{"HTTP/1.0", http_version::HTTP_1},
               {"HTTP/1.1", http_version::HTTP_1_1}};
    const auto it = map.find(v);
    if (it != std::end(map)) {
        return it->second;
    } else {
        std::cout << "http_version=" << v << " not handled" << std::endl;
    }
    return {};
}

http_request::request_type http_request::make_type(const std::string& v) {
    const static std::unordered_map<std::string, http_request::request_type>
        map = {{"GET", request_type::GET}, {"POST", request_type::POST}};
    const auto it = map.find(v);
    if (it != std::end(map)) {
        return it->second;
    } else {
        std::cout << "http_request_type=" << v << " not handled" << std::endl;
    }
    return {};
}

std::ostream& operator<<(std::ostream& os, http_request::request_type t) {
    os << http_request::type_string(t);
    return os;
}

std::ostream& operator<<(std::ostream& os, http_request::http_version v) {
    os << http_request::version_string(v);
    return os;
}

std::ostream& operator<<(std::ostream& os, const http_request& req) {
    os << "http_request={"
       << "type=" << req.type() << "endpoint=" << req.endpoint()
       << "version=" << req.version() << "meta=" << req.meta()
       << "cookies=" << req.cookies() << "data=" << req.data() << "}";
    return os;
}

} // namespace shrek
