#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<ctype.h>
#include<strings.h>
#include<arpa/inet.h>
#include<string.h>

#define SERV_PORT 8000

int main(int argc,char *argv[]){
    int sfd;
    char buf[1024];
    socklen_t addrlen;
    struct sockaddr_in serv_addr;
    int i,len;

    sfd = socket(AF_INET,SOCK_DGRAM,0);

    bzero(&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET,"127.0.0.1",&serv_addr.sin_addr.s_addr);

    while(1){
        fgets(buf,sizeof(buf),stdin);
        addrlen = sizeof(serv_addr);
        sendto(sfd,buf,strlen(buf),0,(struct sockaddr *)&serv_addr,addrlen);
        len = recvfrom(sfd,buf,sizeof(buf),0,NULL,0);
        buf[len] = '\0';
        printf("%s",buf);
    }
    close(sfd);

    return 0;
}
