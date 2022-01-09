#ifndef __Basics__q
#define __Basics__q

#include<windows.h>

HINSTANCE hinst;

COLORREF transparentColor=0x00FF00;

bool kpsAllKey=true;
HANDLE hKCF;//Key Count File

inline ULONG ProcIDFromWnd(HWND hwnd){
	ULONG idProc;
	GetWindowThreadProcessId( hwnd, &idProc );
	return idProc;
}

HWND GetWndHandle(ULONG pid){
	HWND tempHwnd = FindWindow(NULL,NULL);//Find handle of top window
	ULONG p;
	while(tempHwnd!=NULL){
		if(GetParent(tempHwnd)==NULL)if(pid==ProcIDFromWnd(tempHwnd))return tempHwnd;
		tempHwnd = GetWindow(tempHwnd, GW_HWNDNEXT);
	}
	return NULL;
}

void color(HDC hdc,COLORREF Pen, COLORREF Brush){SetDCPenColor(hdc, Pen),SetDCBrushColor(hdc, Brush);}
void txtColor(HDC hdc,COLORREF TXT, COLORREF BG){SetTextColor(hdc,TXT);SetBkColor(hdc,BG);}

#endif//__Basics__q
