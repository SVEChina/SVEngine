//
// SV2DFaceMaskNode.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_2DFACEMASK_NODE_H
#define SV_2DFACEMASK_NODE_H

#include "SVNode.h"
#include "../base/SVPreDeclare.h"

#define MAX_FACEPOINTS_NUM 800

namespace sv {
    
    namespace node{
    
        class SV2DFaceMaskNode : public SVNode {
        public :
            SV2DFaceMaskNode(SVInst *_app);
            
            ~SV2DFaceMaskNode();
            
            void update(f32 dt);
            
            void render();
            
            void setMaterial(SVMtlFace2DPtr _mtlFace2D);
            
            SVMtlFace2DPtr getMaterial();
            
        protected:
            SVRenderObjectPtr m_renderObject;
            SVRenderMeshDvidPtr m_pFaceMesh;
            SVDataSwapPtr m_pIndexs;
            SVDataSwapPtr m_pVerts;
            SVDataSwapPtr m_pT0;
            SVMtlFace2DPtr m_mtlFace2D;
            f32 m_faceDateExt[MAX_FACEPOINTS_NUM];
            f32 m_standervers[MAX_FACEPOINTS_NUM];
            f32 m_standertexcoord[MAX_FACEPOINTS_NUM];
            u16 m_newmesh[600*3];
            u32 m_pointsize;
            u32 m_meshsize;
            f32 m_texwidth;
            f32 m_texheight;
        };
        
    }//!namespace node
    
}//!namespace sv




#endif //SV_2DFACEMASK_NODE_H
