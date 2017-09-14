#include "response.h"

namespace HTTPStatus {
    const statusCode OK              = 200;
    const statusCode NOT_FOUND       = 404;
    const statusCode INTERNAL_ERR    = 500;
}

Response::Response(char const * s, statusCode status)
: data(s), status_code(status){
    set_status_message();
}

Response::Response(std::string s, statusCode status)
: data(s), status_code(status){
    set_status_message();
}

std::string & Response::str(){
    return data;
}

std::string & Response::get_header(){
    char buf[1024];
    size_t n = std::sprintf(buf, "HTTP/1.1 %s\r\n", status_message.c_str());
    for(auto & cookie : cookies){
        n += std::sprintf(buf + n, "Set-Cookie: %s=%s\r\n",
                cookie.name.c_str(), cookie.value.c_str());
    }
    n += std::sprintf(buf + n, "\r\n");
    header=std::string(buf, n);
    return header;
}

void Response::set_status_message(){
    switch(status_code){
        case(HTTPStatus::OK):
            status_message = std::string("200 OK");
            break;
        case(HTTPStatus::INTERNAL_ERR):
            status_message = std::string("500 INTERNAL");
            break;
        case(HTTPStatus::NOT_FOUND):
            status_message = std::string("404 NOT FOUND");
            break;
    }
}

void Response::set_session_var(std::string name, std::string value){
    cookies.push_back(Cookie(name, value));
}
