#include "Classes.h"
#include "Utils.h"
#include <iostream>
#include <algorithm>
extern Globals::Values values;
Entity entityCache[500];
extern D3DXMATRIX viewMatrix;
extern ActiveObject helicopter;

DWORD64 lookingAt;

bool HasFlag(int flag, int state);

void workerLoop()
{
	while (true)
	{
		while (values.inGame)
		{
			if (values.noWater)
			{
				write<float>(entity[2].modelState + 0x10, 0);
				write<int>(entity[2].modelState + 0x20, 4);
			}
			if (values.noFall)
			{
				write<BYTE>(entity[2].movement + 0xE6, 0); //need update
				write<BYTE>(entity[2].movement + 0xE7, 0); //need update
			}
			Sleep(2);
		}
		Sleep(200);
	}
}

void posDistUpdater()
{
	while (true) {
		while (values.inGame)
		{
			for (int x = 0; x <= values.numberOfTaggedObjects; x++)
			{
				entity[x].position = read<D3DXVECTOR3>(entity[x].visualState + 0x90);
				if (entity[x].tag == 5)
				{
					ULONG_PTR objectClass = read<DWORD64>(entity[x].gameObject + 0x30);
					ULONG_PTR entityPtr = read<DWORD64>(objectClass + 0x18);
					viewMatrix = read<D3DXMATRIX>(entityPtr + 0xC0);
				}
				else if (entity[x].tag == 6) {
					entity[x].distance = get3DDistance(entity[0].position, entity[x].position);
					entity[x].health = read<float>(entity[x].baseEntity + 0x1F8);
					entity[x].state = read<BYTE>(entity[x].modelState + 0x20);
					if (!entity[x].isLocalPlayer) {
						entity[x].bodyAngles = calcmyangles(&entity[0].position, &entity[x].position);
						entity[x].activeItem = read<int>(entity[x].baseEntity + 0x49C); 
					}
					else
					{
						entity[x].bodyAngles = read<D3DXVECTOR2>(entity[x].playerInput + 0x44);
					
					}
				}
				else if (entity[x].tag == 20009)
				{
					entity[x].distance = get3DDistance(entity[0].position, entity[x].position);
				}
			}
			
			Sleep(1);

		}
		Sleep(200);
	}

}

void itemUpdate()
{

	while (true) {
		while (values.inGame && values.activeItemESP) {
			for (int x = 0; x <= values.numberOfTaggedObjects; x++)
			{
				
				if (entity[x].inventory && x!= 0 && x != 1 && x != 2 && entity[x].tag == 6 && entity[x].health > 0 && !HasFlag(8, entity[x].state) && entity[x].distance < values.espDistance)
				{
					ULONG_PTR belt = read<ULONG_PTR>(entity[x].inventory + 0x28);
					if (!belt)
						continue;
					ULONG_PTR beltList = read<ULONG_PTR>(belt + 0x20);
					if (!beltList)
						continue;
					ULONG_PTR items = read<ULONG_PTR>(beltList + 0x10);
					if (!items)
						continue;
					for (int z = 0; z <= 6; z++)
					{
						ULONG_PTR item = read<ULONG_PTR>(items + 0x20 + (0x8 * z));
						ULONG_PTR itemUID = read<int32_t>(item + 0x78);
						if (itemUID != entity[x].activeItem || !entity[x].activeItem || !itemUID)
							continue;
						
						ULONG_PTR itemDef = read<ULONG_PTR>(item + 0x10);
						if (!item || !itemDef)
							continue;
						ULONG_PTR weaponNamePtr = read<ULONG_PTR>(itemDef + 0x18);
						wchar_t weaponName[50];
						readUString(weaponNamePtr + 0x14, 15, weaponName);
						ZeroMemory(entity[x].activeWeaponName, 50);
						memcpy(entity[x].activeWeaponName, weaponName, 50);
					}
				}
						

			}
			Sleep(2000);
		}
		Sleep(150);
	}


}

void posDistUpdaterActive()
{
	while (true) {
		while (values.inGame) {
			for (int x = 0; x <= values.activeObjectCount; x++)
			{
				activeObjects[x].position = read<D3DXVECTOR3>(activeObjects[x].visualState + 0x90);
				activeObjects[x].distance = get3DDistance(entity[0].position, activeObjects[x].position);
				if (activeObjects[x].gameObject == helicopter.gameObject)
				{
					values.helicopterFound = true;
					ULONG_PTR objectClass = read<ULONG_PTR>(helicopter.gameObject + 0x30);
					ULONG_PTR entityPtr = read<ULONG_PTR>(objectClass + 0x18);
					ULONG_PTR baseEntity = read<ULONG_PTR>(entityPtr + 0x28);
					ULONG_PTR weakspots = read<ULONG_PTR>(baseEntity + 0x1E8);
					ULONG_PTR weakspot1 = read<ULONG_PTR>(weakspots + 0x20);
					ULONG_PTR weakspot2 = read<ULONG_PTR>(weakspots + 0x28);
					helicopter.health = read<float>(weakspot1 + 0x4C) + read<float>(weakspot1 + 0x4C);
					if (helicopter.health <= 0)
					{
						ZeroMemory(&helicopter, sizeof(helicopter));
					}

				}
			}
			Sleep(50);
		}
		Sleep(100);
	}

}



//(c) Apflmus