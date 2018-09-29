//
// SVRenderCmdOut.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_RENDERCMD_OUT_H
#define SV_RENDERCMD_OUT_H

#include "SVRenderCmd.h"
#include "SVRenderMesh.h"
#include "../base/SVObject.h"
#include "../base/SVPreDeclare.h"

namespace sv {
    
    namespace render{
        
        //普通的流输出
        class SVRenderCmdStreamOutNor : public SVRenderCmdNor {
        public:
            SVRenderCmdStreamOutNor(SVInst *_app);
            
            ~SVRenderCmdStreamOutNor();
            
            virtual void render();
            
            void setParam(SVRenderTexturePtr _fbo,SVTexturePtr _tex,SVDataSwapPtr _dataswap,cb_out_stream _cb);

        protected:
            
            virtual void _fectchdata();
            
            SVRenderTexturePtr m_fbo;
            
            SVTexturePtr m_tex;
            
            SVDataSwapPtr m_dataswap;
            
            cb_out_stream m_pCB;

            SVInst *mApp;
        };
        
        //读取显存数据到NodeDataSwap
        class SVRenderCmdStreamOutIOS : public SVRenderCmdStreamOutNor {
        public:
            SVRenderCmdStreamOutIOS(SVInst *_app);
            
            ~SVRenderCmdStreamOutIOS();
            
            virtual void _fectchdata();
        };
        

    }//!namespace render
    
}//!namespace sv




//
#endif //SV_RENDERCMD_OUT_H
