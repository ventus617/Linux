.PHONY:copy

CC=gcc
INCLUDE_PATH=../include/
LIBRARY_PATH=../library/
TARGET=copy
CFLAGS=-I$(INCLUDE_PATH) -g -Wall -c
RM=rm -rf
SRC_FILE=$(wildcard *.c)
DES_FILE=$(patsubst %.c,%.o,$(SRC_FILE))

$(TARGET):$(DES_FILE)
	$(CC) $^ -o $@

%.o:%.c
	$(CC) $(CFLAGS) $<

clean:
	$(RM) $(DES_FILE)

