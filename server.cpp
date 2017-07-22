#include "server.h"

Server::Server(const char * port): sock(std::string(port)){
}


void Server::run(){
   sock.listen();
   Socket * client = nullptr;
   sock.get_address();
   while(1){
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
    // TODO Just return the string from recieve ....
    std::string req(rd_buf, n);
    Request & request = *(new Request(req));
    handler_dispatch(*client, request);
}


void Server::handler_dispatch(Socket & client, Request & r){
    std::string route = r.get_end_pt();
    printf("Endpoint:%s\n", route.c_str());
    RouteHandler & f = routes.get(route);
    Response res = f(r);
    
    client.send(res.get_header());
   // std::cout << "sent header" << std::endl;
    client.send(res.str());
    std::cout << "sent body:" << std::endl;
    std::cout << res.str() << std::endl;
    client.close();
    delete &client;
    delete &r;
}


void Server::set_default_handler(RouteHandler rh){
    routes.set_default_data(rh);
}

