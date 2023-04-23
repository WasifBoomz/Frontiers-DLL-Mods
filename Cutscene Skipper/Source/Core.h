#pragma once
#ifndef Core_H
#define Core_H
#include <iostream>
#include "pch.h"
using u64 = uint64_t;
using u32 = uint32_t;

struct dvScene {
	u32 commonPointer;
	u32 resourcePointer;
	u64 padding1;
	u64 padding2;
	u64 padding3;
	u64 padding4;
	float frameStart;
	float frameEnd;
	u32 nodeDrawNum;
	u32 cutInfoPointer;
	u32 authPagePointer;
	u32 disableFrameInfoPointer;
	u32 resourceCutInfoPointer;
	u32 soundInfoPointer;
	u32 nodeInfoPointer;
	//more shit??
};

#define Log(message) std::cout<<message<<std::endl;
#endif