#include "pch.h"
FUNCTION_PTR(__int64, __fastcall, sub_140BD6C60, 0x140BD6C60, __int64 a1, __int64 a2);

float range = 0.0f;

SIG_SCAN
(
	sigsub_1401BE650,
	0x1401BE650,
	"\x48\x89\x54\x24\x10\x53\x41\x57\x48\x83\xEC\x78\x48\x8B\x05\x2D\xDF\x99\x03\x4C\x8B\xFA\x48\x8B\x52\x08\x48\x8B\xD9\x48\x8B\x88",
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
)
HOOK(__int64, __fastcall, rangeSet, sigsub_1401BE650(), __int64 a1, __int64 a2) {
	auto v2 = (unsigned int*)a2;
	auto result = sub_140BD6C60((long long)v2, (long long)"RangeSpawning");
	auto x = *(unsigned int**)(result + 24);
	*(float*)x *= range;
	return originalrangeSet(a1, a2);
}
extern "C" {

	__declspec(dllexport) void Init()
	{
		const INIReader reader("mod.ini");
		if (reader.ParseError() != 0)
		{
			MessageBoxA(nullptr, "Cannot load configuration file, this mod will not function.", "Custom Pop-in", MB_ICONERROR);
			return;
		}
		range = reader.GetFloat("Main", "multiplier", 2.0f);

		INSTALL_HOOK(rangeSet);
	}
}