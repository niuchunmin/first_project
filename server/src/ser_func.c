/*********************************************************************************
 *      Copyright:  (C) 2024 LiYi<1751425323@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  ser_func.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(02/04/24)
 *         Author:  LiYi <1751425323@qq.com>
 *      ChangeLog:  1, Release initial version on "02/04/24 20:11:43"
 *                 
 ********************************************************************************/

#include "ser_func.h"

void   print_usage(char *progname)
{
	printf("Usage:%s [OPTION]...\n",progname);
	printf("%s is a socket server program\n",progname);

	printf("-b(--daemon) set program running on background\n");
	printf("-p(--port):specify server port\n");
	printf("-h(--help):print this help information.\n");
	return ;
}
