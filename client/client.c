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

#include "proc.h"
#include "logger.h"

int check_sample_time(time_t *last_time,int interval)
{
	time_t          current_time;
	int             need = 0;

	time(&current_time);

	if( current_time >= (*last_time+interval) )
	{
		need = 1;
		*last_time = current_time;
		log_debug ("test:last_time:%lld\n",(long long)*last_time);
	}

	return need;
}

int main (int argc, char **argv)
{
	char                *servip=NULL;
	int                  port=0;
	int                  rv=-1;
	struct sockaddr_in   serv_addr;
	char                 buf[1024];
	int                  ch=-1;
	int                  temp_interval=0;
	float                temp;
	char                 id[16];
	int                  len=16;
	char                 time_buf[128];
	char                 all_buf[256];
	int                  error = 0;
	socklen_t            err_len = 0;
	sqlite3             *db;
	int                  tb_num=-1;
	char                 rt_buf[128];
	int                  isConnect=-1;
	const char           db_name[]="client_db";
	sock_s               sock;
	data_s               all_data;
	pack_func_t          pack_data_p = pack_data;
	time_t               last_time;
	int                  sample_flag;
	int                  interval = 60;
	int                  pack_bytes = 0;
	char                *logfile = "sock_client.log";
	int                  loglevel = LOG_LEVEL_INFO;
	int                  logsize = 10;
	int                  daemon_run = 0;

	struct option       opts[]={
		{"ipaddr",required_argument,NULL,'i'},
		{"port",required_argument,NULL,'p'},
		{"temp_interval",required_argument,NULL,'t'},
		{"daemon",no_argument,NULL,'b'},
		{"help",no_argument,NULL,'h'},
		{NULL,0,NULL,0}
	};

	while((ch=getopt_long(argc,argv,"i:p:t:bh",opts,NULL))!=-1)
	{
		switch(ch)
		{
			case 'i':
				servip = optarg;
				break;
			case 'p':
				port = atoi(optarg);
				break;
			case 't':
				temp_interval = atoi(optarg);
				break;
            case 'b':
				daemon_run = 1;
				logfile = "console";
				loglevel = LOG_LEVEL_DEBUG;
				break;
			case 'h':
				print_usage(argv[0]);
				return 0;
		}
	}

	if(!servip || !port || !temp_interval)
	{
		print_usage(argv[0]);
		return 0;
	}

	if( daemon_run )
	{
		daemon(0,0);
	}

	if( log_open(logfile, loglevel, logsize, THREAD_LOCK_NONE) < 0 )
	{
		fprintf(stderr, "Initial log system failed\n");
		return 1;
	}

	install_default_signal();

	log_info("Program start running.\n");

	rv = socket_info_init(&sock,servip,port);

	if((rv = create_table("client.db")) <0) 
	{   

		 log_error ("Create database failure:%s\n",strerror(errno));
		 database_term();
	 }   

	time(&last_time);
	log_debug ("test:last_time:%lld\n",(long long)last_time);

	while( !g_sigstop )
	{
         /* -------Sample data or not-------- */
		sample_flag = 0;
		if( 1 == (rv=check_sample_time(&last_time,temp_interval)) )
		{
			//Start to sample
 			if( (rv=get_devid(all_data.devid, DEVICEID_LEN, 1))<0 )
			{
				log_error ("Get device id failure:%s\n",strerror(errno));
				continue;
			}
			get_time(all_data.sample_time,TIME_LEN);
			if( (rv=get_temperature(&all_data.temp))<0 )
			{
				log_error ("Get temperature failure:%s\n",strerror(errno));
				continue;
			}

			log_debug ("test:temp:%.2f\n",all_data.temp);

			memset(all_buf, 0, sizeof(all_buf));
			pack_bytes = pack_data_p(&all_data, all_buf, sizeof(all_buf));
			log_debug ("test:all data:%s %s %.2f\n",all_data.devid,all_data.sample_time,all_data.temp);
            sample_flag = 1;
		}

		/* ------Start to connect to server ------*/
		if( sock.conn_fd < 0 )
		{
			connect_to_server(&sock);
		}

		/*------- Connected or not ---------*/
		isConnect = socketconnected(&sock);
	
		log_debug ("test:%d\n",isConnect);
		if( 1 != isConnect)
		{
			if( sock.conn_fd > 0 )
			{
				log_error ("Socket got disconnected,reconnect now.\n");
				socket_close(&sock);//close the socket fd
			}
		}
		if( sock.conn_fd < 0 )
		{
			//disconnected. sample or not,if sample,store data to database
		    if( 1 == sample_flag)
			{
				rv=table_insert(&all_data);
				rv = table_select();	
			}

			continue;
		}

		//Socket connected 
		if( sample_flag )//connected successfully!
		{
			log_debug ("test:ready to write %d bytes data[%s] to server\n",
					pack_bytes,all_buf);
			
			if( socket_write(&sock, all_buf, pack_bytes) <0 )
			{
				log_warn ("socket sent data failure,store data to database.\n");
                table_insert(&all_data);
				socket_close(&sock);

			}
			log_info ("Update %s successfully!\n",all_buf);
		}

		memset(rt_buf,0,sizeof(rt_buf));

		if( (tb_num = determine_table_exist(rt_buf)) > 0 )
		{
			log_debug ("test:database has %d data\n",tb_num);

			if( ( rv = socket_write(&sock, rt_buf, strlen(rt_buf))) <0 )
			{
				log_error ("Write data to server from database failure:%s\n",
						strerror(errno));
			}
			else
			{
				rv = table_delete();
			}
		}

		msleep(50);
	}

cleanup:
	database_term();
    socket_close(&sock);
	log_close();

	return 0;
}

