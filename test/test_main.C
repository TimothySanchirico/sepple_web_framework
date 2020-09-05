#include <iostream>

#include "network.H"
#include "server.H"
#include "route_handler.H"
#include "http_request.H"
#include "http_response.H"

using namespace shrek;

int main() {
    auto t  = tcp_server(5000);
    auto rh = route_handler<http_request, http_response>();
    rh.add_endpoint("/index.html", [](const http_request& req) -> std::optional<http_response> {
                              std::cout << "received request" << std::endl;
                              return {http_response{"Hello World"}};
                          });
    auto s = make_server(t, rh);
    std::cout << "Running..." << std::endl;
    t.run();
}
