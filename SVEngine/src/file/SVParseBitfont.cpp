//
// SVParseBitfont.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVParseBitfont.h"
#include "../node/SVBitFontNode.h"
//解析面具
SVNodePtr
SVParseBitfont::parseBitfont(SVInst *_app, RAPIDJSON_NAMESPACE::Value &item, s32 _resid, cptr8 _path) {
    SVBitFontNodePtr t_bitFontNode = MakeSharedPtr<SVBitFontNode>(_app);
    t_bitFontNode->m_rootPath = _path;
    t_bitFontNode->fromJSON(item);
    return t_bitFontNode;
}
