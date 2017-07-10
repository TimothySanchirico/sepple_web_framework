#include "shrek++.h"
#include <iostream>

Response index_get(Request & r){
    return Response("GET REQUEST HELLO WORLD!");
}

int main(void){
        Server s("5000");
        s.set_default_handler([](Request &r){
                return Response("404 Not Found", HTTPStatus::NOT_FOUND);
        });
        s.add_route("/index.html", [](Request &r){
                if(r.type == HTTPRequest::GET)
                    return index_get(r);
                else
                    return Response("Hello World!");
        });
        s.run();

}
