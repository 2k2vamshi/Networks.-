
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
int main(int argc, char*argv[]){
    char buff[100];
    for (int i = 0; i < 100; i++)buff[i] = '\0';
    int fileid = open(argv[1], O_RDONLY);
    if (fileid < 0){
        perror("File not found\n");
        exit(0);
    }
    int sz ;
    if((sz = read(fileid, buff, 100)) < 0){
        perror("File not read properly\n");
        exit(0);
    }
    printf("%s",buff);
    close(fileid);
}
