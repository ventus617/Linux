#pragma once
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<errno.h>
#include<sys/wait.h>
#include<sys/fcntl.h>
#include<pthread.h>
#include<signal.h>
#include<ctype.h>
int SOCKET(int domain,int type,int protocol);

int BIND(int sockfd,struct sockaddr_in addr);

int LISTEN(int sockfd,int backlog);

int CONNECT(int sockfd,struct sockaddr_in addr);

int ACCEPT(int sockfd,struct sockaddr_in *addr);

ssize_t SEND(int sockfd,const void* buf,size_t len,int flags);

ssize_t RECV(int sockfd,void*buf,size_t len,int flags);
