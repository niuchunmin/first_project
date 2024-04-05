/*********************************************************************************
 *      Copyright:  (C) 2024 LiYi<1751425323@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  server.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(02/04/24)
 *         Author:  LiYi <1751425323@qq.com>
 *      ChangeLog:  1, Release initial version on "02/04/24 20:40:24"
 *                 
 ********************************************************************************/


#include "server.h"


int main (int argc, char **argv)
{
	int                 ch=-1;
	int                 rv=0;
	int                 listen_fd,conn_fd=-1;
	int                 on=1;
	int                 daemon_run=0;
	int                 i,j;
	int                 read_rv=-1;
	int                 port;
	char                buf[1024];

	int                 epollfd;
	struct epoll_event  event;
	struct epoll_event  event_array[MAX_EVENTS];
	int                 events;
	sqlite3            *db;
	char                buf_sn[32];
	char                buf_time[128];
	char                table_temp[32];
	float               tb_temp;
	int                 ret=0;

	struct option       opts[]={
		{"daemon",no_argument,NULL,'b'},
		{"port",required_argument,NULL,'p'},
		{"help",no_argument,NULL,'h'},
		{NULL,0,NULL,0}
	};

	while((ch=getopt_long(argc,argv,"bp:h",opts,NULL))!=-1)
	{
		switch(ch)
		{
			case 'b':
				daemon_run=1;
				break;
			case 'p':
				port = atoi(optarg);
				break;
			case 'h':
				print_usage(argv[0]);
				return EXIT_SUCCESS;
			default:
				break;
		}
	}

	if(!port)
	{
		print_usage(argv[0]);
		return -1;
	}
	set_socket_rlimit();
	if((listen_fd=socket_server_init(NULL,port))<0)
	{

		printf ("ERROR:%s server listen on port %d failure:%s\n",argv[0],port);
		return -2;
	}
	printf ("%s server start to listen on port %d\n",argv[0],port);

	if(daemon_run)
		daemon(0,0);

	if((epollfd = epoll_create(MAX_EVENTS))<0)
	{

		printf ("epoll_create() failure %s\n",strerror(errno));
		return -3;
	}

	event.events = EPOLLIN;
	event.data.fd = listen_fd;

	if(epoll_ctl(epollfd,EPOLL_CTL_ADD,listen_fd,&event)<0)
	{

		printf ("epoll add listen socket failure:%s\n",strerror(errno));
		return -4;
	}
	for( ; ; )
	{
		events = epoll_wait(epollfd,event_array,MAX_EVENTS,-1);
		if(events<0)
		{

			printf ("epoll failure:%s\n",strerror(errno));
			break;
		}
		else if(events == 0)
		{

			printf ("epoll get timeout.\n");
			continue;
		}


		printf ("debug03:events:%d\n",events);
		for(i=0;i<events;i++)
		{
			if((event_array[i].events & EPOLLERR)||(event_array[i].events & EPOLLHUP))
			{

				printf ("epoll_wait get error on fd[%d] :%s\n",event_array[i].data.fd,strerror(errno));
				epoll_ctl(epollfd,EPOLL_CTL_DEL,event_array[i].data.fd,NULL);
				close(event_array[i].data.fd);
			}
			if(event_array[i].data.fd == listen_fd)
			{
				if((conn_fd=accept(listen_fd,(struct sockaddr *)NULL,NULL))<0)
				{

					printf ("accept new client failure:%s\n",strerror(errno));
					continue;
				}


				printf ("debug02:%d\n",conn_fd);
				event.data.fd = conn_fd;
				event.events = EPOLLIN;
				if(epoll_ctl(epollfd,EPOLL_CTL_ADD,conn_fd,&event)<0)
				{

					printf ("epoll add client socket failure:%s\n",strerror(errno));
					close(event_array[i].data.fd);
					continue;
				}

				printf ("epoll add new client socket[%d] successfully.\n",conn_fd);
			}
			else

			{
				memset(buf,0,sizeof(buf));
				if((read_rv = read(event_array[i].data.fd,buf,sizeof(buf)))<=0)
				{
					printf ("debug01:the buf of size:%d and rv:%d\n",sizeof(buf),read_rv);

					printf ("socket[%d] read failure or get disconnect and will be removed.\n",event_array[i].data.fd);
					epoll_ctl(epollfd,EPOLL_CTL_DEL,event_array[i].data.fd,NULL);
					close(event_array[i].data.fd);
					continue;
				}
				else
				{

					printf ("socked[%d] read get %d bytes data:%s\n",event_array[i].data.fd,read_rv,buf);
					ret = sscanf(buf,"%s %s %s",buf_sn,buf_time,table_temp);
					printf ("debug04:%s,%s,%.2f\n",buf_sn,buf_time,atof(table_temp));
					tb_temp=atof(table_temp);

					if((rv=sqlite3_open("server.db",&db)))
					{
						fprintf(stderr,"Can't open database:%s\n",sqlite3_errmsg(db));
						exit(0);
					}
					if((rv=create_table(db,SQL3_NAME))<0)
					{

						printf ("Create table failure:%s\n",strerror(errno));
						goto clean;
					}
					printf ("Create table %s successfully\n",SQL3_NAME);
					if((rv=table_insert(db,SQL3_NAME,buf_sn,buf_time,&tb_temp))<0)
					{

						printf ("Insert data to table failure:%s\n",strerror(errno));
						goto clean;
					}
					printf ("Insert data to table successfully!\n");
					if((rv=table_select(db,SQL3_NAME))<0)
					{

						printf ("Select data from table failure:%s\n",strerror(errno));
						goto clean;
					}

					sqlite3_close(db);
				}
			}
		}
	}

clean:
	sqlite3_close(db);
cleanUp:
	close(listen_fd);
	return 0;
}
