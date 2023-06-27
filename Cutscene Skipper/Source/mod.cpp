#include "Core.h"
#define QWORD __int64
float excludeFrames[] = {146, 446, 1067, 890, 764, 2451, 1092};
CL_SCAN_SIGNATURE(sigsub_140C31B80,"\xF3\x0F\x59\x0D\xCC\xCC\xCC\xCC\xF3\x0F\x59\x15\xCC\xCC\xCC\xCC\xF3\x48\x0F\x2C\xC1", "xxxx????xxxx????xxxxx")
HOOK(__int64, __fastcall, ReadFrame, sigsub_140C31B80(), dvScene* scene, float frameStart, float frameEnd) {
	bool include = 0;
	for (int i = 0; i < sizeof(excludeFrames) / 4; i++)
		include += frameEnd == excludeFrames[i];
	include = include != 0;
	return originalReadFrame(scene, frameStart, frameEnd * include);
}

extern "C" {

	__declspec(dllexport) void Init()
	{
		INSTALL_HOOK(ReadFrame);
	}
}