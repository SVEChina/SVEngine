/* Copyright (C) 2005-2013, Unigine Corp. All rights reserved.
 *
 * File:    SVRandom.cpp
 * Desc:    SVRandom number generator
 * Version: 1.05
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
 *
 * This file is part of the Unigine engine (http://unigine.com/).
 *
 * Your use and or redistribution of this software in source and / or
 * binary form, with or without modification, is subject to: (i) your
 * ongoing acceptance of and compliance with the terms and conditions of
 * the Unigine License Agreement; and (ii) your inclusion of this notice
 * in any version of this software that you use or redistribute.
 * A copy of the Unigine License Agreement is available by contacting
 * Unigine Corp. at http://unigine.com/
 */

#include "SVRandom.h"


SVRandom::SVRandom() {
	setSeed((u32)time(nullptr));
}

SVRandom::SVRandom(u32 seed) {
	setSeed(seed);
}

SVRandom::~SVRandom() {
}

//
void SVRandom::setSeed(u32 s) const {
	seed = s;
}

u32 SVRandom::getSeed() const {
	return seed;
}

//
s32 SVRandom::getInt(s32 from,s32 to) const {
	
	s32 range = to - from;
    if(range <= 1){
        return from;
    }
    
	u32 rand = get();
	
	if(range < 0xffff) {
		rand >>= 16;
		u32 rand_max = ((MAX_RANDOM >> 16) / range) * range;
        while(rand > rand_max){
            rand = get() >> 16;
        }
	} else {
		u32 rand_max = (MAX_RANDOM / range) * range;
        while(rand > rand_max) {
            rand = get();
        }
	}
	return from + rand % range;
}

//
f32 SVRandom::getFloat(f32 from,f32 to) const {
	union {
        u32 i;
        f32 f;
    } rand = { (0x3f800000 | (get() & 0x007fffff)) };
	return from + (rand.f - 1.0f) * (to - from);
}

//
f64 SVRandom::getDouble(f64 from,f64 to) const {
	return from + (f64)get() / MAX_RANDOM * (to - from);
}
