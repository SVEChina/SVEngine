//
// SVRResGL.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_RRESMETAL_H
#define SV_RRESMETAL_H

#include "../SVRObjBase.h"
#include "../SVResTex.h"
#include "../SVResShader.h"
#import <Metal/MTLTexture.h>
#import <Metal/MTLBuffer.h>

namespace sv {

    namespace render{

        //Buffer资源
        class SVRResMetalBuf: public SVRObjBase {
        public:
            SVRResMetalBuf(SVInst* _app);

            virtual ~SVRResMetalBuf();

            virtual void create(SVRendererBasePtr _renderer);

            virtual void destroy(SVRendererBasePtr _renderer);
        
        protected:
            id<MTLBuffer> m_pVerBuf;
        };
        
        //mesh
        class SVResMetalRenderMesh : public SVRResMetalBuf {
//        public:
//            static int getVertexFormateSize(VFTYPE _type);
        public:
            SVResMetalRenderMesh(SVInst* _app);
            
            ~SVResMetalRenderMesh();
            
            virtual void create(SVRendererBasePtr _renderer);
            
            virtual void destroy(SVRendererBasePtr _renderer);
            
            virtual void render();
        };

       
        //纹理资源
        class SVRResMetalTex: public SVResTex {
        public:
            SVRResMetalTex(SVInst* _app);

            virtual ~SVRResMetalTex();

            virtual void create(SVRendererBasePtr _renderer);

            virtual void destroy(SVRendererBasePtr _renderer);

            virtual void setTexData(void *_data, s32 _len);

            virtual void commit();  //数据提交到显卡

            virtual bool getbLoad();

            u32  getTexID();

            u32  getuid();
            
        protected:
            id<MTLTexture> m_srcTex;
        };
        
        //Shader资源
        class SVRResMetalShader: public SVResShader {
        public:
            SVRResMetalShader(SVInst* _app);
            
            virtual ~SVRResMetalShader();
        };
        
    }//!namespace render

}//!namespace sv



//
#endif //SV_RRESMETAL_H
