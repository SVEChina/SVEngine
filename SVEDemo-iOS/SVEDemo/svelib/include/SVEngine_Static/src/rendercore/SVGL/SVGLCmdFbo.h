//
// SVGLCmdFbo.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_GLCMDFBO_H
#define SV_GLCMDFBO_H

#include "SVGLCmdBase.h"

//GL渲染命令-fbo

namespace sv {
    
    namespace render{
        
        class SVGLCmdFbo: public SVGLCmdBase {
        public:
            SVGLCmdFbo(SVInst* _app);
            
            ~SVGLCmdFbo();
        };

    }
}


#endif //SV_GLCMDFBO_H
