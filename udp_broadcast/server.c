#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<ctype.h>
#include<strings.h>
#include<arpa/inet.h>
#include<string.h>
#include<net/if.h>

#define CLIENT_PORT 9000
#define MAXLINE 1500
#define SERV_PORT 8000
#define BROADCAST_IP "172.16.191.255"

int main(void){
    int sockfd;
    char buf[MAXLINE] = "gg\n";
    struct sockaddr_in serv_addr,clin_addr;

    sockfd = socket(AF_INET,SOCK_DGRAM,0);

    bzero(&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(SERV_PORT);

    bind(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
    
    //设置socket选项
    int flag = 1;
    setsockopt(sockfd,SOL_SOCKET,SO_BROADCAST,&flag,sizeof(flag));

    //构造client地址
    bzero(&clin_addr,sizeof(clin_addr));
    clin_addr.sin_family = AF_INET;
    clin_addr.sin_port = htons(CLIENT_PORT);
    inet_pton(AF_INET,BROADCAST_IP,&clin_addr.sin_addr.s_addr);

    int i = 0;
    while(1){
        sprintf(buf,"itcast %d\n",i++);
        sendto(sockfd,buf,strlen(buf),0,(struct sockaddr *)&clin_addr,sizeof(clin_addr));
        sleep(1);
    }
    close(sockfd);
    return 0;
}
