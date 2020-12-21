#pragma once

#include <string>
#include <vector>
#include <zlib.h>

namespace shrek {
namespace gzip {

std::vector<uint8_t> compress(const std::string& in);

} // namespace gzip
} // namespace shrek
