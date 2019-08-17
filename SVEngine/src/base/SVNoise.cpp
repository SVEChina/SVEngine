//
// SVNoise.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVNoise.h"

/*
 */
SVNoise::SVNoise(){
	setSeed((u32)time(NULL));
}

SVNoise::SVNoise(u32 _seed){
	setSeed(_seed);
}

SVNoise::~SVNoise() {
	
}

/*
 */
u32 SVNoise::get_random_int() {
	m_seed = (u32)((unsigned long long)m_seed * A + C) & MAX_RANDOM;
	return m_seed;
}

f32 SVNoise::get_random_float() {
	return (f32)get_random_int() / MAX_RANDOM * 2.0f - 1.0f;
}

/*
 */
static void normalize2(f32 *v) {
	f32 ilength = 1.0f / sqrtf(v[0] * v[0] + v[1] * v[1]);
	v[0] = v[0] * ilength;
	v[1] = v[1] * ilength;
}

static void normalize3(f32 *v) {
	f32 ilength = 1.0f / sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	v[0] = v[0] * ilength;
	v[1] = v[1] * ilength;
	v[2] = v[2] * ilength;
}

/*
 */
void SVNoise::setSeed(u32 _s) {
	
	m_seed = _s;
	if(m_seed < 1) m_seed = 1;
	
	// make random arrays
	for(s32 i = 0; i < SAMPLES; i++) {
		permutation[i] = i;
		gradient1[i][0] = get_random_float();
		gradient2[i][0] = get_random_float();
		gradient2[i][1] = get_random_float();
		gradient3[i][0] = get_random_float();
		gradient3[i][1] = get_random_float();
		gradient3[i][2] = get_random_float();
		normalize2(gradient2[i]);
		normalize3(gradient3[i]);
	}
	
	// permutation array
	for(s32 i = 0; i < SAMPLES; i++) {
		s32 j = (get_random_int() >> 16) % SAMPLES;
		s32 k = permutation[i];
		permutation[i] = permutation[j];
		permutation[j] = k;
	}
	
	// duplicate randoms
	for(s32 i = 0; i < SAMPLES + 2; i++) {
		permutation[SAMPLES + i] = permutation[i];
		gradient1[SAMPLES + i][0] = gradient1[i][0];
		gradient2[SAMPLES + i][0] = gradient2[i][0];
		gradient2[SAMPLES + i][1] = gradient2[i][1];
		gradient3[SAMPLES + i][0] = gradient3[i][0];
		gradient3[SAMPLES + i][1] = gradient3[i][1];
		gradient3[SAMPLES + i][2] = gradient3[i][2];
	}
}

u32 SVNoise::getSeed() const {
	return m_seed;
}

/*
 */
static inline void setup(f32 _x, s32 &_b0, s32 &_b1, f32 &_r0, f32 &_r1) {
	_b0 = (s32)(_x + 4096.0f) & 255;
	_b1 = (_b0 + 1) & 255;
	_r0 = (_x + 4096.0f) - (s32)(_x + 4096.0f);
	_r1 = _r0 - 1.0f;
}

static inline f32 fade(f32 _x) {
	return _x * _x * _x * (_x * (_x * 6.0f - 15.0f) + 10.0f);
}

static inline f32 dot2(const f32 *_v, f32 _x, f32 _y) {
	return _v[0] * _x + _v[1] * _y;
}

static inline f32 dot3(const f32 *_v, f32 _x, f32 _y, f32 _z) {
	return _v[0] * _x + _v[1] * _y + _v[2] * _z;
}

static inline f32 lerp(f32 _v0, f32 _v1, f32 _k) {
	return _v0 + (_v1 - _v0) * _k;
}

/*
 */
f32 SVNoise::get1(f32 _x) const {
	
	s32 bx0,bx1;
	f32 rx0,rx1;
	
	setup(_x,bx0,bx1,rx0,rx1);
	
	f32 sx = fade(rx0);
	
	f32 a = gradient1[permutation[bx0]][0] * rx0;
	f32 b = gradient1[permutation[bx1]][0] * rx1;
	
	return lerp(a,b,sx);
}

f32 SVNoise::get2(f32 _x, f32 _y) const {
	
	s32 bx0,bx1,by0,by1;
	f32 rx0,rx1,ry0,ry1;
	
	setup(_x,bx0,bx1,rx0,rx1);
	setup(_y,by0,by1,ry0,ry1);
	
	f32 sx = fade(rx0);
	f32 sy = fade(ry0);
	
	s32 b00 = permutation[permutation[bx0] + by0];
	s32 b10 = permutation[permutation[bx1] + by0];
	s32 b01 = permutation[permutation[bx0] + by1];
	s32 b11 = permutation[permutation[bx1] + by1];
	
	f32 a = lerp(dot2(gradient2[b00],rx0,ry0),dot2(gradient2[b10],rx1,ry0),sx);
	f32 b = lerp(dot2(gradient2[b01],rx0,ry1),dot2(gradient2[b11],rx1,ry1),sx);
	
	return lerp(a,b,sy);
}

f32 SVNoise::get3(f32 _x, f32 _y, f32 _z) const {
	
	s32 bx0,bx1,by0,by1,bz0,bz1;
	f32 rx0,rx1,ry0,ry1,rz0,rz1;
	
	setup(_x,bx0,bx1,rx0,rx1);
	setup(_y,by0,by1,ry0,ry1);
	setup(_z,bz0,bz1,rz0,rz1);
	
	f32 sx = fade(rx0);
	f32 sy = fade(ry0);
	f32 sz = fade(rz0);
	
	s32 b00 = permutation[permutation[bx0] + by0];
	s32 b10 = permutation[permutation[bx1] + by0];
	s32 b01 = permutation[permutation[bx0] + by1];
	s32 b11 = permutation[permutation[bx1] + by1];
	
	f32 a0 = lerp(dot3(gradient3[b00 + bz0],rx0,ry0,rz0),dot3(gradient3[b10 + bz0],rx1,ry0,rz0),sx);
	f32 b0 = lerp(dot3(gradient3[b01 + bz0],rx0,ry1,rz0),dot3(gradient3[b11 + bz0],rx1,ry1,rz0),sx);
	
	f32 a1 = lerp(dot3(gradient3[b00 + bz1],rx0,ry0,rz1),dot3(gradient3[b10 + bz1],rx1,ry0,rz1),sx);
	f32 b1 = lerp(dot3(gradient3[b01 + bz1],rx0,ry1,rz1),dot3(gradient3[b11 + bz1],rx1,ry1,rz1),sx);
	
	f32 c0 = lerp(a0,b0,sy);
	f32 c1 = lerp(a1,b1,sy);
	
	return lerp(c0,c1,sz);
}

/*
 */
f32 SVNoise::getTurbulence1(f32 _x, s32 _frequency) const {
	f32 res = 0.0f;
	for(s32 f = _frequency; f >= 1; f >>= 1) {
		res += get1(_x * f) / f;
	}
	return res;
}

f32 SVNoise::getTurbulence2(f32 _x, f32 _y, s32 _frequency) const {
	f32 res = 0.0f;
	for(s32 f = _frequency; f >= 1; f >>= 1) {
		res += get2(_x * f,_y * f) / f;
	}
	return res;
}

f32 SVNoise::getTurbulence3(f32 _x, f32 _y, f32 _z, s32 _frequency) const {
	f32 res = 0.0f;
	for(s32 f = _frequency; f >= 1; f >>= 1) {
		res += get3(_x * f,_y * f,_z * f) / f;
	}
	return res;
}

/*
 */
f32 SVNoise::getTileable1(f32 _x,f32 _width) const {
	_x -= (s32)(_x / _width) * _width;
	if(_x < 0.0f) _x += _width;
	return (get1(_x) * (_width - _x) + get1(_x - _width) * _x) / _width;
}

f32 SVNoise::getTileable2(f32 _x, f32 _y, f32 _width, f32 _height) const {
	_x -= (s32)(_x / _width) * _width;
	_y -= (s32)(_y / _height) * _height;
	if(_x < 0.0f) _x += _width;
	if(_y < 0.0f) _y += _height;
	return (get2(_x,_y) * (_width - _x) * (_height - _y) +
			get2(_x - _width,_y) * _x * (_height - _y) +
			get2(_x,_y - _height) * (_width - _x) * _y +
			get2(_x - _width,_y - _height) * _x * _y) / (_width * _height);
}

f32 SVNoise::getTileable3(f32 _x, f32 _y, f32 _z, f32 _width, f32 _height, f32 _depth) const {
	_x -= (s32)(_x / _width) * _width;
	_y -= (s32)(_y / _height) * _height;
	_z -= (s32)(_z / _depth) * _depth;
	if(_x < 0.0f) _x += _width;
	if(_y < 0.0f) _y += _height;
	if(_z < 0.0f) _z += _depth;
	return (get3(_x,_y,_z) * (_width - _x) * (_height - _y) * (_depth - _z) +
			get3(_x - _width,_y,_z) * _x * (_height - _y) * (_depth - _z) +
			get3(_x,_y - _height,_z) * (_width - _x) * _y * (_depth - _z) +
			get3(_x - _width,_y - _height,_z) * _x * _y * (_depth - _z) +
			get3(_x,_y,_z - _depth) * (_width - _x) * (_height - _y) * _z +
			get3(_x - _width,_y,_z - _depth) * _x * (_height - _y) * _z +
			get3(_x,_y - _height,_z - _depth) * (_width - _x) * _y * _z +
			get3(_x - _width,_y - _height,_z - _depth) * _x * _y * _z) / (_width * _height * _depth);
}

/*
 */
f32 SVNoise::getTileableTurbulence1(f32 _x, f32 _width, s32 _frequency) const {
	f32 res = 0.0f;
	for(s32 f = _frequency; f >= 1; f >>= 1) {
		res += getTileable1(_x * f,_width * f) / f;
	}
	return res;
}

f32 SVNoise::getTileableTurbulence2(f32 _x, f32 _y, f32 _width, f32 _height, s32 _frequency) const {
	f32 res = 0.0f;
	for(s32 f = _frequency; f >= 1; f >>= 1) {
		res += getTileable2(_x * f,_y * f,_width * f,_height * f) / f;
	}
	return res;
}

f32 SVNoise::getTileableTurbulence3(f32 _x, f32 _y, f32 _z, f32 _width, f32 _height, f32 _depth, s32 _frequency) const {
	f32 res = 0.0f;
	for(s32 f = _frequency; f >= 1; f >>= 1) {
		res += getTileable3(_x * f,_y * f,_z * f,_width * f,_height * f,_depth * f) / f;
	}
	return res;
}
