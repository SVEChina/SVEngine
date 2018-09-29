//
// SVParseBitfont.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVParseBitfont.h"
#include "SVDataNode.h"
//解析面具
SVNodePtr
SVParseBitfont::parseBitfont(SVInst *_app, RAPIDJSON_NAMESPACE::Value &item, s32 _resid, cptr8 _path) {
    SVDataNodeBmpFontPtr t_bitfonData = MakeSharedPtr<SVDataNodeBmpFont>(_app);
    t_bitfonData->fromJSON(item);
    t_bitfonData->m_root_path = _path;
    return t_bitfonData->toNode();
}
