#include "Classes.h"
#include "Globals.h"
#include "Utils.h"

extern Globals::Values values;
extern Entity entity[500];
extern D3DXMATRIX viewMatrix;

void fly(int type) {

	static DWORD64 playerMovement = (DWORD64)entity[2].movement;
	static DWORD64 modelState = entity[2].modelState;
	
	switch(type){
	case 0:
	
	//forward
		Sleep(100);
		write<float>(entity[2].visualState + 0x90, read<float>(entity[2].visualState + 0x90) + (2 * viewMatrix.m[0][2]));
		write<float>(entity[2].visualState + 0x98, read<float>(entity[2].visualState + 0x98) + (2 * viewMatrix.m[2][2]));
		Sleep(250);
		write<float>(entity[2].visualState + 0x90, read<float>(entity[2].visualState + 0x90) - (1 * viewMatrix.m[0][2]));
		write<float>(entity[2].visualState + 0x98, read<float>(entity[2].visualState + 0x98) - (1 * viewMatrix.m[2][2]));
		break;
	case 1:
		Sleep(100);
		write<float>(entity[2].visualState + 0x90, read<float>(entity[2].visualState + 0x90) + (2 * viewMatrix.m[0][0]));
		write<float>(entity[2].visualState + 0x98, read<float>(entity[2].visualState + 0x98) + (2 * viewMatrix.m[2][0]));

		Sleep(250);
		write<float>(entity[2].visualState + 0x90, read<float>(entity[2].visualState + 0x90) - (1 * viewMatrix.m[0][0]));
		write<float>(entity[2].visualState + 0x98, read<float>(entity[2].visualState + 0x98) - (1 * viewMatrix.m[2][0]));

	break;
	case 2:
		Sleep(100);
		write<float>(entity[2].visualState + 0x90, read<float>(entity[2].visualState + 0x90) - (2 * viewMatrix.m[0][0]));
		write<float>(entity[2].visualState + 0x98, read<float>(entity[2].visualState + 0x98) - (2 * viewMatrix.m[2][0]));

		Sleep(250);
		write<float>(entity[2].visualState + 0x90, read<float>(entity[2].visualState + 0x90) + (1 * viewMatrix.m[0][0]));
		write<float>(entity[2].visualState + 0x98, read<float>(entity[2].visualState + 0x98) + (1 * viewMatrix.m[2][0]));
	break;
	case 3:
		Sleep(100);
		write<float>(entity[2].visualState + 0x90, read<float>(entity[2].visualState + 0x90) - (2 * viewMatrix.m[0][2]));
		write<float>(entity[2].visualState + 0x98, read<float>(entity[2].visualState + 0x98) - (2 * viewMatrix.m[2][2]));

		Sleep(250);
		write<float>(entity[2].visualState + 0x90, read<float>(entity[2].visualState + 0x90) + (1 * viewMatrix.m[0][2]));
		write<float>(entity[2].visualState + 0x98, read<float>(entity[2].visualState + 0x98) + (1 * viewMatrix.m[2][2]));

		break;
	case 4:
	//up
		Sleep(100);
		write<float>(entity[2].visualState + 0x94, read<float>(entity[2].visualState + 0x94) + 2);

		Sleep(250);
		write<float>(entity[2].visualState + 0x94, read<float>(entity[2].visualState + 0x94) - 1);

		break;
	case 5:
	//down
		Sleep(100);
		write<float>(entity[2].visualState + 0x94, read<float>(entity[2].visualState + 0x94) - 2);

		Sleep(250);
		write<float>(entity[2].visualState + 0x94, read<float>(entity[2].visualState + 0x94) + 1);	
		break;


	}
}


//(c) Apflmus