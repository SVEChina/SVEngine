//
// SVTexAttachment.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_ACT_TEXATTACHMENT_H
#define SV_ACT_TEXATTACHMENT_H

#include "../base/SVGBase.h"
#include "../node/SVNodeDeclare.h"
namespace sv{
    
    namespace logic {
        
        class SVTexAttachment : public SVGBase {
        public:
            struct TEXATTACHSPARAM{
                s32             channel;
                SVString        sourceNodeName;
                SVString        targetNodeName;
                SVString        boneName;
                SVString        slotName;
                SVString        metadata;
            };
            SVTexAttachment(SVInst* _app);
            
            ~SVTexAttachment();
            
            virtual void init();
            
            virtual void destroy();
            
            virtual void enter();
            
            virtual void exit();
            
            virtual bool isEnd();
            
            virtual void update(f32 _dt);

            void setAttachmentTex(void *_data, s32 _width, s32 _height);
            
            void fromJson(RAPIDJSON_NAMESPACE::Value &item);
            
            TEXATTACHSPARAM getParam();
            
            SVString m_rootPath;
        protected:
            SVSpriteNodePtr m_srcNode;
            SVSpineNodePtr  m_tarNode;
            TEXATTACHSPARAM m_param;
        };
        
    }//!namespace logic
}//!namespace sv
#endif //SV_ACT_TEXATTACHMENT_H
