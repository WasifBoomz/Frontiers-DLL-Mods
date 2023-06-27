#pragma once
#include "ModCore.h";
#include "ObjData.h"
#include "ObjStructs.h"

extern "C" {
	__declspec(dllexport) void LevelStart();
}
#pragma region "SigScan"
CL_SCAN_SIGNATURE(sigsub_1401D64C0, "\x48\x89\x54\x24\x10\x53\x41\x57\x48\x83\xEC\x78", "xxxxxxxxxxxx")
CL_SCAN_SIGNATURE(sigsub_14BDFF550, "\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x50\x48\x89\xD7\x48\x89\xCB\xBA\xB4\x21\x00\x00", "xxxxxxxxxxxxxxxxxxxxx")
CL_SCAN_SIGNATURE(sigsub_147967010, "\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x20\x4C\x89\xC7\x48\x89\xD3\x48\x89\xCE\xE8\xCC\xCC\xCC\xCC\x48\x8D\x05", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxx");
CL_SCAN_SIGNATURE(sigsub_15017FC10, "\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x20\x48\x89\xD5\x48\x89\xCE\x48\x85\xC9", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
#pragma endregion "SigScan"