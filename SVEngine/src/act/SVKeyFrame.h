//
//  SVKeyFrame.h
//  SVEngine
//
//  Created by 付一洲 on 2018/9/13.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#ifndef SV_KEYFRAME_H
#define SV_KEYFRAME_H

#include <stdio.h>

#include "../base/SVGBase.h"
#include "SVActDeclare.h"
#include "../base/SVVec3.h"
#include "../mtl/SVMtlCore.h"

namespace sv {
    
    namespace logic {
        
        class SVKeyFrame : public SVGBase {
        public:
            SVKeyFrame(SVInst* _app);
            
            ~SVKeyFrame();
            
            f32 m_time;
            
            inline u32 getUID(){ return m_uid; }
            
        protected:
            u32 m_uid;
            
        public:
            void toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue);
            
            void fromJSON(RAPIDJSON_NAMESPACE::Value &item);
        };
        
        //
        class SVKeyDeform : public SVKeyFrame {
        public:
            SVKeyDeform(SVInst* _app);
            
            ~SVKeyDeform();
            
            FVec3 m_pos;
            FVec3 m_rot;
            FVec3 m_scale;
        };
        //
        class SVKeyMtl : public SVKeyFrame {
        public:
            SVKeyMtl(SVInst* _app);
            
            ~SVKeyMtl();
            
            SVMtlCorePtr m_mtl;
        };
        //
        class SVKeyEvent : public SVKeyFrame {
        public:
            SVKeyEvent(SVInst* _app);
            
            ~SVKeyEvent();
        };
        
    }//!namespace logic
    
}//!namespace sv


#endif /* SV_KEYFRAME_H */
