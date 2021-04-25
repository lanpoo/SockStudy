#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <iostream>
#include <sys/epoll.h>
#include <thread>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
using namespace std;
using SockAddr = struct sockaddr_in;

constexpr int MAX_EVENT_NUMBER = 1024;
constexpr int BUFFER_SIZE = 10;


void setnonblocking(int fd){
    int old_option = fcntl(fd, F_GETFL);
    fcntl(fd, F_SETFL, old_option | O_NONBLOCK);
}

void addfd(int epollfd, int fd, bool enable_et){
    epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = fd;
    if(enable_et){
        event.events |= EPOLLET;
    }

    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);

}

void lt(epoll_event* events, int number, int epollfd, int listenfd){
    char buf[BUFFER_SIZE];
    for(int i = 0; i < number; i++){
        int sockfd = events[i].data.fd;
        if(sockfd == listenfd){                 // have clent want to connect
            cout << "sockfd == listenfd\n";
            SockAddr cliAddr;
            socklen_t len = sizeof(cliAddr);
            int connfd = accept(sockfd, (struct sockaddr*)&cliAddr, &len);
            addfd(epollfd, connfd, false);
        }else if(events[i].events & EPOLLIN){
            /* If there are some data have not read from socket read buffer, the code will be run*/
            cout << "event trigger once\n";
            memset(buf, '\0', BUFFER_SIZE);
            int ret = recv(sockfd, buf, BUFFER_SIZE - 1, 0);
            if(ret <= 0){
                cout << "ret = " << ret << endl;
                close(sockfd);
                continue;
            }
            cout << "get " << ret << " bytes of content: " << buf << endl;
        }
    }
}


int main(int argc, char* argv[]){
    if(argc <= 1){
        cout << "usage: ip_address port_number\n";
        return 1;
    }

    const char* ip = argv[1];
    int port = atoi(argv[2]);
    int ret = 0;

    SockAddr addr;
    bzero(&addr, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &addr.sin_addr);

    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(listenfd >= 0);

    ret = bind(listenfd, (struct sockaddr*)&addr, sizeof(addr));
    assert(ret != -1);

    ret = listen(listenfd, 5);
    assert(ret != -1);

    epoll_event events[MAX_EVENT_NUMBER];
    int epollfd = epoll_create(5);
    assert(epollfd != -1);

    addfd(epollfd, listenfd, true);

    while(1){
        int ret = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
        cout << "ret = " << ret << endl;
        if(ret < 0){
            cout << "epoll failure\n";
            break;
        }
        lt(events, MAX_EVENT_NUMBER, epollfd, listenfd);

    }

}