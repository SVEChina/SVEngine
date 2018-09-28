//
// SVGLCmdTexture.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_GLCMDTEXTURE_H
#define SV_GLCMDTEXTURE_H

#include "SVGLCmdBase.h"

//GL渲染命令-纹理

namespace sv {

    namespace render{
        
        class SVGLCmdTexture: public SVGLCmdBase {
        public:
            SVGLCmdTexture(SVInst* _app);
            
            ~SVGLCmdTexture();
        };

        
    }
}

#endif //SV_GLCMDTEXTURE_H
