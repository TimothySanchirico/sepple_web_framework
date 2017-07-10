#ifndef SERVER_H
#define SERVER_H

#include <functional>
#include <string>

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

    private:
        void start_client_thread(Socket * client);

        void handler_dispatch(Socket & client, Request & r);

        Request get_HTTP_request(Socket * client);

        Routes routes;

        Socket sock;

};

#endif

