#include<iostream>
//using namespace std;
#include "ProtocolHandler.hpp"

#ifndef _RTP_H264_
#define _RTP_H264_
//rtp header fix
typedef struct   
{  
    /*  0                   1                   2                   3 
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
    |V=2|P|X|  CC   |M|     PT      |       sequence number         | 
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
    |                           timestamp                           | 
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
    |           synchronization source (SSRC) identifier            | 
    +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ 
    |            contributing source (CSRC) identifiers             | 
    |                             ....                              | 
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
    */  
    //intel 的cpu 是intel为小端字节序（低端存到底地址） 而网络流为大端字节序（高端存到低地址）  
    /*intel 的cpu ： 高端->csrc_len:4 -> extension:1-> padding:1 -> version:2 ->低端 
     在内存中存储 ： 
     低->4001（内存地址）version:2 
         4002（内存地址）padding:1 
         4003（内存地址）extension:1 
     高->4004（内存地址）csrc_len:4 
 
     网络传输解析 ： 高端->version:2->padding:1->extension:1->csrc_len:4->低端  (为正确的文档描述格式) 
 
     存入接收内存 ： 
     低->4001（内存地址）version:2 
         4002（内存地址）padding:1 
         4003（内存地址）extension:1 
     高->4004（内存地址）csrc_len:4 
     本地内存解析 ：高端->csrc_len:4 -> extension:1-> padding:1 -> version:2 ->低端 ， 
     即： 
     unsigned char csrc_len:4;        // expect 0  
     unsigned char extension:1;       // expect 1 
     unsigned char padding:1;         // expect 0  
     unsigned char version:2;         // expect 2  
    */  
    /* byte 0 */  
     unsigned char csrc_len:4;        /* expect 0 */  
     unsigned char extension:1;       /* expect 1, see RTP_OP below */  
     unsigned char padding:1;         /* expect 0 */  
     unsigned char version:2;         /* expect 2 */  
    /* byte 1 */  
     unsigned char payloadtype:7;     /* RTP_PAYLOAD_RTSP */  
     unsigned char marker:1;          /* expect 1 */  
    /* bytes 2,3 */  
     unsigned int seq_no;              
    /* bytes 4-7 */  
     unsigned int timestamp;          
    /* bytes 8-11 */  
     unsigned int ssrc;              /* stream number is used here. */  
} RTP_FIXED_HEADER;  

//---------------------------------rtp header struct--------------------------------
typedef struct  
{  
    unsigned char version;              //!< Version, 2 bits, MUST be 0x2  
    unsigned char padding;              //!< Padding bit, Padding MUST NOT be used  
    unsigned char extension;            //!< Extension, MUST be zero  
    unsigned char cc;                   //!< CSRC count, normally 0 in the absence of RTP mixers           
    unsigned char marker;               //!< Marker bit  
    unsigned char pt;                   //!< 7 bits, Payload Type, dynamically established  
    unsigned int seq_no;                //!< RTP sequence number, incremented by one for each sent packet   
    unsigned int timestamp;        //!< timestamp, 27 MHz for H.264  
    unsigned int ssrc;             //!< Synchronization Source, chosen randomly  
    unsigned char * payload;      //!< the payload including payload headers  
    unsigned int paylen;           //!< length of payload in bytes  
} RTPpacket_t;

typedef struct  
{  
    unsigned char forbidden_bit;           //! Should always be FALSE  
    unsigned char nal_reference_idc;       //! NALU_PRIORITY_xxxx  
    unsigned char nal_unit_type;           //! NALU_TYPE_xxxx    
    unsigned int startcodeprefix_len;      //! 前缀字节数  
    unsigned int len;                      //! 包含nal 头的nal 长度，从第一个00000001到下一个000000001的长度  
    unsigned int max_size;                 //! 做多一个nal 的长度  
    unsigned char * buf;                   //! 包含nal 头的nal 数据  
    unsigned int lost_packets;             //! 预留  
} NALU_t;  
  
/* 
+---------------+ 
|0|1|2|3|4|5|6|7| 
+-+-+-+-+-+-+-+-+ 
|F|NRI|  Type   | 
+---------------+ 
*/  
typedef struct   
{  
    //byte 0  
    unsigned char TYPE:5;  
    unsigned char NRI:2;  
    unsigned char F:1;          
} NALU_HEADER; // 1 BYTE   
  
/* 
+---------------+ 
|0|1|2|3|4|5|6|7| 
+-+-+-+-+-+-+-+-+ 
|F|NRI|  Type   | 
+---------------+ 
*/  
typedef struct   
{  
    //byte 0  
    unsigned char TYPE:5;  
    unsigned char NRI:2;   
    unsigned char F:1;                
} FU_INDICATOR; // 1 BYTE   
  
/* 
+---------------+ 
|0|1|2|3|4|5|6|7| 
+-+-+-+-+-+-+-+-+ 
|S|E|R|  Type   | 
+---------------+ 
*/  
typedef struct   
{  
    //byte 0  
    unsigned char TYPE:5;  
    unsigned char R:1;  
    unsigned char E:1;  
    unsigned char S:1;      
} FU_HEADER;   // 1 BYTES   


class RtpH264Protocol:public ProtocolHandler{
	private:
		// 0 is no state 1 is Nal 2 start is compuse the package 3 is end of compuse package;
		int currentState = 0;
		unsigned char *pBuffer = NULL;
		int currentBuffer = 0;
		unsigned char * memMalloc(unsigned char *p,int total,int offset=0,int current=0);
			
	public:
		unsigned char* handlerData(unsigned char buffer[],long&recv);
		~RtpH264Protocol();
			
};






#endif

