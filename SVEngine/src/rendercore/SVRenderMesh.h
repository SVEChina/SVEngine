//
// SVRenderMesh.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_RENDERMESH_H
#define SV_RENDERMESH_H

#include "SVResVBO.h"
#include "SVRenderDef.h"
#include "../mtl/SVShaderMgr.h"
#include "../base/SVPreDeclare.h"

namespace sv {
    
    namespace render{
                
        class SVRenderMesh : public SVRObjBase {
        public:
            enum MESHSTATE{
                MESHDATA_WAIT = 0,
                MESHDATA_READY,
                MESHDATA_CREATE
            };
            
            SVRenderMesh(SVInst* _app);
            
            ~SVRenderMesh();
            
            virtual void create(SVRendererBasePtr _renderer);
            
            virtual void destroy(SVRendererBasePtr _renderer);
        
            virtual void render(SVRendererBasePtr _renderer);
            
            void setIndexPoolType(u32 itype);
            
            void setVertexPoolType(u32 vtype);
            
            void setVertexType(VFTYPE type);
            
            void setDrawMethod(DRAWMETHOD drawtype);
            
            void setvisible(bool bVis);
            
            virtual void setIndexData(SVDataSwapPtr _data,s32 _num);
            
            virtual void setVertexDataNum(s32 _vertexNum);
            
            virtual void setVertexData(SVDataSwapPtr _data);
            
            void setInstanceOffsetData(SVDataSwapPtr _pdata, u32 _instanceCount);
            
            void createMesh();
            
        protected:
            virtual void _resetMeshConf();
        
            virtual void _resetMeshData();
            
            virtual void _updateConf();
            
            virtual void _updateData();
            
            SVResVBOPtr m_objVBOPtr;
            
            bool  m_created;
            
            RENDERMESHCONF m_renderMeshConf;
            
            RENDERMESHDATA m_renderMeshData;
        };
        
        //
        class SVRenderMeshDvid : public SVRenderMesh {
        public:
            SVRenderMeshDvid(SVInst* _app);
            
            ~SVRenderMeshDvid();
            
            virtual void create(SVRendererBasePtr _renderer);
            
            virtual void destroy(SVRendererBasePtr _renderer);
            
            virtual void setVertexDataNum(s32 _vertexNum);
            
            virtual void setIndexData(SVDataSwapPtr _data,s32 _num);
            
            void setVertex2Data(SVDataSwapPtr _pdata);
            
            void setVertex3Data(SVDataSwapPtr _pdata);
            
            void setColor0Data(SVDataSwapPtr _pdata);
            
            void setColor1Data(SVDataSwapPtr _pdata);
            
            void setTexcoord0Data(SVDataSwapPtr _pdata);
            
            void setTexcoord1Data(SVDataSwapPtr _pdata);
            
            void setTexcoord2Data(SVDataSwapPtr _pdata);
            
            void setNormalData(SVDataSwapPtr _pdata);
            
            void setTagentData(SVDataSwapPtr _pdata);
            
            void setBTagentData(SVDataSwapPtr _pdata);
        protected:
            virtual void _resetMeshData();
        };
        
    }//!namespace render
    
}//!namespace sv


#endif //SV_RENDERMESH_H
