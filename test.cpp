#include "shrek++.h"
#include <iostream>

Response index_get(Request & r){
    auto res = Response(render_template("index.html"));
    std::cout << r["test_var"] << std::endl;
    res.set_session_var("test_var", "test_val");
    return res;
}

int main(void){
    auto s = Server::create("5000");
    s->set_default_handler([](Request &r){
        return Response("404 Not Found", HTTPStatus::NOT_FOUND);
    });
    s->add_route("/index.html", [](Request &r){
        return index_get(r);
    });
    s->run();
}
