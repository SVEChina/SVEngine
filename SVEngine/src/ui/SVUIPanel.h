//
// SVSpriteNode.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_UIPANEL_H
#define SV_UIPANEL_H

#include "SVUIBase.h"

namespace sv {
    
    namespace node{
        /*
         ui 板子
         */
        class SVUIPanel: public SVUIBase {
        public:
            SVUIPanel(SVInst *_app);
            
            SVUIPanel(SVInst *_app,f32 _w,f32 _h);
            
            ~SVUIPanel();
            
            void update(f32 _dt);
            
            void render();
            
            void setSize(f32 _w,f32 _h);
            
            void getSize(FVec2 &_size);
            
            void setTexture(cptr8 _path, bool enableMipMap = false);
            
            void setColor(f32 _r, f32 _g, f32 _b, f32 _a); //0~1
            
            virtual void setArcho(EUIARCHO _cho);
            
        protected:
            f32 m_width;
            f32 m_height;
            bool m_dirty_mesh;
            SVColor m_color;
            SVMtlColorPtr m_colorMtl;
            //运行时属性
            SVTexturePtr m_pTex;
            SVRenderMeshPtr m_pMesh;
            SVRenderObjectPtr m_pRenderObj;
        };
        
//        //锚点
//        enum EUIARCHO {
//            E_ARCHO_LT = 0,
//            E_ARCHO_CT,
//            E_ARCHO_RT,
//            E_ARCHO_LC,
//            E_ARCHO_CC,
//            E_ARCHO_RC,
//            E_ARCHO_LB,
//            E_ARCHO_CB,
//            E_ARCHO_RB
//        };

    }//!namespace node
    
}//!namespace sv


#endif //SV_UIPANEL_H
