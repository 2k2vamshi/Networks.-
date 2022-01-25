// UDP server

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 100
#define CHUNK 15

int main(){
    int sockfd;
    struct sockaddr_in serv_addr, cli_addr;
    // Create Socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("Server Unable to Create Socket");
        exit(0);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    memset(&cli_addr, 0, sizeof(cli_addr));
    // storing Socket information
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8181);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    // binding socket to port
    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
        perror("Server Unable to bind");
        exit(0);
    }
    int tot_rec, byte_rec;
    socklen_t len;
    char buffer[BUF_SIZE];

    len = sizeof(cli_addr);
    printf("Receiving started ...\n");
    while(1){

      tot_rec = 0;
      for (int i = 0; i < BUF_SIZE; i++)buffer[i] = '\0';
      while(1){
          if((byte_rec = recvfrom(sockfd, buffer + tot_rec, BUF_SIZE, 0, (struct sockaddr*)&cli_addr, &len)) <= 0){
              perror("Server Receive Error\n");
              exit(0);
          }
          tot_rec += byte_rec;
          if (buffer[tot_rec - 1] == '\0'){
            break;
          }
      }
       if (byte_rec < 0){
           perror("Server Receive Error\n");
           exit(0);
       }
       printf ("Received from client: ");

       // computing
       int rec_len = strlen(buffer);
       int num_char = 0, num_sent = 0, num_word = 0;
       num_char = rec_len;
       for (int i = 0; i < rec_len; i++){
           if (buffer [i] == '.'){
               num_sent++;
           }
           if (buffer[i] == '\n'){
               num_char--;
           }
       }
       for (int i = 0; i < rec_len; i++){
           if (i != rec_len  ){
               if ((buffer[i] != ' ' && buffer[i] != '.') && (buffer[i+1] == ' ' || buffer[i+1] == '.')){
                               num_word++;
                           }
           }
       }
       printf("'%s'\n", buffer);
       char* no_char = "The number of characters are ";
       char* no_word = "The number of words are ";
       char* no_sent = "The number of sentences are ";
       char characters [100], words[100], sentences [100];
       sprintf(characters, "%d", num_char);
       sprintf(sentences, "%d", num_sent);
       sprintf(words, "%d", num_word);

       for(int i = 0; i < BUF_SIZE; i++){
           buffer[i] = '\0';
       }
       strcat(buffer, characters);
       strcat(buffer," Characters, ");
       strcat(buffer, words);
       strcat(buffer," Words, ");
       strcat(buffer, sentences);
       strcat(buffer," Sentences ");

      int remsz = strlen(buffer);
      int totalsent = 0;
      int sentsz;
      buffer[remsz] = '\0';
      remsz++;
     while(remsz > 0){
        int send = CHUNK;
        if (send > remsz){
          send = remsz;
        }
         if ((sentsz = sendto(sockfd, buffer + totalsent, send, 0, (const struct sockaddr*)&cli_addr,len)) < 0){
             perror("Error in send\n");
             exit(0);
         }
         remsz = remsz - sentsz;
         totalsent += sentsz;
     }
     printf("\nNext request:\n");
    }
    close(sockfd);
    return 0;
}
