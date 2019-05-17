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
            
            SVDeformImageMovePtr getDeform(s32 _postion);
            
            void clear();
        protected:
            
            void removeEnd();
        
        protected:
            SVLockPtr m_defromLock;
            SVArray<SVDeformImageMovePtr> m_deformArray;
            SVArray<SVDeformImageMovePtr> m_deformCir;
            
        };
    
}//!namespace sv

#endif /* SV_DEFORM_MGR_H */
