#include "mod.h"
FUNCTION_PTR(Tag*, __fastcall, GetTag, sigsub_15017FC10(), Object* object, const char* tag);
FUNCTION_PTR(u64, __fastcall, sub_140C610D0, sigsub_140C610D0(), u64 a1, u64 a2);
FUNCTION_PTR(char, __fastcall, sub_1503F29B0, sigsub_1503F29B0(), u64 a1);
int count = 0;
const float multMax = 900.0f;

Object* lastObj;

bool applyNext = true;

namespace configuration {
	float rangeMultiplier, enemyRangeMultiplier = 0.0f;
	bool refuseFreeze, noHoldMonologue, popInStabilityMode, removeCamTriggers, removeDashPanels, remove2D, forceSpringHoming, forceClassicSprings, nightChallengeRemoval = false;
	std::map<int, int> gearReplacements;
};

void RangeSpawning::Clamp(float multiplier) {
	this->rangeIn = std::clamp(this->rangeIn, 0.0f, max(this->rangeIn / multiplier, multMax));
	this->rangeOut = std::clamp(this->rangeOut, 0.0f, max(this->rangeOut / multiplier, multMax));
}

#pragma region "Hooks"

HOOK(Object*, __fastcall, LevelLoad, sigsub_1401D64C0(), __int64 a1, Object* object) {
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
	u64 v4 = sub_140C610D0(*(u64*)(*((u64*)0x143C25B10) + 296), *(u64*)((u64)object + 8));
	if (applyNext && (!v4 || sub_1503F29B0(v4))) {
		RangeSpawning* data = (RangeSpawning*)GetTag(object, "RangeSpawning")->data;

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
		if (Contains(include, sizeof(include) / sizeof(const char*), type)) {
			float multiplier = configuration::rangeMultiplier;
			if (!Contains(excludeFromStability, sizeof(excludeFromStability) / sizeof(const char*), type) && configuration::popInStabilityMode) {
				data->Clamp(multiplier);
			}
			*data *= multiplier;
		}
		if (Contains(enemies, sizeof(enemies) / sizeof(const char*), type)) {
			*data *= configuration::enemyRangeMultiplier;
		}
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
HOOK(__int64, __fastcall, HoldPlayer, sigsub_14BDFF550(), __int64 a1, __int64 a2) {
	return a1;
}

HOOK(u64, __fastcall, IslandLoad, sigsub_147967010(), u64 a1, u64 a2, u64 a3) {
	count = 0;
	applyNext = true;
	return originalIslandLoad(a1, a2, a3);
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
