#ifndef SERVER_H
#define SERVER_H

#include <functional>
#include <memory>
#include <string>
#include <pthread.h>
#include <unistd.h>

#include "socket.h"
#include "request.h"
#include "response.h"
#include "prefix_tree.h"

typedef std::string & ResponsBody;

typedef std::function<Response(Request&)> RouteHandler;
typedef PrefixTree<RouteHandler> Routes;


class Server : public std::enable_shared_from_this<Server> {
    public:

        // Should probably make private
        Server(const char * port);

        template<typename... Ts>
            static std::shared_ptr<Server> create(Ts&&... params){
                    return std::make_shared<Server>(std::forward<Ts>(params)...);
                    }

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
            ClientInfo(std::shared_ptr<Request> r, std::shared_ptr<Socket> c, 
                    std::shared_ptr<Server> s): req(r), client(c), me(s){
            }
            std::shared_ptr<Server>         me;
            std::shared_ptr<Request>    req;
            std::shared_ptr<Socket>     client;
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

