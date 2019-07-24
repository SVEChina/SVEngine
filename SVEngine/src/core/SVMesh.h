//
// SVMesh.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MESH_H
#define SV_MESH_H

#include "../base/SVGBase.h"
#include "../base/SVPreDeclare.h"
#include "../mtl/SVMtlDeclare.h"
#include "../base/SVDataChunk.h"
#include "../base/SVMap.h"
#include "../rendercore/SVRenderDeclare.h"
#include "SVVertDef.h"

namespace sv {
    
    namespace util {
        
        //普通mesh
        
        class SVMesh :public SVGBase{
        public:
            SVMesh(SVInst* _app);
            
            virtual ~SVMesh();
            
            void setName(cptr8 _name);
            
            cptr8 getName();
            
            void setData(SVDataSwapPtr _data,VFTYPE _vtf,s32 _count,s32 _seqMode);
            
            void setRenderMesh(SVRenderMeshPtr _rmesh);
            
            void setMtl(SVMtlCorePtr _mtl);
            
            void update(f32 _dt);
            
            void render();
            
        protected:
            SVString m_name;
            SVLockPtr m_lock;
            SVRenderMeshPtr m_pRenderMesh;
            SVMtlCorePtr m_pMtl;
        };
        
        //morph动画的mesh
        class SVMorphMesh :public SVMesh{
        public:
            SVMorphMesh(SVInst* _app);
            
            virtual ~SVMorphMesh();
        };
        
    }//!namespace util
    
}//!namespace sv

#endif //SV_MESH_H
