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
    rh.add_endpoint("/index.html", [](const http_request& req) {
                              auto&& r =  http_response{"Hello World"};
                              r.set_cookie("test_cookie", "test_value");
                              r.set_meta("Content-Encoding", "gzip");
                              return r;
                          });
    rh.add_endpoint("/index2.html", [](const http_request& req) {
                              return response_from_template("index.html");
                          });
    auto s = make_server(t, rh);
    std::cout << "Running..." << std::endl;
    t.run();
}
