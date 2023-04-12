#include "ModCore.h";
#include "ObjData.h"

FUNCTION_PTR(Tag*, __fastcall, GetTag, 0x140BD6C60, Object* object, const char* tag);

int count = 0;
bool protectRange = false;
std::array<unsigned char, 16> exTestId;
std::array<unsigned char, 16> testId;

float multiplier;

const float multMax = 900.0f;

void RangeSpawning::Clamp() {
	this->rangeIn = std::clamp(this->rangeIn, 0.0f, max(this->rangeIn/multiplier, multMax));
	this->rangeOut = std::clamp(this->rangeOut, 0.0f, max(this->rangeOut / multiplier, multMax));
}

extern "C" {
	__declspec(dllexport) void LevelStart();
}

SIG_SCAN
(
	sigsub_1407A9510,
	0x1407A9510,
	"\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x40\x48\x8B\xF9\x0F\x29\x74\x24\x30\x48\x8B\xCA\x0F\x28\xF2\x48\x8B\xDA\xE8\x10\x44\x01\x00",
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
)
//We need something to indicate that you're out of a loading screen and actually in gameplay.
HOOK(__int64, __fastcall, Ground, sigsub_1407A9510(), __int64 a1, __int64 a2, float a3) {
	if (count != 0) {
		LevelStart();
	}
	return originalGround(a1, a2, a3);
}
namespace configuration {
	float rangeMultiplier, enemyRangeMultiplier, collectibleRangeMultiplier = 0.0f;
	bool popInStabilityMode, removeCamTriggers, removeDashPanels, remove2D, forceSpringHoming, forceClassicSprings = false;
};
SIG_SCAN
(
	sigsub_1401BE650,
	0x1401BE650,
	"\x48\x89\x54\x24\x10\x53\x41\x57\x48\x83\xEC\x78\x48\x8B\x05\x2D\xDF\x99\x03\x4C\x8B\xFA\x48\x8B\x52\x08\x48\x8B\xD9\x48\x8B\x88",
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
)
HOOK(Object*, __fastcall, LevelLoad, sigsub_1401BE650(), __int64 a1, Object* object) {
	//Prevent repeated multiplication of object range with the training simulator.
	if (count++ == 0) {
		protectRange = exTestId == object->id && testId != object->id;
		exTestId = testId;
		testId = object->id;
	}	
	const char* type = object->type;
	RangeSpawning* data = (RangeSpawning*)GetTag(object, "RangeSpawning")->data;
	if (!protectRange) {
		if (!Contains(exclude, sizeof(exclude) / sizeof(const char*), type)) {
			if (Contains(enemies, sizeof(enemies) / sizeof(const char*), type)) {
				multiplier = configuration::enemyRangeMultiplier;
			}
			else if (Contains(collectibles, sizeof(collectibles) / sizeof(const char*), type)) {
				multiplier = configuration::collectibleRangeMultiplier;
			}
			else {
				multiplier = configuration::rangeMultiplier;
			}
		}
		*data *= multiplier;
		if (!Contains(excludeFromStability, sizeof(excludeFromStability) / sizeof(const char*), type)&&configuration::popInStabilityMode) {
			data->Clamp();
		}
	}
	if ((configuration::removeCamTriggers && Contains(cameraTriggers, sizeof(cameraTriggers) / sizeof(const char*), type)) ||
		(configuration::removeDashPanels && Contains(dashPanels, sizeof(dashPanels) / sizeof(const char*), type))||
		(configuration::remove2D && Contains(dimensionVolumes, sizeof(dimensionVolumes) / sizeof(const char*), type)))
	{
		*data *= 0;
	}
	if ((configuration::forceSpringHoming && Contains(springs, sizeof(springs) / sizeof(const char*), type))) {
		//I've been trying to find this using the struct for 4 hours, I can't take it anymore.
		(*(ObjSpringSpawner**)((u64)object+0x90))->isHoming = true;
	}
	return originalLevelLoad(a1, object);
}
extern "C" {

	__declspec(dllexport) void Init()
	{
		const INIReader reader("tweaks.ini");
		if (reader.ParseError() != 0)
		{
			MessageBoxA(nullptr, "Cannot load configuration file, this mod will not function.", "Custom Pop-in", MB_ICONERROR);
			return;
		}
		configuration::rangeMultiplier = reader.GetFloat("popIn", "multiplier", 2.0f);
		configuration::enemyRangeMultiplier = reader.GetFloat("popIn", "enemyMultiplier", 1.0f);
		configuration::collectibleRangeMultiplier = reader.GetFloat("popIn", "collectibleMultiplier", 5.0f);
		configuration::popInStabilityMode = reader.GetBoolean("popIn", "stabilityMode", true);
		configuration::removeCamTriggers = reader.GetBoolean("objectRemoval", "camTrigger", false);
		configuration::removeDashPanels = reader.GetBoolean("objectRemoval", "dashPanels", false);
		configuration::remove2D = reader.GetBoolean("objectRemoval", "2D", false);
		configuration::forceSpringHoming = reader.GetBoolean("objectTweaks", "springHoming", false);
		INSTALL_HOOK(Ground);
		INSTALL_HOOK(LevelLoad);
	}
	__declspec(dllexport) void LevelStart()
	{
		protectRange = count = 0;
	}
}
