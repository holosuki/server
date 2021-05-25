#include<stdio.h>
#include<netinet/in.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<net/if.h>

#define MAXLINE 4096
#define SERV_PORT 8000
#define CLIENT_PORT 9000

int main(int argc,char *argv[]){
    int confd;
    char buf[MAXLINE];
    struct sockaddr_in serveraddr,localaddr;
    ssize_t len;

    confd = socket(AF_INET,SOCK_DGRAM,0);

    bzero(&localaddr,sizeof(localaddr));
    localaddr.sin_family = AF_INET;
    localaddr.sin_port = htons(CLIENT_PORT);
    inet_pton(AF_INET,"0.0.0.0",&localaddr.sin_addr.s_addr);

    bind(confd,(struct sockaddr *)&localaddr,sizeof(localaddr));

    socklen_t slen;
    while(1){
        slen = sizeof(serveraddr);
        len = recvfrom(confd,buf,sizeof(buf),0,(struct sockaddr *)&serveraddr,&slen);
        printf("%s",buf);
    }
    close(confd);
    return 0;
}
