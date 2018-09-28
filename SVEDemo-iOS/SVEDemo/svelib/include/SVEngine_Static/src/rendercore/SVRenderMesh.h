//
// SVRenderMesh.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_RENDERMESH_H
#define SV_RENDERMESH_H

#include "SVRObjBase.h"
#include "SVRenderDef.h"
#include "../mtl/SVShaderMgr.h"
#include "../core/SVVertDef.h"
#include "../base/SVPreDeclare.h"

namespace sv {
    
    namespace render{
        
        class SVRenderMesh : public SVRObjBase {
            struct RENDERMESHDATA{
                u32 indexNum;    //索引数量
                u32 pointNum;    //顶点数量
                SVDataSwapPtr pDataIndex;
                SVDataSwapPtr pDataVertex;
                bool dirty;
            };
            struct RENDERMESHCONF{
                VFTYPE vftype;
                u32 vertPoolType;
                u32 indexPoolType;
                DRAWMETHOD drawmethod;
                bool bVisible;
                bool dirty;
            };
        public:
            static int getVertexFormateSize(VFTYPE _type);
            
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
        
            virtual void render();
            
            void setIndexPoolType(u32 itype);
            
            void setVertexPoolType(u32 vtype);
            
            void setVertexType(VFTYPE type);
            
            void setDrawMethod(DRAWMETHOD drawtype);
            
            void setvisible(bool bVis);
            
            void pushData();
            
            virtual void setIndexData(SVDataSwapPtr _data,s32 _num);
            
            virtual void setVertexDataNum(s32 _vertexNum);
            
            virtual void setVertexData(SVDataSwapPtr _data);
        protected:
            void _resetMeshConf();
            
            void _updateConf();
            
            virtual void _resetMeshData();
            
            virtual void _updateData();
            
            SVRResGLVBOPtr m_objVBOPtr;
            
            RENDERMESHCONF m_renderMeshConf;
            
            MESHSTATE m_meshState;
        private:
            
            RENDERMESHDATA m_renderMeshData;
            
        };
        
        
        class SVRenderMeshDvid : public SVRenderMesh {
            struct RENDERMESHDVIDDATA{
                u32 indexNum;    //索引数量
                u32 pointNum;    //顶点数量
                SVDataSwapPtr pDataIndex;
                SVDataSwapPtr pDataV2;
                SVDataSwapPtr pDataV3;
                SVDataSwapPtr pDataC0;
                SVDataSwapPtr pDataC1;
                SVDataSwapPtr pDataT0;
                SVDataSwapPtr pDataT1;
                SVDataSwapPtr pDataT2;
                SVDataSwapPtr pDataNor;
                SVDataSwapPtr pDataTag;
                SVDataSwapPtr pDataBTor;
                bool          dirty;
            };
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
            
            virtual void render();
        protected:
            virtual void _resetMeshData();
            
            virtual void _updateData();
        private:
            RENDERMESHDVIDDATA m_renderMeshDvidData;
        };
        
    }//!namespace render
    
}//!namespace sv


#endif //SV_RENDERMESH_H
