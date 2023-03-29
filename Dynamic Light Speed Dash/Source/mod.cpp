#include "pch.h"
#include <xmmintrin.h>
#define Magnitude(a,b) sqrt(a*a+b*b)
#define QWORD __int64

FUNCTION_PTR(__int64, __fastcall, sub_14011AAC0, 0x14011AAC0, __int64 a1);
FUNCTION_PTR(__int64, __fastcall, sub_1407C1B60, 0x1407C1B60, __int64 a1);
FUNCTION_PTR(void*, , sub_1400BC6C0, 0x1400BC6C0, );
FUNCTION_PTR(__int64, __fastcall, sub_1407C2050, 0x1407C2050, __int64 a1, __int64 a2);
FUNCTION_PTR(void*, , sub_1407C24C0, 0x1407C24C0, );
FUNCTION_PTR(__int64, __fastcall, sub_140BCC1F0, 0x140BCC1F0, __int64 a1, __int64 a2);

extern "C" {
	__declspec(dllexport) void Update();
}

struct Vector2 {
	float x;
	float y;
	Vector2()
		: x(0), y(0) {}
	Vector2(float mX, float mY)
		: x(mX), y(mY) {}
};

float* lightdashSpeed;
float brakeMin;
float brakeMax;
Vector2* analogMove;
Vector2* analogCam;
void* velocity;
void* prevVel;

SIG_SCAN
(
	sigsub_1407F6920,
	0x1407F6920,
	"\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x60\x48\x8B\x69\x18\x48\x8B\xF9\x0F\x29\x74\x24\x50",
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
)
HOOK(__int64, __fastcall, lightDash, sigsub_1407F6920(), __m128* a1, float a2) {
	auto v2 = a1[1].m128_i64[1];
	auto v6 = sub_14011AAC0(v2);
	auto v7 = sub_1407C24C0();
	auto v8 = sub_140BCC1F0(v6, (__int64)v7);
	auto v9 = sub_1400BC6C0();
	auto v10 = (float*)sub_1407C2050(v8, (__int64)v9);
	lightdashSpeed = ((float*)(v10 + 5));
	return originallightDash(a1, a2);
}
SIG_SCAN
(
	sigsub_140A172F0,
	0x140A172F0,
	"\x49\x63\xC0\x4C\x6B\xC0\x1C\x48\x8B\x81\xD0\x00\x00\x00\xF2\x41\x0F\x10\x44\x00\x10\x48\x8B\xC2\xF2\x0F\x11\x02\xC3",
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
)
HOOK(QWORD*, __fastcall, analogGrabber, sigsub_140A172F0(), __int64 a1, QWORD* a2, int a3) {
	QWORD xP = (QWORD)(28i64 * a3 + *(QWORD*)(a1 + 208) + 16);
	QWORD* x = (QWORD*)xP;
	if (analogMove == nullptr && analogCam == nullptr) {
		analogMove = (Vector2*)x;
	}
	if (analogCam == nullptr && analogMove != nullptr) {
		if ((__int64)x > (__int64)analogMove) {
			analogCam = (Vector2*)x;
		}
		if ((__int64)x < (__int64)analogMove) {
			analogCam = analogMove;
			analogMove = (Vector2*)x;
		}
	}
	if (analogMove == nullptr && analogCam != nullptr) {
		if ((__int64)x < (__int64)analogCam) {
			analogMove = (Vector2*)x;
		}
		if ((__int64)x > (__int64)analogCam) {
			analogMove = analogCam;
			analogCam = (Vector2*)x;
		}
	}
	return originalanalogGrabber(a1, a2, a3);
}
SIG_SCAN
(
	sigsub_1407BED60,
	0x1407BED60,
	"\x40\x53\x48\x83\xEC\x60\x0F\xB6\x81\xA0\x05\x00\x00\x48\x8B\xD9\xC0\xE8\x04\x0F\x29\x74\x24\x50\x44\x0F\x29\x44\x24\x30\x44\x0F",
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
)
HOOK(char, __fastcall, velocityGrabber, sigsub_1407BED60(), __int64 a1, float a2) {
	__int64 vel = a1 + 208;
	velocity = (void*)vel;
	Update();
	return originalvelocityGrabber(a1, a2);
}
extern "C" {

	__declspec(dllexport) void Init()
	{
		const INIReader reader("mod.ini");
		if (reader.ParseError() != 0)
		{
			MessageBoxA(nullptr, "Cannot load configuration file, this mod will not function.", "Dynamic Light Speed Dash", MB_ICONERROR);
			return;
		}
		brakeMin = reader.GetFloat("Main", "minimumBrake", 2.0f);
		brakeMax = reader.GetFloat("Main", "maximumBrake", 2.0f);

		INSTALL_HOOK(lightDash);
		INSTALL_HOOK(analogGrabber);
		INSTALL_HOOK(velocityGrabber);
	}
	__declspec(dllexport) void Update()
	{
		if (prevVel != velocity)
		{
			analogMove = nullptr;
			analogCam = nullptr;
			lightdashSpeed = nullptr;
		}
		prevVel = velocity;
		if (lightdashSpeed != nullptr&&analogMove!=nullptr) {
			if (Magnitude(analogMove->x, analogMove->y) < 0.4f) {
				*lightdashSpeed = brakeMax;
			}
			else {
				*lightdashSpeed = brakeMin;
			}
		}
	}
}