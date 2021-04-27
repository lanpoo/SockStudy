#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <thread>
#include <iostream>

using namespace std;
using SockAddr = struct sockaddr_in;

constexpr int MAX_EVENT_NUMBER = 1024;
constexpr int BUFFER_SIZE = 1024;

struct fds{
    int epollfd;
    int sockfd;
};

int setnonblocking(int fd){
    auto old = fcntl(fd, F_GETFL);
    fcntl(fd, F_SETFL, old | O_NONBLOCK);
    return old;
}

void addfd(int epollfd, int fd, bool oneshot){
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    if(oneshot){
        event.events |= EPOLLONESHOT;
    }
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    setnonblocking(fd);
}

void reset_oneshot(int epollfd, int fd){
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLONESHOT | EPOLLET;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);
}

void* worker(void* arg){
    int sockfd = ((fds*)arg)->sockfd;
    int epollfd = ((fds*)arg)->epollfd;
    cout << "start new thread to receive data on fd:" << sockfd << endl;
    char buf[BUFFER_SIZE];
    memset(buf, '\0', sizeof(buf));
    while(1){
        int ret = recv(sockfd, buf, BUFFER_SIZE - 1, 0);
        if(ret == 0){
            close(sockfd);
            cout << "client closed the connection\n";
            break;
        }else if(ret < 0){
            if(errno == EAGAIN){
                reset_oneshot(epollfd, sockfd);
                cout << "read later\n";
                break;
            }
        }else{
            cout << "get content:" << buf << endl;
            sleep(5);
        }
    }
    cout << "end thread receiving data on fd:" << sockfd << endl;
}


int main(int argc, char* argv[]){
    if(argc <= 2){
        cout << "error\n";
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

}