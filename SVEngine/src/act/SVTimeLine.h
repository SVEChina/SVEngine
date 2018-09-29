//
//  SVMovie.hpp
//  SVEngine
//
//  Created by 付一洲 on 2018/9/13.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#ifndef SV_TIMELINE_H
#define SV_TIMELINE_H

#include "../base/SVGBase.h"
#include "SVActBase.h"
#include "SVKeyFrame.h"
#include "../node/SVNode.h"

namespace sv {
    
    namespace logic {
        
        class SVTimeLine : public SVGBase {
        public:
            SVTimeLine(SVInst* _app);
            
            ~SVTimeLine();
            
            TIMELINETYPE getType();
            
            void enter(SVNodePtr _nodePtr);
            
            void exit(SVNodePtr _nodePtr);
            
            void update(SVNodePtr _nodePtr,f32 _dt);
            
            void addKey(SVKeyFramePtr _key);
            
            void removeKey(s32 _uid);
            
            SVKeyFramePtr getKeyFrame(s32 _uid);
            
            void refreshKey();
            
        public:
            void toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue);
            
            void fromJSON(RAPIDJSON_NAMESPACE::Value &item);
            
        protected:
            TIMELINETYPE m_type;
            
            typedef SVArray<SVKeyFramePtr> KEYPOOL;
            KEYPOOL m_keyPool;
            
            f32 m_accTime;
        };
        
    }//!namespace logic
    
}//!namespace sv


#endif /* SV_TIMELINE_H */
