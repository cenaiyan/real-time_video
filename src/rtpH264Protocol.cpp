#include<iostream>
#include<string.h>
#include<stdio.h>
#include "rtpH264Protocol.hpp"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>
//ofstream *pWrite = NULL;
unsigned char* RtpH264Protocol::handlerData(unsigned char buffer[],long&recv){
	RTPpacket_t packData;
	int hasRecvFling = 0;
	RTP_FIXED_HEADER* rtpHdr =NULL;
	rtpHdr = (RTP_FIXED_HEADER *)&buffer[0];
    	printf("所有数据的长度 : %ld\n",recv);
	printf("版本号     : %u\n",rtpHdr->version);  
	packData.version  = rtpHdr->version;  
   	packData.padding  = rtpHdr->padding;  
  	packData.extension  = rtpHdr->extension;  
   	packData.cc = rtpHdr->csrc_len;  
   	printf("标志位     : %u\n",rtpHdr->marker);  
   	packData.marker = rtpHdr->marker;  
   	printf("负载类型    :%u\n",rtpHdr->payloadtype);  
   	packData.pt = rtpHdr->payloadtype;  
   	printf("包号      : %u\n",rtpHdr->seq_no);  
   	packData.seq_no = rtpHdr->seq_no;  
   	printf("时间戳     : %u\n",rtpHdr->timestamp);  
   	packData.timestamp = rtpHdr->timestamp;  
   	printf("帧号      : %u\n",rtpHdr->ssrc);  
   	packData.ssrc = rtpHdr->ssrc;  	
	NALU_HEADER *nalu_hdr =(NALU_HEADER *)&buffer[12];
       	printf("forbidden_zero_bit: %u\n",nalu_hdr->F);           
	printf("nal_reference_idc:  %u\n",nalu_hdr->NRI);  
	printf("nal 负载类型:       %u\n",nalu_hdr->TYPE); 
	//删除上一次的数据
/*	if(pWrite == NULL){

		pWrite = new ofstream("test.h264",std::ios::binary);
	}*/
	if(currentState != 2){
		if(pBuffer!=NULL){
			cout<<" currentState !=2 delete pbuffer *************"<<endl;
			delete[] pBuffer;
			currentBuffer = 0;
		}

		pBuffer = NULL;
		currentBuffer = 0;
	}
	if(nalu_hdr->TYPE == 0){
		std::cout<<"package is error"<<endl;
		currentState = 0;
	}else if(nalu_hdr->TYPE > 0 && nalu_hdr->TYPE<24){
		std::cout<<" 单片"<<endl;
		currentState = 1;
	}else if(nalu_hdr->TYPE == 24){
		std::cout<<" stap-a"<<endl;
		currentState = 0;
	}else if(nalu_hdr->TYPE == 25){
		std::cout<<" stap-b"<<endl;
		currentState = 0;
	}else if(nalu_hdr->TYPE == 27){
		std::cout<<" mtap24"<<endl;	
		currentState = 0;
	}else if(nalu_hdr->TYPE == 28){
		cout<<"FU-A package:"<<endl;
		FU_INDICATOR *fu_ind;
		FU_HEADER *fu_hdr;
        	fu_ind=(FU_INDICATOR*)&buffer[12];     //分片包用的是FU_INDICATOR而不是NALU_HEADER  
        	printf("\tFU_INDICATOR->F     :%d\n",fu_ind->F);  
       
        	printf("\tFU_INDICATOR->NRI   :%d\n",fu_ind->NRI);  
       
        	printf("\tFU_INDICATOR->TYPE  :%d\n",fu_ind->TYPE);  
       
        	fu_hdr=(FU_HEADER*)&buffer[13];        //FU_HEADER赋值  
        	printf("\tFU_HEADER->S        :%d\n",fu_hdr->S);  
        	printf("\tFU_HEADER->E        :%d\n",fu_hdr->E);  
        	printf("\tFU_HEADER->R        :%d\n",fu_hdr->R);  
        	printf("\tFU_HEADER->TYPE     :%d\n",fu_hdr->TYPE);  
		    //rtpHdr->marker
        	if(rtpHdr->marker == 1)                      //分片包最后一个包  
        	{  
        	    	//currentState =3;
            		hasRecvFling = 2;
			printf("\t当前包为FU-A分片包最后一个包\n");  
        	}else if (rtpHdr->marker == 0)                 //分片包 但不是最后一个包  
        	{  
            		if (fu_hdr->S == 1)                        //分片的第一个包  
            		{  
				currentState =2;
				printf("\t当前包为FU-A分片包第一个包\n");  
            		}else{                               
            			hasRecvFling = 1;
				printf("\t当前包为FU-A分片包中间包\n");  
			}     
        	}  
	}else if(nalu_hdr->TYPE == 29){
		 currentState = 0;
		 if(rtpHdr->marker == 1)                  //分片包最后一个包  
       		 {  
            		printf("当前包为FU-B分片包最后一个包\n");  
  
        	 }else if (rtpHdr->marker == 0)             //分片包 但不是最后一个包  
        	{  
            		printf("当前包为FU-B分片包\n");  
        	}  

	}
	
	if(currentState != 2){
		if(pBuffer!=NULL){
			cout<<" currentState !=2 delete pbuffer *************"<<endl;
			delete[] pBuffer;
			currentBuffer = 0;
		}

		pBuffer = NULL;
		currentBuffer = 0;
	}
	if(currentState == 0){
		cout<<" currentState is zero return null"<<endl;
		return NULL;	
	}

	if(currentState == 1){
		cout<<"nal  vvvvvvvvvvvvvvvvvvvvvvvv"<<endl;
		//分配单包内存
		//pBuffer = new unsigned char[recv-13+4];
		pBuffer = new unsigned char[recv-13+4+1];
		pBuffer[0]=0;
		pBuffer[1]=0;
		pBuffer[2]=0;
		pBuffer[3]=1;
		//memcpy(&pBuffer[4],&buffer[13],recv-13);

		memcpy(&pBuffer[4],nalu_hdr,1);

		memcpy(&pBuffer[5],&buffer[13],recv-13);
/*		for(int i=0 ; i <recv-13+4 ;i++){
			cout<<hex<<(int)pBuffer[i]<<" ";
		}*/

	/*	
		if(pWrite!=NULL){
			pWrite->write((char *)pBuffer,recv-13+4);
		}
		cout<<endl;*/
		//currentBuffer = recv-13+4;
		currentBuffer = recv-13+4+1;
		recv=currentBuffer;
		return pBuffer;
	}

	if(currentState == 2 && hasRecvFling == 0){
		//分配fu-a内存头包内存
		//cout<<"fu-a start---------------------------"<<endl;
		if(pBuffer != NULL){
			delete[] pBuffer;
			cout<<"delete pBuffer ----fua"<<endl;
		       	currentBuffer=0;	
		}else{
			cout<<"pBuffer is null ---fua"<<endl;
		}
		unsigned char F;  
	        unsigned char NRI;  
		unsigned char TYPE;  
		unsigned char nh;  	
		FU_INDICATOR *fu_ind;
		FU_HEADER *fu_hdr;
        	fu_ind=(FU_INDICATOR*)&buffer[12];     //分片包用的是FU_INDICATOR而不是NALU_HEADER  
		fu_hdr = (FU_HEADER *)&buffer[13];
		F = fu_ind->F << 7;  
		NRI = fu_ind->NRI << 5;
		TYPE = fu_hdr->TYPE;      
		nh = F | NRI | TYPE;  		
		cout<<"fu-a start---------------------------"<<endl;
		pBuffer = new unsigned char[recv-14+4+1];	
		pBuffer[0]=0;
		pBuffer[1]=0;
		pBuffer[2]=0;
		pBuffer[3]=1;

		memcpy(&pBuffer[4],&nh,1);
		memcpy(&pBuffer[5],&buffer[14],recv-14);
		currentBuffer = recv-14+4+1;
		//currentBuffer = recv-14;
		recv=currentBuffer;
		return NULL;
	}

	if(hasRecvFling == 2){
		if(currentState == 2){
		//完成组包
			
			cout<<"fu-a end eeeeeeeeeeeeeeeeeeeeeee"<<endl;
			pBuffer = memMalloc(pBuffer,currentBuffer+recv-14,0,currentBuffer);
			memcpy(&pBuffer[currentBuffer],&buffer[14],recv-14);
			currentBuffer = currentBuffer+recv-14;
			recv=currentBuffer;	
		}
	        return pBuffer;
	}


	if(hasRecvFling == 1){
		if(currentState == 2){
			//分配内存给分包
			cout<<"fu-a ing iiiiiiiiiiiiiiiiiiiiiiiiiiiiiii"<<endl;
			pBuffer = memMalloc(pBuffer,currentBuffer+recv-14,0,currentBuffer);
			memcpy(&pBuffer[currentBuffer],&buffer[14],recv-14);
			currentBuffer = currentBuffer+recv-14;
			recv=currentBuffer;	

		}
		return NULL;		
	}


	return NULL;
}


unsigned char * RtpH264Protocol::memMalloc(unsigned char *p,int total,int offset,int current){

	if(p == NULL){
		return new unsigned char[total];
	}

	unsigned char *pNewBuffer = NULL;
	pNewBuffer= new unsigned char[total];

	if(current > 0 && total>=current+offset){
		memcpy(&pNewBuffer[offset],p,current);
	}
	delete[] p;
	return pNewBuffer;
}
RtpH264Protocol::~RtpH264Protocol(){
	if(pBuffer != NULL){
		delete[] pBuffer;
	}
	/*
	if(pWrite != NULL){
		pWrite->close();
	}*/
			
}	

