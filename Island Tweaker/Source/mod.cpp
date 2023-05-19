#include "mod.h"
FUNCTION_PTR(Tag*, __fastcall, GetTag, 0x140BD6C60, Object* object, const char* tag);

int count = 0;
float multiplier;
const float multMax = 900.0f;

Object* lastObj;

bool applyNext = true;

namespace configuration {
	float rangeMultiplier, enemyRangeMultiplier, collectibleRangeMultiplier = 0.0f;
	bool refuseFreeze, noHoldMonologue, popInStabilityMode, removeCamTriggers, removeDashPanels, remove2D, forceSpringHoming, forceClassicSprings, nightChallengeRemoval = false;
	std::map<int, int> gearReplacements;
};

void RangeSpawning::Clamp() {
	this->rangeIn = std::clamp(this->rangeIn, 0.0f, max(this->rangeIn / multiplier, multMax));
	this->rangeOut = std::clamp(this->rangeOut, 0.0f, max(this->rangeOut / multiplier, multMax));
}

#pragma region "Hooks"

HOOK(Object*, __fastcall, LevelLoad, sigsub_1401BE650(), __int64 a1, Object* object) {
	RangeSpawning* data = (RangeSpawning*)GetTag(object, "RangeSpawning")->data;
	//Prevent repeated multiplication of object range with the training simulator.
	if (count++ == 0) {
		if (lastObj != nullptr) {
			if (memcmp(lastObj, object, sizeof(Object)) == 0) {
				applyNext = false;
			}
		}
		lastObj = object;
	}
	if (!applyNext) {
		return originalLevelLoad(a1, object);
	}
	const char* type = object->type;
	bool refuseObject = false;
	if (applyNext) {
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
		else {
			multiplier = 1;
		}
		if (!Contains(excludeFromStability, sizeof(excludeFromStability) / sizeof(const char*), type) && configuration::popInStabilityMode) {
			data->Clamp();
		}
		if ((configuration::removeCamTriggers && Contains(cameraTriggers, sizeof(cameraTriggers) / sizeof(const char*), type)) ||
			(configuration::removeDashPanels && Contains(dashPanels, sizeof(dashPanels) / sizeof(const char*), type)))
		{
			*data *= 0;
		}
		if ((configuration::forceSpringHoming && Contains(springs, sizeof(springs) / sizeof(const char*), type))) {
			//This doesn't work in the "normal" way so let's do this instead.
			(*(ObjSpringSpawner**)((u64)object + 0x90))->isHoming = true;
		}
		if (configuration::remove2D) {
			if (!std::strcmp(type, "WallJumpBlock")) {
				//Only the first two values end up mattering lol :C
				(*(ObjFanSpawner**)((u64)object + 0x90)) = new ObjFanSpawner(0, 1, NULL, NULL, NULL, NULL, NULL, NULL);
				object->type = "Fan";
			}
			if (!std::strcmp(type, "DimensionVolume")) {
				*data *= 0;
			}
		}
		if (configuration::nightChallengeRemoval) {
			if (!std::strcmp(type, "GimmickInfo")) {
				(*(ObjGimmickInfoSpawner**)((u64)object + 0x90))->playType = 0;
			}
			if (!std::strcmp(type, "SlashCommon")) {
				(*(ObjSlashCommonSpawner**)((u64)object + 0x90))->isLimitedTime = false;
			}
			if (!std::strcmp(type, "GuideLight")) {
				(*(ObjGuideLightSpawner**)((u64)object + 0x90))->isLimitedTime = false;
			}
		}
		if (configuration::removeCamTriggers) {
			if (!std::strcmp(type, "CameraActivator")) {
				(*(ObjCameraActivatorSpawner**)((u64)object + 0x90))->lifeTime = 0.0f;
				(*(ObjCameraActivatorSpawner**)((u64)object + 0x90))->interpolate.easeTimeOff = 0.0f;
				(*(ObjCameraActivatorSpawner**)((u64)object + 0x90))->interpolate.easeTimeOn = 0.0f;
			}
		}
		if (configuration::refuseFreeze) {
			if (!std::strcmp(type, "TimerSwitch")) {
				(*(ObjTimerSwitchSpawner**)((u64)object + 0x90))->startWaitTime = 0.5f;
			}
		}
		if (configuration::noHoldMonologue) {
			if (!std::strcmp(type, "MonologueVolume")) {
				(*(ObjMonologueVolumeSpawner**)((u64)object + 0x90))->hold = 2;
				(*(ObjMonologueVolumeSpawner**)((u64)object + 0x90))->volume.collisionDepth *= 3;
				(*(ObjMonologueVolumeSpawner**)((u64)object + 0x90))->volume.collisionWidth *= 3;
				(*(ObjMonologueVolumeSpawner**)((u64)object + 0x90))->volume.collisionHeight *= 3;
			}
		}
		*data *= multiplier;
		//if (configuration::remove2D) {
		//	float dimensionSize = objDimensions.size();
		//	float wallJumpSize = objWallJumpsBlocks.size();
		//	if (dimensionSize > 0 && wallJumpSize > 0) {
		//		Object* volume = objDimensions[dimensionSize - 1];
		//		Object* wallJump = objWallJumpsBlocks[wallJumpSize - 1];
		//		volume->transformBase = wallJump->transformBase;
		//		volume->transformOffset = wallJump->transformOffset;
		//		RangeSpawning* volumeSpawning = (RangeSpawning*)GetTag(volume, "RangeSpawning")->data;
		//		RangeSpawning* wallJumpSpawning = (RangeSpawning*)GetTag(wallJump, "RangeSpawning")->data;
		//		volumeSpawning->rangeIn = wallJumpSpawning->rangeIn;
		//		volumeSpawning->rangeOut = wallJumpSpawning->rangeOut;
		//		ObjDimensionVolumeSpawner* dimensionVolumeParams = (*(ObjDimensionVolumeSpawner**)((u64)volume + 0x90));
		//		ObjWallJumpBlockSpawner* wallJumpParams = (*(ObjWallJumpBlockSpawner**)((u64)wallJump + 0x90));
		//		dimensionVolumeParams->isSideview = true;
		//		dimensionVolumeParams->volume.basePoint = 0;
		//		dimensionVolumeParams->volume.ShapeType = 0;
		//		dimensionVolumeParams->state = 0;
		//		dimensionVolumeParams->action = 0;
		//		dimensionVolumeParams->volume.collisionDepth = 1;
		//		dimensionVolumeParams->volume.collisionWidth = wallJumpParams->width;
		//		dimensionVolumeParams->volume.collisionHeight = wallJumpParams->height;
		//		Log(volume->name);
		//		objDimensions.pop_back();
		//	}
		//}
	}
	return originalLevelLoad(a1, object);
}
HOOK(__int64, __fastcall, HoldPlayer, sigsub_14BD19820(), __int64 a1, __int64 a2) {
	return a1;
}

HOOK(u64, __fastcall, IslandLoad, sigsub_1479E9C70(), u64 a1, u64 a2, u64 a3) {
	count = 0;
	applyNext = true;
	return originalIslandLoad(a1, a2, a3);;
}
#pragma endregion "Hooks"

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
		configuration::nightChallengeRemoval = reader.GetBoolean("challengeTweaks", "nightChallengeRemoval", false);
		configuration::refuseFreeze = reader.GetBoolean("objectTweaks", "refuseFreeze", false);
		configuration::noHoldMonologue = reader.GetBoolean("objectTweaks", "noHoldMonologue", false);
		INSTALL_HOOK(LevelLoad);
		INSTALL_HOOK(IslandLoad);
		if (configuration::refuseFreeze) {
			INSTALL_HOOK(HoldPlayer);
		}
	}
}
