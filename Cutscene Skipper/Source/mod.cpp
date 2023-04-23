#include "Core.h"
#define QWORD __int64
float excludeFrames[] = {146,446, 1067, 890, 764, 2451, 1092};
SIG_SCAN
(
	sigsub_140C31B80,
	0x140C31B80,
	"\xF3\x0F\x59\x0D\x50\x1D\x49\x00\xF3\x0F\x59\x15\x48\x1D\x49\x00\xF3\x48\x0F\x2C\xC1\x89\x41\x20\xF3\x48\x0F\x2C\xC2\x89\x41\x24",
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
)
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