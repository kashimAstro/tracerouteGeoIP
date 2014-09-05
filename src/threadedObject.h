#ifndef _THREADED_OBJECT
#define _THREADED_OBJECT

#include "ofMain.h"

class threadedObject : public ofThread{

	public:
		string nameHost;

		threadedObject(){
		}

		void start(){
	            startThread(true, false);
        	}

	        void stop(){
        	    stopThread();
	        }

		string setHost(string host){
			nameHost=host;
			return nameHost;
		}

		void threadedFunction(){
			while( isThreadRunning() != 0 ){
				if( lock() ){
					//
					cout<<"thread start>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"<<endl;
					vector<string> real;
					if( nameHost != "" ) {
					        string s = ofSystem("./trace.sh "+nameHost);
						cout<<"risultato-completo:"<<s<<endl;
					        vector<string> sp = ofSplitString(s, "\n");
					        for(int i = 0; i < sp.size(); i++){
					              if(sp[i] != "0,0"){
					                    real.push_back(sp[i]);
						      }
	          				}
					        vector<string> num = ofSplitString(real[0],",");
						ofSendMessage(num[0]+","+num[1]);
					}
					//
					unlock();
					ofSleepMillis(1 * 1000);
					stop();
				}
			}
		}
};

#endif
