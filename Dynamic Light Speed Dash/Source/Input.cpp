#pragma once
#ifndef Input_CPP
#define Input_CPP
#include "Input.h"
#include <winuser.h>
namespace Input {
#pragma comment(lib, "XInput.lib")
	Vibration rumble;
	XINPUT_STATE padState;
	int currentIndex;
	std::map<int, int> GamepadToKey{
		{XINPUT_GAMEPAD_LEFT_SHOULDER, 'Q'},
		{XINPUT_GAMEPAD_RIGHT_SHOULDER, 'E'},
		{XINPUT_GAMEPAD_A, VK_SPACE},
		{XINPUT_GAMEPAD_Y,VK_LBUTTON},
		{XINPUT_GAMEPAD_X, VK_RBUTTON},
		{XINPUT_GAMEPAD_B, VK_CONTROL},
		{XINPUT_GAMEPAD_LEFT_THUMB,'Z'},
		{XINPUT_GAMEPAD_RIGHT_THUMB,'C'},
		{XINPUT_GAMEPAD_DPAD_DOWN,'H'},
		{XINPUT_GAMEPAD_DPAD_UP,'Y'},
		{XINPUT_GAMEPAD_DPAD_RIGHT,'N'},
		{XINPUT_GAMEPAD_DPAD_LEFT,'G'},
		{XINPUT_GAMEPAD_START,'P'},
		{XINPUT_GAMEPAD_BACK,'M'}
	};

	void Vibrate(u16 leftVal, u16 rightVal)
	{
		XINPUT_VIBRATION Vibration;

		ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

		Vibration.wLeftMotorSpeed = leftVal;
		Vibration.wRightMotorSpeed = rightVal;

		XInputSetState(currentIndex, &Vibration);
	}

	bool IsConnected() {
		ZeroMemory(&padState, sizeof(XINPUT_STATE));
		for (int i = 3; i >= 0; i--) {
			if (XInputGetState(i, &padState) == ERROR_SUCCESS) {
				currentIndex = i;
			}
		}
		return XInputGetState(currentIndex, &padState) == ERROR_SUCCESS;
	}

	Vector2 GetAnalogStick(u8 analog) {
		XInputGetState(currentIndex, &padState);

		if (!IsConnected()) {
			if (analog == XINPUT_GAMEPAD_ANALOG_LEFT) {
				Vector2 result = Vector2::ZERO;
				if (GetAsyncKeyState('W')) result.y++;
				if (GetAsyncKeyState('S')) result.y--;
				if (GetAsyncKeyState('D')) result.x++;
				if (GetAsyncKeyState('A')) result.x--;
				return result.normalized();
			}
			return Vector2::ZERO;
		}

		if (analog == XINPUT_GAMEPAD_ANALOG_LEFT) {
			if (abs(padState.Gamepad.sThumbLX) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
				abs(padState.Gamepad.sThumbLY) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
				return Vector2::ZERO;
			}
		}

		if (analog == XINPUT_GAMEPAD_ANALOG_RIGHT) {
			if (abs(padState.Gamepad.sThumbRX) < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
				abs(padState.Gamepad.sThumbRY) < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
				return Vector2::ZERO;
			}
		}

		return analog == XINPUT_GAMEPAD_ANALOG_LEFT ?
			Vector2(padState.Gamepad.sThumbLX / 32767.0f, padState.Gamepad.sThumbLY / 32767.0f) :
			Vector2(padState.Gamepad.sThumbRX / 32767.0f, padState.Gamepad.sThumbRY / 32767.0f);
	}

	bool IsAnalogActive(u8 analog) {
		XInputGetState(currentIndex, &padState);
		u16 threshold = analog == 1 ? XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE : XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
		return GetAnalogStick(analog).magnitude() > 0.45f;
	}
}
#endif