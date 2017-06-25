#include <functional>
#include <string>

#include "socket.h"
#include "prefix_tree.h"

typedef std::string Request;
typedef std::string Response;

typedef std::function<Response(Request)> RouteHandler;
typedef PrefixTree<RouteHandler> Routes;


class Server {
    public:
        Server(const char * port);

        void run();

        void add_route(const char * route, RouteHandler rh){
            std::string r(route);
            routes.insert(r, rh);
        }

    private:
        void start_client_thread(Socket * client);

        Routes routes;

        Socket sock;

};
