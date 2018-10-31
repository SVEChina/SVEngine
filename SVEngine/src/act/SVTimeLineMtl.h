//
//  SVTimeLineMtl.h
//  SVEngine
//
//  Created by 付一洲 on 2018/9/13.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#ifndef SV_TIMELINE_MTL_H
#define SV_TIMELINE_MTL_H

#include "SVTimeLine.h"

namespace sv {
    
    namespace logic {
        
        class SVTimeLineMtl : public SVTimeLine {
        public:
            SVTimeLineMtl(SVInst* _app,f32 _time,s32 _rate);
            
            ~SVTimeLineMtl();
            
            void enter(SVNodePtr _nodePtr);
            
            void exit(SVNodePtr _nodePtr);
            
            void update(SVNodePtr _nodePtr,f32 _dt);
            
        protected:
            SVKeyFramePtr _lerpKey() ;
            
            void _execkey(SVNodePtr _node,SVKeyFramePtr _key) ;
        };
        
    }//!namespace logic
    
}//!namespace sv


#endif /* SV_TIMELINE_MTL_H */
