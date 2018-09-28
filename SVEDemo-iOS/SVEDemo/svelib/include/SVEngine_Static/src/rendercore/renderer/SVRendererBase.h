//
// SVRendererBase.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_RENDERERBASE_H
#define SV_RENDERERBASE_H

#include "SVRenderDef.h"
#include "../../base/SVGBase.h"
#include "../../base/SVArray.h"
#include "../mtl/SVMtlDeclare.h"

namespace sv {
    
    namespace render{
        
        //渲染器
        
        class SVRendererBase: public SVGBase {
        public:
            SVRendererBase(SVInst* _app);
            
            ~SVRendererBase();
            
            virtual void init(s32 _ver,void* _windows,void* context,s32 _w,s32 _h);
            
            virtual void destroy();
            
            virtual void resize(s32 _w,s32 _);
            
            void clearRes();
            
            void addRes(SVRObjBasePtr _res);
            
            void removeRes(SVRObjBasePtr _res);
            
            virtual void removeUnuseRes();  //移除不使用的资源
            
            void setRendererContext(SVContextBasePtr _context);
            
            SVContextBasePtr getRenderContext();
            
            SVRenderTexturePtr getRenderTexture();
            
            SVTexturePtr getSVTex(SVTEXTYPE _type);
            
            bool hasSVTex(SVTEXTYPE _type);
            
            SVTexturePtr createSVTex(SVTEXTYPE _type,s32 _w,s32 _h,s32 _formate);
            
            void destroySVTex(SVTEXTYPE _type);
            
        protected:
            //渲染上下文
            SVContextBasePtr m_pRenderContext;
            //主FBO
            SVRenderTexturePtr m_pRenderTex;
            //
            SVTexturePtr m_svTex[E_TEX_END];
            //
            typedef SVArray<SVRObjBasePtr> ROBJLIST;
            ROBJLIST m_robjList;
            //
            SVLockPtr m_resLock;
        };
    
    }//!namespace render
    
}//!namespace sv





//
#endif //SV_RENDERERBASE_H
