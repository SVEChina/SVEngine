//
// SVActFollow.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_ACTFOLLOW_H
#define SV_ACTFOLLOW_H


#include "SVActBase.h"
#include "../base/SVVec3.h"

namespace sv {
    
    namespace logic {
        
        class SVActFollow : public SVActBase {
        public:
            SVActFollow(SVInst *_app);
            
            ~SVActFollow();
            
            void run(SVNodePtr _nodePtr, f32 dt);
            
            bool isEnd();
            
            void setTargetNode(SVNodePtr _nodePtr);
            
            void setValidDis(f32 _valid);
            
            void setSpeed(f32 _sp);
            
        protected:
            f32 m_validDis;         //有效距离 在有效距离内 是不会跟随目标节点移动的
            f32 m_speed;            //相机移动速度
            SVNodePtr m_targetNodePtr;   //目标节点
        };
        
        //圈飞
        class SVActCircle : public SVActBase {
        public:
            SVActCircle(SVInst *_app);
            
            ~SVActCircle();
            
            void run(SVNodePtr _nodePtr, f32 dt);
            
            bool isEnd();
            
            void setCircleParam(FVec3& _pos,f32 _radius);
            
            void setAngleParam(f32 _sp,f32 _angle);
            
        protected:
            f32 m_radius;       //半径
            FVec3 m_center; //中心
            f32 m_angle;        //角度
            f32 m_angleSp;      //角速度
        };

        class SVActFollowPerson : public SVActBase {
        public:
            SVActFollowPerson(SVInst *_app, s32 _personID);
            
            ~SVActFollowPerson();
            
            void run(SVNodePtr _nodePtr, f32 dt);
            
            bool isEnd();
            
            void setFllowIndex(s32 _index);//设置人脸上的跟随点
            
            void setBindOffset(f32 _offsetX, f32 _offsetY, f32 _offsetZ);//设置跟随点偏移
            
            void setScale(f32 _scaleX, f32 _scaleY, f32 _scaleZ);
        protected:
            f32 m_offsetX;
            f32 m_offsetY;
            f32 m_offsetZ;
            f32 m_scaleX;
            f32 m_scaleY;
            f32 m_scaleZ;
            s32 m_bindIndex;
            s32 m_personID;
        };
    }//!namespace node
    
}//!namespace sv

#endif //SV_ACTFOLLOW_H
