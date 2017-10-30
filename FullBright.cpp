#include "Globals.h"
#include "Utils.h"

float origvalue1;
float origvalue2;
DWORD64 lightColorOrig;
void setToDay()
{
	ULONG_PTR objectClass = read<ULONG_PTR>(entity[1].gameObject + 0x30);
	ULONG_PTR entityPtr = read<ULONG_PTR>(objectClass + 0x18);
	ULONG_PTR skyDome = read<ULONG_PTR>(entityPtr + 0x28);
	ULONG_PTR todCycle = read<ULONG_PTR>(skyDome + 0x18);
	write<float>(todCycle + 0x10, 12);
}


//(c) Apflmus