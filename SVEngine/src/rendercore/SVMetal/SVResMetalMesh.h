//
// SVRResGL.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_RESMETAL_MESH_H
#define SV_RESMETAL_MESH_H

#include "../SVResVBO.h"
#import <Metal/MTLTexture.h>
#import <Metal/MTLBuffer.h>

namespace sv {

    namespace render{

        //Buffer资源
        class SVResMetalBuf: public SVResVBO {
        public:
            SVResMetalBuf(SVInst* _app);
            
            virtual ~SVResMetalBuf();
            
            virtual void create(SVRendererPtr _renderer);
            
            virtual void destroy(SVRendererPtr _renderer);
            
        protected:
            id<MTLBuffer> m_pVerBuf;
        };
        
        //mesh
        class SVResMetalRenderMesh : public SVResMetalBuf {
        public:
            SVResMetalRenderMesh(SVInst* _app);
            
            ~SVResMetalRenderMesh();
            
            virtual void create(SVRendererPtr _renderer);
            
            virtual void destroy(SVRendererPtr _renderer);
            
            virtual void render(SVRendererPtr _renderer);
            
            virtual void updateConf(RENDERMESHCONF& _conf);
            
            virtual void updateData(RENDERMESHDATA& _data);
            
        protected:
            VFTYPE m_vftype;
            DRAWMETHOD m_drawmethod;
            u32 m_indexNum;    //索引数量
            u32 m_pointNum;    //顶点数量
            s32 m_seqMode;
            VFTYPE vftype;
            u32 vertPoolType;
            u32 indexPoolType;
            bool bVisible;
            //
            id<MTLBuffer> m_pIndexBuf;
            id<MTLBuffer> m_pV2Buf;
            id<MTLBuffer> m_pV3Buf;
            id<MTLBuffer> m_pC0Buf;
            id<MTLBuffer> m_pC1Buf;
            id<MTLBuffer> m_pT0Buf;
            id<MTLBuffer> m_pT1Buf;
            id<MTLBuffer> m_pT2Buf;
            id<MTLBuffer> m_pNorBuf;
            id<MTLBuffer> m_pTagBuf;
            id<MTLBuffer> m_pBTorBuf;
            //
            SVDataSwapPtr m_pDataVertex;
            SVDataSwapPtr m_pDataIndex;
            SVDataSwapPtr m_pDataV2;
            SVDataSwapPtr m_pDataV3;
            SVDataSwapPtr m_pDataC0;
            SVDataSwapPtr m_pDataC1;
            SVDataSwapPtr m_pDataT0;
            SVDataSwapPtr m_pDataT1;
            SVDataSwapPtr m_pDataT2;
            SVDataSwapPtr m_pDataNor;
            SVDataSwapPtr m_pDataTag;
            SVDataSwapPtr m_pDataBTor;
        };

        
    }//!namespace render

}//!namespace sv



//
#endif //SV_RRESMETAL_MESH_H
