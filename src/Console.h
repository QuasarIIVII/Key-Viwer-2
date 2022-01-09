#include<windows.h>
#include<stdarg.h>
#include<stdio.h>
int print(const char*str, ...){
	int i,n=0;
	for(i=0;str[i];i++)if(str[i]=='%')n++;
	char dest[1024];
	va_list va;
	va_start(va,n);
	i=vsprintf(dest,str,va);
	WriteFile(GetStdHandle(STD_OUTPUT_HANDLE),dest,i,NULL,NULL);
	va_end(va);
	return n;
}
