#include "gzip.H"
#include <cassert>
#include <iostream>
#include <iterator>
#include <zlib.h>

namespace shrek {
namespace gzip {

void zerr(int ret, const std::string& msg) {
    fputs(msg.c_str(), stderr);
    fputs(": ", stderr);
    switch (ret) {
    case Z_ERRNO:
        if (ferror(stdin))
            fputs("error reading stdin\n", stderr);
        if (ferror(stdout))
            fputs("error writing stdout\n", stderr);
        break;
    case Z_STREAM_ERROR:
        fputs("invalid compression level\n", stderr);
        break;
    case Z_DATA_ERROR:
        fputs("invalid or incomplete deflate data\n", stderr);
        break;
    case Z_MEM_ERROR:
        fputs("out of memory\n", stderr);
        break;
    case Z_VERSION_ERROR:
        fputs("zlib version mismatch!\n", stderr);
    }
}

std::vector<uint8_t> compress(const std::string& s) {

    constexpr size_t Chunk = 16384;

    z_stream z;
    z.zalloc = Z_NULL;
    z.zfree  = Z_NULL;
    z.opaque = Z_NULL;
    auto r = deflateInit2(std::addressof(z), Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                          25, 8, Z_DEFAULT_STRATEGY);
    if (r != Z_OK) {
        zerr(r, "deflateInit2() failed");
        return {};
    }

    gz_header h;
    r = deflateSetHeader(std::addressof(z), std::addressof(h));
    if (r != Z_OK) {
        zerr(r, "deflateSetHeader() failed");
        if (z.msg) {
            std::cout << "z.msg=" << z.msg << std::endl;
        }
        return {};
    }

    std::vector<uint8_t> out;

    const auto* data = s.data();
    const auto* end  = s.data() + s.size();
    while (data != end) {
        const auto* next  = std::min(data + Chunk, end);
        auto        flush = (next == end) ? Z_FINISH : Z_NO_FLUSH;
        z.avail_in        = next - data;
        z.next_in         = const_cast<uint8_t*>(
            reinterpret_cast<const uint8_t*>(data)); // gross

        do {
            std::vector<uint8_t> buffer;
            buffer.resize(Chunk);
            z.avail_out = Chunk;
            z.next_out  = buffer.data();

            auto r = deflate(std::addressof(z), flush);
            assert(r != Z_STREAM_ERROR);
            const auto handled = Chunk - z.avail_out;
            std::copy(std::begin(buffer), std::begin(buffer) + handled,
                      std::back_inserter(out));
        } while (z.avail_out == 0);

        data = next;
    }
    r = deflateEnd(std::addressof(z));
    if (r != Z_OK) {
        zerr(r, "deflateEnd() failed");
        if (z.msg) {
            std::cout << "z.msg=" << z.msg << std::endl;
        }
        return {};
    }

    return out;
}

} // namespace gzip
} // namespace shrek
