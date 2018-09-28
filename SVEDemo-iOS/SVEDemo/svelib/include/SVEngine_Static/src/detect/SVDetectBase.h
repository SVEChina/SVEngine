//
// SVDetectBase.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_DETECTBASE_H
#define SV_DETECTBASE_H

#include "../base/SVListenBase.h"
#include "../base/SVDatatDef.h"

namespace sv {

    namespace detect{
        
        class SVDetectBase : public SVListenBase {
        public:
            SVDetectBase(SVInst *_app);
            
            ~SVDetectBase();
            
            virtual void update(f32 _dt);
            
            virtual s32 transformIndex(s32 index);
            
            virtual BINDREGION getIndexRegion(s32 index);
            
            virtual void pushData(void *_faceData);
            
        protected:
            s32 m_ptNum;
        };
        
    }//!namespace detect
    
}//!namespace sv




#endif //SV_DETECTBASE_H
