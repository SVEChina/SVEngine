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
#include "../base/SVMap.h"
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
            
            void setData(SVDataSwapPtr _data,VFTYPE _vtf,f32 _weight = 0.0f);
            
        protected:
            SVString m_name;
            struct MeshData {
                SVDataSwapPtr m_pDataSwap;  //顶点数据
                VFTYPE m_vertType;          //顶点类型
                s32 m_mode;     //1.代表cross模式 2.代表plane模式
            };
            //数据
            typedef SVMap<f32,MeshData> DATAMAP;
            DATAMAP m_dataMap;
            //子mesh
            typedef SVArray<SVMeshPtr> MESHPOOL;
            MESHPOOL m_meshPool;
        };
        
    }//!namespace util
    
}//!namespace sv

#endif //SV_MESH_H
