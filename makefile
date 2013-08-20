#!/usr/bin/make -f

CFLAGS 	+= -D__STDC_CONSTANT_MACROS


UV_PATH=$(shell pwd)/../libuv
UV_LIB=$(UV_PATH)/libuv.a
CFLAGS=-g -Wall -I$(UV_PATH)/include
LIBS=

uname_S=$(shell uname -s)

ifeq (Darwin, $(uname_S))
CFLAGS+=-framework CoreServices
endif

ifeq (Linux, $(uname_S))
LIBS=-lrt -ldl -lm -pthread
endif

all: $(UV_LIB) gcc $(CFLAGS) -o sample sample/sample.cc $(UV_LIB) $(LIBS)

$(UV_LIB):
	make -C ../libuv



clean:
	rm -f sample


.PHONY: clean