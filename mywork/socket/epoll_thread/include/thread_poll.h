#include"socket.h"
#include<sys/epoll.h>
#pragma once
#define _MAXSOCKET 40000
#define _IP "10.0.0.129"
#define _PORT 8080
#define _BUFSIZE 1500
#define _BACKLOG 128
#define _INCREASE_THREAD 10
#define _THREAD_REDUCE 10
typedef void*(*FB)(void*);
//业务类型
typedef struct
{
	FB Business_Addr;
	void* arg;
}bs_t;
//线程池类型
typedef struct
{
	int Thread_Shutdown;//开关
	int Thread_Max;//线程池最大数量
	int Thread_Min;//线程池最小数量
	int Thread_Alive;//线程池中线程的存活数量
	int Thread_Busy;//线程池中繁忙的线程数量
	int Thread_flag;//缩减数量
	int Front;//队列头
	int Rear;//队列尾
	int Cur;//当前线程数量
	int Max;//队列的最大值
	bs_t* Container;//任务队列
	pthread_cond_t Not_Full;
	pthread_cond_t Not_Empty;
	pthread_mutex_t queue_lock;
	pthread_mutex_t var_lock;
	pthread_t mag_tid;//管理者线程
	pthread_t* cus_tids;//消费者线程链表
}tp_t;
//设置分离态线程属性
pthread_attr_t thread_attr;
//全局数组
int clientfd_list[_MAXSOCKET];
//全局监听描述符
int epfd;
//全局的锁,锁全局资源
pthread_mutex_t global_lock;
tp_t* TP_Create(int Tmax,int Tmin,int Cmax);//线程池创建和初始化数据
int TP_Destroy(tp_t*tp,int sockfd);//释放线程池的内存资源
int TP_Netinit(void);//网络初始化,返回服务器套接字
int TP_Epoll_Create(int sockfd);//创建监听,进行监听设置
int TP_Epoll_Listen(int sockfd,tp_t*tp);//负责持续边缘触发监听(生产者)
int TP_Producer(tp_t*tp,bs_t business);//执行一次添加一次任务
void* TP_Customer(void*arg);//消费者线程默认工作
void* TP_Manager(void* arg);//管理者线程工作
void* Accept_business(void* arg);//TCP连接业务
void* Forward_business(void* arg);//中转业务
