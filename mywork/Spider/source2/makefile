.PHONY:SpiderCpp

CC=g++ -std=c++11 
INCLUDE_PATH=../include2
LIBRARY_PATH=-lssl -lcrypto
SRC_FILE=$(wildcard *.cpp)
DES_FILE=$(patsubst %.cpp,%.o,$(SRC_FILE))
TARGET=SpiderCpp
CFLAGS=-I$(INCLUDE_PATH) -g -Wall -c
RM=rm -rf

$(TARGET):$(DES_FILE)
	$(CC) $^ $(LIBRARY_PATH) -o $@

%.o:%.cpp
	$(CC) $(CFLAGS) $<

clean:
	$(RM) $(DES_FILE)

