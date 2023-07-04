#include "Core.h"
FUNCTION_PTR(void*, __fastcall, sub_140C4A4C0, sigsub_140C4A4C0(), void* a1, u64 a2);

void* LoadPlayerParam(void* firstBCCArg, void* lastArg, void* (*func)(void*, void*)) {
	void* arg = sub_140C4A4C0(firstBCCArg, 0x143B7C410);
	return func(arg, lastArg);
}
float roundToNearest(float x, float y) {
	return roundf(x / y) * y;
}