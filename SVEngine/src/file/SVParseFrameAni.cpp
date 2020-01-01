//
// SVParseFrameAni.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVParseFrameAni.h"
#include "../node/SVFrameAniNode.h"
SVNodePtr
SVParseFrameAni::parseFrameAni(SVInst *_app, RAPIDJSON_NAMESPACE::Value &item, s32 _resid, cptr8 _path) {
    SVFrameAniNodePtr t_aniNode = MakeSharedPtr<SVFrameAniNode>(_app);
    t_aniNode->m_rootPath = _path;
    t_aniNode->fromJSON(item);
    return t_aniNode;
}


