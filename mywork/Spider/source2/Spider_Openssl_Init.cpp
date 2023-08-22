#include"Spider.h"

SSL_t* Spider_Openssl_Init(int sockfd)
{
	SSL_t* ssl=(SSL_t*)malloc(sizeof(SSL_t));//安全套接字
	//加载错误处理函数
	SSL_load_error_strings();
	//初始化openssl库函数
	SSL_library_init();
	//初始化加密方式和散列函数
	OpenSSL_add_ssl_algorithms();
	//创建认证信息
	ssl->sslctx=SSL_CTX_new(SSLv23_method());//里面放版本号函数
	//生成安全套接字
	ssl->sslsocket=SSL_new(ssl->sslctx);
	//使用sockfd对安全套接字设置，使得其知道服务器
	SSL_set_fd(ssl->sslsocket,sockfd);
	//进行安全认证过程
	SSL_connect(ssl->sslsocket);
	printf("[Download] [Step *] [OpenSSL Create,Safe Connection\n]");
	return ssl;
	//保留sslsocket,后续和服务器交互都是使用sslsocket
	//不使用传统的send，recv，使用SSL_read(ssl->sslsocket,buf,sizeof(buf)),SSL_write(ssl->sslsocket,buf,strlen(buf))
	//加密发送和读取解密
}
