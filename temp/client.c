#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close

#define BUF_SIZE 100
int main(){
    int sockfd;
    struct sockaddr_in serv_addr;
    char buf[BUF_SIZE];
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(20000);
    inet_aton("127.0.0.1",&serv_addr.sin_addr) ;
    
    connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(struct sockaddr_in));
    
    strcpy(buf, "Connected to Client1");
    send(sockfd, buf, strlen(buf) + 1, 0);
    
    recv(sockfd, buf, BUF_SIZE, 0);
    printf("%s\n", buf);
    
    strcpy(buf, "Connected to Client2");
    send(sockfd, buf, strlen(buf) + 1, 0);
}
