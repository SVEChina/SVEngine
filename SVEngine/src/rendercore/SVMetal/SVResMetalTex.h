//
// SVRResMetalTex.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_RESMETAL_TEX_H
#define SV_RESMETAL_TEX_H

#include "../SVResTex.h"
#include "../SVResShader.h"
#import <Metal/MTLTexture.h>
#import <Metal/MTLBuffer.h>
#import <Metal/MTLLibrary.h>
#import <Metal/MTLRenderPipeline.h>

namespace sv {

    namespace render{


        /*
        纹理资源
         */
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
        
        /*
         Shader资源
         */
        class SVRResMetalShader: public SVResShader {
        public:
            SVRResMetalShader(SVInst* _app);
            
            virtual ~SVRResMetalShader();
            
            virtual void create(SVRendererBasePtr _renderer);
            
            virtual void destroy(SVRendererBasePtr _renderer);
            
            virtual bool active(SVRendererBasePtr _render);
            
        protected:
            id<MTLFunction> m_pVS;
            id<MTLFunction> m_pFS;
            id<MTLRenderPipelineState> m_pProgram;
        };
        
    }//!namespace render

}//!namespace sv



//
#endif //SV_RRESMETAL_TEX_H
