#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include <ctime>
#include "time.h"
using namespace std;


#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)


int _tmain(int argc, _TCHAR* argv[])
{
	system("cls");
	HANDLE hslot;
	hslot = CreateMailslot(            
									   
		L"\\\\.\\mailslot\\Box",	  
		400,                           
		MAILSLOT_WAIT_FOREVER,        
		NULL                           
	);
	if (hslot == INVALID_HANDLE_VALUE)
	{
		std::cout << "SLOT FAIL" << std::endl;
		std::cout << "Press any key to exit" << std::endl;
		std::cin.get();
		return 0;
	}
	std::cout << "Server is waiting" << std::endl;
	int i = 0;
	char rbuf[50];
	DWORD rms;
	HANDLE hMS;
	double t1, t2;
	do
	{
		i++;

		if (!ReadFile(    
			hslot,         
			rbuf,         
			sizeof(rbuf),  
			&rms,          
			NULL           
		))
		{
			std::cout << "FAIL" << std::endl;
			CloseHandle(hslot);
			std::cout << "Press any key to exit" << std::endl;
			std::cin.get();
			return 0;
		}
		else
		{
			cout << rbuf << endl;
		}
		if (i == 1)
			t1 = clock();

	} while (true);

	t2 = clock();

	cout << "Time: " << (t2 - t1) / 1000 << " сек." << endl;
	cout << "Message count: " << i - 1 << endl << endl;

	if (!CloseHandle(hslot))
		throw "Error: CloseHandle";
	return 0;
}
