#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/fcntl.h>

void change_str(void)
{
	char* str="hello world";
	str[0]="W";
	printf("%s\n",str);
}

void test_func(void)
{
	change_str();
}

int main(void)
{
	printf("output 01\n");
	printf("output 02\n");
	test_func();
	printf("output 03\n");
	return 0;
}
