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

//组名
#define GROUP "239.0.0.2"

int main(void){
    int sockfd;
    char buf[MAXLINE] = "gg\n";
    struct sockaddr_in serv_addr,clin_addr;
    struct ip_mreqn group;

    sockfd = socket(AF_INET,SOCK_DGRAM,0);

    bzero(&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(SERV_PORT);

    bind(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
    
    //设置组地址
    inet_pton(AF_INET,GROUP,&group.imr_multiaddr);
    //本地任意IP
    inet_pton(AF_INET,"0.0.0.0",&group.imr_address);
    group.imr_ifindex = if_nametoindex("eth0");

    //设置组播
    setsockopt(sockfd,IPPROTO_IP,IP_MULTICAST_IF,&group,sizeof(group));

    //构造client地址
    bzero(&clin_addr,sizeof(clin_addr));
    clin_addr.sin_family = AF_INET;
    clin_addr.sin_port = htons(CLIENT_PORT);
    inet_pton(AF_INET,GROUP,&clin_addr.sin_addr.s_addr);

    int i = 0;
    while(1){
        sprintf(buf,"itcast %d\n",i++);
        sendto(sockfd,buf,strlen(buf),0,(struct sockaddr *)&clin_addr,sizeof(clin_addr));
        sleep(1);
    }
    close(sockfd);
    return 0;
}
