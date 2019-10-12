//
// SVActDeform.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_ACTDEFORM_H
#define SV_ACTDEFORM_H

//基于时间控制的Action

#include "SVActTime.h"
#include <memory>

namespace sv {
    
    namespace logic {
        /*************************************************************/

        class SVActDeform : public SVActTime {
        public:
            SVActDeform(SVInst *_app);
            
            ~SVActDeform();
            
            virtual void run(SVNodePtr _node, f32 _dt);
        
        protected:
            
        };

        //设定一个值
        class SVActMove : public SVActDeform {
        public:
            SVActMove(SVInst *_app);
            
            ~SVActMove();
            
            virtual void run(SVNodePtr _nodePtr, f32 _dt);
            
            virtual void enter(SVNodePtr _nodePtr);   //进入action
            
            virtual void exit(SVNodePtr _nodePtr);    //退出action
            
        protected:
            FVec3 m_pos;
        };

        class SVActMoveTo : public SVActDeform {
        public:
            SVActMoveTo(SVInst *_app);
            
            ~SVActMoveTo();
            
            virtual void run(SVNodePtr _node, f32 _dt);
            
            virtual void enter(SVNodePtr _node);   //进入action
            
            virtual void exit(SVNodePtr _node);    //退出action
            
            void setBeginPos(FVec3& _pos);
            
            void setEndPos(FVec3& _pos);
        protected:
            FVec3 m_target;
            FVec3 m_srcpos;
        };

        class SVActMoveBetween : public SVActDeform {
        public:
            SVActMoveBetween(SVInst *_app);
            
            ~SVActMoveBetween();
            
            virtual void run(SVNodePtr _nodePtr, f32 _dt);
            
            virtual void enter(SVNodePtr _nodePtr);   //进入action
            
            virtual void exit(SVNodePtr _nodePtr);    //退出action
            
            void setBeginPos(FVec3& _pos);
            
            void setEndPos(FVec3& _pos);
            
        protected:
            FVec3 m_begin;
            FVec3 m_end;
        };

        //
        class SVActMoveBy : public SVActDeform {
        public:
            SVActMoveBy(SVInst *_app);
            
            ~SVActMoveBy();
            
            virtual void run(SVNodePtr _nodePtr, f32 _dt);
            
            virtual void enter(SVNodePtr _nodePtr);   //进入action
            
            virtual void exit(SVNodePtr _nodePtr);    //退出action
            
            void setDir(FVec3& _dir);
            
            void setSpeed(f32 _sp);
            
        protected:
            FVec3 m_dir;
            f32 m_sp;
        };

        class SVActRot : public SVActDeform {
        public:
            SVActRot(SVInst *_app);
            
            ~SVActRot();
            
            virtual void run(SVNodePtr _nodePtr, f32 _dt);
            
        };

        class SVActRotTo : public SVActDeform {
        public:
            SVActRotTo(SVInst *_app);
            
            ~SVActRotTo();
            
            virtual void run(SVNodePtr _nodePtr, f32 _dt);
            
            void setSrc(FVec3& _src);
            
            void setDir(FVec3& _dir);
        protected:
            FVec3 m_src;
            FVec3 m_dir;
        };

        class SVActRotBetween : public SVActDeform {
        public:
            SVActRotBetween(SVInst *_app);
            
            ~SVActRotBetween();
            
            virtual void run(SVNodePtr _nodePtr, f32 _dt);
            
        protected:
        };
        //
        class SVActAlpha : public SVActDeform {
        public:
            SVActAlpha(SVInst *_app);
            
            ~SVActAlpha();
            
            virtual void run(SVNodePtr _nodePtr, f32 _dt);
            
            virtual void enter(SVNodePtr _nodePtr);   //进入action
            
            virtual void exit(SVNodePtr _nodePtr);    //退出action
            
            void setSrcAlpha(f32 _alpha);
            
            void setTarAlpha(f32 _alpha);
        protected:
            f32 m_srcAlpha;
            f32 m_tarAlpha;
        };

        //
        class SVActScale : public SVActDeform {
        public:
            SVActScale(SVInst *_app);
            
            ~SVActScale();
            
            virtual void run(SVNodePtr _nodePtr, f32 _dt);
            
        protected:
            
        };

        //以一定的速率 缩放到目标
        class SVActScaleTo : public SVActDeform {
        public:
            SVActScaleTo(SVInst *_app);
            
            ~SVActScaleTo();
            
            virtual void run(SVNodePtr _nodePtr, f32 _dt);
            
            void setSrcScale(FVec3 _scale);
            
            void setTargetScale(FVec3 _scale);
        protected:
            FVec3 m_target;
            FVec3 m_srcscale;
        };

        class SVActScaleBetween : public SVActDeform {
        public:
            SVActScaleBetween(SVInst *_app);
            
            ~SVActScaleBetween();
            
            virtual void run(SVNodePtr _nodePtr, f32 _dt);
            
            virtual void enter(SVNodePtr _nodePtr);   //进入action
            
            virtual void exit(SVNodePtr _nodePtr);    //退出action
            
            void setBeginScale(FVec3& _scale);
            
            void setEndScale(FVec3& _scale);
            
        protected:
            FVec3 m_begin;
            
            FVec3 m_end;
        };
        
        class SVActPosition : public SVActDeform {
        public:
            SVActPosition(SVInst *_app);
            
            ~SVActPosition();
            
            virtual void run(SVNodePtr _nodePtr, f32 _dt);
            
            virtual void enter(SVNodePtr _nodePtr);   //进入action
            
            virtual void exit(SVNodePtr _nodePtr);    //退出action
            
            void setEnableRandom(bool _random);
            
            void setMinPosition(FVec3 _minPos);
            
            void setMaxPosition(FVec3 _maxPos);
        protected:
            void _randomPosition(SVNodePtr _nodePtr);
            bool  m_enableRandom;
            FVec3 m_maxPos;
            FVec3 m_minPos;
        };
    }//!namespace logic
    
}//!namespace sv

#endif //SV_ACTDEFORM_H
