#ifndef __KC__
#define __KC__
#include<atomic>
#include<mutex>
#include<stdio.h>
#include<ctime>
#include"qQueue.h"

template<typename randomType>
inline bool getBit(randomType&target,unsigned int bit){
	return target>>bit&1;
}
template<typename randomType>
inline bool setBit(randomType&target,unsigned int bit,bool value){
	target=target&~((randomType)1<<bit) | ((randomType)value<<bit);
	return target>>bit&1;
}

union{
	unsigned __int64 src[256];
	__int8 byte[2048];
}keyCount;
std::atomic<unsigned __int32> createdKeyNum[256];
std::atomic<unsigned __int64> kps=0, total=0, tps=0;//Keys(hold) per Second, Tests per Second
qQueue<clock_t> kpsTimeQueue(64), tpsTimeQueue(256000);

void TF();

namespace keyCheck{
	std::atomic<unsigned __int16> keyState[16]={0,};
	bool run=false;
	const int areaNums=16;
	std::thread kcThrd[areaNums];
	std::thread popKpsTimeQueueThrd, popTpsTimeQueueThrd;
	
	void keyCheck(__int8 keyArea){//0~F
		int i;
		bool state;
		while(keyCheck::run){
			for(i=0;i<0xF;i++){
				if(GetKeyState(VK_SCROLL)&1){
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
					continue;
				}
				state=GetAsyncKeyState(keyArea<<4|i)&0x8000;
				tps++;
				tpsTimeQueue.push(clock());
				if(state^getBit(keyState[keyArea],i)){
					if(state){
						if(createdKeyNum[keyArea<<4|i]){
							keyCount.src[keyArea<<4|i]++;
							total++;
							if(!kpsAllKey){
								kps++;
								kpsTimeQueue.push(clock());
							}
						}
						if(kpsAllKey){
							kps++;
							kpsTimeQueue.push(clock());
						}
					}
					setBit(keyState[keyArea],i,state);
//					TF();
				}
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
//			std::this_thread::sleep_for(std::chrono::microseconds(100));
		}
	}
	
	/*
	void keyCheck(__int8 keyArea){//0~FF
		bool state;
		while(keyCheck::run){
			state=GetAsyncKeyState(keyArea)&0x8000;
			tps++;
			tpsTimeQueue.push(clock());
			if(state^getBit(keyState[keyArea>>4],keyArea&0xF)){
				if(state){
					keyCount[keyArea]++;
					if(kpsAllKey){
						kps++;
						kpsTimeQueue.push(clock());
					}
				}
				setBit(keyState[keyArea>>4],keyArea&0xF,state);
				TF();
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
//			std::this_thread::sleep_for(std::chrono::microseconds(100));
		}
	}*/
	
	void popKpsTimeQueue(){
		while(keyCheck::run){
			while(kpsTimeQueue.size()&&keyCheck::run){
				while(clock()-kpsTimeQueue.front()>=CLOCKS_PER_SEC){
					kpsTimeQueue.pop();
					kps--;
					if(kpsTimeQueue.empty())break;
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
	
	void popTpsTimeQueue(){
		while(keyCheck::run){
			while(tpsTimeQueue.size()&&keyCheck::run){
				while(clock()-tpsTimeQueue.front()>=CLOCKS_PER_SEC){
					tpsTimeQueue.pop();
					tps--;
					if(tpsTimeQueue.empty())break;
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
	
	bool beginKeyChk(){
		if(run)return 0;
		keyCheck::run=true;
		popKpsTimeQueueThrd=std::thread(keyCheck::popKpsTimeQueue);
		popTpsTimeQueueThrd=std::thread(keyCheck::popTpsTimeQueue);
		for(int i=0;i<areaNums;i++)keyCheck::kcThrd[i]=std::thread(keyCheck::keyCheck,i);
		return 1;
	}
	bool endKeyChk(){
		if(!run)return 0;
		keyCheck::run=false;
		popKpsTimeQueueThrd.join();
		popTpsTimeQueueThrd.join();
		for(int i=0;i<areaNums;i++)keyCheck::kcThrd[i].join();
		return 1;
	}
}
inline void TF(){
	static std::mutex mtx;
	mtx.lock();
	int i,j;
	for(i=0;i<12;i++){
		for(j=0;j<16;j++){
			print("%d",getBit(keyCheck::keyState[i],j));
		}
		print(" ");
	}
	print("\n");
	mtx.unlock();
}
#endif//__KC__
