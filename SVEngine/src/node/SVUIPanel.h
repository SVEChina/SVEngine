//
// SVUIPanel.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//
#ifndef SV_UIPanel_H
#define SV_UIPanel_H

#include "SVNode.h"

namespace sv {
    
    namespace node{
        
        class SVUIPanel : public SVNode {
        public:
            SVUIPanel(SVInst *_app);
            
            ~SVUIPanel();
            
            void setSize(f32 _w,f32 _h);
            
            void update(f32 dt);
            
            void render();
            
            void setArcho(EUIARCHO _archo);
            
            EUIARCHO getArcho();
            
        protected:
            void _computeRect();
            
            f32 m_width;
            f32 m_height;
            
            EUIARCHO m_archo;
            
            bool m_dirty;
            SVRenderMeshPtr m_pMesh;
            SVRenderObjectPtr m_pRenderObj;
        };

    }//!namespace node

}//!namespace sv



#endif //SV_MULTPASSNODE_H
