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
using SockAddr = struct sockaddr_in;

int main(int argv, char* argc[]){
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
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(sockfd >= 0);

    ret = connect(sockfd, (struct sockaddr*)&serAddr, sizeof(serAddr));
    assert(ret != -1);
    char buf[1024];
    while(1){
        string str;
        cin >> str;
        ret = send(sockfd, str.c_str(), str.size(), 0);
        assert(ret >= 0);
        memset(buf, '\0', 1024);
        ret = recv(sockfd, buf, 1024-1, 0);
        cout << "recv:" << buf << endl;
    }

    close(sockfd);
    
}