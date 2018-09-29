//
// Created by xuziyu on 2017/2/24.
//

#ifndef SV_IDPOOL_H
#define SV_IDPOOL_H

#include "SVObject.h"

//唯一ID池

namespace sv {
    
    class SVIDPool : public SVObject {
    public:
        SVIDPool();
        
        ~SVIDPool();
        
        u32 applyUID();
        
        void returnUID(u32 uid);
        
    private:
        u32 _random8();
        
        u32 _createID();
        
        bool _hasID(u32 _uid);
        
        u8 m_initialized;
        
        u64 m_seed;
        
        typedef SVArray<s32> IDPOOL;
        IDPOOL m_idpool;
    };
    
}//!namespace sv




#endif //SV_NODECREATOR_H
