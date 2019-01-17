//
// SVActBase.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_ACTIONBASE_H
#define SV_ACTIONBASE_H

#include "SVActDef.h"
#include "../base/SVGBase.h"
#include "../base/SVVec3.h"
#include "../node/SVNodeDeclare.h"
namespace sv {
    
    namespace logic {
        
        class SVActParam : public SVObject {
        public:
            SVActParam();
            
            virtual SVActBasePtr genAct(SVInst *_app){ return nullptr; };
        };
        
        //参数表
        class SVActBase : public SVGBase {
        public:
            SVActBase(SVInst* _app);
            
            ~SVActBase();
            
            virtual void run(SVNodePtr _nodePtr, f32 dt);  //循环action
            
            virtual void enter(SVNodePtr _nodePtr);   //进入action
            
            virtual void exit(SVNodePtr _nodePtr);    //退出action
            
            virtual bool isEnd();
            
            virtual void reset();
            
        protected:
            SVString m_acttype;
            
        };

        
    }//!namespace logic
    
}//!namespace sv



#endif //SV_ACTIONBASE_H
