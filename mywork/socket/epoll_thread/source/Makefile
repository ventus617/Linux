.PHONY:thread_poll

CC=gcc
INCLUDE_PATH=../include
LIBRARY=lpthread
TARGET=thread_poll
CFLAGS=-I$(INCLUDE_PATH)  -g -Wall -c
RM=rm -rf
SRC_FILE=$(wildcard *.c)
DES_FILE=$(patsubst %.c,%.o,$(SRC_FILE))#以SRC——FILE为基准的所有的.o文件

$(TARGET):$(DES_FILE)
	$(CC) $^ -$(LIBRARY) -o $@

%.o:%.c
	$(CC) $(CFLAGS) $<

clean:
	$(RM) $(DES_FILE)
