//
// SVFrameOutNode.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_FRAMEOUTNODE_H
#define SV_FRAMEOUTNODE_H

#include "SVNode.h"
#include "../rendercore/SVRenderDef.h"
#include "../base/SVPreDeclare.h"

//数据输出节点
//所谓的数据输出 其实就是 fbo

namespace sv {
    
    namespace node{
        
        class SVFrameOutNode : public SVNode {
        public:
            SVFrameOutNode(SVInst *_app);
            
            ~SVFrameOutNode();
            
            void update(f32 _dt);
            
            void render();
            
            virtual void lockData();
            
            virtual void unlockData();
            
            virtual void* getData();
            
        protected:
            void* m_pData;
        };
        
    }//!namespace node
    
}//!namespace sv



#endif //SV_FRAMEOUTNODE_H
