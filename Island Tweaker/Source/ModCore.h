#ifndef ModCore_H
#define ModCore_H
#include "pch.h"
#define Log(message) std::cout << message << std::endl;
#include <iostream>
#include <array>
using u64 = uint64_t;
using u32 = uint32_t;
using u16 = uint16_t;
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

struct World {
	alignas(16) u64 unknown1;
	Object** objects;
	u64 count;
	u64 capacity;
	void* allocatorPtr;
};

struct GeditArray {
	u64 unknown1;
	u64 unknown2;
	u64 unknown3;
	u64 unknown4;
};
struct ObjectReference {
	u64 unknown1;
	u64 unknown2;
};
bool Contains(const char* array[], int size, const char* item);
#endif