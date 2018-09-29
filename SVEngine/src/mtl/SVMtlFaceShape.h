//
// SVMtlFaceShape.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#ifndef SV_MTLFACESHAPE_H
#define SV_MTLFACESHAPE_H

#include "SVMtlCore.h"

namespace sv {
    
    namespace mtl{
        
        class SVMtlFaceShape : public SVMtlCore {
        public:
            SVMtlFaceShape(SVInst *_app);
            
            SVMtlFaceShape(SVMtlFaceShape *_mtl);
            
            ~SVMtlFaceShape();
            
            virtual SVMtlCorePtr clone();
            
            void setEyeLarge(f32 fLeftLarge , f32 fRightLarge);
            void setFaceSlim(f32 fSlim);
            void setChinUpSlim(f32 fSlim);
            void setChinMiddleSlim(f32 fSlim);
            void setChinSlim(f32 fSlim);
            
            FVec2 getEyeLarge();
            f32 getFaceSlim();
            f32 getChinUpSlim();
            f32 getChinMiddleSlim();
            f32 getChinSlim();
            
            virtual void update(f32 dt);
            
        protected:
            void _submitMtl(SVRendererBasePtr _render); 

            f32 m_location0[2];
            f32 m_location1[2];
            f32 m_location2[2];
            f32 m_location3[2];
            f32 m_location4[2];
            f32 m_location5[2];
            f32 m_location6[2];
            f32 m_location7[2];
            f32 m_location8[2];
            f32 m_location9[2];
            f32 m_location10[2];
            f32 m_location11[2];
            f32 m_location12[2];
            f32 m_location13[2];
            f32 m_location14[2];
            f32 m_location15[2];
            f32 m_location16[2];
            f32 m_location17[2];
            f32 m_location18[2];
            f32 m_location19[2];
            f32 m_location20[2];
            f32 m_location21[2];
            f32 m_location22[2];
            f32 m_location23[2];
            
            f32 m_eyeLarge[2];
            f32 m_faceSlim[4];

        };
        
    }//!namespace mtl
    
}//!namespace sv


#endif /* SV_MTLFACESHAPE_H */
