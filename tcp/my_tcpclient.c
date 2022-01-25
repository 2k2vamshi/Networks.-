#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close

#define CHUNK 20
#define BUF_SIZE 100
int main(int argc, char*argv[]){
    int sockfd;
    struct sockaddr_in serv_addr;
    char buf[BUF_SIZE];
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0){
        perror("Cannot Create Socket\n");
        exit(0);
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(20000);
    inet_aton("127.0.0.1",&serv_addr.sin_addr) ;
    int fileid;
    if((fileid = open(argv[1], O_RDONLY)) < 0){
        perror("File not found\n");
        exit(0);
    }
    if((connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(struct sockaddr_in)))<0){
        perror("Cannot Connect to server \n");
        exit(0);
    }

    for (int i = 0; i < BUF_SIZE; i++)buf[i] = '\0';
//    if(recv(sockfd, buf, BUF_SIZE, 0) < 0){
//        perror("Client Receive error\n");
//        exit(0);
//    }
//    printf("%s\n", buf);
    printf("Sending File ...\n");

    for (int i = 0; i < BUF_SIZE; i++)buf[i] = '\0';
    int filesz;
    if((filesz = read(fileid, buf, BUF_SIZE)) < 0){
        perror("File read error\n");
        exit(0);
    }
    int remsz = strlen(buf);
    int totalsent = 0;
    int sentsz;
    while(remsz > 0){
//        int sendsz = CHUNK;
//        if (remsz < sendsz){
//            sendsz = remsz;
//        }
        if((sentsz = send(sockfd, buf + totalsent, CHUNK, 0)) < 0){
            perror("Error in Sent\n");
            exit(0);
        }
        remsz = remsz - sentsz;
        totalsent += sentsz;
    }
    close(sockfd);

    //creating new socket and connect

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0){
        perror("Cannot Create Socket\n");
        exit(0);
    }
    if((connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(struct sockaddr_in)))<0){
        perror("Cannot Connect to server \n");
        exit(0);
    }

    // reciving the calculation from server

    // for (int i = 0; i < BUF_SIZE; i++)buf[i] = '\0';
    // int byte_rec;
    // socklen_t len = sizeof(serv_addr);
    // while((byte_rec = recv(sockfd, buf, BUF_SIZE,0)) > 0){
    //     if (byte_rec < 0){
    //         perror("Client Receive error\n");
    //         exit(0);
    //     }
    // }
   for (int i = 0; i < BUF_SIZE; i++)buf[i] = '\0';
   int byte_rec = 0;
   int tot_rec = 0;
   recv(sockfd, buf + tot_rec, BUF_SIZE -tot_rec, 0);
   while((byte_rec = recv(sockfd, buf , BUF_SIZE , 0)) > 0 ){
       if(byte_rec < 0){
           perror("Client Receive error\n");
           exit(0);
       }
       tot_rec += byte_rec;
       printf(" %d %d\n", byte_rec, tot_rec);
   }
    printf("\n Recieved from server \n%s\n", buf);
    close(sockfd);
}
