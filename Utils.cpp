#include <Windows.h>
#include "Utils.h"
#include <iostream>
#include <Psapi.h>
#include <TlHelp32.h>
#include "Structs.h"
#include <sstream>
#include "process.hpp"
#include "dyn_data.hpp"




extern Globals::Values values;


/*
 EAC stuff isn't implementiert
*/
DWORD_PTR GetProcessBaseAddress(DWORD processID)
{
	DWORD_PTR   baseAddress = 0;
	HMODULE     *moduleArray;
	LPBYTE      moduleArrayBytes;
	DWORD       bytesRequired;

	if (values.handle)
	{
		if (EnumProcessModules(values.handle, NULL, 0, &bytesRequired))
		{
			if (bytesRequired)
			{
				moduleArrayBytes = (LPBYTE)LocalAlloc(LPTR, bytesRequired);

				if (moduleArrayBytes)
				{
					unsigned int moduleCount;

					moduleCount = bytesRequired / sizeof(HMODULE);
					moduleArray = (HMODULE *)moduleArrayBytes;

					if (EnumProcessModules(values.handle, moduleArray, bytesRequired, &bytesRequired))
					{
						baseAddress = (DWORD_PTR)moduleArray[0];
						return baseAddress;
					}

					LocalFree(moduleArrayBytes);
				}
			}
		}
	}
}

std::string GetMachineID()
{
	// LPCTSTR szHD = "C:\\";  // ERROR
	std::string ss;
	ss = "Err_StringIsNull";
	UCHAR szFileSys[255],
		szVolNameBuff[255];
	DWORD dwSerial;
	DWORD dwMFL;
	DWORD dwSysFlags;
	int error = 0;

	bool success = GetVolumeInformation(LPCTSTR("C:\\"), (LPTSTR)szVolNameBuff,
		255, &dwSerial,
		&dwMFL, &dwSysFlags,
		(LPTSTR)szFileSys,
		255);
	if (!success) {
		ss = "Err_Not_Elevated";
	}
	std::stringstream errorStream;
	errorStream << dwSerial;
	return std::string(errorStream.str().c_str());
}


void readUString(unsigned long long address, int length, wchar_t* string) {

	ReadProcessMemory(values.handle, (LPCVOID)address, string, (length * 2), NULL);

}


void readCString(unsigned long long address, int length, char* string) {

	ReadProcessMemory(values.handle, (LPCVOID)address, string, length, NULL);

}


double get3DDistance(D3DXVECTOR3 origin, D3DXVECTOR3 player)
{
	return sqrt(
		pow((origin.x - player.x), 2) +
		pow((origin.y - player.y), 2) +
		pow((origin.z - player.z), 2));
}

D3DXVECTOR2 calcmyangles(D3DXVECTOR3* LocalPos, D3DXVECTOR3* EnemyPos) {
	D3DXVECTOR2 toWrite;
	D3DXVECTOR3 relative;
	D3DXVec3Subtract(&relative, LocalPos, EnemyPos);
	float magnitude = D3DXVec3Length(&relative);
	double pitch = asin(relative[1] / magnitude);
	double yaw = -atan2(relative[0], -relative[2]);

	yaw = D3DXToDegree(yaw);
	pitch = D3DXToDegree(pitch);

	toWrite.x = pitch;
	toWrite.y = yaw;
	return toWrite;
}
void setFontSize(int FontSize)
{
	CONSOLE_FONT_INFOEX info = { 0 };
	info.cbSize = sizeof(info);
	info.dwFontSize.Y = FontSize; // leave X as zero
	info.FontWeight = FW_NORMAL;
	wcscpy(info.FaceName, L"Lucida Console");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), NULL, &info);
}


bool isLocalPlayer(Entity entity)
{
	if (strcmp(entity.objectName, "LocalPlayer") == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}



//(c) Apflmus