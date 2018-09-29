//
// SVMtlShapeVaried.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MTLSHAPEVARIED_H
#define SV_MTLSHAPEVARIED_H

#include "SVMtlCore.h"

namespace sv {
    
    namespace mtl{
        
        class SVMtlFaceShapeVaried : public SVMtlCore {
        public:
            SVMtlFaceShapeVaried(SVInst *_app);
            
            SVMtlFaceShapeVaried(SVMtlFaceShapeVaried *_mtl);
            
            ~SVMtlFaceShapeVaried();
            
            virtual SVMtlCorePtr clone();
            
            virtual void update(f32 dt);
            
        protected:
            void _submitMtl(SVRendererBasePtr _render); 

            f32 m_startPoint[48];
            f32 m_endPoint[48];
            f32 m_actionType[24];
            f32 m_intensity[24];
            f32 m_radius[24];
            s32 m_RealStep[1];
            f32 m_outlinePoints[212];
            f32 m_eyesDistance;
            f32 m_inversedEyesDistance;
            f32 m_g_intensity[14];
            f32 m_surfaceWidth;
            f32 m_surfaceHeight;
            f32 m_inv_surfaceWidth;
            f32 m_inv_surfaceHeight;
        };
        
    }//!namespace mtl
    
}//!namespace sv

#endif /* SV_MTLSHAPEVARIED_H */
