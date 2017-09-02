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
    ClientInfo * ci = new ClientInfo(new Request(client->recieve()), client, this);
    pthread_t client_thread;
    pthread_create(&client_thread,
        nullptr, &Server::handler_dispatch, (void*)ci);
    pthread_detach(client_thread); // Let it run loose and clean itself
}



void *Server::handler_dispatch(void * void_ci){
    std::unique_ptr<ClientInfo> ci((ClientInfo *)void_ci);
    Server & server = *(ci->me);
    Request & req   = *(ci->req);
    Socket & client = *(ci->client);
    std::string route = req.get_end_pt();
    RouteHandler & f = server.get_route(route);
    Response res = f(req);
    std::string client_string = client.get_client_address();
    printf("[%s]: %s -- %s\n", res.get_status_message().c_str(), client_string.c_str(), route.c_str());
    client.send(res.get_header());
    client.send(res.str());
    client.close();
    pthread_exit(NULL);
}


void Server::set_default_handler(RouteHandler rh){
    routes.set_default_data(rh);
}

