#include "http_request.H"

#include <iostream>

namespace shrek {

std::optional<std::string> http_request::meta(const std::string& f) const {
    auto it = meta_.find(f);
    if (it == std::end(meta_)) {
        return std::nullopt;
    } else {
        return it->second;
    }
}

std::ostream& operator<<(std::ostream& os, const http_request& req) {
    os << "http_request={"
       << "type=" << req.type() << ", endpoint=" << req.endpoint()
       << ", version=" << req.version() << ", meta=" << req.meta()
       << ", cookies=" << req.cookies() << ", data=" << req.data() << "}";
    return os;
}

} // namespace shrek
