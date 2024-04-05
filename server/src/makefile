LIBNAME = myServer

INSTPATH = `pwd`/../lib/
INCLUpath = `pwd`/../include/

CC = gcc
AD = ar

all:dynamic_lib
	@make clear
	@make install

dynamic_lib:
	${CC} -shared -fPIC *.c -o lib${LIBNAME}.so -I ${INCLUpath}

install:
	cp -rf lib${LIBNAME}.* ${INSTPATH}

uninstall:
	rm -f ${INSTPATH}/lib${LIBNAME}.*

clear:
	rm -f .o

clean:clear
	rm -f lib${LIBNAME}.*
