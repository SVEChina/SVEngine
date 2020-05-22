// SVDataSwap.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_DATASWAP_H
#define SV_DATASWAP_H

//数据交换区 有信号量互斥 共享区域

#include "../base/SVObject.h"
#include "../work/SVTdCore.h"
#include "../base/SVPreDeclare.h"

namespace sv {
    
    class SVDataSwap : public SVObject {
    public:
        SVDataSwap();
        
        ~SVDataSwap();
        
        void resize(u32 _size);
        
        void extendSize(u32 _size);
        
        void reback();
        
        void writeData(void *_data, u32 len);
        
        void appendData(void *_data, u32 len);
        
        void fetchData(void *_data, u32 len);
        
        void *getData();
        
        u32 getSize();
        
        u32 getCacheSize();
        
        void lockData();
        
        void unlockData();
        
        void unlockData(u32 _realSize);
        
        void swap(SVDataSwapPtr pTarget);
        
    protected:
        void *m_pData;
        u32 m_cacheSize;        //缓存空间大小
        u32 m_realSize;         //真实空间大小
        SVLockPtr m_swapLock;
    };
    
}//!namespace sv


#endif //SV_DATASWAP_H
