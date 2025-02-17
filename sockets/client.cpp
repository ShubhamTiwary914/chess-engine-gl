#include<iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>
#include <netdb.h>
#include <cassert>

#define PORT 8080


void defineServerAddr(sockaddr_in *serverAddr, const char *hostname);


int main(int argc, char **argv){
    // conn
    int line = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddr;
    defineServerAddr(&serverAddr, "ec2-13-233-193-101.ap-south-1.compute.amazonaws.com");
    connect(line, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    //send
    const char* msg = "";
    if(argc > 1)
        msg = argv[1];
    send(line, msg, strlen(msg), 0);
    close(line);

    return 0;
}



void defineServerAddr(sockaddr_in *serverAddr, const char *hostname){
   struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(hostname, NULL, &hints, &res) != 0) {
        perror("getaddrinfo failed");
        exit(1);
    }

    // Copy resolved IP address
    memcpy(serverAddr, res->ai_addr, sizeof(struct sockaddr_in));
    serverAddr->sin_port = htons(PORT); // Set port
    freeaddrinfo(res);
}