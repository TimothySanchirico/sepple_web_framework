#ifndef COOKIE_H
#define COOKIE_H
#include <string>

struct Cookie {
    Cookie(std::string& s, std::string& v): name(s), value(v) {}
    std::string name;
    std::string value;
};
#endif //COOKIE_H
