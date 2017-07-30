#include<iostream>
#include"DataSource.hpp"
#include<string>
#include "ProtocolHandler.hpp"
#include<boost/thread.hpp>
using namespace std;
//using namespace boost;
#ifndef _NET_SOURCE
#define _NET_SOURCE
#define NET_BUFFER_SIZE 1024*500
class NetSource:public DataSource{
	public:
		NetSource(string name,int port,void (*p)(unsigned char buffer[],int total,int recv));
		NetSource(string name,void (*p)(unsigned char buffer[],int total,int recv));
		virtual ~NetSource();
		void setProtocol(ProtocolHandler *protocol);	
		void init();
		void start();
	        void stop();
//int getData(char data_buf[],int buffer_length,int&real_size);	
		void destory();
	private:
		void mainLoopThread();
		unsigned char recivieBur[NET_BUFFER_SIZE]={0};
		ProtocolHandler *protocol = NULL;
		int netFd;
		string address="127.0.0.1";
		int port=8888;
		int isLoop = 1;
		int isReady = 0;
};

#endif


