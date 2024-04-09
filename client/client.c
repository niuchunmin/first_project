/*********************************************************************************
 *      Copyright:  (C) 2024 LiYi<1751425323@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  client.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(02/04/24)
 *         Author:  LiYi <1751425323@qq.com>
 *      ChangeLog:  1, Release initial version on "02/04/24 16:08:43"
 *                 
 ********************************************************************************/


#include "client.h"

int main (int argc, char **argv)
{
	int                  port=0;
	char                *servip;
	int                  conn_fd=-1;
	int                  rv=-1;
	struct sockaddr_in   serv_addr;
	char                 buf[1024];
	int                  ch=-1;
	int                  temp_time=10;
	float                temp;
	char                 id[16];
	int                  len=16;
	char                 time_buf[128];
	char                 all_buf[256];
	int                  error = 0;
	socklen_t            err_len = 0;
	sqlite3             *db;
	char                *sqlnam3="DB_TB_CLI";
	int                  tb_num=-1;
	char                 rt_buf[128];

	struct option       opts[]={
		{"ipaddr",required_argument,NULL,'i'},
		{"port",required_argument,NULL,'p'},
		{"temp_time",required_argument,NULL,'t'},
		{"help",no_argument,NULL,'h'},
		{NULL,0,NULL,0}
	};
	printf ("crazy!\n");
	while((ch=getopt_long(argc,argv,"i:p:t:h",opts,NULL))!=-1)
	{
		switch(ch)
		{
			case 'i':
				servip=optarg;
				break;
			case 'p':
				port=atoi(optarg);
				break;
			case 't':
				temp_time=atoi(optarg);
				break;
			case 'h':
				print_usage(argv[0]);
				return 0;
		}
	}

	if(!servip||!port||!temp_time)
	{
		print_usage(argv[0]);
		return 0;
	}
	conn_fd=connect_to_server(servip,&port);
	printf ("test:00\n");
	while(1)
	{
		/* Acquire device id */
		if((rv=get_devid(id))<0)
		{
			
			printf ("Get device id failure:%s\n",strerror(errno));
			goto cleanUp;
		}
		printf("Debug_Device id:%s\n",id);
		get_time(time_buf);
		if((rv=get_temperature(&temp))<0)
		{
			printf("Acquire temperature failure:%s\n",strerror(errno));
			goto cleanUp;
		}
		snprintf(all_buf,sizeof(all_buf),"%s %s %.2f",id,time_buf,temp);
		/* connect or not */
		rv=socketconnected(conn_fd);
		if(1==rv)//connected successfully!upload data to server!!
		{
			if(write(conn_fd,all_buf,strlen(all_buf))<0)
			{
				printf ("Write to server failure:%s\n",strerror(errno));
				goto cleanUp;
			}
			printf ("Update %s successfully!\n",all_buf);
		}
		else//连接失败，进行重连，并把数据库储存在数据库中
		{
			/* 打开数据库 */
			if((rv=sqlite3_open("test.db",&db)))
			{
				fprintf(stderr,"Can't open database:%s\n",sqlite3_errmsg(db));
				exit(0);
			}
			else
				fprintf(stdout,"Table created successfully\n");
		//将数据暂存在数据库中
		/*   创建表 */
	    	if((rv = create_table(db,SQL_NAME)) <0)
	    	{

		    	printf ("Create database failure:%s\n",strerror(errno));
		    	goto clean;
		    }
            /* 插入数据 */
		    if((rv=table_insert(db,SQL_NAME,id,time_buf,&temp))<0)
			{

				printf ("Insert data to table failure:%s\n",strerror(errno));
				goto clean;
			}
			/* 查看数据库中表里的数据 */
			if((rv=table_select(db,SQL_NAME))<0)
			{

				printf ("Select data failure:%s\n",strerror(errno));
				goto clean;
			}
			/* 数据已存入数据库，等待重连 */
			close(conn_fd);
			conn_fd=connect_to_server(servip,&port);
			if(conn_fd <= 0)
			{

				printf ("Reconnect to server failure:%s\n",strerror(errno));
			}
			else
			{
				printf("client %d reconnect to server successfully!\n",conn_fd);
				do
				{


					printf ("debug02:Repeat!!where is segmatation out!!\n");
					memset(rt_buf,0,sizeof(rt_buf));
					tb_num=determine_table_exist(SQL_NAME,db,rt_buf);
					printf ("debug03:The table %s has %d num\n",SQL_NAME,tb_num);


					if((rv=write(conn_fd,rt_buf,strlen(rt_buf)))<0)
					{
	
						printf ("Write data to server from table failure:%s\n",strerror(errno));
						goto cleanUp;
					}
					else
						printf ("debug04:Write %s from table to server successfully!\n",rt_buf);
					rv=table_delete(db,SQL_NAME);
					tb_num--;

				}while(0!=tb_num);
			}
		}
clean:
		sqlite3_close(db);

		sleep(temp_time);
	}
cleanUp:
	close(conn_fd);
	return 0;
}

