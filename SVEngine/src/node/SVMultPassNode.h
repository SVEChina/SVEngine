//
// SVMultPassNode.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//
#ifndef SV_MULTPASSNODE_H
#define SV_MULTPASSNODE_H

#include "SVNode.h"

namespace sv {
    
    namespace node{
        
        class SVMultPassNode : public SVNode {
        public:
            SVMultPassNode(SVInst *_app);
            
            ~SVMultPassNode();
            
            void create(s32 _w,s32 _h);
            
            void destroy();
            
            void update(f32 dt);
            
            void render();
        
            void addPass(SVPassPtr _pass);
            
            void clearPass();
            
        protected:
            typedef SVArray<SVPassPtr> PASSPOOL;
            PASSPOOL m_passPool;
            //
            SVRenderTexturePtr m_fbo;
        };

    }//!namespace node

}//!namespace sv



#endif //SV_MULTPASSNODE_H
