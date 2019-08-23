//
// SVNoise.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef __SV_NOISE_H__
#define __SV_NOISE_H__

#include "../base/SVGBase.h"

/*
 */
namespace sv {
    
    namespace util {
        
        class SVNoise{
            
        public:
            
            SVNoise();
            SVNoise(u32 _seed);
            ~SVNoise();
            
            void setSeed(u32 _seed);
            u32 getSeed() const;
            
            f32 get1(f32 _x) const;
            f32 get2(f32 _x, f32 _y) const;
            f32 get3(f32 _x, f32 _y, f32 _z) const;
            
            f32 getTurbulence1(f32 _x, s32 _frequency) const;
            f32 getTurbulence2(f32 _x, f32 _y, s32 _frequency) const;
            f32 getTurbulence3(f32 _x, f32 _y, f32 _z, s32 _frequency) const;
            
            f32 getTileable1(f32 _x, f32 _width) const;
            f32 getTileable2(f32 _x, f32 _y, f32 _width, f32 _height) const;
            f32 getTileable3(f32 _x, f32 _y, f32 _z, f32 _width, f32 _height, f32 _depth) const;
            
            f32 getTileableTurbulence1(f32 _x, f32 _width, s32 _frequency) const;
            f32 getTileableTurbulence2(f32 _x, f32 _y, f32 _width, f32 _height, s32 _frequency) const;
            f32 getTileableTurbulence3(f32 _x, f32 _y, f32 _z, f32 _width, f32 _height, f32 _depth, s32 _frequency) const;
            
        private:
            
            enum {
                A = 1664525,
                C = 1013904223,
                MAX_RANDOM = 0x7fffffff,
                SAMPLES = 256,
            };
            
            u32 get_random_int();
            f32 get_random_float();
            
            u32 m_seed;
            
            s32 permutation[SAMPLES * 2 + 2];
            f32 gradient1[SAMPLES * 2 + 2][1];
            f32 gradient2[SAMPLES * 2 + 2][2];
            f32 gradient3[SAMPLES * 2 + 2][3];
        };
        
        
    }//!namespace util
    
}//!namespace sv

#endif /* __SV_NOISE_H__ */
