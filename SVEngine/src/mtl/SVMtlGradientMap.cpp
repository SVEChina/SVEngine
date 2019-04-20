//
//  SVMtlGradientMap.cpp
//  SVEngine
//
//  Created by 徐子昱 on 2019/3/22.
//  Copyright © 2019 李晓帆. All rights reserved.
//

#include "SVMtlGradientMap.h"
#include "../rendercore/SVRendererBase.h"


SVMtlGradientMap::SVMtlGradientMap(SVInst *_app)
:SVMtlCore(_app,"GradientMap") {
    
}

SVMtlGradientMap::SVMtlGradientMap(SVMtlGradientMap* _mtl)
:SVMtlCore(_mtl){
    m_lSmoothSize = _mtl->m_lSmoothSize;
}

SVMtlGradientMap::~SVMtlGradientMap(){
    
}

SVMtlCorePtr SVMtlGradientMap::clone(){
     return PointerSharedPtr<SVMtlGradientMap>(new SVMtlGradientMap(this));
}

void SVMtlGradientMap::setSmooth(f32 _smooth){
    if (_smooth >= 0.0) {
        m_lSmoothSize = clamp(0.0f,_smooth,1.0f);
    }
}

void SVMtlGradientMap::_submitMtl(SVRendererBasePtr _render){
    SVMtlCore::_submitMtl(_render);
    _render->submitUniformf("softenStrength", m_lSmoothSize);
}

