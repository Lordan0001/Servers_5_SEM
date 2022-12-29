#define _CRT_SECURE_NO_WARNINGS

#include "Global.h"
#include "AcceptServer.h"
#include "DispathServer.h"
#include "GarbageCleaner.h"
#include "ConsolePipe.h"
#include "ResponseServer.h"
#include "tchar.h"

//WIN-HGMUQ1VQC5E
int _tmain(int argc, _TCHAR* argv[]) {
	setlocale(LC_ALL, "Russian");

	try {

		if (argc > 1) {
			int tmp = atoi(argv[1]);
			if (tmp >= 0 && tmp <= 65535) {
				port = atoi(argv[1]);
				cout << "Entered TCP port: " << port << endl;
			}
			else {
				cout << "Entered wrong TCP port" << endl;
			}
		}
		else {
			cout << "ez lab 4 all my mates" << endl;
			cout << "TCP: " << port << endl;
		}

		if (argc > 2) {
			int tmp = atoi(argv[2]);
			if (tmp >= 0 && tmp <= 65535) {
				uport = atoi(argv[2]);
				cout << "Entered UDP port: " << uport << endl;
			}
			else {
				cout << "Entered wrong TCP port" << endl;
			}
		}
		else {
			cout << "UDP: " << uport << endl;
		}

		if (argc > 3) { //3 параметр - им€ библиотеки
			dllname = argv[3];
		}

		if (argc > 4) {
			npname = argv[4];
			cout << "Entered namedPipe: " << npname << endl;
		}
		else cout << "namedPipe: " << npname << endl;

		if (argc > 5) {
			ucall = argv[5];
			cout << "Entered alias:   " << ucall << endl;
		}
			cout << "Alias: " << ucall << endl;
		srand((unsigned)time(NULL));

		volatile TalkersCommand  cmd = START;

		InitializeCriticalSection(&scListContact);


		st1 = LoadLibrary(dllname);
		sss = (HANDLE(*)(char*, LPVOID))GetProcAddress(st1, "SSS");
		if (st1 == NULL) { cout << "ќшибка при загрузке DLL" << endl; }
		else { cout << "DLL checked " << dllname << endl << endl; 
		cout << "Let's check some headers :)" << endl;
		cout << " " << endl<<endl;
		}


		hAcceptServer = CreateThread(NULL, NULL, AcceptServer, (LPVOID)&cmd, NULL, NULL);            //main
		HANDLE hDispathServer = CreateThread(NULL, NULL, DispathServer, (LPVOID)&cmd, NULL, NULL);

		HANDLE hConsolePipe = CreateThread(NULL, NULL, ConsolePipe, (LPVOID)&cmd, NULL, NULL);       //main - —ервер именованного канала
		HANDLE hGarbageCleaner = CreateThread(NULL, NULL, GarbageCleaner, (LPVOID)&cmd, NULL, NULL); //main


		HANDLE hResponseServer = CreateThread(NULL, NULL, ResponseServer, (LPVOID)&cmd, NULL, NULL);
			
		SetThreadPriority(hAcceptServer, THREAD_PRIORITY_HIGHEST);			//более активным станет подключение клиентов
		SetThreadPriority(hGarbageCleaner, THREAD_PRIORITY_BELOW_NORMAL);	//пониженный (в фоновом режиме)
		SetThreadPriority(hConsolePipe, THREAD_PRIORITY_NORMAL);

		SetThreadPriority(hResponseServer, THREAD_PRIORITY_ABOVE_NORMAL);
		SetThreadPriority(hDispathServer, THREAD_PRIORITY_NORMAL);

		WaitForSingleObject(hAcceptServer, INFINITE);
		WaitForSingleObject(hDispathServer, INFINITE);
		WaitForSingleObject(hConsolePipe, INFINITE);
		WaitForSingleObject(hGarbageCleaner, INFINITE);
		WaitForSingleObject(hResponseServer, INFINITE);

		CloseHandle(hAcceptServer);
		CloseHandle(hDispathServer);
		CloseHandle(hGarbageCleaner);
		CloseHandle(hConsolePipe);
		CloseHandle(hResponseServer);

		DeleteCriticalSection(&scListContact);

		FreeLibrary(st1);
	}
	catch (...) {
		cout << "error" << endl;
	}

	system("pause");
	return 0;
}