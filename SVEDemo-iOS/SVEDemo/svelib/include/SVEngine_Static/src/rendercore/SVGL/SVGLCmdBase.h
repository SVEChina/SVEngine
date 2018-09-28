//
// SVGLCmdBase.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_GLCMDBASE_H
#define SV_GLCMDBASE_H

#include "../SVRObjBase.h"

//GL渲染命令

namespace sv {

    namespace render{
        
        class SVGLCmdBase: public SVRObjBase {
        public:
            SVGLCmdBase(SVInst* _app);
            
            ~SVGLCmdBase();
        };
    }
}



#endif //SV_GLCMDBASE_H
