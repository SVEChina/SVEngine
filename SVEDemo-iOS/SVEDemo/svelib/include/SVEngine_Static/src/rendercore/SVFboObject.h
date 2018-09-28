//
// SVFboObject.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_FBOOBJECT_H
#define SV_FBOOBJECT_H

#include "SVRenderDef.h"
#include "../base/SVColor.h"
#include "SVRObjBase.h"
#include "SVResFBO.h"

namespace sv {
    
    namespace render{
        
        class SVFboObject : public SVRObjBase {
        public:
            SVFboObject(SVInst *_app);
            
            virtual ~SVFboObject();
            
            virtual void create(SVRendererBasePtr _renderer);
            
            virtual void destroy(SVRendererBasePtr _renderer);
            
            virtual void refresh();
            
            virtual void bind();
            
            virtual void clear();
            
            virtual void unbind();
            
            u32 getFboID();
            
            u32 getWidth();
            
            u32 getHeight();
            
            bool hasDepth();
            
            bool hasStencil();
        protected:
            SVResFBOPtr m_objFBOPtr;
            
        };
        
        //外部传递的一个fbo
        class SVOutFboObject : public SVFboObject {
        public:
            SVOutFboObject(SVInst *_app,u32 _fboid);
            
            ~SVOutFboObject();
            
            virtual void create(SVRendererBasePtr _renderer);
            
            virtual void destroy(SVRendererBasePtr _renderer);
        private:
            u32 m_fboID;
        };

    }//!namespace render
    
}//!namespace sv



#endif //SV_FBOOBJECT_H
