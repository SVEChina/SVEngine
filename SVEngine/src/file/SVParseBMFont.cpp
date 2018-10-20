//
// SVParseBMFont.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVParseBMFont.h"
#include "SVDataNode.h"

SVNodePtr
SVParseBMFont::parseBMFont(SVInst *_app, RAPIDJSON_NAMESPACE::Value &item, s32 _resid, cptr8 _path) {
    SVDataNodeBMFontPtr t_bmfonData = MakeSharedPtr<SVDataNodeBMFont>(_app);
    t_bmfonData->fromJSON(item);
    t_bmfonData->m_root_path = _path;
    return t_bmfonData->toNode();
}
