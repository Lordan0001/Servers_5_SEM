#include <iostream>
#include <string>
#include <stdio.h>
#include <clocale>
#include <ctime>
#include "Winsock2.h"
#include "WS2tcpip.h"
#pragma comment (lib, "WS2_32.lib")
#pragma warning(disable:4996)
using namespace std;



string GetErrorMsgText(int code)    // cформировать текст ошибки 
{
	string msgText;
	switch (code)                      // проверка кода возврата  
	{
	case WSAEINTR:          msgText = "WSAEINTR";         break;
	case WSAEACCES:         msgText = "WSAEACCES";        break;
	case WSASYSCALLFAILURE: msgText = "WSASYSCALLFAILURE"; break;
	default:                msgText = "***ERROR***";      break;
	};
	return msgText;
};



string SetErrorMsgText(string msgText, int code)
{
	return msgText + GetErrorMsgText(code);
}



int main()
{
	SOCKET  cC;           // дескриптор сокета 
	WSADATA wsaData;

	try
	{
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
			throw  SetErrorMsgText("Startup:", WSAGetLastError());
		if ((cC = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
			throw  SetErrorMsgText("socket:", WSAGetLastError());

		SOCKADDR_IN serv;                    // параметры  сокета сервера
		serv.sin_family = AF_INET;           // используется IP-адресация  
		serv.sin_port = htons(2000);                   // TCP-порт 2000

		serv.sin_addr.s_addr = inet_addr("127.0.0.1");  
		if ((connect(cC, (sockaddr*)&serv, sizeof(serv))) == SOCKET_ERROR)
			throw  SetErrorMsgText("connect:", WSAGetLastError());




		char fromServer[24], 
			 toServer[24] = "Hello from Client"; 
		int count;
		cout << "Enter number of messages:\n";
		cin >> count;

		int time = clock();
		for (int i = 0; i < count; i++)
		{

			if (SOCKET_ERROR == send(cC, toServer, sizeof(toServer), NULL))
				cout << "send:" << GetLastError() << endl;;
			if (SOCKET_ERROR == recv(cC, fromServer, sizeof(fromServer), NULL))
				cout << "recv:" << GetLastError() << endl;
			cout << fromServer << " " << (i + 1) << endl;
		}
		cout << "\nProgram was running for " << time << " ticks or " << ((float)time) / CLOCKS_PER_SEC << " seconds.\n";




		if (closesocket(cC) == SOCKET_ERROR)
			throw  SetErrorMsgText("closesocket:", WSAGetLastError());
		if (WSACleanup() == SOCKET_ERROR)
			throw SetErrorMsgText("Cleanup: ", WSAGetLastError());
	}

	
	catch (string errorMsgText)
	{
		cout << endl << "WSAGetLastError: " << errorMsgText;
	}
	system("pause");
}