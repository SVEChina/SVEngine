//
//  SVTimeLineDeform.cpp
//  SVEngine
//
//  Created by 付一洲 on 2018/9/13.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#include "SVTimeLineDeform.h"

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
    SVKeyFramePtr t_key1 = nullptr;//_preKey();
    SVKeyFramePtr t_key2 = nullptr;//_nxtKey();
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
            FVec3 t_pos = tt_key1->m_pos * (1.0f-t_lerp) + tt_key2->m_pos;
            _nodePtr->setPosition(t_pos);
            //需要用到四元数插值
            //FVec3 t_rot = tt_key1->m_rot * (1.0f-t_lerp) + tt_key2->m_rot;
            //_nodePtr->setRotation(t_rot);
            FVec3 t_scale = tt_key1->m_scale * (1.0f-t_lerp) + tt_key2->m_scale;
            _nodePtr->setScale(t_scale);
        }
    }
}
