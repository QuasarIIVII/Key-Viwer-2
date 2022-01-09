//Required Parameters
//Optional Parameters
#include<windows.h>
#include<iostream>
#include<math.h>
#include<thread>
#include<chrono>
//#include<locale.h>
#include"Basics.h"
#include"Console.h"
#include"Viewer.h"
//FILE*ferr=fopen("W:/R/log.log","w");
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		case WM_CREATE:{
			SetLayeredWindowAttributes(hwnd,transparentColor, 0x80, LWA_COLORKEY|LWA_ALPHA); //Set transparent window
			
			keyCheck::beginKeyChk();
			View::CreateViewWnd('Q',L"Q",{0,0,80,80},{80,80},{{0x808080,0x000000},{0xFFADFF,0xFFADFF},{0xFFFFFF,0x808080}},hwnd,hinst);
			View::CreateViewWnd('W',L"W",{0,80,80,80},{80,80},{{0x808080,0x000000},{0xFFADFF,0xFFADFF},{0xFFFFFF,0x808080}},hwnd,hinst);
			View::CreateViewWnd('E',L"E",{0,160,80,80},{80,80},{{0x808080,0x000000},{0xFFADFF,0xFFADFF},{0xFFFFFF,0x808080}},hwnd,hinst);
			View::CreateViewWnd('R',L"R",{0,240,80,80},{80,80},{{0x808080,0x000000},{0xFFADFF,0xFFADFF},{0xFFFFFF,0x808080}},hwnd,hinst);
			View::CreateViewWnd(0x20,L"SP",{0,320,80,80},{80,80},{{0x808080,0x000000},{0xFFADFF,0xFFADFF},{0xFFFFFF,0x808080}},hwnd,hinst);
			
			View::CreateViewWnd(0x67,L"N7",{80,240,80,80},{80,80},{{0x808080,0x000000},{0xE8E826,0xE8E826},{0xFFFFFF,0x808080}},hwnd,hinst);
			View::CreateViewWnd(0x68,L"N8",{80,160,80,80},{80,80},{{0x808080,0x000000},{0xE8E826,0xE8E826},{0xFFFFFF,0x808080}},hwnd,hinst);
			View::CreateViewWnd(0x69,L"N9",{80,80,80,80},{80,80},{{0x808080,0x000000},{0xE8E826,0xE8E826},{0xFFFFFF,0x808080}},hwnd,hinst);
			View::CreateViewWnd(0x6B,L"N+",{80,0,80,80},{80,80},{{0x808080,0x000000},{0xE8E826,0xE8E826},{0xFFFFFF,0x808080}},hwnd,hinst);
			View::CreateViewWnd(0x27,L"\x2192",{80,320,80,80},{80,80},{{0x808080,0x000000},{0xE8E826,0xE8E826},{0xFFFFFF,0x808080}},hwnd,hinst);
			
//			View::CreateViewWnd('A',L"\xBBF8",{20,20,80,80},{80,80},{{0x808080,0x000000},{0xE8E826,0xE8E826},{0xFFFFFF,0xC0C0C0}},hwnd,hinst);
//			View::CreateViewWnd('S',L"\xB9C8",{120,20,80,80},{80,80},{{0x808080,0x000000},{0xE8E826,0xE8E826},{0xFFFFFF,0xC0C0C0}},hwnd,hinst);
//			View::CreateViewWnd('D',L"\xBBF8",{220,20,80,80},{80,80},{{0x808080,0x000000},{0xE8E826,0xE8E826},{0xFFFFFF,0xC0C0C0}},hwnd,hinst);
//			View::CreateViewWnd('F',L"\xB9C8",{320,20,80,80},{80,80},{{0x808080,0x000000},{0xE8E826,0xE8E826},{0xFFFFFF,0xC0C0C0}},hwnd,hinst);
			
//			View::CreateViewTim({0,400,160,40},{20,20},0x78E826,0xFFFFFF,0x000000,hwnd,hinst);
//			View::CreateViewKps({0,440,80,40},{20,20},0x78E826,0xFFFFFF,0x000000,hwnd,hinst);
//			View::CreateViewTot({80,440,80,40},{20,20},0x78E826,0xFFFFFF,0x000000,hwnd,hinst);
			View::CreateViewTim({0,400,160,40},{20,20},0xF3CA92,0xFFFFFF,0x000000,hwnd,hinst);
			View::CreateViewKps({0,440,80,40},{20,20},0xF3CA92,0xFFFFFF,0x000000,hwnd,hinst);
			View::CreateViewTot({80,440,80,40},{20,20},0xF3CA92,0xFFFFFF,0x000000,hwnd,hinst);

			unsigned char ek[]={'Q','W','E','R',0x20,0x67,0x68,0x69,0x6B,0x27,0};
			HWND h=View::CreateViewEk(ek,16,{0,480,160,40},{20,20},0xFFFFFF,0xFFFFFF,0x000000,hwnd,hinst);
			
			View::CreateViewTpsNT({0,520,160,20},{20,20},0xFFFFFF,0xFFFFFF,0x000000,hwnd,hinst);
//			TF();
			break;
		}
		case WM_DESTROY: {
			keyCheck::endKeyChk();
			
			SetFilePointerEx(hKCF,{0,0},NULL,FILE_BEGIN);
			WriteFile(hKCF,keyCount.byte,2048,NULL,NULL);
			FlushFileBuffers(hKCF);
			CloseHandle(hKCF);
			
			PostQuitMessage(0);
			break;
		}
		case WM_NCHITTEST:{
			UINT nHit=DefWindowProc(hwnd,Message,wParam,lParam);
			if(nHit==HTCLIENT)nHit=HTCAPTION;
			return nHit;
		}
		
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	hinst=hInstance;
//	AllocConsole();
//	freopen("CONOUT$","w",stdout);
//	freopen("CONIN$","r",stdin);
//	freopen("CONERR$","w",stderr);

//	std::ios::sync_with_stdio();
	SetConsoleOutputCP(65001);
//	CONSOLE_FONT_INFOEX cfi={sizeof(cfi),0,{10,18},FF_DONTCARE,FW_NORMAL,L"Noto Sans"};
//	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE),FALSE,&cfi);
	
//	wchar_t ws[65536];
//	MultiByteToWideChar(CP_UTF8,0,"\xf0\x9f\xa9\xb9",128,ws,65536);
//	for(int i=0;ws[i];i++)print("%04X ",ws[i]);print("\n");
	
//	setlocale(LC_ALL,".65001");
//	_wsetlocale()
	
//	print("\xea\xb0\x80\x0A");
//	print("\xc6\xb8");
//	print("\xf0\x90\x80\x80");
//	print("\xf0\x9f\xa9\xb9");
	
	{
		char kcfPath[65537];//Key Count File Path
		int fileNameSeperatorPos=0,i;
//		print("Enter the path of key count file : ");
//		std::cin.getline(kcfPath,65537);
		
		FILE*fl=fopen("data path.txt","r");
		if(fl){
			fgets(kcfPath,65537,fl);
			fclose(fl);
		}
		
		if(!kcfPath[0]){
			char*t="W:/KC.data";
			for(i=0;t[i];i++)kcfPath[i]=t[i];
			kcfPath[i]=0;
		}
		
//		print("%s\n",kcfPath);
		if((hKCF=CreateFile(kcfPath,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL))==INVALID_HANDLE_VALUE){
			for(i=0;kcfPath[i];i++)if(kcfPath[i]=='/' || kcfPath[i]=='\\')fileNameSeperatorPos=i;
			i=kcfPath[fileNameSeperatorPos];
			kcfPath[fileNameSeperatorPos]=0;
			
			if(!CreateDirectory(kcfPath,NULL)){
				MessageBox(NULL, "Cannot open the key count file!","Error!",MB_ICONEXCLAMATION|MB_OK);
				return 0;
			}else{
				kcfPath[fileNameSeperatorPos]=i;
				if((hKCF=CreateFile(kcfPath,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL))==INVALID_HANDLE_VALUE){
					MessageBox(NULL, "Cannot open the key count file!","Error!",MB_ICONEXCLAMATION|MB_OK);
					return 0;
				}
			}
		}
		
		DWORD fileSize;
		ReadFile(hKCF,keyCount.byte,2048,&fileSize,NULL);
		if(fileSize!=2048){
			unsigned char default_data[2048]={0,};
			SetFilePointerEx(hKCF,{0,0},NULL,FILE_BEGIN);
			WriteFile(hKCF,default_data,2048,&fileSize,NULL);
			for(int i=0;i<256;i++)keyCount.src[i]=0;
		}
	}
	
	WNDCLASSEX wc;
	HWND hwnd;
	MSG msg;

	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc;
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);

//	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = "WindowClass";
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION);

	wc.hbrBackground = CreateSolidBrush(transparentColor|0xFF00);

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
	
	defKeyLabel();
	View::ViewRegister();

	hwnd = CreateWindowEx(WS_EX_LAYERED,"WindowClass","Key Viwer (V.alpha)",WS_POPUP|WS_VISIBLE,
		960,480,160,540,
		NULL,NULL,hInstance,NULL);

	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	while(GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	FreeConsole();
	return msg.wParam;
}


/*
case WM_PAINT:{
			PAINTSTRUCT ps;
//			HFONT hof;
			HPEN hop;
			HBRUSH hob;
			
			BeginPaint(hwnd,&ps);
			
//			hof=(HFONT)SelectObject(ps.hdc,CreateFont(16,0,0,0,300,0,0,0,ANSI_CHARSET,false,false,false,VARIABLE_PITCH|FF_ROMAN,"Noto Sans"));
//			hop=(HPEN)SelectObject(ps.hdc,GetStockObject(DC_PEN));
//			hob=(HBRUSH)SelectObject(ps.hdc,GetStockObject(DC_BRUSH));
			
//			RECT r={10,300,100,600};
//			DrawTextW(ps.hdc,L"\xd800\xdc00",-1,&r,DT_LEFT);
			
//			color(ps.hdc,0xFF00FF,0xFF00FF);
//			Rectangle(ps.hdc,50,50,70,70);

//			DeleteObject(SelectObject(ps.hdc,hof));
//			SelectObject(ps.hdc,hop);
//			SelectObject(ps.hdc,hob);
			
			EndPaint(hwnd,&ps);
			break;
		}
*/
