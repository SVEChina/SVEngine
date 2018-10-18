//
// SVDataChunk.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_DATACHUNK_H
#define SV_DATACHUNK_H

#include "../basesys/SVSysBase.h"

//文件路径管理器 增加搜索路径相关
namespace sv {
    
    class SVDataChunk {
    public:
        SVDataChunk();
        
        virtual ~SVDataChunk();
        
        void apply(s32 size);
        
        void reback();
        
        void toString();
        
        u64 m_size;
        
        c8 *m_data;
    };
    
}//!namespace sv



#endif //SV_DATACHUNK_H
