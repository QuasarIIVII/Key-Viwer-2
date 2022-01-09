#ifndef __KV__
#define __KV__
#include<windows.h>
#include<thread>
#include<chrono>
#include<algorithm>
#include"KeyLabel.h"
#include"KeyCheck.h"

namespace View{
	typedef struct ltwh{SHORT l,t, w,h;}ltwh;//Left Top Width Height
	typedef struct POS{SHORT x,y;}POS;
	typedef struct keyColor{struct{COLORREF release,hold;} text,line,bg;}keyColor;
	
	typedef struct CreateParams{//struct for sending default information
		unsigned int key;
		ltwh pos;
		POS r;
		keyColor color;
		wchar_t* label;
		unsigned char* excludingKeys;//Only for 'Else Keys' (type 5)
	}CreateParams;
	
	typedef struct ViewMem{//struct for saving data
		unsigned int key;
		ltwh pos;
		POS r;
		keyColor color;
		
		bool run,paint;
		
		std::thread t;
		
		HDC hdc;
		HDC hmdc;
		HBITMAP hmbmp;
		HFONT hof;
		
		unsigned char* excludingKeys;//Only for 'Else Keys' (type 5)
	}ViewMem;
	
	inline void drawCount(ViewMem* mem){
		char buf[24];
		RECT rectRelease={0,mem->pos.h+mem->pos.h/2, mem->pos.w,mem->pos.h<<1};
		RECT rectHold={mem->pos.w,mem->pos.h+mem->pos.h/2, mem->pos.w<<1,mem->pos.h<<1};
		
		BitBlt(mem->hmdc,0,mem->pos.h, mem->pos.w<<1,mem->pos.h,mem->hmdc,0,0,SRCCOPY);
		sprintf(buf,"%d",keyCount.src[mem->key&0xFF]);
		SetTextColor(mem->hmdc,mem->color.text.release);	//Text color
		DrawText(mem->hmdc,buf,-1,&rectRelease,DT_CENTER);
		SetTextColor(mem->hmdc,mem->color.text.hold);	//Text color
		DrawText(mem->hmdc,buf,-1,&rectHold,DT_CENTER);
	}
	
	void ThrdViewr(ViewMem* mem){
//		print("%d %d %d %d\n",rect.left,rect.top,rect.right,rect.bottom);
//		rect={0,mem->pos.h/6,mem->pos.w-1,(mem->pos.h-1)/2};
		switch(mem->key>>29){
			case 0:{
				bool prestate,state;
				__int8 keyHigh=mem->key>>4, keyLow=mem->key&0xF;
				
				drawCount(mem);
//				BitBlt(mem->hdc,0,0,mem->pos.w,mem->pos.h,mem->hmdc, 0,mem->pos.h,SRCCOPY);
				while(mem->run){
//					state=GetAsyncKeyState(mem->key)&0x8000;
					state=getBit(keyCheck::keyState[keyHigh],keyLow);
					if(mem->paint){
						drawCount(mem);
						if(state)/*Hold*/BitBlt(mem->hdc,0,0,mem->pos.w,mem->pos.h,mem->hmdc, mem->pos.w,mem->pos.h,SRCCOPY);
						else/*Release*/  BitBlt(mem->hdc,0,0,mem->pos.w,mem->pos.h,mem->hmdc, 0,mem->pos.h,SRCCOPY);
						mem->paint=false;
					}
					if(prestate^state){
						drawCount(mem);
						if(state)/*Hold*/BitBlt(mem->hdc,0,0,mem->pos.w,mem->pos.h,mem->hmdc, mem->pos.w,mem->pos.h,SRCCOPY);
						else/*Release*/  BitBlt(mem->hdc,0,0,mem->pos.w,mem->pos.h,mem->hmdc, 0,mem->pos.h,SRCCOPY);
						prestate=state;
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
				}
				break;
			}
			case 1:{
				char buf[24];
				RECT rect={mem->pos.w,mem->pos.h>>1,mem->pos.w<<1,mem->pos.h};
				
				while(mem->run){
					BitBlt(mem->hmdc,mem->pos.w,0, mem->pos.w,mem->pos.h,mem->hmdc,0,0,SRCCOPY);
					sprintf(buf,"%lld",(__int64)kps);
					DrawText(mem->hmdc,buf,-1,&rect,DT_CENTER);
					
					BitBlt(mem->hdc,0,0,mem->pos.w,mem->pos.h,mem->hmdc, mem->pos.w,0,SRCCOPY);
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
				}
				break;
			}
			case 2:{
				char buf[24];
				RECT rect={mem->pos.w,mem->pos.h>>1,mem->pos.w<<1,mem->pos.h};
				
				while(mem->run){
					BitBlt(mem->hmdc,mem->pos.w,0, mem->pos.w,mem->pos.h,mem->hmdc,0,0,SRCCOPY);
					sprintf(buf,"%lld",(__int64)total);
					DrawText(mem->hmdc,buf,-1,&rect,DT_CENTER);
					
					BitBlt(mem->hdc,0,0,mem->pos.w,mem->pos.h,mem->hmdc, mem->pos.w,0,SRCCOPY);
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
				}
				break;
			}
			case 3:{
				char buf[16];
				RECT rect={mem->pos.w,mem->pos.h>>1,mem->pos.w<<1,mem->pos.h};
				
				time_t tim;
				tm TM;
				int preSec=-1;
				
				while(mem->run){
					time(&tim);
					TM=*localtime(&tim);
					if(TM.tm_sec^preSec | mem->paint){
						BitBlt(mem->hmdc,mem->pos.w,0, mem->pos.w,mem->pos.h,mem->hmdc,0,0,SRCCOPY);
						sprintf(buf,"%02d:%02d:%02d",TM.tm_hour,TM.tm_min,preSec=TM.tm_sec);
						DrawText(mem->hmdc,buf,-1,&rect,DT_CENTER);
						BitBlt(mem->hdc,0,0,mem->pos.w,mem->pos.h,mem->hmdc, mem->pos.w,0,SRCCOPY);
						mem->paint=false;
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}
				break;
			}
			case 4:{
				char buf[64];
				RECT rect={mem->pos.w,mem->pos.h>>1,mem->pos.w<<1,mem->pos.h};
				while(mem->run){
					BitBlt(mem->hmdc,mem->pos.w,0, mem->pos.w,mem->pos.h,mem->hmdc,0,0,SRCCOPY);
					sprintf(buf,"%05.2lf",(double)tps/256);
					DrawText(mem->hmdc,buf,-1,&rect,DT_CENTER);
					
					BitBlt(mem->hdc,0,0,mem->pos.w,mem->pos.h,mem->hmdc, mem->pos.w,0,SRCCOPY);
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
				}
				break;
			}
			case 6:{
				char buf[64];
				RECT rect={mem->pos.w,0,mem->pos.w<<1,mem->pos.h};
				while(mem->run){
					BitBlt(mem->hmdc,mem->pos.w,0, mem->pos.w,mem->pos.h,mem->hmdc,0,0,SRCCOPY);
					sprintf(buf,"%05.2lf",(double)tps/256);
					DrawText(mem->hmdc,buf,-1,&rect,DT_CENTER);
					
					BitBlt(mem->hdc,0,0,mem->pos.w,mem->pos.h,mem->hmdc, mem->pos.w,0,SRCCOPY);
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
				}
				break;
			}
			case 5:{
				RECT rect={mem->r.x>>1,0,mem->pos.w,mem->pos.h};
				char buf[65536];
				int len,p=0,i,bufp;
				
				for(len=0;mem->excludingKeys[len];len++);
				print("%d\n",len);
				std::sort(mem->excludingKeys,mem->excludingKeys+len);
				
//				for(i=0;i<len;i++)print("%02X ",(int)(mem->excludingKeys[i]));
//				print("\n");
				
				color(mem->hmdc,mem->color.line.release,mem->color.bg.release);
				SetTextColor(mem->hmdc,mem->color.text.release);
				
				print("%d\n",mem->excludingKeys[0]);
				while(mem->run){
					for(i=0;i<256;i++){
						if(i==mem->excludingKeys[p]){
							p++;
							if(p==len)p=0;
							continue;
						}
						if(getBit(keyCheck::keyState[i>>4],i&0xF)){
							if(keyLabel[i])bufp+=sprintf(buf+bufp,"%s ",keyLabel[i]);
							else bufp+=sprintf(buf+bufp,"%02X ",i);
						}
					}

					RoundRect(mem->hmdc, 0, 0, mem->pos.w, mem->pos.h, mem->r.x, mem->r.y);
					DrawText(mem->hmdc,buf,-1,&rect,DT_LEFT|DT_WORDBREAK);
					BitBlt(mem->hdc,0,0,mem->pos.w,mem->pos.h,mem->hmdc, 0,0,SRCCOPY);
					
					buf[bufp=0]=0;
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
				}
				break;
			}
		}
	}
	
	LRESULT CALLBACK ViewProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam){
		ViewMem* mem=0;
		if(msg!=WM_CREATE)mem=(ViewMem*)GetWindowLongPtr(hwnd,0);
		
		switch(msg){
			case WM_PAINT:{
				PAINTSTRUCT ps;
				BeginPaint(hwnd, &ps);
				switch(mem->key>>29){
					case 0:case 3:
					mem->paint=true;
				}
				EndPaint(hwnd,&ps);
				break;
			}
			
			case WM_CREATE:{
				try{//Mem Alloc Exception
					SetWindowLongPtr(hwnd,0,(LONG_PTR)new ViewMem);
					mem=(ViewMem*)GetWindowLongPtr(hwnd,0);
				}catch(const std::bad_alloc& e){
					
				}
				
				//Get Create Params
				CreateParams* cp=(CreateParams*)(((LPCREATESTRUCT)lParam)->lpCreateParams);
				if(!cp){//NULL Ptr ERROR
					
				}
				
				mem->key=cp->key;
				mem->pos=cp->pos;
				mem->color=cp->color;
				mem->r=cp->r;
				
				mem->hdc=GetDC(hwnd);															//Create DC
				mem->hmdc=CreateCompatibleDC(mem->hdc);											//Compatible DC
				switch(mem->key>>29){
					case 0:{//Default
						createdKeyNum[mem->key&0xFF]++;
						
						//Draw Images
						mem->hmbmp=CreateCompatibleBitmap(mem->hdc,cp->pos.w<<1,cp->pos.h<<1);	//Compatible Bitmap
						SelectObject(mem->hmdc,mem->hmbmp);
						
						SetBkMode(mem->hmdc,1);
			
						HFONT	hof=(HFONT)	SelectObject(mem->hmdc,CreateFont(cp->pos.h/3,0,0,0,300,0,0,0,ANSI_CHARSET,false,false,ANTIALIASED_QUALITY,VARIABLE_PITCH|FF_ROMAN,"Noto Sans"));
						HPEN	hop=(HPEN)	SelectObject(mem->hmdc,CreatePen(PS_SOLID,2,transparentColor));
						HBRUSH	hob=(HBRUSH)SelectObject(mem->hmdc,GetStockObject(DC_BRUSH));
						
						SetDCBrushColor(mem->hmdc,transparentColor);
						Rectangle(mem->hmdc,0,0,cp->pos.w<<1,cp->pos.h);
						
						//Draw Image - Release
						RECT rect={0, cp->pos.h/6, cp->pos.w, cp->pos.h/2};
						DeleteObject(SelectObject(mem->hmdc,CreatePen(PS_SOLID,2,cp->color.line.release)));//Pen color
						SetDCBrushColor(mem->hmdc,cp->color.bg.release);//Brush color
						SetTextColor(mem->hmdc,cp->color.text.release);	//Text color
						RoundRect(mem->hmdc, 0, 0, cp->pos.w, cp->pos.h, cp->r.x, cp->r.y);
						DrawTextW(mem->hmdc,cp->label,-1,&rect,DT_CENTER);
						
						//Draw Image - Hold
						rect={cp->pos.w, cp->pos.h/6, cp->pos.w<<1, cp->pos.h/2};
						DeleteObject(SelectObject(mem->hmdc,CreatePen(PS_SOLID,2,cp->color.line.hold)));//Pen color
						SetDCBrushColor(mem->hmdc,cp->color.bg.hold);//Brush color
						SetTextColor(mem->hmdc,cp->color.text.hold);	//Text color
						RoundRect(mem->hmdc, cp->pos.w, 0, cp->pos.w<<1, cp->pos.h, cp->r.x, cp->r.y);
						DrawTextW(mem->hmdc,cp->label,-1,&rect,DT_CENTER);
						
						
						DeleteObject(SelectObject(mem->hmdc,hof));
						DeleteObject(SelectObject(mem->hmdc,hop));
						SelectObject(mem->hmdc,hob);
						
						mem->hof=(HFONT)SelectObject(mem->hmdc,CreateFont(cp->pos.h/4,0,0,0,300,0,0,0,ANSI_CHARSET,false,false,ANTIALIASED_QUALITY,VARIABLE_PITCH|FF_ROMAN,"Consolas"));
						break;
					}
					case 1:{//KPS
						mem->hmbmp=CreateCompatibleBitmap(mem->hdc,cp->pos.w<<1,cp->pos.h);	//Compatible Bitmap
						SelectObject(mem->hmdc,mem->hmbmp);
						
						SetBkMode(mem->hmdc,1);
						
						HPEN	hop=(HPEN)	SelectObject(mem->hmdc,CreatePen(PS_SOLID,2,transparentColor));
						HBRUSH	hob=(HBRUSH)SelectObject(mem->hmdc,GetStockObject(DC_BRUSH));
						mem->hof=(HFONT)SelectObject(mem->hmdc,CreateFont(cp->pos.h/2,0,0,0,300,0,0,0,ANSI_CHARSET,false,false,ANTIALIASED_QUALITY,VARIABLE_PITCH|FF_ROMAN,"Consolas"));
						
						SetDCBrushColor(mem->hmdc,transparentColor);//Brush color
						Rectangle(mem->hmdc,0,0,cp->pos.w<<1,cp->pos.h);
						
						RECT rect={0, 0, cp->pos.w, cp->pos.h/2};
						DeleteObject(SelectObject(mem->hmdc,CreatePen(PS_SOLID,2,cp->color.line.release)));//Pen color
						SetDCBrushColor(mem->hmdc,cp->color.bg.release);//Brush color
						SetTextColor(mem->hmdc,cp->color.text.release);	//Text color
						RoundRect(mem->hmdc, 0, 0, cp->pos.w, cp->pos.h, cp->r.x, cp->r.y);
						DrawTextW(mem->hmdc,L"Cur",-1,&rect,DT_CENTER);
						
						DeleteObject(SelectObject(mem->hmdc,hop));
						SelectObject(mem->hmdc,hob);
						break;
					}
					case 2:{//Tot
						total=0;
						for(int i=0;i<255;i++)if(createdKeyNum[i])total+=keyCount.src[i];
						mem->hmbmp=CreateCompatibleBitmap(mem->hdc,cp->pos.w<<1,cp->pos.h);	//Compatible Bitmap
						SelectObject(mem->hmdc,mem->hmbmp);
						
						SetBkMode(mem->hmdc,1);
						
						HPEN	hop=(HPEN)	SelectObject(mem->hmdc,CreatePen(PS_SOLID,2,transparentColor));
						HBRUSH	hob=(HBRUSH)SelectObject(mem->hmdc,GetStockObject(DC_BRUSH));
						mem->hof=(HFONT)SelectObject(mem->hmdc,CreateFont(cp->pos.h/2,0,0,0,300,0,0,0,ANSI_CHARSET,false,false,ANTIALIASED_QUALITY,VARIABLE_PITCH|FF_ROMAN,"Consolas"));
						
						SetDCBrushColor(mem->hmdc,transparentColor);//Brush color
						Rectangle(mem->hmdc,0,0,cp->pos.w<<1,cp->pos.h);
						
						RECT rect={0, 0, cp->pos.w, cp->pos.h/2};
						DeleteObject(SelectObject(mem->hmdc,CreatePen(PS_SOLID,2,cp->color.line.release)));//Pen color
						SetDCBrushColor(mem->hmdc,cp->color.bg.release);//Brush color
						SetTextColor(mem->hmdc,cp->color.text.release);	//Text color
						RoundRect(mem->hmdc, 0, 0, cp->pos.w, cp->pos.h, cp->r.x, cp->r.y);
						DrawTextW(mem->hmdc,L"Tot",-1,&rect,DT_CENTER);
						
						DeleteObject(SelectObject(mem->hmdc,hop));
						SelectObject(mem->hmdc,hob);
						break;
					}
					case 3:{//Time
						mem->hmbmp=CreateCompatibleBitmap(mem->hdc,cp->pos.w<<1,cp->pos.h);	//Compatible Bitmap
						SelectObject(mem->hmdc,mem->hmbmp);
						
						SetBkMode(mem->hmdc,1);
						
						HPEN	hop=(HPEN)	SelectObject(mem->hmdc,CreatePen(PS_SOLID,2,transparentColor));
						HBRUSH	hob=(HBRUSH)SelectObject(mem->hmdc,GetStockObject(DC_BRUSH));
						mem->hof=(HFONT)SelectObject(mem->hmdc,CreateFont(cp->pos.h/2,0,0,0,300,0,0,0,ANSI_CHARSET,false,false,ANTIALIASED_QUALITY,VARIABLE_PITCH|FF_ROMAN,"Consolas"));
						
						SetDCBrushColor(mem->hmdc,transparentColor);//Brush color
						Rectangle(mem->hmdc,0,0,cp->pos.w<<1,cp->pos.h);
						
						RECT rect={0, 0, cp->pos.w, cp->pos.h/2};
						DeleteObject(SelectObject(mem->hmdc,CreatePen(PS_SOLID,2,cp->color.line.release)));//Pen color
						SetDCBrushColor(mem->hmdc,cp->color.bg.release);//Brush color
						SetTextColor(mem->hmdc,cp->color.text.release);	//Text color
						RoundRect(mem->hmdc, 0, 0, cp->pos.w, cp->pos.h, cp->r.x, cp->r.y);
						DrawTextW(mem->hmdc,L"Time",-1,&rect,DT_CENTER);
						
						DeleteObject(SelectObject(mem->hmdc,hop));
						SelectObject(mem->hmdc,hob);
						break;
					}
					case 4:{//Tps
						mem->hmbmp=CreateCompatibleBitmap(mem->hdc,cp->pos.w<<1,cp->pos.h);	//Compatible Bitmap
						SelectObject(mem->hmdc,mem->hmbmp);
						
						SetBkMode(mem->hmdc,1);
						
						HPEN	hop=(HPEN)	SelectObject(mem->hmdc,GetStockObject(DC_PEN));
						HBRUSH	hob=(HBRUSH)SelectObject(mem->hmdc,GetStockObject(DC_BRUSH));
						mem->hof=(HFONT)SelectObject(mem->hmdc,CreateFont(cp->pos.h/2,0,0,0,300,0,0,0,ANSI_CHARSET,false,false,ANTIALIASED_QUALITY,VARIABLE_PITCH|FF_ROMAN,"Consolas"));
						
						color(mem->hmdc,transparentColor,transparentColor);
						Rectangle(mem->hmdc,0,0,cp->pos.w<<1,cp->pos.h);
						
						RECT rect={0, 0, cp->pos.w, cp->pos.h/2};
						color(mem->hmdc,cp->color.line.release,cp->color.bg.release);//Use Color for Release
						SetTextColor(mem->hmdc,cp->color.text.release);	//Text color
						RoundRect(mem->hmdc, 0, 0, cp->pos.w, cp->pos.h, cp->r.x, cp->r.y);
						DrawTextW(mem->hmdc,L"TPS",-1,&rect,DT_CENTER);
						
						SelectObject(mem->hmdc,hop);
						SelectObject(mem->hmdc,hob);
						break;
					}
					case 6:{//Tps
						mem->hmbmp=CreateCompatibleBitmap(mem->hdc,cp->pos.w<<1,cp->pos.h);	//Compatible Bitmap
						SelectObject(mem->hmdc,mem->hmbmp);
						
						SetBkMode(mem->hmdc,1);
						
						HPEN	hop=(HPEN)	SelectObject(mem->hmdc,GetStockObject(DC_PEN));
						HBRUSH	hob=(HBRUSH)SelectObject(mem->hmdc,GetStockObject(DC_BRUSH));
						mem->hof=(HFONT)SelectObject(mem->hmdc,CreateFont(cp->pos.h,0,0,0,300,0,0,0,ANSI_CHARSET,false,false,ANTIALIASED_QUALITY,VARIABLE_PITCH|FF_ROMAN,"Consolas"));
						
						color(mem->hmdc,transparentColor,transparentColor);
						Rectangle(mem->hmdc,0,0,cp->pos.w<<1,cp->pos.h);
						
						color(mem->hmdc,cp->color.line.release,cp->color.bg.release);//Use Color for Release
						SetTextColor(mem->hmdc,cp->color.text.release);	//Text color
						RoundRect(mem->hmdc, 0, 0, cp->pos.w, cp->pos.h, cp->r.x, cp->r.y);
						
						SelectObject(mem->hmdc,hop);
						SelectObject(mem->hmdc,hob);
						break;
					}
					case 5:{//Else Keys
						mem->excludingKeys=cp->excludingKeys;
						mem->hmbmp=CreateCompatibleBitmap(mem->hdc,cp->pos.w,cp->pos.h);	//Compatible Bitmap
						SelectObject(mem->hmdc,mem->hmbmp);
						
						SetBkMode(mem->hmdc,1);
//						SetBkColor(mem->hmdc,transparentColor);
						
						SelectObject(mem->hmdc,GetStockObject(DC_PEN));
						SelectObject(mem->hmdc,GetStockObject(DC_BRUSH));
						mem->hof=(HFONT)SelectObject(mem->hmdc,CreateFont(cp->key&0x1FFFFFFF,0,0,0,300,0,0,0,ANSI_CHARSET,false,false,ANTIALIASED_QUALITY,VARIABLE_PITCH|FF_ROMAN,"Consolas"));
						
						color(mem->hmdc,transparentColor,transparentColor);
						Rectangle(mem->hmdc,0,0,cp->pos.w,cp->pos.h);
						
						color(mem->hmdc,cp->color.line.release,cp->color.bg.release);//Use Color for Release
						SetTextColor(mem->hmdc,cp->color.text.release);	//Text color
						break;
					}
				}


				mem->run=true;
				mem->t=std::thread(ThrdViewr,mem);
				
				
				delete cp;
				break;
			}
			case WM_DESTROY:{
				mem->run=false;
				mem->t.join();
				if(mem->key>>29 == 5)delete[] mem->excludingKeys;
				if(!mem->key>>29)createdKeyNum[mem->key&0xFF]--;
				DeleteObject(SelectObject(mem->hmdc,mem->hof));
				DeleteObject(mem->hmbmp);
				DeleteDC(mem->hmdc);
				ReleaseDC(hwnd,mem->hdc);
				
				delete (ViewMem*)GetWindowLongPtr(hwnd,0);
				SetWindowLongPtr(hwnd,0,0);	//NULL Ptr
				break;
			}
			case WM_NCHITTEST:{
				UINT nHit=DefWindowProc(hwnd,msg,wParam,lParam);
//				SendMessage(mem->hpwnd,WM_NCHITTEST,27,lParam);
				return HTTRANSPARENT;
			}
			default:return DefWindowProc(hwnd, msg, wParam, lParam);
		}
		return 0;
	}
	
	HWND CreateViewWnd(const unsigned int key,wchar_t* label,const ltwh pos,POS r,keyColor color,const HWND hwndParent,const HINSTANCE hInstance){
		CreateParams* cp=0;
		try{
			cp=new CreateParams;
		}catch(const std::bad_alloc e){//Mem Alloc Exception
			return 0;
		}
		*cp={key,pos,r,color,label};
		HWND hwnd=CreateWindowEx(0,"ViewWnd","",WS_CHILD|WS_VISIBLE,pos.l,pos.t,pos.w,pos.h,hwndParent,NULL,hInstance,cp); //Create View Window
		return hwnd;
	}
	HWND CreateViewWnd(const unsigned int key,wchar_t* label,const ltwh pos,POS r,keyColor color,const HWND hwndParent)
	{return CreateViewWnd(key,label,pos,r,color,hwndParent,(HINSTANCE)GetWindowLongPtr(hwndParent,GWLP_HINSTANCE));}
	
	HWND CreateViewKps(const ltwh pos,POS r,COLORREF Line,COLORREF BG,COLORREF Text,const HWND hwndParent,const HINSTANCE hInstance){
		CreateParams* cp=0;
		try{
			cp=new CreateParams;
		}catch(const std::bad_alloc e){//Mem Alloc Exception
			return 0;
		}
		*cp={1u<<29,pos,r,{{Text,0},{Line,0},{BG,0}},0};
		HWND hwnd=CreateWindowEx(0,"ViewWnd","",WS_CHILD|WS_VISIBLE,pos.l,pos.t,pos.w,pos.h,hwndParent,NULL,hInstance,cp); //Create View Window
		return hwnd;
	}
	
	HWND CreateViewTot(const ltwh pos,POS r,COLORREF Line,COLORREF BG,COLORREF Text,const HWND hwndParent,const HINSTANCE hInstance){
		CreateParams* cp=0;
		try{
			cp=new CreateParams;
		}catch(const std::bad_alloc e){//Mem Alloc Exception
			return 0;
		}
		*cp={2u<<29,pos,r,{{Text,0},{Line,0},{BG,0}},0};
		HWND hwnd=CreateWindowEx(0,"ViewWnd","",WS_CHILD|WS_VISIBLE,pos.l,pos.t,pos.w,pos.h,hwndParent,NULL,hInstance,cp); //Create View Window
		return hwnd;
	}
	
	HWND CreateViewTim(const ltwh pos,POS r,COLORREF Line,COLORREF BG,COLORREF Text,const HWND hwndParent,const HINSTANCE hInstance){
		CreateParams* cp=0;
		try{
			cp=new CreateParams;
		}catch(const std::bad_alloc e){//Mem Alloc Exception
			return 0;
		}
		*cp={3u<<29,pos,r,{{Text,0},{Line,0},{BG,0}},0};
		HWND hwnd=CreateWindowEx(0,"ViewWnd","",WS_CHILD|WS_VISIBLE,pos.l,pos.t,pos.w,pos.h,hwndParent,NULL,hInstance,cp); //Create View Window
		return hwnd;
	}
	
	HWND CreateViewTps(const ltwh pos,POS r,COLORREF Line,COLORREF BG,COLORREF Text,const HWND hwndParent,const HINSTANCE hInstance){
		CreateParams* cp=0;
		try{
			cp=new CreateParams;
		}catch(const std::bad_alloc e){//Mem Alloc Exception
			return 0;
		}
		*cp={4u<<29,pos,r,{{Text,0},{Line,0},{BG,0}},0};
		HWND hwnd=CreateWindowEx(0,"ViewWnd","",WS_CHILD|WS_VISIBLE,pos.l,pos.t,pos.w,pos.h,hwndParent,NULL,hInstance,cp); //Create View Window
		return hwnd;
	}
	HWND CreateViewTpsNT(const ltwh pos,POS r,COLORREF Line,COLORREF BG,COLORREF Text,const HWND hwndParent,const HINSTANCE hInstance){
		CreateParams* cp=0;
		try{
			cp=new CreateParams;
		}catch(const std::bad_alloc e){//Mem Alloc Exception
			return 0;
		}
		*cp={6u<<29,pos,r,{{Text,0},{Line,0},{BG,0}},0};
		HWND hwnd=CreateWindowEx(0,"ViewWnd","",WS_CHILD|WS_VISIBLE,pos.l,pos.t,pos.w,pos.h,hwndParent,NULL,hInstance,cp); //Create View Window
		return hwnd;
	}
	
	HWND CreateViewEk(unsigned char*excludingKeys,int fontSize,const ltwh pos,POS r,COLORREF Line,COLORREF BG,COLORREF Text,const HWND hwndParent,const HINSTANCE hInstance){
		CreateParams* cp=0;
		unsigned char* ek=0;
		int len;
		
		try{
			cp=new CreateParams;
		}catch(const std::bad_alloc e){//Mem Alloc Exception
			return 0;
		}
		for(len=0;excludingKeys[len];len++);
		try{
			ek=new unsigned char[len];
		}catch(const std::bad_alloc e){//Mem Alloc Exception
			return 0;
		}
		for(int i=0;i<len;i++)ek[i]=excludingKeys[i];
		
		*cp={5u<<29 | fontSize&0x1FFFFFFF,pos,r,{{Text,0},{Line,0},{BG,0}},0};
		cp->excludingKeys=ek;
		HWND hwnd=CreateWindowEx(0,"ViewWnd","",WS_CHILD|WS_VISIBLE,pos.l,pos.t,pos.w,pos.h,hwndParent,NULL,hInstance,cp); //Create View Window
		return hwnd;
	}
	
	
	int ViewRegister(){
		static bool isRegistered=false;
		if(isRegistered){
			return -1;
		}else{
			WNDCLASS wc;
			
			memset(&wc,0,sizeof(wc));
			wc.cbClsExtra	 = 0;
			wc.cbWndExtra	 = sizeof(LONG_PTR);
//			wc.hbrBackground = CreateSolidBrush(RGB(0xC0,0xC0,0xC0));
			wc.hbrBackground = CreateSolidBrush(transparentColor);
			wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
			wc.hIcon		 = NULL;
			wc.hInstance	 = GetModuleHandle(NULL);
			wc.lpfnWndProc	 = ViewProc;
			wc.lpszClassName = "ViewWnd";
//			wc.lpszMenuName	 = NULL;
//			wc.style		 = 0;
			if(RegisterClass(&wc)){
				isRegistered=true;
				return 1;
			}else return 0;
		}
	}
	/*
	class ViewRegister{
		public:ViewRegister(){
			static bool isRegistered=false;
			if(isRegistered){
				return;
			}else{
				WNDCLASS wc;

				wc.cbClsExtra	 = 0;
				wc.cbWndExtra	 = sizeof(LONG_PTR);
				wc.hbrBackground = CreateSolidBrush(RGB(0xC0,0xC0,0xC0));
				wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
				wc.hIcon		 = NULL;
				wc.hInstance	 = GetModuleHandle(NULL);
				wc.lpfnWndProc	 = ViewProc;
				wc.lpszClassName = "ViewWnd";
				wc.lpszMenuName	 = NULL;
				wc.style		 = 0;

				if(RegisterClass(&wc))isRegistered=true;
			}
		}
	}ViewRegister;*/
}
#endif//__KV__
