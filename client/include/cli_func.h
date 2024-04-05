/********************************************************************************
 *      Copyright:  (C) 2024 LiYi<1751425323@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  cli_func.h
 *    Description:  This file 
 *
 *        Version:  1.0.0(02/04/24)
 *         Author:  LiYi <1751425323@qq.com>
 *      ChangeLog:  1, Release initial version on "02/04/24 18:52:59"
 *                 
 ********************************************************************************/

#ifndef        _CLI_FUNC_H_
#define        _CLI_FUNC_H_

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <stdlib.h>
#include "cli_func.h"
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>


#define        MAX_SIZE 20
#define CONFIG_DEBUG
#ifdef  CONFIG_DEBUG
#define dbg_print(format,args...) printf(format,##args)
#else
#define dbg_print(format,args...) do{} while(0)
#endif

extern int     get_devid(char *id);
extern void    get_time(char *time_buf);
extern void    print_usage(char *progname);
extern int     get_temperature(float *temp);
#endif
