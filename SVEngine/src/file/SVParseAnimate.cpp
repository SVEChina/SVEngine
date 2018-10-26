//
// SVParseAnimate.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVParseAnimate.h"
#include "../node/SVSpineNode.h"
SVNodePtr
SVParseAnimate::parseAnim(SVInst *_app, RAPIDJSON_NAMESPACE::Value &item, s32 _resid, cptr8 _path) {
    SVSpineNodePtr t_spineNode = MakeSharedPtr<SVSpineNode>(_app);
    t_spineNode->m_rootPath = _path;
    t_spineNode->fromJSON(item);
    return t_spineNode;
}


