// TCP server

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close

#define BACKLOG 5
#define BUF_SIZE 100
#define CHUNK 30
int main(){
	int sockfd, new_sockfd;
    socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
    char buffer[BUF_SIZE];
    int i;
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0){
		perror("Cannot Create Socket \n");
		exit(0);
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(20000);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	memset(&(serv_addr.sin_zero), '\0', 8);
	if(bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
		perror("Cannot bind to port \n");
		exit(0);
	}
	listen(sockfd, BACKLOG);

//    while(1){
//        clilen = sizeof(struct sockaddr_in);
//        new_sockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);
//        if(new_sockfd < 0){
//            perror("Accept error \n");
//            exit(0);
//        }
//    }





	while(1){
		clilen = sizeof(struct sockaddr_in);
        new_sockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);
        if(new_sockfd < 0){
            perror("Accept error \n");
            exit(0);
        }
    //    strcpy(buffer, "Connected to Server");
    //    send(new_sockfd, buffer, strlen(buffer) + 1, 0);
    //    printf("Sent connection message to client");

        //receving
        int byte_rec = 0;
        int tot_rec = 0;
        for(int i = 0; i < BUF_SIZE; i++){
            buffer[i] = '\0';
        }
        while((byte_rec = recv(new_sockfd, buffer + tot_rec, BUF_SIZE - tot_rec, 0)) > 0){
            if(byte_rec < 0){
                perror("Server Receive error\n");
                exit(0);
            }
            tot_rec += byte_rec;
        }
            printf("Received : '%s'\n", buffer);
        printf("Received Complete File\n");
        // printf("%s", buffer);
    //    close(new_sockfd);
    //    new_sockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);
    //    recv(new_sockfd, buffer, BUF_SIZE, 0);
    //    printf("%s\n", buffer);

        // calculation of number of words and charcters

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
    // //    printf("%s\n", buffer);
    // //    printf("%d %d %d", num_char, num_word, num_sent);
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
        buffer[strlen(buffer)] = '\0';
        close(new_sockfd);

        //returning the calculation
        clilen = sizeof(struct sockaddr_in);
        new_sockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);
        if(new_sockfd < 0){
            perror("Accept error \n");
            exit(0);
        }

        int remsz = strlen(buffer) + 1;
        int totalsent = 0;
        int sentsz;
        // send(new_sockfd, buffer + totalsent, remsz, 0);
        // printf("\n%d", remsz);
        printf("Sending ...\n");
        while(remsz > 0){
            if((sentsz = send(new_sockfd, buffer + totalsent, remsz, 0)) <= 0){
                perror("Error in Sent\n");
                exit(0);
            }
            remsz = remsz - sentsz;
            totalsent += sentsz;
        }
        printf("Sent\n");
            printf("New request:\n");
            close(new_sockfd);
	}
		close(sockfd);
    return 0;
}
