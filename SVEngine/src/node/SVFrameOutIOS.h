//
// SVFrameOutIOS.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_FRAMEOUT_IOS_H
#define SV_FRAMEOUT_IOS_H

#include "SVFrameOutNode.h"

#ifdef SV_IOS

#include "VideoToolBox/VideoToolbox.h"
#include <CoreVideo/CVOpenGLESTextureCache.h>

namespace sv {
    
    namespace node{
        
        class SVFrameOutIOS : public SVFrameOutNode {
        public:
            SVFrameOutIOS(SVInst *_app);
            
            virtual ~SVFrameOutIOS();
            
            void init(SV_OUTSTEAMFORMATE _outformate,s32 _w,s32 _h);
            
            void destroy();
            
            void setCallback(cb_out_stream _cb);
            
            void update(f32 dt);
            
            void render();
            
            virtual void lockData();
            
            virtual void unlockData();
            
            virtual void* getData();
            
        protected:
            SVRenderTexturePtr m_outStreamFbo;
            SVDataSwapPtr m_dataswap;
            SVRenderMeshPtr m_mesh;
            cb_out_stream m_pOutStreamCB;
        };
        
    }//!namespace node
    
}//!namespace sv


#endif  //ios 

#endif //SV_FRAMEOUT_IOS_H
