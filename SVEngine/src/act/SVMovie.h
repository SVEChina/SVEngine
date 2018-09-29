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

#include "../base/SVGBase.h"
#include "SVActDef.h"

namespace sv {
    
    namespace logic {
        
        //mv
        class SVMovie : public SVGBase {
        public:
            SVMovie(SVInst* _app);
            
            ~SVMovie();
            
            void play();
            
            void pause();
            
            void stop();
            
            bool isLoop();
            
            void setLoop(bool _loop);
            
            bool isEnd(){ return false; }
            
            inline MOVSTATE getMovState() { return m_state; }
            
            inline void setTotalTime(f32 _time){ m_totalTime = _time; }
            
            inline f32 getTotalTime(){ return m_totalTime; }
            
            //
            void addDragma(SVDragmaPtr _dragma);
            
            void delDragma(SVDragmaPtr _dragma);
            
            void delDragmaByUID(u32 _uid);
            
            void delDragmaByIndex(u32 _index);
            
            SVDragmaPtr getDragmaByUID(u32 _uid);
            
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
            
            typedef SVArray<SVDragmaPtr> DragmaPool;
            DragmaPool m_dragmaPool;
        };
        
    }//!namespace logic
    
}//!namespace sv


#endif /* SV_MOVIE_H */
