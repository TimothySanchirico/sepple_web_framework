#include "socket.h"

int main(void) {
        std::string port("5000");
        Socket s(port);
        s.listen();
        Socket * c = nullptr;
        char * buf;
        size_t n;
        while(420){
                if((c = s.accept())){
                        std::tie(buf, n) = c->recieve();
                        for(size_t i = 0; i < n; i++){
                                printf("%c", buf[i]);
                        }
                        delete buf;
                }
        }

}
