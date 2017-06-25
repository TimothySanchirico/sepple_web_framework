#include "shrek++.h"
#include <iostream>


int main(void){
        Server s("5000");
        s.add_route("/index.html", [](std::string r){
                return std::string("HTTP/1.1 200 OK\r\n\r\nHello World!\n"); 
        });
        s.run();

}
