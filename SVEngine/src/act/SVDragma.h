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
        class SVDragma : public SVGBase {
        public:
            SVDragma(SVInst* _app);
            
            ~SVDragma();

            void create(f32 _time,u32 _rate);
            
            void create(TIMELINETYPE _type,f32 _time,u32 _rate);
            
            void destroy();
            
            inline u32 getUID(){ return m_uid; }
            
            void update(f32 dt);  //循环action
            
            void enter();   //进入action
            
            void exit();    //退出action
            
            SVTimeLinePtr getTimeLine(s32 _type);
            
            void setTotalTime(f32 _t);
            
            void setCurTime(f32 _t);
            
            SVNodePtr getBindNode();
            
            void bind(SVNodePtr _node);
            
            void unbind();
            
        public:
            void toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue);
            
            void fromJSON(RAPIDJSON_NAMESPACE::Value &item);

        protected:
            typedef SVArray<SVTimeLinePtr> TLPOOL;
            TLPOOL m_timeLinePool;
            u32 m_uid;
            //绑定的剧本
            SVLockPtr m_lock;
            SVNodePtr m_node;
        };
        
    }//!namespace logic
    
}//!namespace sv


#endif /* SV_DRAGMA_H */
