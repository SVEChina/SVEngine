//
// SVAniTexAttachment.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_ACT_TEXATTACHMENT_H
#define SV_ACT_TEXATTACHMENT_H

#include "SVAniBase.h"
#include "../node/SVNodeDeclare.h"
#include "../mtl/SVMtlDeclare.h"
namespace sv{
    
    namespace logic {
        
        class SVAniTexAttachment : public SVAniBase {
        public:
            struct TEXINFOPARAM{
                SVDataSwapPtr   dataSwap;
                s32             width;
                s32             height;
            };
            struct TEXATTACHSPARAM{
                s32             channel;
                SVString        sourceNodeName;
                SVString        targetNodeName;
                SVString        boneName;
                SVString        slotName;
                SVString        matadata;
            };
            SVAniTexAttachment(SVInst* _app);
            
            ~SVAniTexAttachment();
            
            virtual void init();
            
            virtual void destroy();
            
            virtual void enter();
            
            virtual void exit();
            
            virtual bool isEnd();
            
            virtual void update(f32 _dt);

            void setAttachmentTex(void *_data, s32 _width, s32 _height);
            
            void setParam(TEXATTACHSPARAM &_param);
            
            void getParam(TEXATTACHSPARAM &_param);
            
            void setChannel(s32 _channel);
            
            void setSrcNodeName(cptr8 _name);
            
            void setTarNodeName(cptr8 _name);
            
            void setBoneName(cptr8 _name);
            
            void setSlotName(cptr8 _name);
            
            void setMataData(cptr8 _mata);
            
            s32  getChannel();
            
            cptr8 getSrcNodeName();
            
            cptr8 getTarNodeName();
            
            cptr8 getBoneName();
            
            cptr8 getSlotName();
            
            cptr8 getMataData();
            
            void fromJson(RAPIDJSON_NAMESPACE::Value &item);
            
            TEXATTACHSPARAM getParam();
            
            SVString m_rootPath;
        protected:
            void _genTexture();
            void _findAttachmentNode();
            void _refreshAttachmentNode();
        protected:
            SVSpriteNodePtr m_srcNode;
            SVSpineNodePtr  m_tarNode;
            SVTexturePtr    m_texture;
            TEXINFOPARAM    m_texInfo;
            TEXATTACHSPARAM m_param;
            bool            m_dirty;
        };
        
    }//!namespace logic
}//!namespace sv
#endif //SV_ACT_TEXATTACHMENT_H
