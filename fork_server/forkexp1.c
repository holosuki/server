/*
 * 程序名：book252.cpp，此程序用于演示用fork生成一个子进程。
 * 作者：C语言技术网(www.freecplus.net) 日期：20190525
 */
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
 
int main()
{
	printf("本程序的进程编号是：%d\n",getpid());
	int ipid=fork();
	sleep(1);       // sleep等待进程的生成。
	printf("pid=%d\n",ipid);		 
	if (ipid!=0) printf("父进程编号是：%d\n",getpid());
	else printf("子进程编号是：%d\n",getpid());
				 
	sleep(30);    // 是为了方便查看进程在shell下用ps -ef|grep book252查看本进程的编号。
}

