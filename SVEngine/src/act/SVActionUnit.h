//
// SVActionUnit.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_ACTIONUNIT_H
#define SV_ACTIONUNIT_H

#include "../base/SVGBase.h"
#include "SVActDef.h"
#include "../node/SVNodeDeclare.h"

namespace sv{
    
    namespace logic {
        
        class SVActionUnit : public SVGBase {
        public:
            SVActionUnit(SVInst* _app);
            
            ~SVActionUnit();
            
            void init();
            
            void destroy();
            
            void enter();
            
            void exit();
            
            void update(f32 _dt);
            
            void setAct(SVActBasePtr _actPtr);
            
            void setNode(SVNodePtr _nodePtr);
            
            bool isEnd();
        protected:
            bool m_isEnd;
            SVActBasePtr m_actPtr;
            SVNodePtr m_nodePtr;
        };
    }//!namespace logic
}//!namespace sv





#endif //SV_ACTIONUNIT_H
