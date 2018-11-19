//
//  SVTimeLineDeform.cpp
//  SVEngine
//
//  Created by 付一洲 on 2018/9/13.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#include "SVTimeLineDeform.h"
#include "../base/SVQuat.h"

SVTimeLineDeform::SVTimeLineDeform(SVInst* _app,f32 _time,s32 _rate)
:SVTimeLine(_app,_time,_rate){
    m_type = E_TL_T_DEFORM;
}

SVTimeLineDeform::~SVTimeLineDeform() {
}

void SVTimeLineDeform::initKey() {
    SVKeyDeformPtr m_startKey = MakeSharedPtr<SVKeyDeform>(mApp,0);
    m_keyPool.append(m_startKey);
    SVKeyDeformPtr m_endKey = MakeSharedPtr<SVKeyDeform>(mApp,m_maxFrame);
    m_keyPool.append(m_endKey);
}

void SVTimeLineDeform::enter(SVNodePtr _nodePtr) {
    SVTimeLine::enter(_nodePtr);
}

void SVTimeLineDeform::exit(SVNodePtr _nodePtr) {
    SVTimeLine::exit(_nodePtr);
}

void SVTimeLineDeform::_execkey(SVNodePtr _nodePtr,f32 _dt) {
    //
    SVKeyFramePtr t_key1 = nullptr;
    SVKeyFramePtr t_key2 = nullptr;
    //
    for(s32 i=0;i<m_keyPool.size();i++) {
        s32 t_index = m_keyPool[i]->getIndex();
        f32 t_keytime = 1.0f*t_index/m_rate;
        if(t_keytime>m_accTime) {
            t_key2 = m_keyPool[i];
            if(i == 0){
                t_key1 = t_key2;
            }else{
                t_key2 = m_keyPool[i-1];
            }
            break;
        }
    }
    if(t_key1 && t_key2) {
        if(t_key1 == t_key2) {
            SVKeyDeformPtr tt_key1 = std::dynamic_pointer_cast<SVKeyDeform>(t_key1);
            _nodePtr->setPosition(tt_key1->m_pos);
            _nodePtr->setRotation(tt_key1->m_rot);
            _nodePtr->setScale(tt_key1->m_scale);
        }else {
            f32 t_time1 = indexToTime(t_key1->getIndex());
            f32 t_time2 = indexToTime(t_key2->getIndex());
            f32 t_lerp = (m_accTime - t_time1)/(t_time2 - t_time1);
            //进行刷新操作
            SVKeyDeformPtr tt_key1 = std::dynamic_pointer_cast<SVKeyDeform>(t_key1);
            SVKeyDeformPtr tt_key2 = std::dynamic_pointer_cast<SVKeyDeform>(t_key2);
            //位移插值
            FVec3 t_pos = tt_key1->m_pos * (1.0f-t_lerp) + tt_key2->m_pos;
            _nodePtr->setPosition(t_pos);
            //四元数旋转插值
            SVQuat t_qt1(tt_key1->m_rot.x,tt_key1->m_rot.y,tt_key1->m_rot.z);
            SVQuat t_qt2(tt_key2->m_rot.x,tt_key2->m_rot.y,tt_key2->m_rot.z);
            SVQuat t_qt = slerp(t_qt1,t_qt2,t_lerp);
            _nodePtr->setQuat(t_qt);
            //缩放插值
            FVec3 t_scale = tt_key1->m_scale * (1.0f-t_lerp) + tt_key2->m_scale;
            _nodePtr->setScale(t_scale);
        }
    }
}
