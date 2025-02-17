#include<iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <cassert>



#define PORT 8080


void defineServerAddr(sockaddr_in *serverAddr);




int main(){
    //address, socket set & bind
    int line = socket(AF_INET, SOCK_STREAM, 0);
    assert(line != -1);
    sockaddr_in serverAddr;
    defineServerAddr(&serverAddr);
    bind(line, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    //listen & accept
    listen(line, 10);
    while (true){
        int conn = accept(line, nullptr, nullptr);
        char buffer[1024] = {0};
        recv(conn, buffer, sizeof(buffer), 0);
        std::cout << "Message from client: " << buffer << std::endl;
    }
    
    close(line);
    return 0;
}



void defineServerAddr(sockaddr_in *serverAddr){
    (*serverAddr).sin_family = AF_INET;
    (*serverAddr).sin_port = htons(PORT);
    (*serverAddr).sin_addr.s_addr = INADDR_ANY;
}