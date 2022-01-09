#ifndef __QQ__
#define __QQ__
#include<mutex>
template<typename randomType>
class qQueue{
	private:
		__int32 s,e,sz,rsz;//sz : size, rsz : real size
		__int32 blockSize;
		std::mutex mtx;
	public:
		randomType *q, *t;
		
	public:qQueue(const __int32 blockSize=8){
		s=e=sz=0;
		this->blockSize=blockSize;
		rsz=blockSize;
		q=new randomType[blockSize];
	}
	public:~qQueue(){
		delete[] q;
	}
	
	public:void push(const randomType x){
		mtx.lock();
		if(sz==rsz){
			t=new randomType[rsz+blockSize];
			for(int i=0, iter=s;i<sz;i++){
				t[i]=q[iter++];
				if(iter==sz)iter=0;
			}
			delete[] q;
			q=t;
			s=0;e=sz;
			rsz+=blockSize;
		}
		sz++;
		q[e++]=x;
		if(e==rsz)e=0;
		mtx.unlock();
	}
	public:void pop(){
		mtx.lock();
		if(!sz){
			mtx.unlock();
			return;
		}
		if(sz==rsz-blockSize+1 && rsz>blockSize){
			t=new randomType[rsz-blockSize];
			for(int i=0, iter=(s+1)%rsz;i<rsz-blockSize;i++){
				t[i]=q[iter++];
				if(iter==rsz)iter=0;
			}
			delete[] q;
			q=t;
			s=0;e=0;
			rsz-=blockSize;
		}else{
			s++;
			if(s==rsz)s=0;
		}
		
		sz--;
		mtx.unlock();
	}
	public:randomType front(){
		if(!sz)return 0;
		mtx.lock();
		randomType a=q[s];
		mtx.unlock();
		return a;
	}
	public:inline __int32 size(){return sz;}
	public:inline bool empty(){return !sz;}
	public:__int32 getRsz(){return rsz;}
	public:__int32 getS(){return s;}
	public:__int32 getE(){return e;}
};
#endif//__QQ__
