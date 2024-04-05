/********************************************************************************
 *      Copyright:  (C) 2024 LiYi<1751425323@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  server.h
 *    Description:  This file 
 *
 *        Version:  1.0.0(03/04/24)
 *         Author:  LiYi <1751425323@qq.com>
 *      ChangeLog:  1, Release initial version on "03/04/24 09:50:23"
 *                 
 ********************************************************************************/

#ifndef _SERVER_H_
#define _SERVER_H_

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <getopt.h>
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
#include <sqlite3.h>
#include "ser_func.h"
#include "ser_sql.h"
#include "sock_func.h"

#define  MAX_EVENTS        512
#define  ARRAY_SIZE(x)     (sizeof(x)/sizeof(x[0]))
#define  SQL3_NAME         "SERVER_TANLE"


#define  CONFIG_DEBUG
#ifdef   CONFIG_DEBUG
#define  dbg_print(format,args...) printf(format,##args)
#else 
#define  dbg_print(format,args...) do{} while(0)
#endif

#endif /* _SERVER_H_ */
