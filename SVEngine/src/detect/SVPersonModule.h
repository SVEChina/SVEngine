//
// SVPersonModule.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PERSONMODULE_H
#define SV_PERSONMODULE_H

#include "../event/SVEventProc.h"
namespace sv {
    
    namespace detect{
        
        class SVPersonModule : public SVEventProc {
        public:
            SVPersonModule(SVInst* _app);
            
            ~SVPersonModule();
            
            void initModule();
            
            void destoryModule();
            
            SVPersonPtr getPerson(s32 _personId);
        protected:
            SVPersonPtr m_persons[SV_SUPPORT_MAX_PERSON_NUM];
        };
        
    }//!namespace detect
    
}//!namespace sv



//
#endif //SV_PERSONMODULE_H
