#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<poll.h>
#include<errno.h>
#include"wrap.h"

#define MAXLINE 80
#define SERV_PORT 8000
#define OPEN_MAX 1024

int main(int argc,char *argv[]){
    int i,j,maxi,listenfd,connfd,sockfd;
    int nready;
    ssize_t n;
    char buf[MAXLINE],str[INET_ADDRSTRLEN];
    socklen_t clilen;
    struct pollfd client[OPEN_MAX];
    struct sockaddr_in cliaddr,servaddr;

    listenfd = Socket(AF_INET,SOCK_STREAM,0);

    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    int opt = 1;
    setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

    Bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

    Listen(listenfd,20);

    client[0].fd = listenfd;
    client[0].events = POLLIN;//listenfd监听普通读事件
    
    for(i = 1 ; i < OPEN_MAX ; i++)
        client[i].fd = -1;//用-1初始化剩下元素
    maxi = 0;//client[]中最大有效元素下标
    for(;;){
        nready = poll(client,maxi + 1,-1);
        //阻塞，直到有客户端连接
        if(client[0].revents & POLLIN){//位与，注意一下
            clilen = sizeof(cliaddr);
            connfd = Accept(listenfd,(struct sockaddr *)&cliaddr,&clilen);
            printf("receive from %s at PORT %d\n",
                    inet_ntop(AF_INET,&cliaddr.sin_addr,str,sizeof(str)),
                    ntohs(cliaddr.sin_port));
            for(i = 1 ; i < OPEN_MAX ; i++){
                if(client[i].fd < 0){
                    client[i].fd = connfd;//找到空闲位置存放accept返回的connfd
                    break;
                }
            }
            if(i == OPEN_MAX)
                perr_exit("too many clients");
            client[i].events = POLLIN;
            if(i > maxi)
                maxi = i;
            if(--nready <= 0)
                continue;
        }
        for(i = 1 ; i <= maxi ; i++){
            //检测client[]
            if((sockfd =  client[i].fd) < 0)
                continue;
            if(client[i].revents & (POLLIN)){
                if((n = Read(sockfd,buf,MAXLINE)) < 0){
                    if(errno == ECONNRESET){//当收到rst标志时,connection reset by client
                        printf("client[%d] aborted connection\n",i);
                        Close(sockfd);
                        client[i].fd = -1;
                    }
                    else perr_exit("read error");
                }
                else if(n == 0){ //connection closed by client
                    printf("client[%d] closed connection\n",i);
                    Close(sockfd);
                    client[i].fd = -1;
                }
                else{
                    for(j = 0 ; j < n ; j++)
                        buf[j] = toupper(buf[j]);
                    Writen(sockfd,buf,n);
                }
                if(--nready <= 0)
                    break;
            }
        }
    }
    return 0;
}
