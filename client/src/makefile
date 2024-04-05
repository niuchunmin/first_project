LIBNAME = myClient

INSTPATH = `pwd`/../lib/
INCLPATH = `pwd`/../include/

CC = gcc 
AD = ar

all:dynamic_lib
	@make install
	@make clear

dynamic_lib:
	${CC} -shared -fPIC *.c -o lib${LIBNAME}.so -I ${INCLPATH}

install:
	cp -rf lib${LIBNAME}.* ${INSTPATH}

uninstall:
	rm -f ${INSTPATH}/lib${LIBNAME}.*

clear:
	rm -f *.so

clean:clear
	rm -f lib${LIBNAME}.*
