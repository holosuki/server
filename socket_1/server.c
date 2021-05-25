#include <stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<strings.h>
#include<arpa/inet.h>
#include<ctype.h>
#include<unistd.h>


#define SERV_PORT 8000

int main(void){
	int sfd,cfd;
    int i,len;
    char buf[4096],client_ip[128];
    struct sockaddr_in serv_addr,client_addr;
    socklen_t addr_len;

	sfd = socket(AF_INET,SOCK_STREAM,0);//第一个ipv4，第二个协议类型，第三个一般是0；
    
    //绑定前构造服务器地址
    bzero(&serv_addr,sizeof(serv_addr));//清零；
    serv_addr.sin_family = AF_INET;
    //网络字节序
    serv_addr.sin_port = htons(SERV_PORT);
    //INADR_ANY主机所有ip
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(sfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
        //因为serv_addr是sockaddr_in，表示ipv4，ipv6则是后面加一个6，所以为了泛型，就搞成了sockaddr;
    
    //服务器能接受并发链接的能力
    listen(sfd,128);
    printf("wait for connect..."); 
    addr_len = sizeof(client_addr);
    //阻塞，等待客户链接，成功返回新的文件描述符，用于和客户端通信；
    cfd = accept(sfd,(struct sockaddr *)&client_addr,&addr_len);
    printf("client IP:%s\t%d\n",
            inet_ntop(AF_INET,&client_addr.sin_addr,client_ip,sizeof(client_ip)),
            ntohs(client_addr.sin_port));
        //inet_ntop()二进制转字符串；
    while(1){
        //阻塞接收客户端数据
        len = read(cfd,buf,sizeof(buf));
        write(STDOUT_FILENO,buf,len); 
        //处理业务
        for(i = 0 ; i < len ; i++)
            buf[i] = toupper(buf[i]);
        //返回给客户端数据
        write(cfd,buf,len);
    }
    close(cfd);
    close(sfd);

    return 0;
}
