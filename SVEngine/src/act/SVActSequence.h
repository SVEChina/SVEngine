//
// SVActSequence.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_ACTSEQUENCE_H
#define SV_ACTSEQUENCE_H

#include "SVActBase.h"
#include "../act/SVActDef.h"

namespace sv {
    
    namespace logic {
        
        class SVActSequence : public SVActBase {
        public:
            SVActSequence(SVInst *_app);
            
            ~SVActSequence();
            
            virtual void run(SVNodePtr _nodePtr, f32 dt);
            
            void addAct(SVActBasePtr _actPtr);
            
            void clearAct();
            
            void reset();
            
            bool isEnd();
            
        protected:
            typedef SVArray<SVActBasePtr> ACTQUENE;
            ACTQUENE m_actQuene;
            bool m_end;
            s32 m_curIndex;
        };

    }//!namespace logic
    
}//!namespace sv



#endif //SV_ACTSEQUENCE_H
