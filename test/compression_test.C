#include "gzip.H"

#include <gtest/gtest.h>

#include <fstream>

using namespace shrek;

// Write a compressed string to a file and then read it back an make sure that its the same
TEST(GZIP, simple_file) {
    const std::string data = "Hello World";

    std::fstream test{"simple.test.txt.gzip", std::ios_base::out | std::ios_base::binary};
    auto compressed = gzip::compress(data);
    test.write(reinterpret_cast<char*>(compressed.data()), compressed.size());
    test.flush();
    test.close();

    auto output = ::popen("zcat simple.test.txt.gzip", "r");
    std::vector<char> buffer;
    buffer.resize(15);
    fread(buffer.data(), 1, buffer.size(), output);

    std::string out = buffer.data();
    EXPECT_EQ(out, data);
}
