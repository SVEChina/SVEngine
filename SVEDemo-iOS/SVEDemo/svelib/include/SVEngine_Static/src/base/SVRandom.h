#ifndef SV_RANDOM_H
#define SV_RANDOM_H

#include "SVTypes.h"
//
namespace sv {
    
    class SVRandom {
    public:
        SVRandom();
        
        SVRandom(u32 seed);
        
        ~SVRandom();
        
        void setSeed(u32 seed) const;
        
        u32 getSeed() const;
        
        enum {
            A = 1664525,
            C = 1013904223,
            MAX_RANDOM = 0x7fffffff,
        };
        
        inline u32 get() const {
            seed = (u32)((u64)seed * A + C) & MAX_RANDOM;
            return seed;
        }
        
        s32 getInt(s32 from,s32 to) const;
        
        f32 getFloat(f32 from,f32 to) const;
        
        f64 getDouble(f64 from,f64 to) const;
        
    private:
        mutable u32 seed;
    };
    
}//!namespace



#endif /* SV_RANDOM_H */
