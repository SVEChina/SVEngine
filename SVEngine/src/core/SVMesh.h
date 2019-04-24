//
// SVMesh.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MESH_H
#define SV_MESH_H

#include "../base/SVObject.h"
#include "../base/SVPreDeclare.h"
#include "../base/SVDataChunk.h"
#include "SVVertDef.h"

namespace sv {
    
    namespace util {
        
        class SVMesh :public SVObject{
        public:
            SVMesh();
            
            virtual ~SVMesh();
            
            void setName(cptr8 _name);
            
            cptr8 getName();
            
            void addMesh(SVMeshPtr _mesh);
            
            void removeMesh(cptr8 _name);
            
            void clearMesh();
            
            void setData(SVDataSwapPtr _data,VFTYPE _vtf);
            
        protected:
            SVString m_name;
            //顶点数据
            SVDataSwapPtr m_pDataSwap;
            //顶点类型
            VFTYPE m_vertType;
            //
            typedef SVArray<SVMeshPtr> MESHPOOL;
            MESHPOOL m_meshPool;
        };
        
    }//!namespace util
    
}//!namespace sv

#endif //SV_MESH_H
