//
// SVParseMask.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVParseMask.h"
#include "../node/SV2DFaceMaskSTNode.h"
#include "../node/SV2DFaceMaskNode.h"
//解析面具
SVNodePtr
SVParseMask::parseMask(SVInst *_app, RAPIDJSON_NAMESPACE::Value &item, s32 _resid, cptr8 _path) {
    SV2DFaceMaskSTNodePtr t_maskNode = MakeSharedPtr<SV2DFaceMaskSTNode>(_app);
    t_maskNode->m_rootPath = _path;
    t_maskNode->fromJSON(item);
    return t_maskNode;
}


