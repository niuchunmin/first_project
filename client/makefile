LIBNAME = myClient
LIBNAME2= sqlite3
APP_NAME= client 

LDNAME = LD_LIBRARY_PATH
INSTPATH = `pwd`/lib 
INCLPATH = `pwd`/include 

CC = gcc
AR = ar 

all:client_make
	clean

client_make:
	${CC} client.c -g -o client -L ${INSTPATH} -l${LIBNAME} -l${LIBNAME2} -I ${INCLPATH} 
	export ${LDNAME}=:`pwd`/lib && ./client -i bilibili.com -p 30321 -t 3 
	

clean:
	@rm -f cscope.* tags
	@rm -f *.log *.db
	@rm ${APP_NAME}
