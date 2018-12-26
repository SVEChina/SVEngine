//
// SVParseBMFont.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVParseBMFont.h"
#include "../node/SVBMFontNode.h"
SVNodePtr
SVParseBMFont::parseBMFont(SVInst *_app, RAPIDJSON_NAMESPACE::Value &item, s32 _resid, cptr8 _path) {
    SVBMFontNodePtr t_bmFontNode = MakeSharedPtr<SVBMFontNode>(_app);
    t_bmFontNode->m_rootPath = _path;
    t_bmFontNode->fromJSON(item);
    return t_bmFontNode;
}
