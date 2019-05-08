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
#include "../base/SVDataChunk.h"
#include "../base/SVMap.h"
#include "../rendercore/SVRenderDeclare.h"
#include "SVVertDef.h"

namespace sv {
    
    namespace util {
        
        class SVMesh :public SVGBase{
        public:
            SVMesh(SVInst* _app);
            
            virtual ~SVMesh();
            
            void setName(cptr8 _name);
            
            cptr8 getName();
            
            void addMesh(SVMeshPtr _mesh);
            
            void removeMesh(cptr8 _name);
            
            void clearMesh();
            
            void setData(SVDataSwapPtr _data,VFTYPE _vtf,s32 _count,s32 _seqMode);
            
            void render();
            
        protected:
            SVString m_name;
            SVDataSwapPtr m_pDataSwap;  //顶点数据
            VFTYPE m_vertType;          //顶点类型
            s32 m_verCount;
            s32 m_seqMode; //1.代表cross模式 2.代表plane模式
            //子mesh
            typedef SVArray<SVMeshPtr> MESHPOOL;
            MESHPOOL m_meshPool;
            //
            SVLockPtr m_lock;
            SVRenderMeshPtr m_pRenderMesh;
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
