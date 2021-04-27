#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
using namespace std;

using SockAddr = struct sockaddr_in;

int main(int argc, char* argv[]){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        cout << "error in sockfd\n";
    }
    SockAddr addr;
    int port = atoi(argv[1]);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = 0;
    int res = connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    if(res < 0){
        cout << "error in conncet\n";
    }
    while(true){
        string buf; 
        cin >> buf;
        if(buf == "abc"){
            break;
        }
        if(send(sockfd, buf.c_str(), buf.size(), 0) < 0){
            cout << "error in send\n";
            return 0;
        }
    }
    
    close(sockfd);
    return 0;
}