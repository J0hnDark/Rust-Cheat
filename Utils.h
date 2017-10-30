#pragma once
#include <Windows.h>
#include "Globals.h"
#include <math.h>
#include "Classes.h"
#include <D3DX10math.h>
#include <string>

void readCString(unsigned long long address, int length, char* string);
void readUString(unsigned long long address, int length, wchar_t* string);
extern Globals::Values values;
template <class T>
T read(unsigned long long address) {
	T buffer;
	ReadProcessMemory(values.handle, (LPCVOID)address, &buffer, sizeof(T), NULL);
	return buffer;
}

template <class T>
void write(unsigned long long address, T value) {
	WriteProcessMemory(values.handle, (LPVOID)address, &value, sizeof(T), NULL);
}
double get3DDistance(D3DXVECTOR3 origin, D3DXVECTOR3 player);
D3DXVECTOR2 calcmyangles(D3DXVECTOR3* LocalPos, D3DXVECTOR3* EnemyPos);
void setFontSize(int FontSize);
std::string GetMachineID();
void workerLoop();
DWORD_PTR GetProcessBaseAddress(DWORD processID);
void EAC();
bool isLocalPlayer(Entity entity);
BOOL ListProcessThreads(DWORD dwOwnerPID);
//(c) Apflmus