//
//  SVDragma.h
//  SVEngine
//
//  Created by 付一洲 on 2018/9/13.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#ifndef SV_DRAGMA_H
#define SV_DRAGMA_H

#include <stdio.h>

#include "SVActBase.h"

namespace sv {
    
    namespace logic {
        
        //SVDragma 节点和时间轴的关系
        class SVDragma : public SVActBase {
        public:
            SVDragma(SVInst* _app);
            
            ~SVDragma();

            void create(f32 _time,u32 _rate);
            
            void create(TIMELINETYPE _type,f32 _time,u32 _rate);
            
            void destroy();
            
            inline u32 getUID(){ return m_uid; }
            
            void run(SVNodePtr _nodePtr, f32 dt);  //循环action
            
            void enter(SVNodePtr _nodePtr);   //进入action
            
            void exit(SVNodePtr _nodePtr);    //退出action
            
            bool isEnd();
            
            SVTimeLinePtr getTimeLine(s32 _type);
            
            void setTotalTime(f32 _t);
            
            void setCurTime(f32 _t);
            
        public:
            void toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue);
            
            void fromJSON(RAPIDJSON_NAMESPACE::Value &item);

        protected:
            typedef SVArray<SVTimeLinePtr> TLPOOL;
            TLPOOL m_timeLinePool;
            u32 m_uid;
        };
        
    }//!namespace logic
    
}//!namespace sv


#endif /* SV_DRAGMA_H */
