//
// SV3DBox.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_3DBOX_H
#define SV_3DBOX_H

#include "SVNode.h"
#include "../base/SVBounds.h"

namespace sv {
    
    namespace node{
        
        class SV3DBox : public SVNode {
        public:
            SV3DBox(SVInst *_app);
            
            virtual ~SV3DBox();
            
            virtual void update(f32 dt);
            
            virtual void render();
            
            void randomInit();
            
            SVMtlCorePtr getMtl();
            
            SVMtlGeo3dPtr m_mtl_box;
        public:
            SVRenderObjectPtr m_pRenderObj;
            
            SVRenderMeshPtr m_pMesh;
            
            SVColor m_color;
        };
        
    }//!namespace node
    
}//!namespace sv



#endif //SV_3DBOX_H
