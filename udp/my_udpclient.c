// UDP client

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

#define CHUNK 10
#define BUF_SIZE 101

int main(int argc, char*argv[]){
    int sockfd;
    struct sockaddr_in serv_addr;
    char buf[BUF_SIZE];
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("Client Unable to Create Socket");
        exit(0);
    }
    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8181);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    socklen_t len = sizeof(serv_addr);

    int fileid;
    if((fileid = open(argv[1], O_RDONLY)) < 0){
        perror("File not found\n");
        exit(0);
    }
    for (int i = 0; i < BUF_SIZE; i++)buf[i] = '\0';
    int filesz;
    if((filesz = read(fileid, buf, BUF_SIZE)) < 0){
        perror("File read error\n");
        exit(0);
    }
    int remsz = strlen(buf);
    int totalsent = 0;
    int sentsz;
    buf[remsz] = '\0';
    remsz++;
   while(remsz > 0){
      int send = CHUNK;
      if (send > remsz){
        send = remsz;
      }
       if ((sentsz = sendto(sockfd, buf + totalsent, send, 0, (const struct sockaddr*)&serv_addr,len)) < 0){
           perror("Error in send\n");
           exit(0);
       }
       remsz = remsz - sentsz;
       totalsent += sentsz;
   }
    if (sentsz < 0){
        perror("Error in send\n");
        exit(0);
    }
    printf("File sent from client\n");

    int byte_rec, tot_rec;
    for (int i = 0; i < BUF_SIZE; i++)buf[i] = '\0';
    for (int i = 0; i < BUF_SIZE; i++)buf[i] = '\0';
    printf("Receiving started ...\n");
   while(1){
       if((byte_rec = recvfrom(sockfd, buf + tot_rec, BUF_SIZE, 0, (struct sockaddr*)&serv_addr, &len)) <= 0){
           perror("Server Receive Error\n");
           exit(0);
       }
       tot_rec += byte_rec;
       if (buf[tot_rec - 1] == '\0'){
         break;
       }
   }
    printf("Received from server\n");
    printf("In file there are %s", buf);
    close(sockfd);
    return 0;

}
