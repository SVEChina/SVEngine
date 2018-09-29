//
// SVParseMask.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVParseMask.h"
#include "SVDataNode.h"
//解析面具
SVNodePtr
SVParseMask::parseMask(SVInst *_app, RAPIDJSON_NAMESPACE::Value &item, s32 _resid, cptr8 _path) {
    SVDataNodeMaskPtr t_maskModel = MakeSharedPtr<SVDataNodeMask>(_app);
    t_maskModel->fromJSON(item);
    t_maskModel->m_root_path = _path;
    return t_maskModel->toNode();
}


