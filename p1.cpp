#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
using namespace std;
int main()
{
    vector<pid_t> proID;
    auto pid1 = fork();
    if(pid1 == 0){
        cout << "pid1:" << getpid() << endl;
        for(int i = 0; i < 12; i++){
            //system("whoami");
        }
    }else{
        proID.push_back(pid1);
    }

    auto pid2 = fork();
    if(pid2 == 0){
        cout << "pid2:" << getpid() << endl;
    }else{
        proID.push_back(pid2);
    }
    
    auto pid3 = fork();
    if(pid3 == 0){
        cout << "pid3:" << getpid() << endl;
    }else{
        proID.push_back(pid3);
    }
    if(pid3 != 0){
        cout << "**********\n";
        for(int i  = 0; i < proID.size(); i++){
            cout << proID[i] << endl;
        }
    }
    
    return 0;
}