#include<iostream>
#include<string>
using namespace std;
#ifndef _DATA_SOURCE_HPP
#define _DATA_SOURCE_HPP
class DataSource{

	protected:
		string sourceName;
		void (*pDataHandler)(unsigned char buffer[],int total,int recv) = NULL;
	public:
		DataSource(string name,void (*p)(unsigned char buffer[],int bufferlength,int recvLen)=NULL){
		
			this->sourceName = name;
		
			this->pDataHandler = p;
		}

		virtual ~DataSource(){
			cout<<"~DataSource"<<endl;
		}

		string getName(){
			return this->sourceName;
		}

		virtual void start(){
			cout<<"start the DataSource"<<endl;
		}

		virtual void stop(){
			cout<<"stop the DataSource"<<endl;

		}	

/*		virtual int getData(char data_buf[],int buf_size,int&real_size){
			cout<<"Data Source get data"<<endl;
		}
*/

		virtual void init(){
			cout<<"init the DataSource"<<endl;
		}
	
		virtual void destory(){

			cout<<"destory the DataSource"<<endl;
		}


};
#endif
