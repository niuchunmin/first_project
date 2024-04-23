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


#include <netdb.h>

#include <regex.h>
#include "socket.h" 

int socket_info_init(sock_s *sock,char *servip,int port)
{
	if( !sock || port<=0 )
	{
		printf ("test:ip:%s\n",servip);
		return -1;
	}

	memset(sock,0,sizeof(sock));
	sock->port = port;
	sock->conn_fd = -1;
	if( servip )
	{
		strncpy(sock->servip, servip, HOSTNAME_LEN);

		printf ("test:ip:%s\n",servip);
	}

	return 0;
}


int connect_to_server(sock_s *sock)
{
	struct sockaddr_in      addr;
	socklen_t               len=sizeof(addr);
	regex_t                 regex;
	struct hostent          iphost;
	int                     sockfd = 0;
	int                     rv = 0;
	char                    service[20];
	struct addrinfo         hints,*rp,*res = NULL;
	struct in_addr          inaddr;

	if( !sock )
		return -1;



	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;


 	if( inet_aton(sock->servip, &inaddr) )
	{ 
		hints.ai_flags |= AI_NUMERICHOST;
		printf ("2222\n");
	}

	snprintf(service, sizeof(service), "%d", sock->port);
	if( (rv = getaddrinfo(sock->servip, service, &hints, &res)) )
	{
		log_error("Getaddrinfo() parser [%s:%s] failure:%s\n",sock->servip, 
				service,gai_strerror(rv));
		return -3;
	}
	
	for(rp=res; rp != NULL; rp=rp->ai_next)
	{
 	 	char                    ipaddr[INET_ADDRSTRLEN];
		struct sockaddr_in     *sp = (struct sockaddr_in *) rp->ai_addr;

		memset( ipaddr, 0, sizeof(ipaddr));
		if( inet_ntop(AF_INET, &sp->sin_addr, ipaddr, sizeof(ipaddr)) )
		{
			log_info("domain name resolution [%s->%s]\n",sock->servip,ipaddr);
		//	printf("domain name resolution [%s->%s]\n",sock->servip,ipaddr);
		}

		sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if(sockfd < 0)
		{
			log_error("socket() create failure:%s\n",strerror(errno));
			rv = -3;
			continue;
		}

		rv = connect(sockfd, rp->ai_addr, len);
		if( 0==rv )
		{
			sock->conn_fd = sockfd;
			log_info("Connect to server [%s:%d] on fd[%d] successfully!\n",
					sock->servip, sock->port, sockfd);
			break;
		}
		else
		{
			printf ("domain name resolution [%s->%s]\n",sock->servip,inaddr);
			close(sockfd);
			continue;
		}
	}


	freeaddrinfo(res);
	return rv;
}


int socket_write(sock_s *sock, char *data, int bytes)
{
	int     rv = 0;
    int     i  = 0;
	int     left_bytes = bytes;

	if( !sock || !data || bytes <= 0)
	{
		log_error ("Invaild input arguments\n");
		return -1;
	}
	
	while( left_bytes > 0 )
	{
		rv = write(sock->conn_fd, &data[i], left_bytes);
		if ( rv<0 )
		{
			log_info ("socket[%d] write() failure:%s,close socket now\n",
					sock->conn_fd, strerror(errno));
			socket_close (sock);
			return -2;
		}
		else if( rv == left_bytes )
		{
			log_info ("Socket write %d bytes data successfully\n",bytes);
			return 0;
		}
		else
		{
			i += rv;
			left_bytes -= rv;
			continue;
		}
	}

	return 0;
}


int socketconnected(sock_s *sock)
{
	struct tcp_info info;
	int    len=sizeof(info);

	if(sock->conn_fd <=0)
	{
		log_info ("connected failure:sockfd<=0\n");
		return 0;
	}
	getsockopt(sock->conn_fd,IPPROTO_TCP,TCP_INFO,&info,(socklen_t *)&len);

	if(info.tcpi_state == 1)
	{

//		printf ("socket connected!\n");
		return 1;
	}
	else
	{

		dbg_print ("socket disconnected\n");
		return 0;
	}
}


int socket_close(sock_s *sock)
{
	if( !sock )
		return -1;

	if( sock->conn_fd >0 )
	{
		close(sock->conn_fd);
		sock->conn_fd = -1;
	}

	return 0;
}
