//
// SVFrameOutRead.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_FRAMEOUT_READ_H
#define SV_FRAMEOUT_READ_H

#include "SVFrameOutNode.h"

namespace sv {

    namespace node {
        
        //
        class SVFrameOutRead : public SVFrameOutNode {
        public:
            SVFrameOutRead(SVInst *_app);

            virtual ~SVFrameOutRead();
            
            virtual void create(s32 _width,s32 _height);
            
            virtual void destroy();
            
            void setFormat(SV_OUTSTEAMFORMATE _fromate);

            virtual void update(f32 dt);

            virtual void render();

            virtual void lockData();
            
            virtual void unlockData();
            
            virtual void* getData();
            
        private:
            void _resize();
            
            SV_OUTSTEAMFORMATE m_outFormat;
            SVDataSwapPtr m_pDataSwap;
            SVRenderTexturePtr m_fbo;
            SVMtlCorePtr m_pMtl;
            s32 m_width;
            s32 m_height;
        };
    }
}


#endif //SV_FRAMEOUT_READ_H
