#ifndef REQUEST_H
#define REQUEST_H

#include <string>
#include <regex>
#include "prefix_tree.h"

typedef unsigned short RequestType;

namespace HTTPRequest {
    extern RequestType OPTIONS;
    extern RequestType GET;
    extern RequestType HEAD;
    extern RequestType POST;
    extern RequestType PUT;
    extern RequestType DELETE;
    extern RequestType TRACE;
    extern RequestType CONNECT;
}


typedef std::string EndPoint;

// Temporary solution to avoid comparing strings
inline RequestType type_string_hash(std::string & s){
    size_t j(0);
    for(size_t i = 0; i < s.size(); j+=s[i++]);
    return j;
}


class Request {
    public:
        Request(std::string&& r);
        ~Request();

        EndPoint & get_end_pt();
        RequestType type;
        std::string & get_raw_data();

        std::string operator [](std::string && s);
    private:
        void parse_raw();
        inline size_t set_req_type(std::string & s);
        inline size_t set_end_pt(std::string & s, size_t r);
        std::string data;
        EndPoint ep;
        size_t cookie_start;
        void load_cookies(); // TODO
        void load_individual_cookie(const std::string & s); // TODO
        PrefixTree<std::string> session;
};

#endif
