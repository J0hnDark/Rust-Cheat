//Defines the entry point for the console application.

#define WIN32_LEAN_AND_MEAN
#include "Blowfish.h"
#include "Socket.h"
#include "Globals.h"
#include <iostream>
#include "Utils.h"
#include "EntityLoop.h"
#include "OtherLoops.h"
#include "Classes.h"
#include <TlHelp32.h>
#include "DrawingUtils.h"
#include "Aimbot.h"
#include <wininet.h>
#include <Psapi.h>
#include <iostream>
#include <sstream>
#include "windefs.h"
#include <stdio.h>
#include <stdlib.h>
#include "process.hpp"
#include "dyn_data.hpp"

#pragma comment(lib, "wininet")
#pragma comment(lib, "urlmon")


Globals::Values values;
bool firstLaunch = true;
HANDLE authThread = INVALID_HANDLE_VALUE;




void keyListener()
{
	D3DXVECTOR3 tempPos;
	D3DXVECTOR2 tempScreenPos;
	DWORD exitCode = -1;
	DWORD integrityCheck = 0;
	while (true)
	{
		if (integrityCheck != 200)
		{
			if (GetExitCodeThread(authThread, &exitCode))
			{
				if (exitCode != 259)
					exit(0);
				if (ResumeThread(authThread) != 0)
					exit(0);

			}
			integrityCheck++;
		}
		else
			integrityCheck = 0;
		if (GetAsyncKeyState(VK_INSERT) & 1)
		{
			values.menuEnabled = !values.menuEnabled;
			Sleep(100);
		}
		if (GetAsyncKeyState(VK_DELETE) & 1)
		{
			values.inGame = !values.inGame;
			Sleep(100);
		}
		if (values.speedHack)
		{
			if (GetAsyncKeyState(0x43) & 1 || GetAsyncKeyState(0x43))
			{
				write<float>(values.timeManager + 0xF4, 8);
			}
			else
			{
				write<float>(values.timeManager + 0xF4, 1);
			}
		}
		if (GetAsyncKeyState(0x46) & 1 || GetAsyncKeyState(0x46))
		{
			if (values.inGame) {
				for (int x = 3; x <= values.numberOfTaggedObjects; x++) {

					if (isTargetableEntity(entity[x])) {
						tempPos = entity[x].position;
						tempPos.y += 0.8;
						if (WorldToScreen(tempPos, &tempScreenPos)) {
							if (inFOV(tempScreenPos.x, tempScreenPos.y))
								aimAtPlayer(entity[x]);
						}
					}


				}
			}
		}
		Sleep(2);
	}
}



char buffer[512];
char ipbuffer[32];
void auth()
{
	int sequence = 0;
	int timeleft = 1;
	std::stringstream packet;
	HINTERNET hInternet, hFile;
	DWORD rSize;
	char ipbuffer[32];
	packet.clear();
	hInternet = InternetOpen(NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	hFile = InternetOpenUrl(hInternet, "https://api.ipify.org/?format=txt", NULL, 0, INTERNET_FLAG_RELOAD, 0);
	InternetReadFile(hFile, &ipbuffer, sizeof(ipbuffer), &rSize);
	ipbuffer[rSize] = '\0';
	std::string HWID = GetMachineID();
	while (true) {
		packet.str("");
		if (firstLaunch)
		{
			packet << "02 cnt {" << HWID << "} [" << ipbuffer << "]";
			sendReceivePacket((char*)packet.str().c_str(), " 79.67.143.161", buffer);
			firstLaunch = 0;

			if (strstr("time", buffer))
			{
				std::cout << "\nReceived term.";
				Sleep(2000);
				exit(0);
			}
		}
		else
		{
			packet.clear();
			packet << "03 cin <" << sequence << ">" << "  {" << HWID << "}";
			ZeroMemory(buffer, sizeof(buffer));
			sendReceivePacket((char*)packet.str().c_str(), "79.67.143.161", buffer);

			if (strstr("term", buffer))
			{
				std::cout << "\nReceived term.";
				Sleep(3000);
				exit(0);
			}
			std::string timeLeft = buffer;
			timeleft = atoi(timeLeft.substr(timeLeft.find_first_of("{") + 1, timeLeft.find_first_of("{") - timeLeft.find_first_of("}")).c_str());
			sequence++;

		}
		if (timeleft <= 0)
		{
			std::cout << "\nYou have no time remaining!";
			exit(0);
		}

		Sleep(2500);
	}

}

void exitHandler()
{
	std::cout << "\nExiting!";
	char* out = new char[50];
	std::string ip = ipbuffer;
	CloseHandle(values.handle);
	std::string pcket = "exit {" + ip + "}";
	sendReceivePacket((char*)pcket.c_str(), "79.67.143.161", out);
}

int main()
{
	PROCESS_BASIC_INFORMATION info;
	ULONG bytes;
	std::atexit(exitHandler);
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)exitHandler, true);
	initalizeSocket();
	authThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)auth, NULL, NULL, NULL);
	while (!FindWindow("UnityWndClass", NULL) && !values.handle) {
		
		Sleep(5);
	}
	std::cout << "Launch detected.";
	dyn_data::load_information();
	//EAC();

	process::attach(GetCurrentProcessId());
	values.pid = process::find("RustClient.exe");
	values.handle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, false, values.pid);
	process::grant_handle_access(values.handle, PROCESS_ALL_ACCESS);
	process::detach();
	values.baseAddress = GetProcessBaseAddress(values.pid);
	values.gameObjectManager = 0;
	values.gameObjectManager = read<ULONG_PTR>(values.gameObjectOffset + values.baseAddress);
	values.timeManager = read<ULONG_PTR>(values.timeManagerOffset + values.baseAddress);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)EntityLoop, NULL, NULL, NULL);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)RockLoop, NULL, NULL, NULL);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)posDistUpdaterActive, NULL, NULL, NULL);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)workerLoop, NULL, NULL, NULL);

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)posDistUpdater, NULL, NULL, NULL);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)itemUpdate, NULL, NULL, NULL);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)keyListener, NULL, NULL, NULL);
	//CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)initializeImguiAndWindow, NULL, NULL, NULL);
	initializeImguiAndWindow();

	return 1;
}



//(c) Apflmus