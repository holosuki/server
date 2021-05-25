#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<ctype.h>
#include<strings.h>
#include<arpa/inet.h>
#include<string.h>

#define SERV_PORT 8000

int main(void){
    int sfd;
    char buf[1024],clin_ip[128];
    socklen_t addrlen;
    struct sockaddr_in serv_addr,clin_addr;
    int i,len;

    sfd = socket(AF_INET,SOCK_DGRAM,0);

    bzero(&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(SERV_PORT);

    bind(sfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));

    while(1){
        addrlen = sizeof(clin_addr);
        len = recvfrom(sfd,buf,sizeof(buf),0,(struct sockaddr *)&clin_addr,&addrlen);
        printf("client IP:%s PORT:%d\n",
                inet_ntop(AF_INET,&clin_addr.sin_addr.s_addr,clin_ip,sizeof(clin_ip)),
                ntohs(clin_addr.sin_port));
        for(i = 0 ; i < len ; i++)
            buf[i] = toupper(buf[i]);
        sendto(sfd,buf,len,0,(struct sockaddr *)&clin_addr,addrlen);
    }
    close(sfd);

    return 0;
}
