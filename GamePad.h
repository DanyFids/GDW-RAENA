#pragma once
#include <Windows.h>
#include "Xinput.h"

#pragma comment(lib, "XInput9_1_0.lib")

class Gamepad
{
private:
	int cId = -1;
	XINPUT_STATE state;

	float deadzoneX = 0;;
	float deadzoneY = 0;;

public:
	Gamepad() : deadzoneX(0.05f), deadzoneY(0.02f) {}
	Gamepad(float dzX, float dzY) : deadzoneX(dzX), deadzoneY(dzY) {}

	float leftStickX = 0;
	float leftStickY = 0;
	float rightStickX = 0;
	float rightStickY = 0;
	float leftTrigger = 0;
	float rightTrigger = 0;

	int  GetPort();
	XINPUT_GAMEPAD *GetState();
	bool CheckConnection();
	bool Refresh();
	bool IsPressed(WORD);
};

