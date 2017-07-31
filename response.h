#ifndef RESPONSE_H
#define RESPONSE_H

#include <string>
#include <cstdio>
#include <iostream>
#include <fstream>

typedef unsigned short statusCode;
typedef std::string statusMessage;
typedef size_t statusIndex;

namespace HTTPStatus {
    extern const statusCode OK;
    extern const statusCode NOT_FOUND;
    extern const statusCode INTERNAL_ERR;
}

inline std::string render_template(const char * fname) {
    std::ifstream f(fname);
    std::string s((std::istreambuf_iterator<char>(f)),
        std::istreambuf_iterator<char>());
    return s;
}

class Response {
    public:
        Response(char const * s, statusCode status=200);
        Response(std::string s, statusCode status=200);

        std::string & str();

        std::string & get_header();

        ~Response(){}
    private:
        void set_status_message();
        statusMessage status_message;
        statusCode status_code;
        std::string data;
        std::string header;
};

#endif

