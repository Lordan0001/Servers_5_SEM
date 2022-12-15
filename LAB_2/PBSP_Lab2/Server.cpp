#include <iostream>
#include <clocale>
#include <ctime>
#include "Winsock2.h"
#pragma comment (lib, "WS2_32.lib")
#pragma warning(disable:4996)
using namespace std;





string  GetErrorMsgText(int code)    // cформировать текст ошибки 
{
	string msgText;
	switch (code)                      // проверка кода возврата  
	{
	case WSAEINTR:          msgText = "WSAEINTR";         break;
	case WSAEACCES:         msgText = "WSAEACCES";        break;
		//..........коды WSAGetLastError ..........................
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


	SOCKET  sS;         
	WSADATA wsaData;

	try
	{

		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
			throw  SetErrorMsgText("Startup:", WSAGetLastError());
		if ((sS = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
			throw  SetErrorMsgText("socket:", WSAGetLastError());
		//...........................................................
		SOCKADDR_IN serv;                     // параметры  сокета sS
		serv.sin_family = AF_INET;           // используется IP-адресация  
		serv.sin_port = htons(2000);          // порт 2000
		serv.sin_addr.s_addr = INADDR_ANY;   // любой собственный IP-адрес 

		if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
			throw  SetErrorMsgText("bind:", WSAGetLastError());
		if (listen(sS, SOMAXCONN) == SOCKET_ERROR)
			throw  SetErrorMsgText("listen:", WSAGetLastError());



	
///

		int i = 0;
		WSADATA ws;
		SOCKET s;
		SOCKET c;
		char fromClient[50];
		char toClient[50] = "Hello from Client";
		int t;

		if (FAILED(WSAStartup(MAKEWORD(1, 1), &ws)))
			cout << "socket:" << WSAGetLastError() << endl;
		if (INVALID_SOCKET == (s = socket(AF_INET, SOCK_STREAM, 0)))
			cout << "socket:" << WSAGetLastError() << endl;

		sockaddr_in c_adr;
		sockaddr_in s_adr;
		{
			s_adr.sin_port = htons(2000);
			s_adr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
			s_adr.sin_family = AF_INET;
		}

		if (SOCKET_ERROR == (bind(s, (LPSOCKADDR)&s_adr, sizeof(s_adr))))
			cout << "bind:" << WSAGetLastError() << endl;

		if (SOCKET_ERROR == listen(s, 2))
			cout << "listen:" << WSAGetLastError << endl;


		while (true)
		{
			int lcInt = sizeof(c_adr);
			if (INVALID_SOCKET == (c = accept(s, (sockaddr*)&c_adr, &lcInt)))
				cout << "accept:" << WSAGetLastError() << endl;

			cout << "Client connected." << endl;
			cout << "Client IP:    " << inet_ntoa(c_adr.sin_addr) << endl;
			cout << "Client port:  " << htons(c_adr.sin_port) << "\n\n";

			int time = clock();
			while (true)
			{
				if (SOCKET_ERROR == recv(c, fromClient, sizeof(fromClient), NULL))
					break;

				cout << fromClient << " " << i << "\n";
				i++;

				if (!strcmp(fromClient, "CLOSE")) { break; }

				if (SOCKET_ERROR == send(c, toClient, strlen(toClient) + 1, NULL))
					break;
			}
			i = 0;
			cout << "Client Disconected.\n";
			cout << "Program was running for " << time << " ticks or " << ((float)time) / CLOCKS_PER_SEC << " seconds.\n\n";
		}
		if (closesocket(c) == SOCKET_ERROR)
			throw SetErrorMsgText("closesocket:", WSAGetLastError());

		if (WSACleanup() == SOCKET_ERROR)
			throw SetErrorMsgText("Cleanup:", WSAGetLastError());












		if (closesocket(sS) == SOCKET_ERROR)
			throw  SetErrorMsgText("closesocket:", WSAGetLastError());
		if (WSACleanup() == SOCKET_ERROR)
			throw  SetErrorMsgText("Cleanup:", WSAGetLastError());

	}
	catch (string errorMsgText)
	{
		cout << endl << "WSAGetLastError: " << errorMsgText;
	}
}
