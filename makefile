#!/usr/bin/make -f

UV_PATH=$(shell pwd)/./libuv
UV_LIB=$(UV_PATH)/libuv.la
CFLAGS=-g -Wall -I$(UV_PATH)/include
LIBS=

CFLAGS 	+= -D__STDC_CONSTANT_MACROS

uname_S=$(shell uname -s)

ifeq (Darwin, $(uname_S))
CC = llvm-g++
CFLAGS+=-framework CoreServices
endif

ifeq (Linux, $(uname_S))
CC = g++
LIBS=-lrt -ldl -lm -pthread
endif

all: $(UV_LIB) $(SAMPLE)

$(UV_LIB):
	make -C ./libuv

$(SAMPLE):
	$(CC) $(CFLAGS) -o sample sample.cc $(UV_LIB) $(LIBS)	



clean:
	rm -f sample


.PHONY: clean