//
// SVAniTrigger.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_ACT_ANITRIGGER_H
#define SV_ACT_ANITRIGGER_H

#include "../base/SVGBase.h"
#include "../node/SVNodeDeclare.h"
namespace sv{
    
    namespace logic {
        
        class SVAniTrigger : public SVGBase {
        public:
            struct ANITRIGGERPARAM{
                SVString           tarName;
                SVString           condition;
                SVString           action;
            };
            SVAniTrigger(SVInst* _app);
            
            ~SVAniTrigger();
            
            virtual void init();
            
            virtual void destroy();
            
            virtual void enter();
            
            virtual void exit();
            
            virtual bool isEnd();
            
            virtual void update(f32 _dt);

            void noticeTriggerCondition(cptr8 _condition);
            
            void fromJson(RAPIDJSON_NAMESPACE::Value &item);
        protected:
            ANITRIGGERPARAM m_triggerParam;
        };
        
    }//!namespace logic
}//!namespace sv
#endif //SV_ACT_ANITRIGGER_H
