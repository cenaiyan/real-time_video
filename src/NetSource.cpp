#include<iostream>
#include<boost/thread.hpp>
#include "NetSource.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
using namespace std;
//using namespace boost;

NetSource::NetSource(string name,void (*p)(unsigned char buffer[],int total,int recv)):DataSource(name,p){

	netFd=-1;
	address="127.0.0.1";
	port=8888;
	protocol = NULL;
	isLoop = 0;
	isReady = 0;
}


NetSource::NetSource(string name,int port,void (*p)(unsigned char buffer[],int total,int recv)):DataSource(name,p){

	netFd=-1;
	address="127.0.0.1";
	this->port=port;
	isLoop = 0;
	isReady = 0;
	protocol = NULL;
}

void NetSource::init(){
	//get the socket fd
	sockaddr_in local_addr;
	//sockaddr_in remote_addr;

	netFd=socket(AF_INET,SOCK_DGRAM,0);

	if(netFd == -1){
		cout<<"fail to get socket"<<endl;
		return ;
	}

	memset(&local_addr,0,sizeof(local_addr));
	local_addr.sin_family = AF_INET;
	local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	local_addr.sin_port = htons(this->port);
	if(bind(netFd,(struct sockaddr *)&local_addr,sizeof(local_addr)) == -1){
		cout<<"bind the port fail"<<endl;
		close(netFd);
		netFd = -1;
		return;
	}

	isReady = 1;

}


	
void NetSource::start(){
	if(!isReady){
		return;
	}
	
	isLoop = 1;
	boost::thread t1(boost::bind(&NetSource::mainLoopThread,this));
	t1.detach();
	return;
} 

void NetSource::setProtocol(ProtocolHandler *pro){
	this->protocol = pro;
}

void NetSource::stop(){

	if(!isReady){
		return;
	}
	isLoop = 0;
}


void NetSource::destory(){

	if(!isReady){
		return ;
	}

	if(netFd != -1){
		close(netFd);
	}
	isReady = 0;
}

NetSource::~NetSource(){
	destory();
	netFd=-1;
	address="127.0.0.1";
	port=8888;
	isLoop = 0;
	if(protocol != NULL){
		delete protocol;
	}
	protocol = NULL;
	isReady = 0;
	


}

void NetSource::mainLoopThread(){

	sockaddr_in remoteaddr;
	int sockeLen = sizeof(remoteaddr);
	while(isLoop){
		unsigned char *pBuffer = recivieBur;
		long recvNum = recvfrom(netFd,recivieBur,sizeof(recivieBur),0,(sockaddr*)&remoteaddr,(socklen_t*)&sockeLen);
		if(recvNum <= 0){
			continue;
		}
		
		if(NULL!=protocol){
			//TODO handle the data in protocol
			pBuffer = protocol->handlerData(pBuffer,recvNum);			

			if(pBuffer == NULL){
				continue;
			}
		}

		if(pDataHandler != NULL){
			pDataHandler(pBuffer,sizeof(recivieBur),recvNum);
		}



	}



}

