.PHONY:download

CC=gcc 
INCLUDE_PATH=../include
LIBRARY_PATH=-lssl -lcrypto
SRC_FILE=$(wildcard *.c)
DES_FILE=$(patsubst %.c,%.o,$(SRC_FILE))
TARGET=Spider
CFLAGS=-I$(INCLUDE_PATH) -g -Wall -c
RM=rm -rf

$(TARGET):$(DES_FILE)
	$(CC) $^ $(LIBRARY_PATH) -o $@

%.o:%.c
	$(CC) $(CFLAGS) $<

clean:
	$(RM) $(DES_FILE)

