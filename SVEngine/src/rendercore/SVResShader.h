//
// SVResShader.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_RESSHADER_H
#define SV_RESSHADER_H

#include "SVRObjBase.h"
#include "../base/SVPreDeclare.h"

namespace sv {
    
    namespace render{
        
        class SVResShader: public SVRObjBase{
        public:
            SVResShader(SVInst* _app);
            
            virtual ~SVResShader();
        };
        
    }//!namespace render
    
}//!namespace sv


#endif /* SV_RESSHADER_H */
