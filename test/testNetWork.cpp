#include<iostream>
#include "NetSource.hpp"
#include<stdio.h>
#include <unistd.h>
#include "rtpH264Protocol.hpp"
#include<fstream>
#include<sstream>
#include "cv.h"
#include "highgui.h"
#include "DecodeUtil.hpp"
using namespace std;
using namespace cv;
ofstream *pWrite = NULL;
DecodeUtil *pDecoder = NULL;
int countData = 0;
void handlerData(unsigned char buffer[],int total,int recv){
/*
	for(int i=0 ; i< recv ;i++){
		cout<<hex<<(short int)(buffer[i])<<" ";
	}
	cout<<endl;
*/	

	cout<<"=============================recv:"<<recv<<endl;
	
	if(pWrite!=NULL){
		pWrite->write((char *)buffer,recv);
	}
	if(pDecoder != NULL){
		try{
			cv::Mat mat = pDecoder->decodeData(buffer,recv);
			cout<<"get mat --------------------------------"<<endl;
			if(!mat.empty()){
	
				IplImage img(mat);
				cvShowImage("testWindows",&img);
			}			
			char key = cv::waitKey(1);
//			char key = cv::waitKey(10);
		}catch(cv::Exception e){
			cout<<"xxxxxxrror is "<<e.what()<<endl;
		}
	}
}


int main(int argc,char **argv){
	pWrite = new ofstream("test.h264",std::ios::binary);
	pDecoder = new DecodeUtil();
	NetSource *netWork = new NetSource("rtp",handlerData);	
	ProtocolHandler *protocol = new RtpH264Protocol();
	cvNamedWindow("testWindows",CV_WINDOW_NORMAL);
	netWork->setProtocol(protocol);
	netWork->init();

	netWork->start();

	getchar();

	netWork->stop();
	netWork->destory();
	delete netWork;
	pWrite->close();
	delete pWrite;
	delete pDecoder;
	cvDestroyAllWindows();
	return 0;

}
