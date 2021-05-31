#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <signal.h>
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
static int pipefd[2];

int setnonblocking(int fd){
    int old = fcntl(fd, F_GETFL);
    int new_op = old | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_op);
    return old;
}

void addfd(int epollfd, int fd){
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    setnonblocking(fd);
}



/* signal handler function */
void sig_handler(int sig){
    /* save old errno, and recover in the end, keep rewritable*/
    int save_errno = errno;
    int msg = sig;
    send(pipefd[1], (char*)&msg, 1, 0);
    errno = save_errno;
}

void addsig(int sig){
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler  = sig_handler;
    sa.sa_flags |= SA_RESTART;
    sigfillset(&sa.sa_mask);
    assert(sigaction(sig, &sa, NULL) != -1);

}

int main(int argc, char* argv[]){
    if(argc <= 2){
        cout << "error in input\n";
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);

    SockAddr addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &addr.sin_addr);

    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(listen >= 0);
    int opt = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));

    int ret = bind(listenfd, (struct sockaddr*)&addr, sizeof(addr));
    assert(ret != -1);

    ret = listen(listenfd, 5);
    assert(ret != -1);

    epoll_event events[MAX_EVENT_NUMBER];
    int epollfd = epoll_create(5);
    assert(epollfd != -1);
    addfd(epollfd, listenfd);

    ret = socketpair(AF_UNIX, SOCK_STREAM, 0, pipefd);
    assert(ret != -1);

    setnonblocking(pipefd[1]);
    addfd(epollfd, pipefd[0]);


    addsig(SIGHUP);
    addsig(SIGCHLD);
    addsig(SIGTERM);
    addsig(SIGINT);

    bool stop_server = false;

    while(!stop_server){
        int num = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
        if((num < 0) && (errno != EINTR)){
            cout << "epoll failure\n";
            break;
        }

        for(int i = 0; i < num; i++){
            int sockfd = events[i].data.fd;
            if(sockfd == listenfd){
                SockAddr cliAddr;
                bzero(&cliAddr, sizeof(cliAddr));
                socklen_t len = sizeof(cliAddr);
                int connfd = accept(listenfd, (struct sockaddr*)&cliAddr, &len);
                addfd(epollfd, connfd);
            }else if((sockfd == pipefd[0]) && (events[i].events & EPOLLIN)){
                int sig;
                char signals[1024];
                ret = recv(pipefd[0], signals, sizeof(signals), 0);
                cout << "ret = " << ret << endl;
                cout << "recv:" << signals << endl;
                if(ret == -1){
                    continue;
                }else if(ret = 0){
                    continue;
                }else{
                    for(int i = 0; i < ret; i++){
                        cout << "signals[i]:" << signals[i] << endl;
                        switch(signals[i]){
                            case SIGCHLD:
                            case SIGHUP:{
                                continue;
                            }
                            case SIGTERM:
                            case SIGINT:{
                                cout << "coming here\n";
                                stop_server = true;
                            }

                        }
                    }    
                }
            }else{

            }
        }
    }
    cout << "close fds\n";
    close(listenfd);
    close(pipefd[0]);
    close(pipefd[1]);
}
























