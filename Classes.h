#pragma once
#include <Windows.h>
#include <D3DX10math.h>
class Entity {
public:
	float health;
	wchar_t name[50];
	int nameLength;
	float distance;
	ULONG_PTR visualState;
	ULONG_PTR baseEntity;
	ULONG_PTR playerInput;
	D3DXVECTOR2 bodyAngles;
	int flags;
	wchar_t activeWeaponName[50];
	D3DXVECTOR3 position;
	ULONG_PTR modelState;
	ULONG_PTR modelStateTick;
	ULONG_PTR inventory;
	int activeItem;
	BYTE state;
	INT16 tag;
	ULONG_PTR movement;
	bool isLocalPlayer;
	ULONG_PTR gameObject;
	char objectName[100];
};

class ActiveObject {
public:
	float distance;
	ULONG_PTR visualState;
	D3DXVECTOR2 screenPos;
	D3DXVECTOR3 position;
	INT16 layer;
	float health;
	ULONG_PTR gameObject;
	char objectName[100];
};



extern ActiveObject activeObjects[3000];
extern Entity entity[500];
//(c) Apflmus