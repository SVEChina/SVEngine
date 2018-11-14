//
// SVMtlAni2D.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MTL_ANI2D_H
#define SV_MTL_ANI2D_H


#include "SVMtlCore.h"

//2D动画材质
namespace sv {
    
    namespace mtl{
        
        class SVMtlAni2D : public SVMtlCore {
        public:
            SVMtlAni2D(SVInst *_app);
            
            SVMtlAni2D(SVMtlAni2D *_mtl);
            
            ~SVMtlAni2D();
            
            virtual void reset();
            
            virtual SVMtlCorePtr clone();
            
            void setBlendMode(SVMTLBLENDMODE _blendMode);
            
        protected:
            virtual void _submitUniform(SVRendererBasePtr _render);

            SVBlendModeParam m_LogicParamBlendMode;      //融合模式参数
        };

    }//!namespace mtl
    
}//!namespace sv


#endif //SV_MTL_ANI2D_H
