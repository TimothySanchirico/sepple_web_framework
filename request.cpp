#include "request.h"

namespace HTTPRequest {
   RequestType OPTIONS  = 556;
   RequestType GET      = 224;
   RequestType HEAD     = 274;
   RequestType POST     = 326;
   RequestType PUT      = 249;
   RequestType DELETE   = 435;
   RequestType TRACE    = 367;
   RequestType CONNECT  = 522;
}


Request::Request(std::string&& s){
    data = s;
    parse_raw();
    load_cookies();
    session.set_default_data("None");
}


void Request::parse_raw(){
    size_t t = set_req_type(data);
    t = set_end_pt(data, t);
    cookie_start = data.find("Cookie:");
}


inline size_t Request::set_req_type(std::string  & s){
    size_t _r = s.find_first_of(" ", 0);
    std::string t = s.substr(0, _r);
    type = type_string_hash(t);
    return _r;
}


// r is the location of the first space
inline size_t Request::set_end_pt(std::string & s, size_t _r){
    size_t _l = s.find_first_of(" ", _r+1);
    // +/-1 to ditch the spaces on either side
    ep = s.substr(_r+1, (_l - _r-1));
    return _l;
}


EndPoint & Request::get_end_pt(){ return ep; }


std::string & Request::get_raw_data(){
    return data;
}

void Request::load_cookies(){
    std::string regex_str("Cookie: (.*)=(.*)\r\n");
    std::regex rg(regex_str);
    std::sregex_iterator it(data.begin(), data.end(), rg), it_end;
    while(it != it_end){
        load_individual_cookie((*it).str());
        it++;
    }
}

void Request::load_individual_cookie(const std::string & cookie){
    size_t s = std::string("Cookie: ").size();
    size_t mid = cookie.find_first_of('=', s);
    std::string name = cookie.substr(s, mid-s);
    size_t end = cookie.find_first_of('\r', mid);
    std::string value = cookie.substr(mid+1, end-mid-1);
    // TODO decrypt
    session.insert(name, value);
}

std::string Request::operator [](std::string && s){
    return session.get(s);
}


Request::~Request(){

}
