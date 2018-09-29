//
// SVParseSprite.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVParseSprite.h"
#include "SVDataNode.h"
SVNodePtr
SVParseSprite::parseSprite(SVInst *_app, RAPIDJSON_NAMESPACE::Value &item, s32 _resid, cptr8 _path) {
    SVDataNodeSpritePtr t_spriteData = MakeSharedPtr<SVDataNodeSprite>(_app);
    t_spriteData->fromJSON(item);
    t_spriteData->m_root_path = _path;
    return t_spriteData->toNode();
}

