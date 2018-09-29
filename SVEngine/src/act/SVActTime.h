//
// SVActTime.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_ACTIONTIME_H
#define SV_ACTIONTIME_H

//基于时间控制的Action

#include "SVActBase.h"
namespace sv {
    
    namespace logic {
        
        //时间参数表
        class SVActParamTime : public SVActParam {
        public:
            SVActParamTime();
       
            SVActBasePtr genAct(SVInst *_app);
            
            f32 m_time;
        };
        
        //等待
        class SVActParamWait : public SVActParamTime {
        public:
            SVActParamWait();
            
            SVActBasePtr genAct(SVInst *_app);
        };
        
        //隐藏
        class SVActParamHide : public SVActParamTime {
        public:
            SVActParamHide();
            
            SVActBasePtr genAct(SVInst *_app);
        };
        
        
        class SVActTime : public SVActBase {
        public:
            SVActTime(SVInst *_app);
            
            ~SVActTime();
            
            virtual void initParam(SVActParamPtr _paramPtr);
            
            virtual void run(SVNodePtr _nodePtr, f32 _dt);
            
            virtual bool isEnd();
            
            virtual void reset();
            
            void setTime(f32 _time);
            
        protected:
            f32 m_time;
            f32 m_acctime;
        };
        
        //可见不变
        class SVActWait : public SVActTime {
        public:
            SVActWait(SVInst *_app);
            
            ~SVActWait();
            
        };
        
        class SVActHide : public SVActWait {
        public:
            SVActHide(SVInst *_app);
            
            ~SVActHide();
            
            virtual void run(SVNodePtr _node, f32 _dt);
            
            virtual void enter(SVNodePtr _node);   //进入action
            
            virtual void exit(SVNodePtr _node);    //退出action
        };
        
    }//!namespace logic
    
}//!namespace sv


#endif //SV_ACTIONTIME_H
