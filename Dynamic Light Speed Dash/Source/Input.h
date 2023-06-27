#pragma once
#include "Core.h"
#include <Xinput.h>

namespace Input {

	#define XINPUT_GAMEPAD_ANALOG_LEFT 0x0
	#define XINPUT_GAMEPAD_ANALOG_RIGHT 0x1

	#define XINPUT_GAMEPAD_TRIGGER_LEFT 0x0
	#define XINPUT_GAMEPAD_TRIGGER_RIGHT 0x1

	struct Vibration{
		float leftMotor;
		float rightMotor;
	};

	extern Vibration rumble;

	void Update();

	bool GetInputDown(int button);
	Vector2 GetAnalogStick(u8 analog);
	float GetTrigger(u8 trigger);
	bool IsAnalogActive(u8 analog);
	bool IsTriggerDown(u8 trigger);
}