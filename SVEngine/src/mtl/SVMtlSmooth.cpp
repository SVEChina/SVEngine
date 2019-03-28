//
//  SVMtlSmooth.cpp
//  SVEngine
//
//  Created by 徐子昱 on 2019/3/26.
//  Copyright © 2019 李晓帆. All rights reserved.
//

#include "SVMtlSmooth.h"
#include "../rendercore/renderer/SVRendererBase.h"


SVMtlSmooth::SVMtlSmooth(SVInst *_app , SVString _name)
:SVMtlCore(_app,_name.c_str()) {
    m_smooth = 0.0f;
    m_w = 0.0f;
    m_h = 0.0f;
}

SVMtlSmooth::SVMtlSmooth(SVMtlSmooth* _mtl)
:SVMtlCore(_mtl){
    m_smooth = _mtl->m_smooth;
    m_w = _mtl->m_w;
    m_h = _mtl->m_h;
}

SVMtlSmooth::~SVMtlSmooth(){
    
}

SVMtlCorePtr SVMtlSmooth::clone(){
    return PointerSharedPtr<SVMtlSmooth>(new SVMtlSmooth(this));
}

void SVMtlSmooth::_submitMtl(SVRendererBasePtr _render){
    SVMtlCore::_submitMtl(_render);
    _render->submitUniformf("m_smooth", m_smooth);
    _render->submitUniformf("m_width", m_w);
    _render->submitUniformf("m_height", m_h);
}
