#include<iostream>
#include "cv.h"
#include "highgui.h"
extern "C"
{
#include "libavformat/avformat.h"
#include "libswresample/swresample.h"
#include "libavutil/opt.h"
#include "libavutil/channel_layout.h"
#include "libavutil/parseutils.h"
#include "libavutil/samplefmt.h"
#include "libavutil/fifo.h"
#include "libavutil/intreadwrite.h"
#include "libavutil/dict.h"
#include "libavutil/mathematics.h"
#include "libavutil/pixdesc.h"
#include "libavutil/avstring.h"
#include "libavutil/imgutils.h"
#include "libavcodec/avcodec.h"
#include "libavfilter/avfilter.h"
#include "libavfilter/buffersrc.h"
#include "libavfilter/buffersink.h"
#include "libswscale/swscale.h"
}
using namespace std;
using namespace cv;
#ifndef _DECODE_UTIL_
#define _DECODE_UTIL_
class DecodeUtil{
	private:
		SwsContext *imgConverCtx;
		AVCodec *codec;
		AVCodecContext *codecCtx;
		AVCodecParserContext *paseCtx;
		int frame_count;
		AVFrame *picture;
		AVFrame *Frame;		
		AVPacket avpkt;
		int Width;
		int height;
//		void frameToMat(cv::Mat *mat,int width,int height,SwsContext *sws,AVFrame *pFrame);
		void init();
		void unInit();
		int isReady;
	public:
		cv::Mat decodeData(unsigned char *buf,int buf_len);	
		DecodeUtil();
		~DecodeUtil();

};

#endif
