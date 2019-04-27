//
//  SVParseDeform.cpp
//  SVEngine
//
//  Created by 徐子昱 on 2018/10/29.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#include "SVParseDeform.h"
#include "../core/SVDeformImageMove.h"
#include "../rendercore/SVRendererBase.h"
#include "../rendercore/SVRenderTexture.h"
#include "../rendercore/SVRenderMgr.h"
#include "../app/SVInst.h"


SVDeformImageMovePtr SVParseDeform::parseDeform(SVInst *app, RAPIDJSON_NAMESPACE::Value &item, s32 _resid, cptr8 _path){
    SVDeformImageMovePtr m_deform=MakeSharedPtr<SVDeformImageMove>(app);
    SVTexturePtr t_innerTex = app->getRenderer()->getSVTex(E_TEX_MAIN);
    m_deform->init(t_innerTex,t_innerTex);
    m_deform->setIsDetect(true);
    // m_deform->setvisiPoint(true);
    m_deform->setFlip(true);
    m_deform->m_rule=0;
    m_deform->fromJSON(item);
    return m_deform;
}
