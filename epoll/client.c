#include<stdio.h>
#include<unistd.h>
#include<netinet/in.h>
#include<string.h>
#include"wrap.h"

#define MAXLINE 80
#define SERV_PORT 8000

int main(int argc,char *argv[]){
    int sockfd,n;
    char buf[MAXLINE];
    struct sockaddr_in servaddr;

    sockfd = Socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr.s_addr);

    Connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

    while(fgets(buf,MAXLINE,stdin) != NULL){
        Write(sockfd,buf,strlen(buf));
        n = Read(sockfd,buf,MAXLINE);
        if(n == 0)
            printf("the other side has been closed.\n");
        else
            Write(STDOUT_FILENO,buf,n);
    }
    Close(sockfd);
    return 0;
}
