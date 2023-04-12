#include "pch.h"
#include <iostream>
#include <array>
#pragma once
#define Log(message) std::cout << message << std::endl;

using u64 = uint64_t;
using u32 = uint32_t;

struct Vector3 {
	float x;
	float y;
	float z;
	bool operator==(Vector3 const& mult) {
		return this->x == mult.x && this->y == mult.y && this->z == mult.z;
	}
	bool operator!=(Vector3 const& mult) {
		return !(this->x == mult.x && this->y == mult.y && this->z == mult.z);
	}
	Vector3(float mX, float mY, float mZ)
		:x(mX),
		y(mY),
		z(mZ)
	{}
};

namespace configuration {
	float rangeMultiplier, enemyRangeMultiplier, collectibleRangeMultiplier = 0.0f;
	bool popInStabilityMode, removeCamTriggers, removeDashPanels, remove2D, forceSpringHoming, forceClassicSprings = false;
}

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
	RangeSpawning& operator*=(float const& mult) {
		const float multMax = configuration::popInStabilityMode ? 900.0f : 0;
		this->rangeIn = std::clamp(this->rangeIn * mult, 0.0f, max(this->rangeIn, multMax));
		this->rangeOut = std::clamp(this->rangeOut * mult, 0.0f, max(this->rangeOut, multMax));
		return *this;
	}
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

bool Contains(const char* array[], int size, const char* item);