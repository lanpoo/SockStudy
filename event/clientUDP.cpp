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
#include <thread>

using namespace std;
using SockAddr = struct sockaddr_in;



void fuck(){
    cout << "nmsl" << endl;
}
int main(int argv, char* argc[]){
    thread t(fuck);
    t.detach();
    if(argv < 2){
        cout << "error in argv\n";
        return 1;
    }
    const char* ip = argc[1];
    int port = atoi(argc[2]);
    SockAddr serAddr;
    bzero(&serAddr, sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &serAddr.sin_addr);

    int ret = 0;
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    assert(sockfd >= 0);

    
    char buf[1024];
    while(1){
        string str;
        cin >> str;
        ret = sendto(sockfd, str.c_str(), str.size(), 0, (struct sockaddr*)&serAddr, sizeof(serAddr));
        assert(ret >= 0);
        memset(buf, '\0', 1024);
        socklen_t len = sizeof(serAddr);
        recvfrom(sockfd, buf, 1023, 0, (struct sockaddr*)&serAddr, &len);
        cout << "recv:" << buf << endl;
    }

    close(sockfd);
    
}