/*********************************************************************************
 *      Copyright:  (C) 2024 linuxer<linuxer@email.com>
 *                  All rights reserved.
 *
 *       Filename:  socket.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(01/04/24)
 *         Author:  linuxer <linuxer@email.com>
 *      ChangeLog:  1, Release initial version on "01/04/24 16:26:12"
 *                 
 ********************************************************************************/



#include "socket.h" 

int connect_to_server(char *servip,int *port)
{
	struct sockaddr_in      server_addr;
	int                     sockfd;
	socklen_t               len=0;

	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
	{

		printf ("Create socket failure:%s\n",strerror(errno));
		return -1;
	}

	memset(&server_addr,0,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(*port);

	if(inet_aton(servip,&server_addr.sin_addr)==-1)
	{

		printf ("inet_pton:%s\n",strerror(errno));
		return -2;
	}
	if(connect(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr))<0)
	{

		printf ("debug00:sockfd is %d\n",sockfd);

		printf ("Connect failure:%s\n",strerror(errno));
		close(sockfd);
		return -3;
	}

	printf ("connectd to server successfully!\n");
	return sockfd;
}

int socketconnected(int sockfd)
{
	struct tcp_info info;
	int    len=sizeof(info);

	if(socket<=0)
	{
		printf ("connected failure:sockfd<=0\n");
		return 0;
	}
	getsockopt(sockfd,IPPROTO_TCP,TCP_INFO,&info,(socklen_t *)&len);

	if(info.tcpi_state == 1)
	{

		printf ("socket connected!\n");
		return 1;
	}
	else
	{

		printf ("socket disconnected\n");
		return 0;
	}
}
