LIBNAME = myServer
LIBNAME2= sqlite3

LDNAME = LD_LIBRARY_PATH
INSTPATH = `pwd`/lib 
INCLPATH = `pwd`/include

CC = gcc
AR = ar 

all:server_make

server_make:
	${CC} server.c -o server -L ${INSTPATH} -l${LIBNAME} -l${LIBNAME2} -I ${INCLPATH}
	export ${LDNAME}=:`pwd`/lib && ./server -p 30321
	
