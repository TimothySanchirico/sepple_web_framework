#ifndef SERVER_H
#define SERVER_H

#include <functional>
#include <string>
#include <pthread.h>

#include "socket.h"
#include "request.h"
#include "response.h"
#include "prefix_tree.h"

typedef std::string & ResponsBody;

typedef std::function<Response(Request&)> RouteHandler;
typedef PrefixTree<RouteHandler> Routes;


class Server {
    public:
        Server(const char * port);

        void run();

        void add_route(const char * route, RouteHandler rh){
            std::string r(route);
            routes.insert(r, rh);
        }

        void set_default_handler(RouteHandler rh);

        RouteHandler & get_route(std::string route){
            return routes.get(route);
        }

        struct ClientInfo {
            ClientInfo(Request * r, Socket * c, Server * s): req(*r), client(*c){
                me = s;
            }
            ~ClientInfo(){
                delete &client;
                delete &req;
            }
            Server * me; // pointer to the server
            Request & req;
            Socket & client;
        };

        static void *handler_dispatch(void * ci);

    private:
        pthread_t dispatch_thread;

        void start_client_thread(Socket * client);

        Request get_HTTP_request(Socket * client);

        Routes routes;

        Socket sock;

};




#endif

