#pragma once

#include <iosfwd>

namespace shrek {
namespace http {

enum class request_type { INVALID, GET, POST, UNHANDLED };
enum class http_version { INVALID, HTTP_1, HTTP_1_1, UNHANDLED };

std::ostream& operator<<(std::ostream&, request_type);
std::ostream& operator<<(std::ostream&, http_version);

std::string type_string(request_type);
std::string version_string(http_version);

request_type make_type(const std::string&);
http_version make_version(const std::string&);

} // namespace http
} // namespace shrek
