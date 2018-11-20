//
//  SVMovie.hpp
//  SVEngine
//
//  Created by 付一洲 on 2018/9/13.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#ifndef SV_MOVIE_H
#define SV_MOVIE_H

#include <stdio.h>

#include "SVAniBase.h"
#include "SVActDef.h"

namespace sv {
    
    namespace logic {
        
        //电影和剧本的关系
        
        class SVMovie : public SVAniBase {
        public:
            SVMovie(SVInst* _app);
            
            ~SVMovie();
            
            virtual void init();
            
            virtual void destroy();
            
            virtual void enter();
            
            virtual void exit();
            
            virtual bool isEnd();
            
            virtual void update(f32 _dt);
            
            void play();
            
            void pause();
            
            void stop();
            
            bool isLoop();
            
            void setLoop(bool _loop);
            
            MOVSTATE getMovState() { return m_state; }
            
            void setTotalTime(f32 _time);
            
            f32 getTotalTime();
            
            void setCurTime(f32 _time);
            
            f32 getCurTime();
            
            void setRate(s32 _rate);

            s32 getRate(){ return m_rate; }
            
            //剧本相关
            void addDragma(SVDragmaPtr _dragma);
            
            void delDragma(SVDragmaPtr _dragma);
            
            void clearDragma();
            
            //通过uid删除
            void delDragmaByUID(u32 _uid);
            
            //通过索引删除
            void delDragmaByIndex(u32 _index);
            
            //
            SVDragmaPtr getDragmaByUID(u32 _uid);
            
            //
            SVDragmaPtr getDragmaByIndex(u32 _index);
            
            s32 getDragmaNum();
            
        public:
            void toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue);
            
            void fromJSON(RAPIDJSON_NAMESPACE::Value &item);
            
        protected:
            SVString m_name;
            
            bool m_loop;
            
            f32 m_accTime;
            
            f32 m_totalTime;
            
            MOVSTATE m_state;
            
            s32 m_rate;
            
            typedef SVArray<SVDragmaPtr> DragmaPool;
            DragmaPool m_dragmaPool;
            
            SVLockPtr m_dragmaLock;
            
        };//class SVMovie
        
    }//!namespace logic
    
}//!namespace sv


#endif /* SV_MOVIE_H */
