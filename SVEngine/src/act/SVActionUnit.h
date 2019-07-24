//
// SVActionUnit.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_ACTIONUNIT_H
#define SV_ACTIONUNIT_H

#include "SVAniBase.h"
#include "../node/SVNodeDeclare.h"

namespace sv{
    
    namespace logic {
        
        class SVActionUnit : public SVAniBase {
        public:
            SVActionUnit(SVInst* _app);
            
            SVActionUnit(SVInst* _app, SVActBasePtr _action, SVNodePtr _node);
            
            ~SVActionUnit();
            
            virtual void enter();
            
            virtual void exit();
            
            virtual bool isEnd();
            
            virtual void update(f32 _dt);
            
            void setAct(SVActBasePtr _actPtr);
            
            void setNode(SVNodePtr _nodePtr);
            
            SVActBasePtr getAct();
            
            SVNodePtr getNode();
            
            void stop();
        protected:
            bool m_isEnd;
            SVActBasePtr m_actPtr;
            SVNodePtr m_nodePtr;
        };
        
    }//!namespace logic
}//!namespace sv





#endif //SV_ACTIONUNIT_H
