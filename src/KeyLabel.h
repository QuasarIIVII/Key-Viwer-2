#ifndef __KeyLabel__
#define __KeyLabel__
#include<stdlib.h>
char*keyLabel[256]={0,};
void defKeyLabel(){//define keyLabel
	int charsz=sizeof(char),t,i;
//	VK_CLEAR
	keyLabel[0x01]=(char*)"BL";
	keyLabel[0x02]=(char*)"BR";
	keyLabel[0x03]=(char*)"Cancel";
	keyLabel[0x04]=(char*)"BM";
	keyLabel[0x05]=(char*)"X1";
	keyLabel[0x06]=(char*)"X2";
	keyLabel[0x08]=(char*)"Bs";
	keyLabel[0x09]=(char*)"Tab";
	keyLabel[0x0C]=(char*)"Clear";
	keyLabel[0x0D]=(char*)"Enter";
	
	keyLabel[0x10]=(char*)"";
	keyLabel[0x11]=(char*)"";
	keyLabel[0x12]=(char*)"";
	
	keyLabel[0x13]=(char*)"Pause";
	keyLabel[0x14]=(char*)"Capital";
	keyLabel[0x15]=(char*)"Hangul";
	keyLabel[0x17]=(char*)"Junja";
	keyLabel[0x18]=(char*)"Final";
	keyLabel[0x19]=(char*)"Hanja";
	
	keyLabel[0x1B]=(char*)"ESC";
	keyLabel[0x1C]=(char*)"Convert";
	keyLabel[0x1D]=(char*)"NonConvert";
	keyLabel[0x1E]=(char*)"ACCEPT";
	keyLabel[0x1F]=(char*)"ModeChange";
	
	keyLabel[0x20]=(char*)"SP";
	keyLabel[0x21]=(char*)"PgUp";
	keyLabel[0x22]=(char*)"PgDn";
	keyLabel[0x23]=(char*)"End";
	keyLabel[0x24]=(char*)"Home";
	keyLabel[0x25]=(char*)"¡ç";
	keyLabel[0x26]=(char*)"¡è";
	keyLabel[0x27]=(char*)"¡æ";
	keyLabel[0x28]=(char*)"¡é";
	keyLabel[0x29]=(char*)"Select";
	keyLabel[0x2A]=(char*)"Print";
	keyLabel[0x2B]=(char*)"Execute";
	keyLabel[0x2C]=(char*)"PrtSc";
	keyLabel[0x2D]=(char*)"Insert";
	keyLabel[0x2E]=(char*)"Delete";
	keyLabel[0x2F]=(char*)"Help";
	
	for(i=0x30;i<0x40;i++){
		keyLabel[i]=(char*)calloc(2,charsz);
		keyLabel[i][0]=i;
	}
	
	for(i=0x41;i<0x5B;i++){
		keyLabel[i]=(char*)calloc(2,charsz);
		keyLabel[i][0]=i;
	}
	
	
	keyLabel[0x5B]=(char*)"LWin";
	keyLabel[0x5C]=(char*)"RWin";
	keyLabel[0x5D]=(char*)"APPS";
	keyLabel[0x5F]=(char*)"Sleep";
	
	for(i=0x60;i<0x70;i++){
		keyLabel[i]=(char*)calloc(5,charsz);
		keyLabel[i][0]='N';
		keyLabel[i][1]='U';
		keyLabel[i][2]='M';
		keyLabel[i][3]=i-0x30;
	}
	keyLabel[0x6A]=(char*)"NUM*";
	keyLabel[0x6B]=(char*)"NUM+";
	keyLabel[0x6C]=(char*)"Separator";
	keyLabel[0x6D]=(char*)"NUM-";
	keyLabel[0x6E]=(char*)"NUM.";
	keyLabel[0x6F]=(char*)"NUM/";
	
	for(i=0x70;i<0x79;i++){
		keyLabel[i]=(char*)calloc(3,charsz);
		keyLabel[i][0]='F';
		keyLabel[i][1]=i-0x3F;
	}
	for(;i<0x88;i++){
		keyLabel[i]=(char*)calloc(4,charsz);
		t=i-0x6F;
		keyLabel[i][0]='F';
		keyLabel[i][1]=t/10+0x30;
		keyLabel[i][2]=t%10+0x30;
	}
	
	keyLabel[0x90]=(char*)"NumLock";
	keyLabel[0x91]=(char*)"ScrollLock";
	
	keyLabel[0xA0]=(char*)"LShift";
	keyLabel[0xA1]=(char*)"RShift";
	keyLabel[0xA2]=(char*)"LCtrl";
	keyLabel[0xA3]=(char*)"RCtrl";
	keyLabel[0xA4]=(char*)"LAlt";
	keyLabel[0xA5]=(char*)"RAlt";
	
	keyLabel[0xBA]=(char*)";";
	keyLabel[0xBB]=(char*)"+";
	keyLabel[0xBC]=(char*)",";
	keyLabel[0xBD]=(char*)"-";
	keyLabel[0xBE]=(char*)".";
	keyLabel[0xBF]=(char*)"/";
	
	keyLabel[0xC0]=(char*)"`";
	
	keyLabel[0xDB]=(char*)"[";
	keyLabel[0xDC]=(char*)"\\";
	keyLabel[0xDD]=(char*)"]";
	keyLabel[0xDE]=(char*)"\'";
}
#endif//__KeyLabel__
