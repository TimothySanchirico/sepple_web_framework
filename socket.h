
#ifndef SOCKET_H
#define SOCKET_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>

#include <utility>
#include <tuple>
#include <string>

typedef struct sockaddr_in ClientAddress;
class Socket {
    public:
        Socket(std::string&& port);

        Socket(int fd, ClientAddress client);
        
        void send(std::string res);

        std::string recieve();

        Socket* accept();

        void listen();
        
        void close();

        void get_address();

    private:
        
        int sockfd;
        
        int BACKLOG = 128;

        ClientAddress c;

};

#endif
