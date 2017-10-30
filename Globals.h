#pragma once
#include <Windows.h>
#include "DrawingUtils.h"
#include "Renderer.h"
namespace Globals {
	class Values {
	public:
		ULONG_PTR baseAddress;
		ULONG_PTR gameObjectManager;
		ULONG_PTR gameObjectOffset = 0x14C1790;
		ULONG_PTR timeManagerOffset = (0x14C1930 + 0x38);
		ULONG_PTR timeManager;
		HANDLE handle;
		DWORD pid;
		bool dropESP;
		bool espScalingColor;
		bool corpseESP;
		bool activeItemESP;
		bool foodESP;
		bool sulfurnodeESP;
		bool metalnodeESP;
		bool stonenodeESP;
		int numberOfTaggedObjects;
		int espOtherDistance = 600;
		int activeObjectCount;
		bool inGame;
		bool noWater;
		HANDLE entityLoop;
		HANDLE posLoop;
		bool speedHack;
		bool noGrass;
		HANDLE consoleLoop;
		int espDistance = 450;
		bool menuEnabled;
		bool playerESP = true;
		bool playerBox = true;
		bool animalESP;
		bool tcESP;
		bool forceAutomatic;
		bool smallChestESP;
		bool sleepersESP;
		bool recoilEnabled;
		bool noDeploy;
		bool swayEnabled;
		bool spreadEnabled;
		bool freeCam;
		bool fastGather;
		bool flyHack;
		bool nightvision;
		bool noFall;
		bool sleepingBagsESP;
		bool lootESP;
		bool largeChestESP;
		bool stashESP;
		bool nodeESP;
		bool collectablesESP;
		bool helicopterESP;
		bool healthBars = true;
		bool helicopterFound;
		bool crosshair;
		bool radar;
		int aimbotFOV = 35;
		int fontSize = 18;
		int recoilMultiplier;
		int aimbotDistance = 150;
		DWORD playerColorClose = D3DCOLOR_ABGR(255, 255, 0, 0);
		DWORD playerBoxColor = D3DCOLOR_ABGR(255, 255, 127, 127);
		DWORD sleepersColor = D3DCOLOR_ABGR(255, 255, 255, 0);
		DWORD tcColor = D3DCOLOR_ABGR(255, 255, 255, 255);
		DWORD animalColor = D3DCOLOR_ABGR(255, 147, 10, 168);
		DWORD chestColor = D3DCOLOR_ABGR(255, 0, 255, 119);
		DWORD sulfurNodeColor = D3DCOLOR_ABGR(255, 233, 255, 95);
		DWORD stoneNodeColor = D3DCOLOR_ABGR(255, 121, 122, 114);
		DWORD metalNodeColor = D3DCOLOR_ABGR(255, 183, 183, 182);
		DWORD friendsColor = D3DCOLOR_ABGR(255, 0, 128, 255);
		DWORD foodColor = D3DCOLOR_ABGR(255, 255, 182, 193);
		DWORD stashColor = D3DCOLOR_ABGR(255, 51, 255, 255);
		DWORD dropColor = D3DCOLOR_ABGR(255, 51, 255, 255);
		DWORD corpseColor = D3DCOLOR_ABGR(255, 175, 108, 218);
		DWORD hempColor = D3DCOLOR_ABGR(255, 0, 153, 0);
		DWORD helicopterColor = D3DCOLOR_ABGR(255, 80, 0, 0);
		DWORD playerColorFar = D3DCOLOR_ABGR(255, 0, 255, 0);
		DWORD lootBoxColor = D3DCOLOR_ABGR(255, 167, 0, 173);
		DWORD crosshairColor = D3DCOLOR_ABGR(255, 0, 255, 0);
		DWORD barrelColor = D3DCOLOR_ABGR(255, 122, 77, 0);
		DWORD trashColor = D3DCOLOR_ABGR(255, 99, 77, 52);
		DWORD sulfurColor = D3DCOLOR_ABGR(255, 233, 255, 95);
		DWORD logColor = D3DCOLOR_ABGR(255, 140, 95, 44);
		DWORD stoneColor = D3DCOLOR_ABGR(255, 121, 122, 114);
		DWORD metalColor = D3DCOLOR_ABGR(255, 183, 183, 182);
		DWORD airDropColor = D3DCOLOR_ABGR(255, 31, 152, 173);
		DWORD sleepingBagsColor = D3DCOLOR_ABGR(255, 47, 96, 94);
	};
}
//(c) Apflmus