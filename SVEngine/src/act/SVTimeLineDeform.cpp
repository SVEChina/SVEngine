//
//  SVTimeLineDeform.cpp
//  SVEngine
//
//  Created by 付一洲 on 2018/9/13.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#include "SVTimeLineDeform.h"

SVTimeLineDeform::SVTimeLineDeform(SVInst* _app)
:SVTimeLine(_app){
    m_type = E_TL_T_DEFORM;
}

SVTimeLineDeform::~SVTimeLineDeform() {
}

void SVTimeLineDeform::enter(SVNodePtr _nodePtr) {
    SVTimeLine::enter(_nodePtr);
}

void SVTimeLineDeform::exit(SVNodePtr _nodePtr) {
    SVTimeLine::exit(_nodePtr);
}

void SVTimeLineDeform::update(SVNodePtr _nodePtr,f32 _dt) {
    SVTimeLine::update(_nodePtr,_dt);
    //插值
//    for(s32 i=0;i<m_keyPool.size();i++) {
//        if(m_keyPool[i]->m_time >= t_start_t && m_keyPool[i]->m_time < t_end_t) {
//            //执行key
//            SVKeyMtlPtr t_mtl_key = std::dynamic_pointer_cast<SVKeyMtl>(m_keyPool[i]);
//            if(t_mtl_key && t_mtl_key->m_mtl) {
//                _nodePtr->setMtl(t_mtl_key->m_mtl);
//            }
//        }
//    }
}


