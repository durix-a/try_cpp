#include <iostream>
#include <Windows.h>
#include <string.h>
#include <string>
#include "ControlCodes.h"

using namespace std;

int main()
{
	string outputMessage;
	HANDLE handle = CreateFile(L"\\\\.\\DosDriver1Decice1", GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (handle == INVALID_HANDLE_VALUE)
	{
		cout << "CreateFile failed, error " << GetLastError() << "\n";
		return 0;
	}

	char inputBuffer[15];
	char outputBuffer[30];
	DWORD bytesReturned = 0;

	strcpy_s(inputBuffer, sizeof(inputBuffer), "abcdefghijklmno");
	outputMessage.assign((char*)inputBuffer, (char*)inputBuffer + sizeof(inputBuffer));
	cout << "IOCTL input " << outputMessage << "\n";

	BOOL result = DeviceIoControl(handle, IOCTL_DRIVER1_GET_SOMETHING, inputBuffer, sizeof(inputBuffer), outputBuffer, sizeof(outputBuffer), &bytesReturned, NULL);
	if (result == FALSE)
	{
		cout << "DeviceControl failed, error " << GetLastError() << "\n";
		return 0;
	}

	cout << "IOCTL returned " << bytesReturned << " bytes\n";
	outputMessage.assign((char*)outputBuffer, (char*)outputBuffer + sizeof(outputBuffer));
	cout << "IOCTL ouputput " << outputMessage << "\n";
}
