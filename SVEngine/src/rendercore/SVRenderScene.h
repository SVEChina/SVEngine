//
// SVRenderScene.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_RENDERSCENE_H
#define SV_RENDERSCENE_H

#include "SVRenderTexture.h"
#include "SVRenderStream.h"
#include "../base/SVStack.h"

namespace sv {
    
    namespace render{

        class SVRenderScene : public SVGBase {
        public:
            SVRenderScene(SVInst *_app);
            
            ~SVRenderScene();
            
            cptr8 getName();
            
            void swapPipline();
            
            void render();
            
            void pushRenderCmd(RENDERSTREAMTYPE _rst, SVRenderCmdPtr _rcmd);
            
            void clearRenderCmd();
            
            void pushCacheCmd(RENDERSTREAMTYPE _rst, SVRenderCmdPtr _rcmd);
            
            void clearCacheCmd();
            
            bool isSuspend();
            
            SVRenderPiplinePtr getPiplineRead() { return m_readPipline; }
            
            SVRenderPiplinePtr getPiplineWrite(){ return m_writePipline; }
            
            SVRenderPiplinePtr getPiplineCache(){ return m_cachePipline; }
            
        protected:
            SVString m_name;
            
            bool m_suspend; //是否挂起

            SVRenderPiplinePtr m_writePipline; //写管线
            
            SVRenderPiplinePtr m_readPipline;  //读管线
            
            SVRenderPiplinePtr m_cachePipline;  //缓存管线
        };
        
    }//!namespace render
    
}//!namespace sv



//
#endif //SV_RENDERSCENE_H
