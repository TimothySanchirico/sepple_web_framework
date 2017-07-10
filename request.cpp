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


Request::Request(std::string s){
    parse_raw(s);
}


void Request::parse_raw(std::string & s){
    size_t t = set_req_type(s);
    t = set_end_pt(s, t);
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
}


EndPoint & Request::get_end_pt(){ return ep; }


Request::~Request(){

}
