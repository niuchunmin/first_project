#********************************************************************************
#      Copyright:  (C) 2023 LingYun IoT System Studio
#                  All rights reserved.
#
#       Filename:  Makefile
#    Description:  This file is the project top Makefie
#
#        Version:  1.0.0(11/08/23)
#         Author:  Guo Wenxue <guowenxue@gmail.com>
#      ChangeLog:  1, Release initial version on "11/08/23 16:18:43"
#
#*******************************************************************************

PRJ_PATH=$(shell pwd)
APP_NAME = sock_client

BUILD_ARCH=$(shell uname -m)
ifneq ($(findstring $(BUILD_ARCH), "x86_64" "i386"),)
    CROSS_COMPILE=arm-linux-gnueabihf-
endif

# C source files in top-level directory
SRCFILES = $(wildcard *.c)

# common CFLAGS for our source code
CFLAGS = -Wall -Wshadow -Wundef -Wmaybe-uninitialized -D_GNU_SOURCE

# sub-directory compiled to a library and need to link
SRCS=booster
SRCS_PATH=$(patsubst %,${PRJ_PATH}/%,$(SRCS))
CFLAGS+=$(patsubst %,-I%,$(SRCS_PATH))
LDFLAGS+=$(patsubst %,-L%,$(SRCS_PATH))
LIBS=$(patsubst %,-l%,$(SRCS))
LDFLAGS+=${LIBS}

# Open source libraries
CFLAGS+=-I ${PRJ_PATH}/sqlite/install/include
LDFLAGS+=-L ${PRJ_PATH}/sqlite/install/lib
LDFLAGS+=-lsqlite3
LDFLAGS+=-lpthread

# sub-directory need to entry and compile
SUBDIR=sqlite ${SRCS}

all: entry subdir
	${CROSS_COMPILE}gcc ${CFLAGS} sock_client.c -o sock_client ${LDFLAGS}

entry:
	@echo "Building ${APP_NAME} on ${BUILD_ARCH}"

subdir:
	@for dir in ${SUBDIR} ; do if [ ! -e $${dir} ] ; then ln -s ../$${dir}; fi; done
	@for dir in ${SUBDIR} ;  do make CFLAGS="${CFLAGS}" -C $${dir} ; done

install:
	cp ${APP_NAME} /tftp

clean:
	@for dir in ${SRCS} ; do if [ -e $${dir} ] ; then make clean -C $${dir}; fi; done
	@rm -f ${APP_NAME}

distclean:
	@for dir in ${SUBDIR} ; do if [ -e $${dir} ] ; then make clean -C $${dir}; fi; done
	@rm -f ${APP_NAME}
	@rm -f cscope.* tags
	@rm -f *.log *.db

