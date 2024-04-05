/*********************************************************************************
 *      Copyright:  (C) 2024 LiYi<1751425323@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  sock_func.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(02/04/24)
 *         Author:  LiYi <1751425323@qq.com>
 *      ChangeLog:  1, Release initial version on "02/04/24 20:25:08"
 *                 
 ********************************************************************************/



#include "sock_func.h"
int    socket_server_init(char *listen_ip,int listen_port)
{
	struct sockaddr_in        servaddr;
	int                       rv = 0;
	int                       on = 1;
	int                       listenfd;

	if( (listenfd = socket(AF_INET,SOCK_STREAM,0))<0)
	{

		printf ("Use socket() to vreate a TCP socket failure:%s\n",strerror(errno));
		return -1;
	}

	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(listen_port);

	if(!listen_ip)
		servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	else{
		if( inet_pton(AF_INET,listen_ip,&servaddr.sin_addr)<=0 )
		{

			printf ("inet_pton() set listen IP failure.\n");
			rv=-2;
			goto cleanUp;
		}
	}

	setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
	if( bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr))<0)
	{

		printf ("Use bind() to TCP socket failure:%s\n",strerror(errno));
		rv=-3;
		goto cleanUp;
	}

	if(listen(listenfd,64)<0)
	{

		printf ("Listen to TCP failure:%s\n",strerror(errno));
		rv=-4;
		goto cleanUp;
	}

cleanUp:
	if(rv<0)
		close(listenfd);
	else
		rv=listenfd;

	return rv;
}
void   set_socket_rlimit(void)
{
	struct rlimit limit = {0};

	getrlimit(RLIMIT_NOFILE,&limit);
	limit.rlim_cur = limit.rlim_max;
	setrlimit(RLIMIT_NOFILE,&limit);

	printf ("Set socket open fd max count to %d\n",limit.rlim_max);
}
