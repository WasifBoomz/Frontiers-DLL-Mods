#pragma once
#ifndef Core_H
#define Core_H
#include <iostream>
#include "pch.h"
#include <xmmintrin.h>
#include "Vector2.hpp"
using namespace M3D;

using u64 = uint64_t;
using u32 = uint32_t;
using u16 = uint16_t;
using u8 = uint8_t;

CL_SCAN_SIGNATURE(sigsub_140C4A4C0, "\x4C\x8B\x41\x58\x4C\x8B\xD2\x48\x8B\x41\x60\x4C\x8B\x5A\x18", "xxxxxxxxxxxxxxx");

struct PlayerParamLightDash {
	float dashSpeed, dashSpeedMax, speed, speedMax, accel, brake;
};

void* LoadPlayerParam(void* firstBCCArg, void* lastArg, void* (*func)(void*, void*));

#define Log(message) std::cout<<message<<std::endl;
#endif