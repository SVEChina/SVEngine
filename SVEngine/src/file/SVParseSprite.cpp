//
// SVParseSprite.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVParseSprite.h"
#include "../node/SVSpriteNode.h"
SVNodePtr
SVParseSprite::parseSprite(SVInst *_app, RAPIDJSON_NAMESPACE::Value &item, s32 _resid, cptr8 _path) {
    SVSpriteNodePtr t_spriteNode = MakeSharedPtr<SVSpriteNode>(_app);
    t_spriteNode->m_rootPath = _path;
    t_spriteNode->fromJSON(item);
    return t_spriteNode;
}

