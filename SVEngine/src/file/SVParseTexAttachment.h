//
// SVParseTexAttachment.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PARSETEXATTACHMENT_H
#define SV_PARSETEXATTACHMENT_H

#include "SVParseData.h"
#include "../act/SVActDeclare.h"
#include "../act/SVAniTexAttachment.h"
namespace sv {
    
    namespace logic{
        
        class SVParseTexAttachment : public SVParseData {
        public:
            static SVAniTexAttachmentPtr parseTexAttachmet(SVInst *_app, RAPIDJSON_NAMESPACE::Value &item, s32 _resid,cptr8 _path);
        };

        
    }//!namespace logic
    
}//!namespace sv
#endif //SV_PARSETEXATTACHMENT_H
