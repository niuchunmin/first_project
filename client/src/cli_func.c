/*********************************************************************************
 *      Copyright:  (C) 2024 LiYi<1751425323@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  packdata.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(02/04/24)
 *         Author:  LiYi <1751425323@qq.com>
 *      ChangeLog:  1, Release initial version on "02/04/24 14:52:27"
 *                 
 ********************************************************************************/



#include "cli_func.h"

void print_usage(char *progname)
{
	printf("%s:usage:\n",progname);
	printf("-i(--ipaddr):specify server IP address.\n");
	printf("-p(--port):specify server port.\n");
	printf ("-t(--time):temp upload time\n");
	printf("-h(--help):print this help information.\n");

	return ;
}
int  get_devid(char *id)
{
	int          sn=1;
	int          rv=0;

	if(!id)
	{

		printf ("Argument error:%s\n",strerror(errno));
		rv=-1;
	}
	snprintf(id,16,"DEVID%03d",sn);
	return rv;
}

void  get_time(char *time_buf)
{
	time_t        timep;
	struct tm    *p;
	time(&timep);
	p=localtime(&timep);

	memset(time_buf,0,sizeof(time_buf));

	snprintf(time_buf,128,"%d/%d/%d--%d:%d:%d",1900+p->tm_year,1+p->tm_mon,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);

	printf("%s\n",time_buf);
	return ;
	//  strncpy(time_buf,asctime(gmtime(&timep)),sizeof(time_buf));
}
