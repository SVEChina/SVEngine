//
// SVStreamIn.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_STREAMIN_H
#define SV_STREAMIN_H

#include "../base/SVGBase.h"
#include "../base/SVMap.h"

namespace sv {

    namespace logic {
        
        class SVStreamIn : public SVGBase {
        public:
            SVStreamIn(SVInst *_app);
            
            ~SVStreamIn();
            
            void createInStream(cptr8 _name,s32 _type,s32 _formate,s32 _w,s32 _h,f32 _angle);
    
            void destroyInStream(cptr8 _name);
            
            void active(cptr8 _name);
            
            void unactive(cptr8 _name);
            
            void pushStreamData(cptr8 _name,u8* _srcPtr,s32 width,s32 height,s32 pixelFormat,s32 _angle);
            
        protected:
            typedef SVMap<SVString,SVNodePtr> STREAMMAP;
            STREAMMAP m_streamMap;
            SVLockPtr m_streamLock;
        };
        
    }//!namespace logic

}//!namespace sv



#endif //SV_STREAMIN_H
