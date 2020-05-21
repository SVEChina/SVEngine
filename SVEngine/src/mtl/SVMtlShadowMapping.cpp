//
//  SVMtlShadowMapping.cpp
//  SVEngine
//
//  Created by 徐子昱 on 2019/9/13.
//  Copyright © 2019 李晓帆. All rights reserved.
//

#include "SVMtlShadowMapping.h"
#include "../mtl/SVTexMgr.h"
#include "../rendercore/SVRenderer.h"

//
SVMtlShadowMapping::SVMtlShadowMapping(SVInst *_app)
:SVMtlCore(_app,"depthPlan") {
 
}

SVMtlShadowMapping::SVMtlShadowMapping(SVMtlShadowMapping* _mtl)
:SVMtlCore(_mtl) {
    memcpy(m_LightVPMat, _mtl->m_LightVPMat, sizeof(f32) * 16);
}

SVMtlShadowMapping::~SVMtlShadowMapping(){
}

void SVMtlShadowMapping::setLightVPMat(f32 *_vp){
      memcpy(m_LightVPMat, _vp, sizeof(f32) * 16);
      m_LogicParamDepth.clear=true;
      setDepthEnable(true);
}

SVMtlCorePtr SVMtlShadowMapping::clone() {
    return PointerSharedPtr<SVMtlShadowMapping>(new SVMtlShadowMapping(this));
}

void SVMtlShadowMapping::_submitMtl(SVRendererPtr _render) {
    SVMtlCore::_submitMtl(_render);
   _render->submitUniformMatrix("aMatrixVPlight", m_LightVPMat);
}



