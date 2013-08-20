#!/usr/bin/make -f

UV_PATH=$(shell pwd)/./libuv
UV_LIB=$(UV_PATH)/.libs/libuv.a
CFLAGS=-g -Wall -I$(UV_PATH)/include
LIBS=

CC = g++
CFLAGS 	+= -D__STDC_CONSTANT_MACROS

uname_S=$(shell uname -s)

ifeq (Darwin, $(uname_S))
CFLAGS+=-framework CoreServices
endif

ifeq (Linux, $(uname_S))
LIBS=-lrt -ldl -lm -pthread
endif

all: 
	$(CC) $(CFLAGS) -o sample src/DeathClock.cc sample.cc $(UV_LIB) $(LIBS)

$(UV_LIB):
	make -C ./libuv


clean:
	rm -f sample


.PHONY: clean