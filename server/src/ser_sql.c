/*********************************************************************************
 *      Copyright:  (C) 2024 LiYi<1751425323@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  ser_sql.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(02/04/24)
 *         Author:  LiYi <1751425323@qq.com>
 *      ChangeLog:  1, Release initial version on "02/04/24 20:32:10"
 *                 
 ********************************************************************************/


#include "ser_sql.h"

#define   SER_DB    "server.db"


int create_table(sqlite3 *db,char *table_name)
{
	char            *zErrMsg = 0;
	int              rc; 
	char             sql[512];

	snprintf(sql,512,"CREATE TABLE IF NOT EXISTS %s(sn TEXT NOT NULL,time TEXT NOT NULL,temp REAL NOT NULL)",table_name);

	rc = sqlite3_exec(db,sql,0,0,&zErrMsg);
	if(rc != SQLITE_OK)
	{   
		fprintf(stderr,"SQL error:%s\n",zErrMsg);
		sqlite3_free(zErrMsg);
		return rc=-2;
	}   
	else
		fprintf(stdout,"Table created successfully\n");
	printf ("debug:%s\n",table_name);
	return 0;
}
int table_insert(sqlite3 *db,char *table_name,char *device_num,char *time,float *temp)
{
	char          *zErrMsg = 0;
	int            rc; 
	char           sql[512];
	snprintf(sql,512,"INSERT INTO %s(sn,time,temp) VALUES ('%s','%s',%.2f)",table_name,device_num,time,*temp);
	rc = sqlite3_exec(db,sql,0,0,&zErrMsg);
	if( rc != SQLITE_OK )
	{
		fprintf(stderr,"SQL error:%s\n",zErrMsg);
		sqlite3_free(zErrMsg);
		return rc=-2;
	}
	else
		fprintf(stdout,"Records created successfully\n");
	return 0;
}

int table_delete(sqlite3 * db,char *table_name)
{
	char            *zErrMsg=NULL;
	int              rc;
	char             sql[128];
	const char*      data = "Callback function called";
//	snprintf(sql,128,"DELETE from %s where ROWID IN(SELECT ROWID from %s LIMIT 1)",table_name,table_name);
    snprintf(sql,128,"DELECT * from %s",table_name);
	rc = sqlite3_exec(db,sql,0,0,&zErrMsg);
	if( rc!= SQLITE_OK )
	{
		fprintf(stderr,"SQL error:%s\n",zErrMsg);
		sqlite3_free(zErrMsg);
		return rc=-2;
	}
	else
		fprintf(stdout,"Operation done successfully\n");
	return 0;
}
static int callback(void *data,int argc,char **argv,char **azColName)
{
	int i;
	fprintf(stderr,"%s:",(const char*)data);
	for(i=0;i<argc;i++)
		printf("%s=%s\t",azColName[i],argv[i] ? argv[i]:NULL);

	printf ("\n");
	return 0;
}
int table_select(sqlite3 *db,char *table_name)
{
	char           *zErrMsg=0;
	int             rc;
	char            sql[128];
	const char*     data="Callback function called";
	snprintf(sql,128,"SELECT * from %s",table_name);
	rc = sqlite3_exec(db,sql,callback,(void*)data,&zErrMsg);
	if(rc != SQLITE_OK)
	{
		fprintf(stderr,"SQL error:%s\n",zErrMsg);
		sqlite3_free(zErrMsg);
		return rc=-2;
	}
	else
		fprintf(stdout,"Operation done successfully\n");
	return rc;
}
