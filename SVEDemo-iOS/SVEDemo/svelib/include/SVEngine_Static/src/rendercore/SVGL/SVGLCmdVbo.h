//
// SVGLCmdVbo.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_GLCMDVBO_H
#define SV_GLCMDVBO_H

#include "SVGLCmdBase.h"

//GL渲染命令-vbo

namespace sv {
    
    namespace render{
        
        class SVGLCmdVbo: public SVGLCmdBase {
        public:
            SVGLCmdVbo(SVInst* _app);
            
            ~SVGLCmdVbo();
        };

    }
}


#endif //SV_GLCMDVBO_H
