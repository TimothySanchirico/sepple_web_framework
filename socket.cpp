#include "socket.h"


Socket::Socket(std::string port){
    struct addrinfo hints, *servinfo, *p;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family     = AF_UNSPEC;
    hints.ai_socktype   = SOCK_STREAM;
    hints.ai_flags      = AI_PASSIVE;
    int status; 
    if(status = getaddrinfo(nullptr, port.c_str(), &hints, &servinfo)){
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }
    size_t one = 1;

    for(p = servinfo; p; p = p->ai_next)
        if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) != -1
            && setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int)) != 1
            && bind(sockfd, p->ai_addr, p->ai_addrlen) != -1) break;

    if(p == nullptr){
        perror("Could not get a socket");
        exit(1);
    }

    freeaddrinfo(servinfo);
}


Socket::Socket(int fd, ClientAddress ca) : sockfd(fd), c(ca) {}

void Socket::listen(){
    if(::listen(sockfd, BACKLOG) == -1){
        perror("listen()::");
    }
}


Socket * Socket::accept(){
    ClientAddress c_addr;
    socklen_t size = sizeof(c_addr);
    int fd;
    Socket * conn = nullptr;
    if((fd = ::accept(sockfd, (struct sockaddr*) &c_addr, &size)) != 1){
        conn = new Socket(fd, c_addr); 
    }
    return conn;
}


std::pair<char *, size_t> Socket::recieve(void){
    char * buf = new char[2048];
    size_t n;
    n = recv(sockfd, buf, 2047, 0);
    buf[n] = '\0'; // pretty much useless
    return std::make_pair(buf, n); 
}


void Socket::send(std::string req){
    size_t t = req.size(), n = t;
    while(n -= ::send(sockfd, req.c_str()+(t-n), n, 0));
}


void Socket::close(){
    ::close(sockfd);
}


void Socket::get_address(){
    struct sockaddr_in sa;
    socklen_t sa_l = sizeof(sa);
    getsockname(sockfd, (struct sockaddr *) &sa, &sa_l);
    printf("Local IP: %s\n", inet_ntoa(sa.sin_addr));
    printf("Local port: %d\n", (int)ntohs(sa.sin_port));
}
