
#ifndef ModCore_H
#define ModCore_H
#include "pch.h"
#define Log(message) std::cout << message << std::endl;
#include <iostream>
#include <array>
#include <vector>
using u64 = uint64_t;
using u32 = uint32_t;

struct Vector3 {
	float x;
	float y;
	float z;
	bool operator==(Vector3 const& mult);
	bool operator!=(Vector3 const& mult);
	Vector3(float mX, float mY, float mZ);
};

namespace configuration {
	extern bool popInStabilityMode;
};

struct Transform {
	Vector3 position;
	Vector3 rotation;
};

struct Tag {
	u64 unknown1;
	char* type;
	u64 size;
	void* data;
};

struct RangeSpawning {
	float rangeIn;
	float rangeOut;
	RangeSpawning& operator*=(float const& mult);
	void Clamp();
};

struct Object {
	u32 unknown1;
	u32 unknown2;
	const char* type;
	const char* name;
	alignas(16) std::array<unsigned char, 16> id;
	alignas(16) std::array<unsigned char, 16> parentId;
	Transform transformBase;
	Transform transformOffset;
	alignas(32) Tag* tags;
	void* paramData;
};

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
bool Contains(const char* array[], int size, const char* item);
#endif