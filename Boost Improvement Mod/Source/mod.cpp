#include "Core.h"
#include "Input.h"

#define QWORD __int64

float originalMaxSpeed[3];
float* boostGauge;
bool prevBoosting;

namespace Config {
	namespace Input {
		bool analogBoost, analogBoostDrain, rumble;
	}
	namespace Gameplay {
		bool enabled = true;
		float boostDrain, additionalBoostDrain, naturalBoostGain;
		float trickBoostGain, ringBoostGain, combatBoostGain, reigniteRatio;
	}
}

#pragma region Scans
CL_SCAN_SIGNATURE(sigsub_140828C90, "\x4C\x63\xD2\x0F\x57\xC0\x4D\x8B\xCA\x49\xC1\xE1\x05\x49\x8B\x44\x09\x18\x49\x8B\x54\x09\x10\x4C\x8D\x04\xC0\x4A\x8D\x04\x82\x48\x3B\xC2\x74\x6C\x0F\x2F\x40\xE4", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
CL_SCAN_SIGNATURE(sigsub_1408D44B0, "\x40\x53\x55\x56\x57\x41\x56\x48\x81\xEC\x80\x00\x00\x00\x4C\x8B\x71\x20", "xxxxxxxxxxxxxxxxxx")
CL_SCAN_SIGNATURE(sigsub_1408D35B0, "\x40\x53\x48\x83\xEC\x20\x0F\xBE\x91\x84\x00\x00\x00", "xxxxxxxxxxxxx")
CL_SCAN_SIGNATURE(sigsub_1408CC830, "\x48\x89\x5C\x24\x10\x48\x89\x74\x24\x18\x55\x57\x41\x56\x48\x8D\x6C\x24\xB9\x48\x81\xEC\xC0\x00\x00\x00\x48\x8B\x71\x20", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
#pragma endregion Scans

#pragma region Function Pointers
FUNCTION_PTR(char, __fastcall, sub_1408D35B0, sigsub_1408D35B0(), u64 a1);
#pragma endregion Function Pointers

#pragma region Hooks
HOOK(__m128, __fastcall, TopSpeedGrabber, sigsub_140828C90(), u64 a1, TypeOfSpeed typeOfSpeed) {
	u64 v4 = 32i64 * typeOfSpeed;
	u64 v5 = *(u64*)(v4 + a1 + 16);
	u64 v6 = v5 + 36i64 * *(u64*)(v4 + a1 + 24);

	float* speedInit = (float*)(v6 - 8);
	float* speedMax = (float*)(v6 - 12);
	float* speedMin = (float*)(v6 - 16);

	if (std::fmod(*speedMax,1.0f) == 0.0f) {
		originalMaxSpeed[typeOfSpeed] = *speedMax;
	}

	if (typeOfSpeed != Normal) {
		if (Config::Input::analogBoost) {
			//Set it to a weird arbitrary float so we can check if it's a real one.
			*speedInit = *speedMax = roundToNearest(Lerp(*speedMin, originalMaxSpeed[typeOfSpeed], Input::GetTrigger(XINPUT_GAMEPAD_TRIGGER_RIGHT)), 0.0123f);
		}
	}
	return originalTopSpeedGrabber(a1, typeOfSpeed);
}
HOOK(u64, __fastcall, Trick, sigsub_1408CC830(), u64 a1) {
	if (boostGauge != nullptr) {
		*boostGauge += Config::Gameplay::trickBoostGain;
	}
	return originalTrick(a1);
}
HOOK(bool, __fastcall, BoostGaugeUser, sigsub_1408D44B0(), u64 a1, float a2) {
	boostGauge = (float*)(a1 + 60);
	float oldBoost = *boostGauge;
	bool result = originalBoostGaugeUser(a1, a2);
	bool boosting = !(bool)sub_1408D35B0(a1);
	if (boosting) {
		prevBoosting = true;
		if (Config::Input::rumble) {
			Input::Vibrate(0, 2500);
		}
	}
	if (prevBoosting && !boosting) {
		if (Config::Input::rumble) {
			Input::Vibrate(0, 1);
		}
		prevBoosting = false;
	}
	if (Config::Input::analogBoost && Config::Input::analogBoostDrain) {
		if (oldBoost > *boostGauge) {
			*boostGauge = Lerp(oldBoost, *boostGauge, Input::GetTrigger(XINPUT_GAMEPAD_TRIGGER_RIGHT));
		}
	}
	return result;
}
#pragma endregion Hooks

extern "C" {

	__declspec(dllexport) void Init()
	{
		INIReader reader("config.ini");
		Config::Input::analogBoost = reader.GetBoolean("input", "analogBoost", true);
		Config::Input::analogBoostDrain = reader.GetBoolean("input", "analogBoostDrain", true);
		Config::Input::rumble = reader.GetBoolean("input", "rumble", true);

		Config::Gameplay::enabled = reader.GetBoolean("gameplay", "enabled", true);
		Config::Gameplay::boostDrain = reader.GetFloat("gameplay", "boostDrain", 20.0f);
		Config::Gameplay::additionalBoostDrain = reader.GetFloat("gameplay", "additionalBoostDrain", 100.0f);
		Config::Gameplay::naturalBoostGain = reader.GetFloat("gameplay", "naturalBoostGain", 2.0f);
		Config::Gameplay::trickBoostGain = reader.GetFloat("gameplay", "trickBoostGain", 10.0f);
		Config::Gameplay::ringBoostGain = reader.GetFloat("gameplay", "ringBoostGain", 2.0f);
		Config::Gameplay::combatBoostGain = reader.GetFloat("gameplay", "combatBoostGain", 0.5f);
		Config::Gameplay::reigniteRatio = reader.GetFloat("gameplay", "reigniteRatio", 20.0f);

		INSTALL_HOOK(TopSpeedGrabber);
		INSTALL_HOOK(BoostGaugeUser);
		INSTALL_HOOK(Trick);
	}
}