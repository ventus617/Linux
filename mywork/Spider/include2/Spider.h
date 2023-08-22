#pragma once
#include"Bloom_Filter.h"
#include<sys/socket.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<openssl/ssl.h>
#include<openssl/err.h>
#include<regex.h>
#include<sys/mman.h>
#include<iostream>
#include<map>
using namespace std;
#define Result_Max 100


typedef struct
{
	SSL* sslsocket;//安全套接字
	SSL_CTX *sslctx;//认证信息
}SSL_t;


//用于存储url结构体信息的内容
typedef struct
{
	char origin_url[0x1000];//原始地址
	char domain[1024];//域名
	char sava_path[1024];//存储路径
	char src_name[1024];//资源名
	char ip[16];//公网IP
	int port;//端口号
	int type;//[1]对应https 443 [0]对应http 80
}url_t;

//循环队列
typedef struct
{
	int max;
	int cur;
	int front;
	int rear;
	url_t*url_list;
}Container;

int Analyse_url(url_t* url,map<unsigned int,string>&hostmap);//对原始地址进行分析，提取关键要素
int Connect_Web(url_t*url,int sockfd);//与WEB服务进行TCP链接,传入链接的sockfd
int TP_Netinit(void);//网络初始化
int Download(url_t* url,int webfd,char*request,SSL_t*ssl);//交互下载流程
int Create_request(char* request,url_t* url);//传入请求地址,传出请求内容
SSL_t* Spider_Openssl_Init(int sockfd);//使用sockfd完成OpenSSL的认证(三次握手之后才会SSL认证进行数据交互)
int Spider_Get_Code(const char* response);//从响应头中提取响应码
int Container_set(Container* ct,url_t url);//入队处理
int Container_get(Container*ct,url_t* url);//出队处理
int RemoveDuplication(Container*yct,Container*wct,char* new_link);//1是不重复 0是重复 处理解析出来的url地址
int Analyze_html(url_t* url,Container* wct,Container* yct,int& analyze_shutdown,int& result_num,unsigned int* bit_array,int result_fd);//网页源码解析,分析标题，描述，新链接，持久化，新链接循环处理
Container* Create_Container(int num);//创建出队和入队队列
int Save_result(const char* title,const char* description,int result_fd,int & result_num);//用html格式存储在表格中
int Remove_Bloom( char* str,int len,unsigned int* bit_array);
int Insert_Bloom( char* str,int len,unsigned int* bit_array);
