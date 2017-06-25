#include "server.h"

Server::Server(const char * port): sock(std::string(port)){
}


void Server::run(){
   sock.listen();
   Socket * client = nullptr;
   while(420){
        if((client= sock.accept())){
            start_client_thread(client);
        }
   }
}

void Server::start_client_thread(Socket * client){
    // Create Request;
    char * rd_buf;
    size_t n;
    std::tie(rd_buf, n) = client->recieve();
    std::string req(rd_buf, n);
    size_t l, r;
    l = req.find_first_of(' ', 0);
    r = req.find_first_of(' ', l+1);
    const std::string route = req.substr(l+1, (r-l-1));
    std::cout << route  << std::endl;
    RouteHandler f = routes.get(route);
    client->send(f(std::string("Hi")));
    client->close();

}

