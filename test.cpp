#include "shrek++.h"
#include <iostream>

Response index_get(Request & r){
    auto res = Response(render_template("index.html"));
    res.set_session_var("test_var", "poop");
    return res;
}

int main(void){
    Server s("5000");
    s.set_default_handler([](Request &r){
        return Response("404 Not Found", HTTPStatus::NOT_FOUND);
    });
    s.add_route("/index.html", [](Request &r){
        std::cout << r.get_raw_data() << std::endl;
        return index_get(r);
    });
    s.run();
}
