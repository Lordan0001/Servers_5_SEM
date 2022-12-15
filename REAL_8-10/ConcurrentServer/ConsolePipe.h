#pragma once
#include "Global.h"

DWORD WINAPI ConsolePipe(LPVOID pPrm) {
	cout << "ConsolePipe checked\n";
	DWORD rc = 0;
	HANDLE hPipe;
	try 
	{
		char rnpname[50];
		strcpy_s(rnpname, "\\\\.\\pipe\\");
		strcat_s(rnpname, npname);
		if ((hPipe = CreateNamedPipe(rnpname, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_WAIT, 1, NULL, NULL, INFINITE, NULL)) == INVALID_HANDLE_VALUE) throw SetErrorMsgText("Create:", GetLastError());

		while (*((TalkersCommand*)pPrm) != EXIT) {
			if (!ConnectNamedPipe(hPipe, NULL)) throw SetErrorMsgText("Connect:", GetLastError());
			char ReadBuf[50], WriteBuf[50];
			DWORD nBytesRead = 0, nBytesWrite = 0;
			TalkersCommand SetCommand;
			bool serverCommand;

			//����������  ��������� ����� ������� ������ �������
			while (*((TalkersCommand*)pPrm) != EXIT) {
				//����� ��������� �������  getcommand ����������� ��������� ������� ���������� ��������
				if (*((TalkersCommand*)pPrm) == GETCOMMAND) {

					if (!ReadFile(hPipe, ReadBuf, sizeof(ReadBuf), &nBytesRead, NULL)) break;
					if (nBytesRead > 0) {
						serverCommand = true;
						int n = atoi(ReadBuf);
						switch (n) {
							case 0:
								sprintf_s(WriteBuf, "%s", "START");
								SetCommand = TalkersCommand::START;
								break;
							case 1:
								sprintf_s(WriteBuf, "%s", "STOP");
								SetCommand = TalkersCommand::STOP;
								break;
							case 2:
								sprintf_s(WriteBuf, "%s", "EXIT");
								SetCommand = TalkersCommand::EXIT;
								break;
							case 3:
								sprintf_s(WriteBuf, "\nActive: \t%i\nDenied: \t%i\nClosed: \t%i\n", Accept, Fail, Finished);
								serverCommand = false;
								break;
							case 4:
								sprintf_s(WriteBuf, "%s", "WAIT");
								SetCommand = TalkersCommand::WAIT;
								break;
							case 5:
								sprintf_s(WriteBuf, "%s", "SHUTDOWN");
								SetCommand = TalkersCommand::SHUTDOWN;
								break;
							default:
								sprintf_s(WriteBuf, "%s", "nocmd");
								serverCommand = false;
								break;
						}
						if (serverCommand == true) {
							*((TalkersCommand*)pPrm) = SetCommand;
							printf_s("ConsolePipe: ������� %s\n", WriteBuf);
						}
						if (!WriteFile(hPipe, WriteBuf, sizeof(WriteBuf), &nBytesRead, NULL)) throw new string("CP WRITE ERROR");
					}
				}
				else Sleep(1000);
			}
			if (!DisconnectNamedPipe(hPipe)) throw SetErrorMsgText("disconnect:", GetLastError());
		}
		DisconnectNamedPipe(hPipe);
		CloseHandle(hPipe);
		cout << "ConsolePipe closed" << endl;
	}
	catch (string ErrorPipeText) {
		cout << ErrorPipeText << endl;
	}
	catch (...) {
		cout << "Error ConsolePipe" << endl;
	}
	ExitThread(rc);
}