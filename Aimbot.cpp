#include "Aimbot.h"
extern Globals::Values values;
extern int Width;
extern int Height;



float get2DDistance(D3DXVECTOR2 a, D3DXVECTOR2 b) {

	return sqrt(powf(a.x - b.x, 2) + powf(a.y - b.y, 2));

}

bool inFOV(double x, double y)
{


	if (get2DDistance(D3DXVECTOR2(Width / 2, Height / 2), D3DXVECTOR2(x, y)) < values.aimbotFOV) {
		return true;
	}
	else
		return false;



}

bool HasFlag(int flag, int state)
{
	return (state & (int)flag) == (int)flag;
}

bool isTargetableEntity(Entity player) {
	BYTE state;
	if (player.health <= 0)
		return false;
	if (get3DDistance(player.position, entity[0].position) > values.aimbotDistance)
		return false;
	
	if (HasFlag(0x8, player.state))
	{
		return false;
	}
		return true;
}

void aimAtPlayer(Entity player) {
	D3DXVECTOR2 toWrite = { 0,0 };
	D3DXVECTOR3 headPos = { 0,0,0 };
	D3DXVECTOR3 bodyPos = { 0,0,0 };


	while ((GetAsyncKeyState(0x46) & 1 || GetAsyncKeyState(0x46)))
	{
		//getPosition((void*)player->getObjectClass()->getEntity()->getBaseEntity()->getPlayerModel()->getSkinnedMultiMesh()->getBoneDict()->getValues()->getBoneObject(48)->getTransform(), &headPos);
		
		headPos = read<D3DXVECTOR3>(player.visualState + 0x90);
		BOOL ducking = HasFlag(1, player.state);
		if (ducking)
			headPos.y += 0.85;
		else
			headPos.y += 1.45;
		toWrite = calcmyangles(&entity[0].position, &headPos);
		write<D3DXVECTOR2>(entity[2].playerInput + 0x44, toWrite);
		if (!isTargetableEntity(player)) {
			Sleep(100);
			break;
		}
		Sleep(2);
	}

}
