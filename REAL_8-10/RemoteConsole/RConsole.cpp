#include <stdio.h>
#include <tchar.h>
#include "iostream"
#include "Windows.h"                
#include "ctime"
#include <string>
using std::string;
using namespace std;

string GetErrorMsgText(int code) // c����������� ����� ������
{
	char buff[50];
	DWORD bufflen = sizeof(buff);
	DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0) {
		return std::string(); //No error message has been recorded
	}
	else {
		return std::system_category().message(errorMessageID);
	}
};
string SetPipeError(string msgText, int code)
{
	return msgText + to_string(code) + ". " + GetErrorMsgText(code);
};


int _tmain(int argc, _TCHAR* argv[]) {
	setlocale(LC_ALL, "Russian");

	char ReadBuf[50] = "";
	char WriteBuf[2] = "";
	DWORD nBytesRead;
	DWORD nBytesWrite;

	#pragma region SecurityAttributes
		SECURITY_DESCRIPTOR* m_pSecDesc = (SECURITY_DESCRIPTOR*)LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
		SECURITY_ATTRIBUTES m_pSecAttrib = { sizeof(SECURITY_ATTRIBUTES), m_pSecDesc, TRUE };
		InitializeSecurityDescriptor(m_pSecDesc, SECURITY_DESCRIPTOR_REVISION);
		SetSecurityDescriptorDacl(m_pSecDesc, TRUE, (PACL)NULL, FALSE);
	#pragma endregion


	int Code = 0;

	char serverName[256];
	char PipeName[512];
	bool result;

	try 
	{
		printf_s("\n ---------- All commands ---------- \n");
		printf_s("1 - start  \t \n");
		printf_s("2 - stop  \t \n");
		printf_s("3 - exit  \t \n");
		printf_s("4 - statistics\t \n");
		printf_s("5 - wait  \t \n");
		printf_s("6 - shutdown  \t \n");
		printf_s("0 - close\n");
		printf_s("\n ---------- ---------- ---------- ---------- \n");

		cout << "Server name: ";
		cin >> serverName;
		result = sprintf_s(PipeName, "\\\\%s\\pipe\\cpipe", serverName);


		HANDLE hNamedPipe = CreateFile(PipeName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, &m_pSecAttrib);

		do {
			printf_s("Pick a number: ");
			scanf_s("%d", &Code);
			if (Code > 0 && Code < 7) {
				sprintf_s(WriteBuf, "%d", Code - 1);
				if (!WriteFile(hNamedPipe, WriteBuf, strlen(WriteBuf) + 1, &nBytesWrite, NULL)) throw "WriteFile: ������ ";
				if (!ReadFile(hNamedPipe, ReadBuf, sizeof(ReadBuf), &nBytesRead, NULL)) throw "ReadFile: ������ ";
				cout << ReadBuf << endl;
			}
			if (Code == 0) break;
		}
		while (true); 

		if (!CloseHandle(hNamedPipe)) throw SetPipeError("CloseHandle: ", GetLastError());
	}
	catch (string ErrorPipeText)
	{
		cout << endl << ErrorPipeText;
	}
	cout << "RConsole Close :(\n\n";
	return 0;
}
