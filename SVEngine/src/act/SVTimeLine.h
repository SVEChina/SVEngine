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
            static u32 maxFrame(f32 _time,s32 _rate);
            
        public:
            SVTimeLine(SVInst* _app,f32 _time,s32 _rate);
            
            ~SVTimeLine();
  
            TIMELINETYPE getType();
            
            void enter(SVNodePtr _nodePtr);
            
            void exit(SVNodePtr _nodePtr);
            
            void update(SVNodePtr _nodePtr,f32 _dt);
            
            void addKey(SVKeyFramePtr _key);
            
            void removeKey(s32 _uid);
            
            SVKeyFramePtr getKeyFrame(s32 _uid);
            
            void refreshKey();
            
            SVKeyFramePtr getBeginKey();
            
            SVKeyFramePtr getEndKey();
            
            void setTotalTime(f32 _t);
            
            void setCurTime(f32 _t);
            
            //刷新节点
            void refreshNode(SVNodePtr _nodePtr);
            
        public:
            void toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator,
                        RAPIDJSON_NAMESPACE::Value &_objValue);
            
            void fromJSON(RAPIDJSON_NAMESPACE::Value &item);
            
        protected:
            //
            void _refreshKey();
            //
            virtual SVKeyFramePtr _lerpKey();
            //
            s32 _getCurKeyIndex();
            //
            f32 indexToTime(u32 _index);
            //
            virtual SVKeyFramePtr _preKey();
            //
            virtual SVKeyFramePtr _nxtKey();
            //
            virtual void _execkey(SVNodePtr _node,SVKeyFramePtr _key);
            //
            TIMELINETYPE m_type;
            //
            SVLockPtr m_keyLock;
            //中间帧
            typedef SVArray<SVKeyFramePtr> KEYPOOL;
            KEYPOOL m_keyPool;
            //开始帧
            SVKeyFramePtr m_startKey;
            //结束帧
            SVKeyFramePtr m_endKey;
            //累计时间
            f32 m_accTime;
            //
            s32 m_rate;
            //
            f32 m_totalTime;
        };
        
    }//!namespace logic
    
}//!namespace sv


#endif /* SV_TIMELINE_H */
