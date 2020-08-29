#include "http_request.H"

#include <iostream>

namespace shrek {

std::ostream& operator<<(std::ostream& os, const http_request& req) {
    os << "http_request={"
       << "type=" << req.type() << "endpoint=" << req.endpoint()
       << "version=" << req.version() << "meta=" << req.meta()
       << "cookies=" << req.cookies() << "data=" << req.data() << "}";
    return os;
}

} // namespace shrek
