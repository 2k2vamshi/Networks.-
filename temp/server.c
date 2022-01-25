#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close

#define BACKLOG 5
#define BUF_SIZE 100
int main(){
    int sockfd, new_sockfd;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    char buf[BUF_SIZE];
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(20000);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    
    bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    
    listen(sockfd, BACKLOG);
    
    clilen = sizeof(struct sockaddr_in);
    new_sockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);
    
    recv(new_sockfd, buf, BUF_SIZE, 0);
    printf("%s\n", buf);
    
    strcpy(buf, "Connected to Server1");
    send(new_sockfd, buf, strlen(buf) + 1, 0);
    
    recv(sockfd, buf, BUF_SIZE, 0);
    printf("%s\n", buf);
}
