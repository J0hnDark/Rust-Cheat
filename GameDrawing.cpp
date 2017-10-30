#include "DrawingUtils.h"
#include <Windows.h>
#include "Utils.h"
#include "Renderer.h"
#include <iostream>
#include <string>
#include <clocale>
#include <cstdlib>
#include "XorString.h"


extern Globals::Values values;
extern Entity entity[500];
extern ActiveObject helicopter;
char display[50];
char weapon[50];
char name[50];



void DrawBoxS(int x) {
	D3DXVECTOR2 tempHead;
	D3DXVECTOR2 tempFeet;
	D3DXVECTOR3 tempHeadPos;

		
		if (WorldToScreen(entity[x].position, &tempFeet)) {
			tempHeadPos = entity[x].position + D3DXVECTOR3(0, 1.7, 0);
			WorldToScreen(tempHeadPos, &tempHead);
			float h = tempHead.y - tempFeet.y;
			float w = h / 4; //= head.y - foot.y) /4
							 //DrawBorder(tempScreen.x - w, tempFeet.y, w * 2, h, 2, playerBoxColor);
			DrawBorder(ImVec2((int)tempFeet.x - w, (int)tempFeet.y), ImVec2((int)w * 2, (int)h), values.playerBoxColor, 0, -1, 2);
			//DrawBorder(ImVec2(tempScreen.x - w, tempFeet.y), ImVec2(w * 2, h), playerBoxColor, 0.0f, -1, 2);
		}


}

void DrawHealthActive(int x) {

	D3DXVECTOR3 torsoPos;
	D3DXVECTOR2 tempHealthScreen;
	torsoPos = activeObjects[x].position;


	if (WorldToScreen(torsoPos, &tempHealthScreen)) {
		tempHealthScreen.y += 18;
		DrawHealthBox2(tempHealthScreen.x, tempHealthScreen.y, D3DCOLOR_ABGR(255, 255, 0, 0), D3DCOLOR_ABGR(255, 0, 255, 0), (int)helicopter.health, 1800);

	}
}

void DrawHealth(int x) {
	D3DXVECTOR3 torsoPos;
	D3DXVECTOR2 tempHealthScreen;
	torsoPos = entity[x].position;
		

		if (WorldToScreen(torsoPos, &tempHealthScreen)){
			tempHealthScreen.y += 18;
			DrawHealthBox(tempHealthScreen.x, tempHealthScreen.y, D3DCOLOR_ABGR(255, 255, 0, 0), D3DCOLOR_ABGR(255, 0, 255, 0), (int)entity[x].health, 100);

		}
}

std::string UnicodeToUTF8(const std::wstring& ws)
{
	std::string s;
	for (int i = 0; i < ws.size(); ++i)
	{
		wchar_t wc = ws[i];
		if (0 <= wc && wc <= 0x7f)
		{
			s += (char)wc;
		}
		else if (0x80 <= wc && wc <= 0x7ff)
		{
			s += (0xc0 | (wc >> 6));
			s += (0x80 | (wc & 0x3f));
		}
		else if (0x800 <= wc && wc <= 0xffff)
		{
			s += (0xe0 | (wc >> 12));
			s += (0x80 | ((wc >> 6) & 0x3f));
			s += (0x80 | (wc & 0x3f));
		}
		else if (0x10000 <= wc && wc <= 0x1fffff)
		{
			s += (0xf0 | (wc >> 18));
			s += (0x80 | ((wc >> 12) & 0x3f));
			s += (0x80 | ((wc >> 6) & 0x3f));
			s += (0x80 | (wc & 0x3f));
		}
		else if (0x200000 <= wc && wc <= 0x3ffffff)
		{
			s += (0xf8 | (wc >> 24));
			s += (0x80 | ((wc >> 18) & 0x3f));
			s += (0x80 | ((wc >> 12) & 0x3f));
			s += (0x80 | ((wc >> 6) & 0x3f));
			s += (0x80 | (wc & 0x3f));
		}
		else if (0x4000000 <= wc && wc <= 0x7fffffff)
		{
			s += (0xfc | (wc >> 30));
			s += (0x80 | ((wc >> 24) & 0x3f));
			s += (0x80 | ((wc >> 18) & 0x3f));
			s += (0x80 | ((wc >> 12) & 0x3f));
			s += (0x80 | ((wc >> 6) & 0x3f));
			s += (0x80 | (wc & 0x3f));
		}
	}
	return s;
}
void DrawEnts()
{
	
	D3DXVECTOR3 tempHeadPos;
	D3DXVECTOR2 tempScreen;
	XorS(A, "Corpse ");
	for (int x = 0; x <= values.numberOfTaggedObjects; x++) {
			if (entity[x].distance > values.espDistance)
				continue;
			if (entity[x].tag == 20009 && values.corpseESP)
			{
				sprintf(display, A.decrypt(), "[%d]", (int)entity[x].distance);
				if (WorldToScreen(entity[x].position, &tempScreen))
					DrawText2(ImVec2(tempScreen.x, tempScreen.y), values.fontSize, values.corpseColor, true, display);
			}
			else if (x != 0&& x != 1 && !entity[x].isLocalPlayer &&  entity[x].tag == 6 && entity[x].health > 0) {


				std::string temp = UnicodeToUTF8(entity[x].name);

				sprintf(display, u8"%s [%d]", temp.c_str(), (int)entity[x].distance);
				if (values.activeItemESP) {
					std::string temp2 = UnicodeToUTF8(entity[x].activeWeaponName);
					sprintf(weapon, u8"%s", temp2.c_str());
				}
				if (WorldToScreen(entity[x].position, &tempScreen)) {
					BYTE state = (entity[x].state & 8);
					if (state == 8) {
						

						if (values.sleepersESP) {
							DrawText2(ImVec2(tempScreen.x, tempScreen.y), values.fontSize, values.sleepersColor, true, display);
						}
					}


					else {
						if (values.playerESP) {
							if (entity[x].distance < 150) {
								DrawText2(ImVec2(tempScreen.x, tempScreen.y), values.fontSize, values.playerColorClose, true, display);
								if (values.activeItemESP)
								{
									DrawText2(ImVec2(tempScreen.x, tempScreen.y+18), values.fontSize, D3DCOLOR_ABGR(255,255,255,255), true, weapon);
								}
							}
							else {
								DrawText2(ImVec2(tempScreen.x, tempScreen.y), values.fontSize, values.playerColorFar, true, display);
								if (values.activeItemESP)
								{
									DrawText2(ImVec2(tempScreen.x, tempScreen.y + 18), values.fontSize, D3DCOLOR_ABGR(255, 255, 255, 255), true, weapon);
								}
							}
						}
						if (values.playerBox)
							DrawBoxS(x);
						if (values.healthBars) {
							DrawHealth(x);
						}
					}
				}
			}
			
	
	}
}

DWORD white = D3DCOLOR_ABGR(255, 255, 255, 255);

void DrawRocks() {

	for (int r = 0; r <= values.activeObjectCount; r++) {

		if (activeObjects[r].distance <= values.espOtherDistance) {
			if (strstr(activeObjects[r].objectName, "metal-ore") && values.metalnodeESP) {

				if (WorldToScreen(activeObjects[r].position, &activeObjects[r].screenPos)) {
					if (!values.espScalingColor)
						DrawText2(ImVec2(activeObjects[r].screenPos.x, activeObjects[r].screenPos.y), values.fontSize, values.metalNodeColor, true, "Metal Node [%d]",
						(int)(get3DDistance(entity[0].position, activeObjects[r].position)));
					//else
					//		DrawText2(ImVec2(activeObjects[r].screenPos.x, activeObjects[r].screenPos.y), values.fontSize, D3DCOLOR_ABGR(255,), true, "Metal Node [%d]",
					//		(int)(get3DDistance(entity[0].position, activeObjects[r].position)));

				}
			}
			if (activeObjects[r].layer == 26 && values.dropESP)
			{
				if (WorldToScreen(activeObjects[r].position, &activeObjects[r].screenPos))
					DrawText2(ImVec2(activeObjects[r].screenPos.x, activeObjects[r].screenPos.y), values.fontSize, values.dropColor, true, "Item [%d]",
					(int)(get3DDistance(entity[0].position, activeObjects[r].position)));
			}
			else if (strstr(activeObjects[r].objectName, "stone-ore") && values.stonenodeESP) {

				if (WorldToScreen(activeObjects[r].position, &activeObjects[r].screenPos))
					DrawText2(ImVec2(activeObjects[r].screenPos.x, activeObjects[r].screenPos.y), values.fontSize, values.stoneNodeColor, true, "Stone Node [%d]",
					(int)(get3DDistance(entity[0].position, activeObjects[r].position)));
			}
			else if (strstr(activeObjects[r].objectName, "sulfur-ore") && values.sulfurnodeESP) {

				if (WorldToScreen(activeObjects[r].position, &activeObjects[r].screenPos))
					DrawText2(ImVec2(activeObjects[r].screenPos.x, activeObjects[r].screenPos.y), values.fontSize, values.sulfurNodeColor, true, "Sulfur Node [%d]",
					(int)(get3DDistance(entity[0].position, activeObjects[r].position)));
			}
			else if (strstr(activeObjects[r].objectName, "cupboard.tool.deployed") && values.tcESP) {


				if (WorldToScreen(activeObjects[r].position, &activeObjects[r].screenPos))
					DrawText2(ImVec2(activeObjects[r].screenPos.x, activeObjects[r].screenPos.y), values.fontSize, values.tcColor, true, "TC [%d]",
					(int)(get3DDistance(entity[0].position, activeObjects[r].position)));
			}
			else if (strstr(activeObjects[r].objectName, "mushroom") && values.foodESP)
			{
				if (WorldToScreen(activeObjects[r].position, &activeObjects[r].screenPos))
					DrawText2(ImVec2(activeObjects[r].screenPos.x, activeObjects[r].screenPos.y), values.fontSize, values.foodColor, true, "Mushroom [%d]",
					(int)(get3DDistance(entity[0].position, activeObjects[r].position)));
			}
			else if (strstr(activeObjects[r].objectName, "pumpkin") && values.foodESP)
			{
				if (WorldToScreen(activeObjects[r].position, &activeObjects[r].screenPos))
					DrawText2(ImVec2(activeObjects[r].screenPos.x, activeObjects[r].screenPos.y), values.fontSize, values.foodColor, true, "Pumpkin [%d]",
					(int)(get3DDistance(entity[0].position, activeObjects[r].position)));
			}
			else if (strstr(activeObjects[r].objectName, "corn") && values.foodESP)
			{
				if (WorldToScreen(activeObjects[r].position, &activeObjects[r].screenPos))
					DrawText2(ImVec2(activeObjects[r].screenPos.x, activeObjects[r].screenPos.y), values.fontSize, values.foodColor, true, "Corn [%d]",
					(int)(get3DDistance(entity[0].position, activeObjects[r].position)));
			}
			else if (strstr(activeObjects[r].objectName, "stash") && values.stashESP) {

				if (WorldToScreen(activeObjects[r].position, &activeObjects[r].screenPos))
					DrawText2(ImVec2(activeObjects[r].screenPos.x, activeObjects[r].screenPos.y), values.fontSize, values.stashColor, true, "Stash [%d]",
					(int)(get3DDistance(entity[0].position, activeObjects[r].position)));
			}
			else if (strstr(activeObjects[r].objectName, "chicken") && values.animalESP) {

				if (WorldToScreen(activeObjects[r].position, &activeObjects[r].screenPos))
					DrawText2(ImVec2(activeObjects[r].screenPos.x, activeObjects[r].screenPos.y), values.fontSize, values.animalColor, true, "Chicken [%d]",
					(int)(get3DDistance(entity[0].position, activeObjects[r].position)));
			}
			else if (strstr(activeObjects[r].objectName, "horse") && values.animalESP) {

				if (WorldToScreen(activeObjects[r].position, &activeObjects[r].screenPos))
					DrawText2(ImVec2(activeObjects[r].screenPos.x, activeObjects[r].screenPos.y), values.fontSize, values.animalColor, true, "Horse [%d]",
					(int)(get3DDistance(entity[0].position, activeObjects[r].position)));
			}
			else if (strstr(activeObjects[r].objectName, "boar.") && values.animalESP) {

				if (WorldToScreen(activeObjects[r].position, &activeObjects[r].screenPos))
					DrawText2(ImVec2(activeObjects[r].screenPos.x, activeObjects[r].screenPos.y), values.fontSize, values.animalColor, true, "Boar [%d]",
					(int)(get3DDistance(entity[0].position, activeObjects[r].position)));
			}
			else if (strstr(activeObjects[r].objectName, "bear") && values.animalESP) {

				if (WorldToScreen(activeObjects[r].position, &activeObjects[r].screenPos))
					DrawText2(ImVec2(activeObjects[r].screenPos.x, activeObjects[r].screenPos.y), values.fontSize, values.animalColor, true, "Bear [%d]",
					(int)(get3DDistance(entity[0].position, activeObjects[r].position)));
			}
			else if (strstr(activeObjects[r].objectName, "stag") && values.animalESP) {

				if (WorldToScreen(activeObjects[r].position, &activeObjects[r].screenPos))
					DrawText2(ImVec2(activeObjects[r].screenPos.x, activeObjects[r].screenPos.y), values.fontSize, values.animalColor, true, "Stag [%d]",
					(int)(get3DDistance(entity[0].position, activeObjects[r].position)));
			}
			else if (strstr(activeObjects[r].objectName, "wolf") && values.animalESP) {

				if (WorldToScreen(activeObjects[r].position, &activeObjects[r].screenPos))
					DrawText2(ImVec2(activeObjects[r].screenPos.x, activeObjects[r].screenPos.y), values.fontSize, values.animalColor, true, "Wolf [%d]",
					(int)(get3DDistance(entity[0].position, activeObjects[r].position)));
			}
			else if (strstr(activeObjects[r].objectName, "zombie") && values.animalESP) {

				if (WorldToScreen(activeObjects[r].position, &activeObjects[r].screenPos))
					DrawText2(ImVec2(activeObjects[r].screenPos.x, activeObjects[r].screenPos.y), values.fontSize, values.animalColor, true, "Zombie [%d]",
					(int)(get3DDistance(entity[0].position, activeObjects[r].position)));
			}

			else if (strstr(activeObjects[r].objectName, "crate") && values.largeChestESP) {

				if (WorldToScreen(activeObjects[r].position, &activeObjects[r].screenPos))
					DrawText2(ImVec2(activeObjects[r].screenPos.x, activeObjects[r].screenPos.y), values.fontSize, values.lootBoxColor, true, "Crate [%d]",
					(int)(get3DDistance(entity[0].position, activeObjects[r].position)));
			}
			else if (strstr(activeObjects[r].objectName, "supply_drop") && values.lootESP) {

				if (WorldToScreen(activeObjects[r].position, &activeObjects[r].screenPos))
					DrawText2(ImVec2(activeObjects[r].screenPos.x, activeObjects[r].screenPos.y), values.fontSize, values.airDropColor, true, "Airdrop [%d]",
					(int)(get3DDistance(entity[0].position, activeObjects[r].position)));
			}
			else if (strstr(activeObjects[r].objectName, "patrolhelicopter") && values.helicopterESP && values.helicopterFound) {

				if (WorldToScreen(activeObjects[r].position, &activeObjects[r].screenPos)) {
					DrawText2(ImVec2(activeObjects[r].screenPos.x, activeObjects[r].screenPos.y), values.fontSize, values.helicopterColor, true, "Helicopter [%d]",
						(int)(get3DDistance(entity[0].position, activeObjects[r].position)));
					DrawHealthActive(r);
				}

			}

			else if (strstr(activeObjects[r].objectName, "woodbox") && values.smallChestESP) {

				if (WorldToScreen(activeObjects[r].position, &activeObjects[r].screenPos))
					DrawText2(ImVec2(activeObjects[r].screenPos.x, activeObjects[r].screenPos.y), values.fontSize, values.chestColor, true, "Small Chest [%d]",
					(int)(get3DDistance(entity[0].position, activeObjects[r].position)));

			}
			else if (strstr(activeObjects[r].objectName, "box.wooden.large") && values.largeChestESP) {

				if (WorldToScreen(activeObjects[r].position, &activeObjects[r].screenPos))
					DrawText2(ImVec2(activeObjects[r].screenPos.x, activeObjects[r].screenPos.y), values.fontSize, values.chestColor, true, "Large Chest [%d]",
					(int)(get3DDistance(entity[0].position, activeObjects[r].position)));
			}
			else if (strstr(activeObjects[r].objectName, "sleeping") && values.sleepingBagsESP) {

				if (WorldToScreen(activeObjects[r].position, &activeObjects[r].screenPos))
					DrawText2(ImVec2(activeObjects[r].screenPos.x, activeObjects[r].screenPos.y), values.fontSize, values.sleepingBagsColor, true, "Sleeping Bag [%d]",
					(int)(get3DDistance(entity[0].position, activeObjects[r].position)));
			}
			else if ((strstr(activeObjects[r].objectName, "trash-pile")) && values.lootESP) {

				if (WorldToScreen(activeObjects[r].position, &activeObjects[r].screenPos))
					DrawText2(ImVec2(activeObjects[r].screenPos.x, activeObjects[r].screenPos.y), values.fontSize, values.trashColor, true, "Trash [%d]",
					(int)(get3DDistance(entity[0].position, activeObjects[r].position)));
			}
			else if ((strstr(activeObjects[r].objectName, "barrel")) && values.lootESP) {

				if (WorldToScreen(activeObjects[r].position, &activeObjects[r].screenPos))
					DrawText2(ImVec2(activeObjects[r].screenPos.x, activeObjects[r].screenPos.y), values.fontSize, values.barrelColor, true, "Barrel [%d]",
					(int)(get3DDistance(entity[0].position, activeObjects[r].position)));
			}
			else if ((strstr(activeObjects[r].objectName, "hemp_mature") || strstr(activeObjects[r].objectName, "hemp-collectable.prefab") || strstr(activeObjects[r].objectName, "hemp.entity.prefab")) && values.collectablesESP) {

				if (WorldToScreen(activeObjects[r].position, &activeObjects[r].screenPos))
					DrawText2(ImVec2(activeObjects[r].screenPos.x, activeObjects[r].screenPos.y), values.fontSize, values.hempColor, true, "Hemp [%d]",
					(int)(get3DDistance(entity[0].position, activeObjects[r].position)));
			}
			else if ((strstr(activeObjects[r].objectName, "metal-collectable") && values.collectablesESP)) {

				if (WorldToScreen(activeObjects[r].position, &activeObjects[r].screenPos))
					DrawText2(ImVec2(activeObjects[r].screenPos.x, activeObjects[r].screenPos.y), values.fontSize, values.metalColor, true, "Metal [%d]",
					(int)(get3DDistance(entity[0].position, activeObjects[r].position)));
			}
			else if (strstr(activeObjects[r].objectName, "tree_stump") && values.collectablesESP) {

				if (WorldToScreen(activeObjects[r].position, &activeObjects[r].screenPos))
					DrawText2(ImVec2(activeObjects[r].screenPos.x, activeObjects[r].screenPos.y), values.fontSize, values.logColor, true, "Log [%d]",
					(int)(get3DDistance(entity[0].position, activeObjects[r].position)));
			}
			else if (strstr(activeObjects[r].objectName, "sulfur-collectable.prefab") && values.collectablesESP) {

				if (WorldToScreen(activeObjects[r].position, &activeObjects[r].screenPos))
					DrawText2(ImVec2(activeObjects[r].screenPos.x, activeObjects[r].screenPos.y), values.fontSize, values.sulfurColor, true, "Sulfur [%d]",
					(int)(get3DDistance(entity[0].position, activeObjects[r].position)));
			}
			else if ((strstr(activeObjects[r].objectName, "stone-collectable") && values.collectablesESP)) {

				if (WorldToScreen(activeObjects[r].position, &activeObjects[r].screenPos))
					DrawText2(ImVec2(activeObjects[r].screenPos.x, activeObjects[r].screenPos.y), values.fontSize, values.stoneColor, true, "Stone [%d]",
					(int)(get3DDistance(entity[0].position, activeObjects[r].position)));
			}

		}
	}

}




//(c) Apflmus