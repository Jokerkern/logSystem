#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <tchar.h>
#include <iostream>
#include <thread>
#include <cstdlib>

enum LOGLEVEL {
	debug,
	info,
	warn,
	error,
	critical
};

void main() {
	typedef void (*Open) (const char*);
	typedef void (*Write) (LOGLEVEL, const char*);
	typedef void (*Close) ();
	typedef void (*Start) ();
	Open open;
	Write write;
	Close close;
	Start init;
	Close uninit;
	HINSTANCE hinst,hinst2;
	hinst = LoadLibrary(_T("DLL1.dll"));
	if (hinst == NULL)
	{
		printf("hinst not found");
		getchar();
		system("pause");
	}
	init = (Start)GetProcAddress(hinst, "Init");
	if (init == NULL)
	{
		printf("init not found");
		getchar();
		system("pause");
	}
	
	std::cout << GetLastError() << std::endl;
	open = (Open)GetProcAddress(hinst, "Open");
	if (open == NULL)
	{
		printf("2 not found");
		getchar();
		system("pause");
	}
	write = (Write)GetProcAddress(hinst, "Write");
	if (write == NULL)
	{
		printf("3 not found");
		getchar();
		system("pause");
	}
	close = (Close)GetProcAddress(hinst, "Close");
	if (close == NULL)
	{
		printf("4 not found");
		getchar();
		system("pause");
	}
	uninit = (Close)GetProcAddress(hinst, "Uninit");
	if (uninit == NULL)
	{
		printf("4 not found");
		getchar();
		system("pause");
	}
	init();
	//std::cout << "init" << std::endl;
	getchar();
	open("test");
	char c[10] = {};
	while (1) {
		for (int i = 0; i < 1234; i++) {
			write(error, itoa(i, c, 10));
			//Sleep(1000);
		}
		getchar();
	}
	close();
	uninit();
	

}

//#include <Windows.h>
//#include <stdio.h>
//#include <iostream>
//int main() {
//	typedef int (*ADD) (int,int);
//	HINSTANCE hinst;
//	hinst = LoadLibrary("DLL1.dll");
//	ADD sum;
//	
//	if (hinst == NULL)
//	{
//		printf("1 not found");
//		getchar();
//		system("pause");
//	}
//	sum = (ADD)GetProcAddress(hinst, "add");
//	std::cout << GetLastError() << std::endl;
//	if (sum == NULL)
//	{
//		printf("2 not found");
//		getchar();
//		system("pause");
//	}
//	std::cout << sum(1, 2)<<std::endl;
//
//}