#include "stdafx.h"
#include <string>
#include <iostream>
#include "Winsock2.h" //заголовок WS2_32.dll
#pragma comment(lib, "WS2_32.lib") //экспорт WS2_32.dll
#include "Error.h"
#include <ctime>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
using namespace std;


void main()
{

	cout << "Listening..." << endl;

	try
	{
		HANDLE h_NP;

		char rbuf[50];
		char wbuf[50] = "Serv";
		DWORD rb = sizeof(rbuf);
		DWORD wb = sizeof(wbuf);

		if ((h_NP = CreateNamedPipe(L"\\\\.\\pipe\\Tube", PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_WAIT, 1, NULL, NULL, INFINITE, NULL)) == INVALID_HANDLE_VALUE)
			throw "Error: CreateNamedPipe";
		int i = 1;
		while (true)
		{
			if (!ConnectNamedPipe(h_NP, NULL))
				throw "Error: ConnectNamedPipe";
			if (!ReadFile(h_NP, rbuf, sizeof(rbuf), &rb, NULL))
				throw "Error: ReadFile";

			char inum[6];
			sprintf(inum, "%d", i++);
			char buf[sizeof(wbuf) + sizeof(inum)];
			strcpy(buf, wbuf);
			strcat(buf, inum);

			cout << rbuf << endl;

			if (!WriteFile(h_NP, buf, sizeof(wbuf), &wb, NULL))
				throw "Error: WriteFile";
			if (!DisconnectNamedPipe(h_NP))
				throw "Error: DisconnectNamedPipe";
		}
		if (!CloseHandle(h_NP))
			throw "Error: CloseHandle";
	}
	catch (string e)
	{
		cout << e << endl;
	}
	catch (...)
	{
		cout << "Error" << endl;
	}
	system("pause");
}