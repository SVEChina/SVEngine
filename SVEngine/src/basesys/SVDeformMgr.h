//
//  SVDeformMgr.h
//  SVEngine
//
//  Created by 徐子昱 on 2019/4/13.
//  Copyright © 2019 李晓帆. All rights reserved.
//

#ifndef SV_DEFORM_MGR_H
#define SV_DEFORM_MGR_H



#include "../basesys/SVSysBase.h"

namespace sv {
        
        //图片处理
        class SVDeformMgr : public SVSysBase {
        public:
            SVDeformMgr(SVInst *_app);
            
            ~SVDeformMgr();
            //
            void init();
            
            void update(f32 _dt);
            
            void destroy();
    
            void pushDeform(SVDeformImageMovePtr deform);
        
        protected:
            SVLockPtr m_defromLock;
            SVArray<SVDeformImageMovePtr> m_deformArray;
            
        };
    
}//!namespace sv

#endif /* SV_DEFORM_MGR_H */
