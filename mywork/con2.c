#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>

pthread_mutex_t lock;
int daystate;

pthread_cond_t Day;
pthread_cond_t Night;

void *thread_A(void* arg){
	pthread_detach(pthread_self());
	while(1){
		pthread_mutex_lock(&lock);
		while(daystate==1){
			printf("now is DAY,need daySafer\n");
			pthread_cond_wait(&Day,&lock);
		}
		printf("safer A is working in Night\n");
		daystate=1;
		pthread_cond_signal(&Night);
		pthread_mutex_unlock(&lock);
	}
	pthread_exit(0);
}

void *thread_B(void* arg){
	pthread_detach(pthread_self());
	while(1){
		pthread_mutex_lock(&lock);
		while(daystate==0){
			printf("now is Night,need NightSafer\n");
			pthread_cond_wait(&Night,&lock);
		}
		printf("safer B is working in Day\n");
		daystate=0;
		pthread_cond_signal(&Day);
		pthread_mutex_unlock(&lock);
	}
	pthread_exit(0);
}

int main(){
	daystate=1;
	if(pthread_cond_init(&Day,NULL)!=0||pthread_cond_init(&Night,NULL)!=0||pthread_mutex_init(&lock,NULL)!=0){
		printf("cont or lock init fail");
		exit(0);
	}
	pthread_t tid;
	if(((tid=pthread_create(&tid,NULL,thread_A,NULL))==-1)||(tid=pthread_create(&tid,NULL,thread_B,NULL)==-1))
	{
		printf("pthread create fail\n");
	}
	while(1) sleep(1);
	pthread_cond_destroy(&Day);
	pthread_cond_destroy(&Night);
	pthread_mutex_destroy(&lock);
	return 0;
}
