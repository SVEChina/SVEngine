//
// SVResGLMesh.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_RESGLMESH_H
#define SV_RESGLMESH_H

#include "../SVResVBO.h"
#include "../../base/SVPreDeclare.h"
#include "../../base/SVRect.h"
#include "../../core/SVVertDef.h"
#include "../../base/SVDataChunk.h"

namespace sv {

    namespace render{

        /*GL-VBO资源*/
        class SVResGLVBO: public SVResVBO {
        public:
            SVResGLVBO(SVInst* _app);

            virtual ~SVResGLVBO();

            virtual void create(SVRendererPtr _renderer);

            virtual void destroy(SVRendererPtr _renderer);
            
            virtual void render(SVRendererPtr _renderer);
        };
        
        //mesh
        class SVResGLRenderMesh : public SVResGLVBO {
        public:
            SVResGLRenderMesh(SVInst* _app);

            ~SVResGLRenderMesh();

            virtual void create(SVRendererPtr _renderer);

            virtual void destroy(SVRendererPtr _renderer);

            virtual void render(SVRendererPtr _renderer);
            
            virtual void updateConf(RENDERMESHCONF& _conf);
            
            virtual void updateData(RENDERMESHDATA& _data);

            void setIndexPoolType(u32 itype);

            void setVertexPoolType(u32 vtype);

            void setVertexType(VFTYPE type);

            void setDrawMethod(DRAWMETHOD drawtype);

            void setVertexDataNum(s32 _vertexNum);
            
            virtual void setIndexData(SVDataSwapPtr _data,s32 _num);
            
            virtual void setVertexData(SVDataSwapPtr _data);

            virtual void setInstanceOffsetData(SVDataSwapPtr _pdata, u32 _instanceCount);
        protected:
            void _reset();
            
            virtual void _bindVerts();

            virtual void _updateVertDsp();

            virtual void _updateInstanceDsp();
            
            virtual void _unbindVerts();

            VFTYPE m_vftype;
            u32 m_vaoID;
            u32 m_vboID;
            u32 m_indexID;
            u32 m_indexNum;    //索引数量
            u32 m_pointNum;    //顶点数量
            u8 m_seqMode;       //序列模式 1 cross 2 plane
            u32 m_vertPoolType;
            u32 m_indexPoolType;
            DRAWMETHOD m_drawmethod;
            u32 instanceOffsetID;
            u32 m_instacneCount;
            bool m_useIntance;
            bool m_useVAO;
        };

        class SVResGLRenderMeshDvid : public SVResGLRenderMesh {
        public:
            SVResGLRenderMeshDvid(SVInst* _app);

            ~SVResGLRenderMeshDvid();

            virtual void create(SVRendererPtr _renderer);

            virtual void destroy(SVRendererPtr _renderer);

            virtual void render(SVRendererPtr _renderer);
            
            virtual void updateData(RENDERMESHDATA& _data);

            virtual void updateConf(RENDERMESHCONF& _conf);
            
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
            void _reset();
        
            void _updateVertDsp();

        private:
            u32 vertex2ID_0;
            u32 vertex2ID_1;
            u32 vertex2ID_2;
            u32 vertex3ID;
            u32 color0ID;
            u32 color1ID;
            u32 texcoord0ID;
            u32 texcoord1ID;
            u32 texcoord2ID;
            u32 normalID;
            u32 tagentID;
            u32 btagentID;
        };
       
    }//!namespace render

}//!namespace sv



//
#endif //SV_RESGLMESH_H
