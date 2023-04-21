#ifndef ObjStructs_H
#define ObjStructs_H
#include "ModCore.h"
struct ObjSpringSpawner {
	u32 visual;
	float firstSpeed;
	float outOfControl;
	float keepVelocityDistance;
	bool isHoming;
	bool isEventOn;
	bool isCyloopOn;
	float cyloopSignDistance;
	float cyloopSignTolerance;
	bool isHorizon;
	bool airtrick;
	bool isVisible;
	bool isSmooth;
	void* actions;
	float rollingSpeed;
	float rollingPhase;
};

struct ObjSpringClassicSpawner {
	u32 visual;
	float firstSpeed;
	float outOfControl;
	float keepVelocityDistance;
	bool usedYaw;
	bool isEventOn;
	bool isHorizon;
	bool isVisible;
	void* actions;
};

struct VolumeTriggerSpawner {
	u32 ShapeType;
	u32 basePoint;
	u32 CollisionFilter;
	float collisionWidth;
	float collisionHeight;
	float collisionDepth;
	Vector3 collisionOffset;
};

struct ObjDimensionVolumeSpawner {
	bool isSideview;
	u32 priority;
	const char* pathName;
	u32 state;
	u32 action;
	VolumeTriggerSpawner volume;
};

struct PathMovement {
	u32 moveType;
	const char* pathName;
	void* locaterList;
	Vector3 twoPointOffset;
	u32 patrolType;
	u32 timeType;
	bool eventDriven;
	bool isReverse;
	float waitTime;
	float phase;
	float speed;
	float killWaitTSecOneway;
};
struct ObjFanSpawner {
	u32 type;
	u32 place;
	float length;
	float velocity;
	float durOn;
	float durOff;
	float phase;
	bool eventDriven;
	ObjFanSpawner(u32 m_type, u32 m_place, float m_length, float m_velocity, float m_durOn, float m_durOff, float m_phase, bool m_eventDriven);
};
struct ObjWallJumpBlockSpawner {
	float width;
	float height;
};
struct ObjGimmickInfoSpawner {
	u32 no;
	alignas (4) bool eventDriven;
	const char* puzzleID;
	GeditArray childInfo;
	GeditArray childLogic;
	GeditArray childEvent;
	u32 padding1;
	const char* luaName;
	u64 padding2;
	bool isHideMapIcon;
	bool isPuzzle;
	u16 playType;
};
struct ObjSlashCommonSpawner {
	u32 no;
	bool eventDriven;
	char type;
	u16 condition;
	float activateTime;
	alignas (4) bool isLimitedTime;
	u32 startHour;
	u32 startMin;
	u32 endHour;
	u32 endMin;
};
struct ObjGuideLightSpawner {
	u64 no;
	const char* pathName;
	u64 padding1;
	char setType;
	char num;
	u16 padding2;
	u32 padding3;
	GeditArray setNodeList;
	alignas (4) bool isLimitedTime;
	u32 startHour;
	u32 startMin;
	u32 endHour;
	u32 endMin;
	float delayTime;
};
#endif