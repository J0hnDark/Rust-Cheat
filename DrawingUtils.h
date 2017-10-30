#pragma once
#include <D3DX10math.h>
void initializeImguiAndWindow();
bool WorldToScreen(D3DXVECTOR3 origin, D3DXVECTOR2 * out);
void ChangeClickability(bool canclick, HWND ownd);
//(c) Apflmus