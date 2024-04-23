/********************************************************************************
 *      Copyright:  (C) 2024 LiYi<1751425323@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  client.h
 *    Description:  This file 
 *
 *        Version:  1.0.0(03/04/24)
 *         Author:  LiYi <1751425323@qq.com>
 *      ChangeLog:  1, Release initial version on "03/04/24 15:21:47"
 *                 
 ********************************************************************************/
#ifndef   _CLIENT_H_
#define   _CLIENT_H_

#define   MAX_SIZE  20 
//#define   CONFIG_DEBUG
#ifdef    CONFIG_DEBUG
#define   dbg_print(format,args...) printf(format,##args)
#else
#define   dbg_print(format,args...) do{} while(0)
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <getopt.h>
#include <arpa/inet.h>
#include <time.h>
#include <libgen.h>
#include <linux/tcp.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sqlite3.h>
#include "cli_sql.h"
#include "cli_func.h"
#include "ds18b20.h"
#include "socket.h"
#include "logger.h"

#define  MSG_STR "Hello,I am ready to connect with you!"
#define  HEARTBEAT_INTERVAL 5

#endif
