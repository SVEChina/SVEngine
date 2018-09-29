//
// SVParseAnimate.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVParseAnimate.h"
#include "SVDataNode.h"
SVNodePtr
SVParseAnimate::parseAnim(SVInst *_app, RAPIDJSON_NAMESPACE::Value &item, s32 _resid, cptr8 _path) {
    
    SVDataNodeSpinePtr t_spineData = MakeSharedPtr<SVDataNodeSpine>(_app);
    t_spineData->fromJSON(item);
    t_spineData->m_root_path = _path;
    return t_spineData->toNode();
}


