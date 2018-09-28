#ifndef SV_AHALF_H
#define SV_AHALF_H

#include "SVBase.h"

namespace sv {
    
    struct SVHalf {
        
        sv_inline SVHalf() {
        }
        
        sv_inline SVHalf(const SVHalf &h)
        : h(h.h) {
        }
        
        explicit sv_inline SVHalf(s32 i)
        : h((u16)i) {
        }
        
        explicit sv_inline SVHalf(f32 f){
            setFloat(f);
        }
        
        sv_inline SVHalf &operator=(SVHalf v) {
            h = v.h;
            return *this;
        }
        
        sv_inline SVHalf &operator=(f32 f) {
            setFloat(f);
            return *this;
        }
        
        sv_inline operator f32() const {
            return getFloat();
        }
        
        void setFloat(f32 f);
        
        f32 getFloat() const;
        
        u16 h;
    };
    
    //
    sv_inline void SVHalf::setFloat(f32 f) {
        union { f32 f; u32 i; } value = { f };
        u32 i = value.i;
        u32 e = (i >> 23) & 0x00ff;
        u32 m = i & 0x007fffff;
        if(e <= 127 - 15) {
            h = (u16)(((m | 0x00800000) >> (127 - 14 - e)) >> 13);
        } else {
            h = (u16)(i >> 13) & 0x3fff;
        }
        h |= (u16)(i >> 16) & 0xc000;
    }
    
    /*
     */
    sv_inline f32 SVHalf::getFloat() const {
        u32 i = (h << 16) & 0x80000000;
        u32 em = h & 0x7fff;
        if(em > 0x03ff) {
            i |= (em << 13) + ((127 - 15) << 23);
        } else {
            u32 m = em & 0x03ff;
            if(m != 0) {
                u32 e = (em >> 10) & 0x1f;
                while((m & 0x0400) == 0) {
                    m <<= 1;
                    e--;
                }
                m &= 0x3ff;
                i |= ((e + (127 - 14)) << 23) | (m << 13);
            }
        }
        union {
            u32 i;
            f32 f;
        } value = { i };
        return value.f;
    }
    
}//!namespace sv


#endif /* SV_AHALF_H */
