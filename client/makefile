LIBNAME = myClient
LIBNAME2= sqlite3

LDNAME = LD_LIBRARY_PATH
INSTPATH = `pwd`/lib 
INCLPATH = `pwd`/include 

CC = gcc
AR = ar 

all:client_make

client_make:
	${CC} client.c -o client -L ${INSTPATH} -l${LIBNAME} -l${LIBNAME2} -I ${INCLPATH} 
	export ${LDNAME}=:`pwd`/lib && ./client -i 127.0.0.1 -p 30321 -t 3
