
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
#include <pthread.h>
#include <iostream>


using namespace std;
constexpr int MAX_EVENT_NUMBER = 1024;
constexpr int TCP_BUFFER_SIZE = 512;
constexpr int UDP_BUFFER_SIZE = 1024;

using SockAddr = struct sockaddr_in;

int setnonblocking(int fd){
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

void addfd(int epollfd, int fd){
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    setnonblocking(fd);
}

int main(int argc, char* argv[]){
    if(argc < 2){
        cout << "error in argv\n";
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
    int val = true;
    
    // create TCP socket, bind to port

    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(listenfd > 0);

    if(setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,(char *)&val,sizeof(val))!=0) 
    { 
        printf("error!setsockopt failed!\n"); 
        return -1; 
    }

    ret = bind(listenfd, (struct sockaddr*)&addr, sizeof(addr));
    assert(ret != -1);

    ret = listen(listenfd, 5);
    assert(ret != -1);

    // create UDP socket, bind to port

    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &addr.sin_addr);

    int udpfd = socket(AF_INET, SOCK_DGRAM, 0);
    assert(udpfd >= 0);

    ret = bind(udpfd, (struct sockaddr*)&addr, sizeof(addr));
    assert(ret != -1);

    epoll_event events[MAX_EVENT_NUMBER];
    int epollfd = epoll_create(5);
    assert(epollfd != -1);

    addfd(epollfd, listenfd);
    addfd(epollfd, udpfd);

    while(1){
        int number = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
        if(number < 0){
            cout << "epoll failure\n";
            break;
        }

        for(int i = 0; i < number; i++){
            int sockfd = events[i].data.fd;
            if(sockfd == listenfd){
                
                SockAddr cliAddr;
                socklen_t len = sizeof(cliAddr);
                int connfd = accept(sockfd, (struct sockaddr*)&cliAddr, &len);
                addfd(epollfd, connfd);
            }else if(sockfd == udpfd){
                cout << "UDP:\n";
                char buf[UDP_BUFFER_SIZE];
                memset(buf, '\0', UDP_BUFFER_SIZE);
                SockAddr cliAddr;
                socklen_t len = sizeof(cliAddr);
                ret = recvfrom(sockfd, buf, UDP_BUFFER_SIZE - 1, 0, (struct sockaddr*)&cliAddr, &len);
                if(ret > 0){
                    sendto(udpfd, buf, UDP_BUFFER_SIZE - 1, 0, (struct sockaddr*)&cliAddr, len);
                }
            }else if(events[i].events & EPOLLIN){
                cout << "TCP:\n";
                char buf[TCP_BUFFER_SIZE];
                while(1){
                    memset(buf, '\0', TCP_BUFFER_SIZE);
                    ret = recv(sockfd, buf, TCP_BUFFER_SIZE - 1, 0);
                    cout << "ret = " << ret << endl;
                    if(ret < 0){
                        if((errno == EAGAIN) || (errno == EWOULDBLOCK)){
                            cout << "coming if\n";
                            break;
                        }
                        close(sockfd);
                        break;
                    }else if(ret == 0){
                        close(sockfd);
                    }else{
                        send(sockfd, buf, ret, 0);
                    }
                }
            }else{
                cout << "something else happend\n";
            }
        }
    }
    close(listenfd);
    return 0;
}






























