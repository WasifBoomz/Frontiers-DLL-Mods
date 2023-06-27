#include "Core.h"
#include "Input.h"

#define QWORD __int64

CL_SCAN_SIGNATURE(sigsub_140123980, "\x48\x8B\x41\x10\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x48\x89\x51\x10\xC3", "xxxxx???????????xxxxx");
FUNCTION_PTR(void*, __fastcall, sub_140123980, sigsub_140123980(), void* a1);
CL_SCAN_SIGNATURE(sigsub_14080B180, "\x48\x89\x5C\x24\x10\x48\x89\x74\x24\x18\x55\x48\x8B\xEC", "xxxxxxxxxxxxxx");
FUNCTION_PTR(void*, __fastcall, sub_14080B180, sigsub_14080B180(), void* a1, void* a2);

namespace Config {
	float minimumBrake;
	float maximumBrake;
}

PlayerParamLightDash* lightDash;

CL_SCAN_SIGNATURE(sigsub_140842EA0, "\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x20\x48\x8B\x71\x18\x48\x8B\xE9", "xxxxxxxxxxxxxxxxxxxxxxxxxxx");

HOOK(void, __fastcall, PostureLightDashStart, sigsub_140842EA0(), __m128* a1) {
	lightDash = (PlayerParamLightDash*)LoadPlayerParam(sub_140123980((void*)a1[1].m128_i64[1]), (void*)0x143C3FB80, sub_14080B180);
	lightDash->brake = Input::IsAnalogActive(XINPUT_GAMEPAD_ANALOG_LEFT) ? Config::minimumBrake : Config::maximumBrake;
	originalPostureLightDashStart(a1);
}
extern "C" {

	__declspec(dllexport) void Init()
	{
		INIReader reader("mod.ini");
		Config::minimumBrake = reader.GetFloat("Main", "minimumBrake", 0.0f);
		Config::maximumBrake = reader.GetFloat("Main", "maximumBrake", 1000.0f);
	
		INSTALL_HOOK(PostureLightDashStart);
	}
}