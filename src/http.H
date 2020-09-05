#pragma once

#include <iosfwd>
#include <unordered_map>

namespace shrek {
namespace http {

static constexpr auto crlf = "\r\n";

enum class general_header_field {
    Cache_Control,
    Connection,
    Date,
    Pragma,
    Trailer,
    Transfer_Encoding,
    Upgrade,
    Via,
    Warning
};

enum class entity_header_field {
    Allow,
    Content_Encoding,
    Content_Language,
    Content_Length,
    Content_Location,
    COntent_MD5,
    Content_Range,
    Content_Type,
    Expires,
    Last_Modified
};

enum class request_type { INVALID, GET, POST, UNHANDLED };
enum class http_version { INVALID, HTTP_1, HTTP_1_1, UNHANDLED };
enum class status_code { INVALID, OK = 200 };

std::ostream& operator<<(std::ostream&, request_type);
std::ostream& operator<<(std::ostream&, http_version);

std::string type_string(request_type);
std::string version_string(http_version);

request_type make_type(const std::string&);
http_version make_version(const std::string&);

std::string reason_phrase(status_code);

} // namespace http
} // namespace shrek
