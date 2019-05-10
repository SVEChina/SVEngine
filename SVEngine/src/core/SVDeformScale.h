//
//  SVDeformScale.h
//  SVEngine
//
//  Created by 徐子昱 on 2019/5/9.
//  Copyright © 2019 李晓帆. All rights reserved.
//

#ifndef SV_DEFORM_SCALE_H

#include "../base/SVObject.h"
#include "../base/SVVec3.h"
#include "../rendercore/SVRenderMesh.h"

namespace sv {
    
    namespace util{
        
        class SVDeformScale :public SVObject{
            
            public:
            
            SVDeformScale();
            
            ~SVDeformScale();
            
            void setSmooth(f32 _smooth);
            
            void pushCtl(f32 x , f32 y, f32 z);
            
            void clearCtl();
            
            FVec2 getScalePostion(const FVec2& _t);
            
            protected:
            f32 m_smooth;
            SVArray<FVec3> m_controlArray;
            SVLockPtr m_lock;
        };
        
    }//!namespace util
    
}//!namespace sv

#endif /* SV_DEFORM_SCALE_H */
