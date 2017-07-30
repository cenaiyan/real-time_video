#include<iostream>
using namespace std;


#ifndef _PROTOCOL_HANDLER_
#define _PROTOCOL_HANDLER_
class ProtocolHandler{

	public:
		virtual unsigned char* handlerData(unsigned char buffer[],long&recv){
			cout<<"ProtocolHandler---"<<endl;
		}

		virtual ~ProtocolHandler(){
			cout<<"~ProtocolHandler"<<endl;
		}
};



#endif
