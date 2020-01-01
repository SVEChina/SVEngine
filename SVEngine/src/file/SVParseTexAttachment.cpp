//
// SVParseTexAttachment.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVParseTexAttachment.h"
#include "../act/SVAniTexAttachment.h"
SVAniTexAttachmentPtr SVParseTexAttachment::parseTexAttachmet(SVInst *_app, RAPIDJSON_NAMESPACE::Value &item, s32 _resid, cptr8 _path) {
    SVAniTexAttachmentPtr attachment = MakeSharedPtr<SVAniTexAttachment>(_app);
    attachment->init();
    attachment->m_rootPath = _path;
    attachment->fromJson(item);
    return attachment;
}
