#include "DecodeUtil.hpp"
using namespace std;
using namespace cv;

DecodeUtil::DecodeUtil(){
	init();
}


DecodeUtil::~DecodeUtil(){
	unInit();
}


void DecodeUtil::init(){
	isReady = 0;
	av_register_all();
	avcodec_register_all();
	av_init_packet(&avpkt);
	codec = avcodec_find_decoder(AV_CODEC_ID_H264);
	if(!codec){
		cout<<"can 't not find the codec "<<endl;
		return ;
	}

	codecCtx = avcodec_alloc_context3(codec);

	if(!codec){
		cout<<"can't not find the content of codec"<<endl;
		return ;
	}

	paseCtx = av_parser_init(AV_CODEC_ID_H264);
	if(codec->capabilities&CODEC_CAP_TRUNCATED){
		codecCtx->flags|= CODEC_FLAG_TRUNCATED;   
	}
	if (avcodec_open2(codecCtx, codec, NULL) < 0) {  
        	cout<<"can not open codec"<<endl;
		return;  
    	}  

	picture = av_frame_alloc();
	Frame = av_frame_alloc();
	if(picture == NULL || Frame == NULL){
		cout<<"can not alloc a Frame"<<endl;
		return ;
	}

	imgConverCtx = NULL;
	isReady = 1;

}

void DecodeUtil::unInit(){
	avcodec_close(codecCtx);
	if(codecCtx != NULL){
		av_free(codecCtx);
		codecCtx =NULL;
	}
	
	if(picture != NULL){
		av_frame_free(&picture);  
		picture = NULL;
	}

	if(Frame != NULL){
		av_frame_free(&Frame);  
		picture = NULL;
	}

	if(imgConverCtx != NULL){
		sws_freeContext(imgConverCtx);
		imgConverCtx = NULL;
	}

	if(paseCtx != NULL){
		av_parser_close(paseCtx);
		paseCtx = NULL;
	}

	isReady = 0;

}

/*
void DecodeUtil::frameToMat(cv::Mat *mat.int width,int height,SwsContext *sws,AVFrame *pFrame){



}*/



cv::Mat DecodeUtil::decodeData(unsigned char *buf,int buf_len){

	int got,len,paser_len;
	cv::Mat mat;
	cv::Mat *pMat = NULL;
	if( isReady == 0 ){
		cout<<"is not ready cccccccccccccc"<<endl;
		return mat;
	}
	if(buf == NULL||buf_len == 0){
		
		cout<<"buff is null cccccccccccccc"<<endl;
		return mat;
	}
	
	avpkt.data = (uint8_t *)buf;
	avpkt.size = buf_len;

	paser_len = av_parser_parse2(paseCtx,codecCtx,&avpkt.data,&avpkt.size,(uint8_t *)buf,buf_len,AV_NOPTS_VALUE,AV_NOPTS_VALUE,AV_NOPTS_VALUE);

	if(paser_len == 0){
		int type = 0;
		switch(paseCtx->pict_type){  
		        case AV_PICTURE_TYPE_I: cout<<"Type: I"<<endl;type=1;break;  
		        case AV_PICTURE_TYPE_P: cout<<"Type: P"<<endl;break;  
		        case AV_PICTURE_TYPE_B: cout<<"Type: B"<<endl;break;  
		        default: cout<<"Type: Other"<<endl;break;  
		}  
		len = avcodec_decode_video2(codecCtx,picture,&got,&avpkt);
		if(len < 0){
			cout<<"codec decode fail-----"<<endl;
			return mat; 
		}
/*		if(type!=1){
			return mat;
		}*/

        	 int picBytes = avpicture_get_size(AV_PIX_FMT_BGR24, codecCtx->width, codecCtx->height);  
        	 uint8_t *picBuf = new uint8_t[picBytes];  
        	 avpicture_fill((AVPicture *)Frame, picBuf, AV_PIX_FMT_BGR24,codecCtx->width, codecCtx->height);  
		 pMat = new cv::Mat();
	       	 pMat->create(cv::Size(codecCtx->width,codecCtx->height),CV_8UC3);	 
            	 if(imgConverCtx == NULL){
		       		 
                	imgConverCtx = sws_getContext(codecCtx->width, codecCtx->height, codecCtx->pix_fmt, codecCtx->width, codecCtx->height, AV_PIX_FMT_BGR24, SWS_BICUBIC, NULL, NULL, NULL);  
            	 }  
  
            	picture->data[0] += picture->linesize[0]*(codecCtx->height-1);  
            	picture->linesize[0] *= -1;                        
            	picture->data[1] += picture->linesize[1]*(codecCtx->height/2-1);  
            	picture->linesize[1] *= -1;  
            	picture->data[2] += picture->linesize[2]*(codecCtx->height/2-1);  
            	picture->linesize[2] *= -1; 
            	sws_scale(imgConverCtx, (const uint8_t* const*)picture->data, picture->linesize, 0, codecCtx->height, Frame->data, Frame->linesize);   
		memcpy(pMat->data,picBuf,picBytes);
//		av_free(Frame);
		delete[] picBuf;
		
		return *pMat;
	}

	if(paser_len == 0){  

	paser_len = av_parser_parse2(paseCtx,codecCtx,&avpkt.data,&avpkt.size,(uint8_t *)buf,buf_len,AV_NOPTS_VALUE,AV_NOPTS_VALUE,AV_NOPTS_VALUE);
	}  

	cout<<"paser_len is not zero ccccccssss"<<endl;
	return mat;	

}
