/********************************************************************************
 *      Copyright:  (C) 2024 LiYi<1751425323@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  ser_func.h
 *    Description:  This file 
 *
 *        Version:  1.0.0(02/04/24)
 *         Author:  LiYi <1751425323@qq.com>
 *      ChangeLog:  1, Release initial version on "02/04/24 20:08:15"
 *                 
 ********************************************************************************/

#ifndef _SER_FUNC_H_
#define _SER_FUNC_H_

#include <stdio.h>
#include <string.h>

#define  CONFIG_DEBUG
#ifdef   CONFIG_DEBUG
#define  dbg_print(format,args...) printf(format,##args)
#else
#define  dbg_print(format,args...) do{} while(0)
#endif

void  print_usage(char *progname);

#endif
