/********************************************************************************
 *      Copyright:  (C) 2024 LiYi<1751425323@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  socket.h
 *    Description:  This file 
 *
 *        Version:  1.0.0(02/04/24)
 *         Author:  LiYi <1751425323@qq.com>
 *      ChangeLog:  1, Release initial version on "02/04/24 19:04:27"
 *                 
 ********************************************************************************/

#ifndef         _SOCKET_H_
#define         _SOCKET_H_

#define         HOSTNAME_LEN      64
#define         MAX_SIZE          20

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <linux/tcp.h>

#include "logger.h"

typedef struct sock_info
{
	int    port;
	char   servip[HOSTNAME_LEN];
	int    conn_fd;
	int    connected;
}sock_s;

//#define CONFIG_DEBUG
#ifdef  CONFIG_DEBUG
#define dbg_print(format,args...) printf(format,##args)
#else
#define dbg_print(format,args...) do{} while(0)
#endif

extern int      socket_info_init(sock_s *sock,char *servip,int port);
extern int      connect_to_server (sock_s *sock);
extern int      socketconnected (sock_s *sock);
extern int      socket_write(sock_s *sock, char *data, int bytes);
extern int      socket_close(sock_s *sock);


#endif
