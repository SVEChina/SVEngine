//
// SVFboObject.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_FBOOBJECT_H
#define SV_FBOOBJECT_H

#include "SVRObjBase.h"
#include "SVResFBO.h"
#include "SVRenderDef.h"
#include "../base/SVColor.h"
#include "../base/SVMat4.h"

namespace sv {
    
    namespace render{
        
        class SVFboObject : public SVRObjBase {
        public:
            SVFboObject(SVInst *_app);
            
            virtual ~SVFboObject();
            
            virtual void create(SVRendererPtr _renderer);
            
            virtual void destroy(SVRendererPtr _renderer);
            
            virtual void refresh();
            
            virtual void bind();
            
            virtual void clear();
            
            virtual void unbind();
            
            u32 getFboID();
            
            u32 getWidth();
            
            u32 getHeight();
            
            bool hasDepth();
            
            bool hasStencil();
            
            void setProjMat(FMat4 _mat);
            
            void setViewMat(FMat4 _view);
            
            void setLink(bool _link);
            
        protected:
            FMat4 m_mat_proj;
            FMat4 m_mat_view;
            FMat4 m_mat_vp;
            bool m_link;
            SVResFBOPtr m_objFBOPtr;
        };
        
        //外部传递的一个fbo
        class SVOutFboObject : public SVFboObject {
        public:
            SVOutFboObject(SVInst *_app,u32 _fboid);
            
            ~SVOutFboObject();
            
            virtual void create(SVRendererPtr _renderer);
            
            virtual void destroy(SVRendererPtr _renderer);
        private:
            u32 m_fboID;
        };

    }//!namespace render
    
}//!namespace sv



#endif //SV_FBOOBJECT_H
