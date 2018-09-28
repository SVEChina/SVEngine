//
// SVRenderPipline.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_RENDERPIPLE_H
#define SV_RENDERPIPLE_H

#include "../base/SVGBase.h"
#include "SVRenderDef.h"

//渲染管线

namespace sv {
    
    namespace render{

        class SVRenderPipline : public SVGBase {
        public:
            SVRenderPipline(SVInst *_app);
            
            ~SVRenderPipline();
            
            void render();
            
            void pushRenderCmd(RENDERSTREAMTYPE _rst, SVRenderCmdPtr _rcmd);
            
            void clearRenderCmd();
            
        protected:
            typedef SVArray<SVRenderStreamPtr> RSTREAMVEC;
            RSTREAMVEC m_rstream_vec;
        };
        
    }//!namespace render
    
}//!namespace sv



//
#endif //SV_RENDERSCENE_H
