#include "shrek++.h"
#include <iostream>

Response index_get(Request & r){
    return Response(render_template("index.html"));
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
