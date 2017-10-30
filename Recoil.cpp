#include "Classes.h"
#include "Globals.h"
#include "Utils.h"
extern Globals::Values values;
ULONG_PTR inventory;
ULONG_PTR weaponClass;
ULONG_PTR recoil;
wchar_t weaponName[50];
class RecoilProperties
{
public:
	float originalx1;
	float originalx2;
	float originaly1;
	float originaly2;
	float originalRepeat;
	ULONG_PTR weaponClass;
};
RecoilProperties origValues[6];
void doRecoil()
{

	if (values.forceAutomatic || values.recoilEnabled || values.spreadEnabled || values.swayEnabled || values.fastGather) {
		for (int x = 0; x <= values.numberOfTaggedObjects; x++)
		{
			if (isLocalPlayer(entity[x]))
			{
				inventory = entity[x].inventory;
			}
		}
		if (inventory)
		{
			ULONG_PTR belt = read<ULONG_PTR>(inventory + 0x28);
			if (!belt)
				return;
			ULONG_PTR beltList = read<ULONG_PTR>(belt + 0x20);
			if (!beltList)
				return;
			ULONG_PTR items = read<ULONG_PTR>(beltList + 0x10);
			if (!items)
				return;
			for (int x = 0; x <= 6; x++)
			{
				ULONG_PTR item = read<ULONG_PTR>(items + 0x20 + (0x8 * x));
				weaponClass = 0;
				ULONG_PTR itemDef = read<ULONG_PTR>(item + 0x10);
				ZeroMemory(weaponName, 50);
				ULONG_PTR weaponNamePtr = read<ULONG_PTR>(itemDef + 0x18);
				readUString(weaponNamePtr + 0x14, 15, weaponName);
				weaponClass = read<ULONG_PTR>(item + 0x58);
				if (wcscmp(weaponName, L"pistol.python") == 0 || wcscmp(weaponName, L"bow.hunting") == 0 || wcscmp(weaponName, L"crossbow") == 0 || wcscmp(weaponName, L"pistol.m92") == 0 || (wcscmp(weaponName, L"smg.mp5") == 0 || wcscmp(weaponName, L"smg.2") == 0 || wcscmp(weaponName, L"rifle.ak") == 0 || wcscmp(weaponName, L"lmg.m249") == 0 || wcscmp(weaponName, L"pistol.revolver") == 0 || wcscmp(weaponName, L"shotgun.pump") == 0 || wcscmp(weaponName, L"shotgun.waterpipe") == 0 || wcscmp(weaponName, L"smg.thompson") == 0 || wcscmp(weaponName, L"rifle.semiauto") == 0 || wcscmp(weaponName, L"pistol.semiauto") == 0 || wcscmp(weaponName, L"crossbow") == 0 || wcscmp(weaponName, L"rifle.bolt") == 0 || wcscmp(weaponName, L"rifle.lr300") == 0 || wcscmp(weaponName, L"shotgun.double") == 0 || wcscmp(weaponName, L"rocket.launcher") == 0 || wcscmp(weaponName, L"smg.thompson") == 0)) {
					


					if (weaponClass)
					{



					
					if (values.swayEnabled)
					{
						write<float>(weaponClass + 0x260, 0.0f);
						write<float>(weaponClass + 0x264, 0.0f);

						write<float>(weaponClass + 0x268, 0.0f);

						write<float>(weaponClass + 0x26C, 0.0f);

						write<float>(weaponClass + 0x270, 0.0f);

						write<float>(weaponClass + 0x274, 0.0f);

						write<float>(weaponClass + 0x278, 0.0f);

						write<float>(weaponClass + 0x27C, 0.0f);
						write<float>(weaponClass + 0x28C, 0.0f);

					}
					/*if (values.spreadEnabled) {
						if (read<float>(weaponClass + 0x230) < 6.0)
							write<float>(weaponClass + 0x230, 0);
						if (read<float>(weaponClass + 0x234) < 6.0)
							write<float>(weaponClass + 0x234, 0);
						if (read<float>(weaponClass + 0x238) < 6.0)
							write<float>(weaponClass + 0x238, 0);
						if (read<float>(weaponClass + 0x23C) < 6.0)
							write<float>(weaponClass + 0x23C, 0);
						if (read<float>(weaponClass + 0x240) < 6.0)
							write<float>(weaponClass + 0x240, 0);
						if (read<float>(weaponClass + 0x244) < 6.0)
							write<float>(weaponClass + 0x244, 0);
					}*/
					if (values.forceAutomatic)
						write<BYTE>(weaponClass + 0x254, 1);
						
					if (values.recoilEnabled)
					{
						recoil = 0;
						recoil = read<ULONG_PTR>(weaponClass + 0x228);
						if (origValues[x].weaponClass != weaponClass)
						{
							origValues[x].originalx1 = read<float>(recoil + 0x28);
							origValues[x].originalx2 = read<float>(recoil + 0x2c);
							origValues[x].originaly1 = read<float>(recoil + 0x30);
							origValues[x].originaly2 = read<float>(recoil + 0x34);
							write<float>(recoil + 0x44, 0.0f);
							origValues[x].weaponClass = weaponClass;
						}
						write<float>(recoil + 0x28, origValues[x].originalx1 * (values.recoilMultiplier / 100));
						write<float>(recoil + 0x2c, origValues[x].originalx2 * (values.recoilMultiplier / 100));
						write<float>(recoil + 0x30, origValues[x].originaly1 * (values.recoilMultiplier / 100));
						write<float>(recoil + 0x34, origValues[x].originaly2 * (values.recoilMultiplier / 100));

					}
					}
				}
				else if (wcsstr(weaponName, L"axe") || wcsstr(weaponName, L"pick") || wcsstr(weaponName, L"hatchet") || wcsstr(weaponName, L"spear") || wcsstr(weaponName, L"sword") || wcsstr(weaponName, L"mace") || wcsstr(weaponName, L"salvaged"))
				{
					if (values.fastGather && weaponClass)
					{
						if (origValues[x].weaponClass != weaponClass) {
							origValues[x].originalRepeat = read<float>(weaponClass + 0x18C);
							origValues[x].weaponClass = weaponClass;
						}
						else
						{
							write<float>(weaponClass + 0x18C, (origValues[x].originalRepeat / 2) + 0.1);
						}
					}
				}
			}



		}

	}
}



//(c) Apflmus