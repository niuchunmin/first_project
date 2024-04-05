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
int get_temperature(float *temp)
{
	int                 fd=-1;
	char                buf[128];
	char               *ptr=NULL;
	DIR                *dirp=NULL;
	struct dirent      *direntp=NULL;
	char                w1_path[64]="/sys/bus/w1/devices/";
	char                chip_sn[32];
	int                 found=0;
	int                 rv=10;

	dirp=opendir(w1_path);
	if(!dirp)
	{   
		printf("Open floder %s failure:%S\n",w1_path,strerror(errno));
		rv=-1;
		goto cleanup;
	}   

	while(NULL != (direntp=readdir(dirp)))
	{   
		if(strstr(direntp->d_name,"28-"))
		{   
			strncpy(chip_sn,direntp->d_name,sizeof(chip_sn));
			found=1;
		}   
	}   
	closedir(dirp);
	dirp=NULL;

	if(!found)
	{   

		printf ("Cannot find ds18b20 chipset\n");
		rv=-2;
		goto cleanup;
	}   
	strncat(w1_path,chip_sn,sizeof(w1_path)-strlen(w1_path));
	strncat(w1_path,"/w1_slave",sizeof(w1_path)-strlen(w1_path));

	fd=open(w1_path,O_RDONLY);
	if(fd<0)
	{   
		printf ("Open file failure:%s\n",strerror(errno));
		rv=-3;
		goto cleanup;
	}   

	memset(buf,0,sizeof(buf));

	if(read(fd,buf,sizeof(buf))<0)
	{   

		printf ("Read file failure:%s\n",strerror(errno));
		rv=-4;
		goto cleanup;
	}   

	ptr = strstr(buf,"t=");
	if(!ptr)
	{   

		printf ("Can not find string t=\n");

		rv =-5;
		goto cleanup;
	}
	ptr += 2;
	*temp=atof(ptr)/1000;
	close(fd);
cleanup:
	if(fd)
		close(fd);
	if(!dirp)
		closedir(dirp);

	return rv;
}
