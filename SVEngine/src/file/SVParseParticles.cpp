//
// SVParseParticles.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVParseParticles.h"
#include "../node/SVParticlesNode.h"
SVNodePtr
SVParseParticles::parseParticles(SVInst *_app, RAPIDJSON_NAMESPACE::Value &item, s32 _resid, cptr8 _path) {
    SVParticlesNodePtr t_particlesNode = MakeSharedPtr<SVParticlesNode>(_app);
    t_particlesNode->m_rootPath = _path;
    t_particlesNode->fromJSON(item);
    return t_particlesNode;
}


