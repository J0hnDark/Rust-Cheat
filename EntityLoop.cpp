#include <Windows.h>
#include "EntityLoop.h"
#include "Classes.h"
#include <iostream>
extern bool inGame;
extern Globals::Values values;
Entity entity[500];
ActiveObject activeObjects[3000];

ActiveObject helicopter;
void EntityLoop()
{
	ULONG_PTR taggedObjectsPtrTemp, lastTaggedObject = 0;
	ULONG_PTR taggedObjectsPtr = read<DWORD64>(values.gameObjectManager + 0x8);
	lastTaggedObject = read<DWORD64>(values.gameObjectManager);
	int c = 0;
	
	bool restart = false;
	while (true) {
		ULONG_PTR taggedObjectsPtr = read<DWORD64>(values.gameObjectManager + 0x8);
		lastTaggedObject = read<DWORD64>(values.gameObjectManager);
		taggedObjectsPtrTemp = taggedObjectsPtr;
		c = 0;
		
		
		while (lastTaggedObject != taggedObjectsPtrTemp) 
		{

			lastTaggedObject = read<DWORD64>(values.gameObjectManager);
			entity[c].gameObject = read<DWORD64>(taggedObjectsPtrTemp + 0x10);
			INT16 tag = read<INT16>(entity[c].gameObject + 0x5C);
			if (tag == 0)
			{
				taggedObjectsPtr = read<DWORD64>(values.gameObjectManager + 0x8);
				lastTaggedObject = read<DWORD64>(values.gameObjectManager);
				break;
			}
			if (tag == 5) 
			{
				ULONG_PTR objectClass = read<DWORD64>(entity[c].gameObject + 0x30);
				ULONG_PTR entityPtr = read<DWORD64>(objectClass + 0x18);
				ULONG_PTR transform = read<DWORD64>(objectClass + 0x8);
				
				entity[c].visualState = read<DWORD64>(transform + 0x38);
				entity[c].tag = tag;
				c++;
			}
			else if (tag == 20009)
			{
				ULONG_PTR objectClass = read<DWORD64>(entity[c].gameObject + 0x30);
				ULONG_PTR entityPtr = read<DWORD64>(objectClass + 0x18);
				ULONG_PTR transform = read<DWORD64>(objectClass + 0x8);
				entity[c].visualState = read<DWORD64>(transform + 0x38);
				entity[c].tag = tag;
				c++;
			}
			else if (tag == 20011)
			{
				ULONG_PTR objectClass = read<DWORD64>(entity[c].gameObject + 0x30);
				entity[c].tag = tag;
				c++;
			}
			else if (tag == 6) 
			{
				entity[c].tag = tag;
				ULONG_PTR objectClass = read<DWORD64>(entity[c].gameObject + 0x30);
				ULONG_PTR entityPtr = read<DWORD64>(objectClass + 0x18);
				entity[c].baseEntity = read<DWORD64>(entityPtr + 0x28);
				entity[c].inventory = read<DWORD64>(entity[c].baseEntity + 0x378);

				ULONG_PTR objectNamePtr = read<ULONG_PTR>(entity[c].gameObject + 0x68);
				readCString(objectNamePtr, 50, entity[c].objectName);
				if (strcmp(entity[c].objectName, "LocalPlayer") == 0)
				{
					entity[c].isLocalPlayer = 1;
					entity[c].playerInput = read<ULONG_PTR>(entity[c].baseEntity + 0x390);
					entity[c].movement = read<ULONG_PTR>(entity[c].baseEntity + 0x398);
					ULONG_PTR transform = read<DWORD64>(objectClass + 0x8);
					entity[c].visualState = read<DWORD64>(transform + 0x38);
					entity[c].modelState = read<DWORD64>(entity[c].baseEntity + 0x418);
					entity[c].modelStateTick = read<DWORD64>(entity[c].baseEntity + 0x438);
				

					if (values.freeCam)
						write<int>(entity[c].baseEntity + 0x440, 260);
				}
				
				else {

				
					entity[c].isLocalPlayer = 0;
					ULONG_PTR transform = read<DWORD64>(objectClass + 0x8);
					entity[c].visualState = read<DWORD64>(transform + 0x38);
					entity[c].modelState = read<DWORD64>(entity[c].baseEntity + 0x418);
					
					ULONG_PTR unicodeString = read<DWORD64>(entity[c].baseEntity + 0x3C8);
					entity[c].nameLength = read<int>(unicodeString + 0x10);
					entity[c].flags = read<int>(entity[c].baseEntity + 0x440);
			

					RtlZeroMemory(entity[c].name, sizeof(entity[c].name));
					readUString(unicodeString + 0x14, entity[c].nameLength , entity[c].name);
				}
				c++;
				
			}
			taggedObjectsPtrTemp = read<DWORD64>(taggedObjectsPtrTemp + 0x8);
			if (lastTaggedObject == taggedObjectsPtrTemp)
			{
				values.numberOfTaggedObjects = c;
				break;
			}
		}
		
		if (c < 3)
		{
			values.inGame = false;
			std::cout << "\nNot in game.";
			Sleep(1000);
			taggedObjectsPtr = read<DWORD64>(values.gameObjectManager + 0x8);
			lastTaggedObject = read<DWORD64>(values.gameObjectManager);
		}
		else {
			values.inGame = true;
			taggedObjectsPtr = read<DWORD64>(values.gameObjectManager + 0x8);
			lastTaggedObject = read<DWORD64>(values.gameObjectManager);
		}
		Sleep(4000);
	}




}

void RockLoop() {
	ULONG_PTR activeObject, activeObjectPtrTemp, lastActiveObject;
	int c = 0;

	while (true) {
		
		if (values.helicopterFound) {
			
			}
		if (values.inGame) {
			activeObject = read<DWORD64>(values.gameObjectManager + 0x18);
			activeObjectPtrTemp = activeObject;
			lastActiveObject = read<DWORD64>(values.gameObjectManager + 0x10);

			c = 0;
			while ((DWORD64)(lastActiveObject) != (DWORD64)(activeObjectPtrTemp)) {
				lastActiveObject = read<DWORD64>(values.gameObjectManager + 0x10);
				activeObjects[c].gameObject = read<DWORD64>(activeObjectPtrTemp + 0x10);
				ULONG_PTR objectNamePtr = read<ULONG_PTR>(activeObjects[c].gameObject + 0x68);

				readCString(objectNamePtr, 100, activeObjects[c].objectName);
				int layer = read<int>(activeObjects[c].gameObject + 0x58);
				if (activeObjectPtrTemp != 0 && (strstr(activeObjects[c].objectName, "metal-ore") || layer == 26 || layer == 11 && strstr(activeObjects[c].objectName, "rust.ai/agents") || strstr(activeObjects[c].objectName, "sleeping") || strstr(activeObjects[c].objectName, "metal-collectable") || strstr(activeObjects[c].objectName, "sulfur-collectable.prefab") || strstr(activeObjects[c].objectName, "tree_stump") || strstr(activeObjects[c].objectName, "stone-collectable") || strstr(activeObjects[c].objectName, "stone-ore") || strstr(activeObjects[c].objectName, "supply_drop") || strstr(activeObjects[c].objectName, "sulfur-ore") || strstr(activeObjects[c].objectName, "cupboard.tool.deployed") || strstr(activeObjects[c].objectName, "stash") || strstr(activeObjects[c].objectName, "trash-pile") || strstr(activeObjects[c].objectName, "loot-barrel") || strstr(activeObjects[c].objectName, "loot_barrel") || strstr(activeObjects[c].objectName, "hemp_mature") || strstr(activeObjects[c].objectName, "hemp-collectable.prefab") || strstr(activeObjects[c].objectName, "hemp.entity.prefab") || strstr(activeObjects[c].objectName, "crate_normal") || strstr(activeObjects[c].objectName, "crate_tools") || strstr(activeObjects[c].objectName, "corn.entity") || strstr(activeObjects[c].objectName, "pumpkin.entity") || strstr(activeObjects[c].objectName, "woodbox") || strstr(activeObjects[c].objectName, "box.wooden.large"))) {
					ULONG_PTR objectClass = read<ULONG_PTR>(activeObjects[c].gameObject + 0x30);
					ULONG_PTR transform = read<ULONG_PTR>(objectClass + 0x8);
					ULONG_PTR entityPtr = read<ULONG_PTR>(objectClass + 0x18);
					ULONG_PTR baseEntity = read<ULONG_PTR>(objectClass + 0x28);
					activeObjects[c].health = read<float>(objectClass + 0x13c);

					activeObjects[c].visualState = read<ULONG_PTR>(transform + 0x38);
					activeObjects[c].layer = layer;
					c++;
				}

				else if (strstr(activeObjects[c].objectName, "grass.v2(Clone)")) {
					if (values.noGrass) {
						ULONG_PTR objectClass = read<ULONG_PTR>(activeObjects[c].gameObject + 0x30);
						ULONG_PTR entityPtr = read<ULONG_PTR>(objectClass + 0x18);
						ULONG_PTR baseEntity = read<ULONG_PTR>(entityPtr + 0x28);
						write<int>(baseEntity + 0x64, 0);
						write<int>(baseEntity + 0x68, 0);
						c++;
					}

					


				}
				else if (strcmp(activeObjects[c].objectName, "assets/prefabs/npc/patrol helicopter/patrolhelicopter.prefab") == 0)
				{
					ULONG_PTR objectClass = read<ULONG_PTR>(activeObjects[c].gameObject + 0x30);
					ULONG_PTR transform = read<ULONG_PTR>(objectClass + 0x8);
					activeObjects[c].visualState = read<ULONG_PTR>(transform + 0x38);
					helicopter = activeObjects[c];
					c++;
				}

				activeObjectPtrTemp = read<DWORD64>(activeObjectPtrTemp + 0x8);
				if ((DWORD64)lastActiveObject == (DWORD64)activeObjectPtrTemp || c > 4998) {
					values.activeObjectCount = c;
					break;
				}
				if (!activeObjectPtrTemp) {
					activeObjectPtrTemp = activeObject;
					break;
				}
				
			}
			Sleep(10000);
		}
		Sleep(2000);

	}


}


//(c) Apflmus