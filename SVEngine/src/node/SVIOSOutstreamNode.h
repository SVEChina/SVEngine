//
// SVIOSOutstreamNode.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_IOSOUTSTREAM_NODE_H
#define SV_IOSOUTSTREAM_NODE_H

#include "SVFrameOutNode.h"

#ifdef SV_IOS

#include "VideoToolBox/VideoToolbox.h"
#include <CoreVideo/CVOpenGLESTextureCache.h>

namespace sv {
    
    namespace node{
        
        class SVIOSOutstreamNode : public SVFrameOutNode {
        public:
            SVIOSOutstreamNode(SVInst *_app);
            
            virtual ~SVIOSOutstreamNode();
            
            void init(SV_OUTSTEAMFORMATE _outformate,s32 _w,s32 _h);
            
            void destroy();
            
            void setCallback(cb_out_stream _cb);
            
            void update(f32 dt);
            
            void render();
            
        protected:
            SVRenderTexturePtr m_outStreamFbo;
            SVDataSwapPtr m_dataswap;
            cb_out_stream m_pOutStreamCB;
        };
        
    }//!namespace node
    
}//!namespace sv


#endif  //ios 

#endif //SV_IOSOUT_H
