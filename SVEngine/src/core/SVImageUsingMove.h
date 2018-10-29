//
// SVImageUsingMove.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_IMAGEUSINGMOVE_H
#define SV_IMAGEUSINGMOVE_H

#include "../base/SVObject.h"
#include "../base/SVVec2.h"
#include "../rendercore/SVRenderMesh.h"

namespace sv {
    
    namespace util{
        
        class SVImageUsingMove :public SVObject{
        public:
            SVImageUsingMove();
            
            ~SVImageUsingMove();
            
            SVArray<FVec2> m_controlArray;
            
            SVArray<FVec2> m_targetArray;
            
            SVArray<f32>  m_weightArray;
            
            FVec2 MLS(const FVec2& t);
            
            void clearContrl();
            
            void setControl(FVec2 _v);
            
            void setTargetControl(FVec2 _v);
        protected:
             SVLockPtr m_lock;
        };
        
    }//!namespace util
    
}//!namespace sv

#endif /* SV_IMAGEUSINGMOVE_H */
