/********************************************************************************
 *      Copyright:  (C) 2024 LiYi<1751425323@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  sock_func.h
 *    Description:  This file 
 *
 *        Version:  1.0.0(02/04/24)
 *         Author:  LiYi <1751425323@qq.com>
 *      ChangeLog:  1, Release initial version on "02/04/24 20:02:43"
 *                 
 ********************************************************************************/

#ifndef _SOCK_FUNC_H_
#define _SOCK_FUNC_H_

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <time.h>
#include <pthread.h>
#include <libgen.h>
#include <netinet/in.h>

#define CONFIG_DEBUG
#ifdef  CONFIG_DEBUG
#define dbg_print(format,args...)  printf(format,##args)
#else
#define dbg_print(format,args...)  do{} while(0)
#endif

int     socket_server_init(char *listen_ip,int listen_port);
void    set_socket_rlimit(void);

#endif
