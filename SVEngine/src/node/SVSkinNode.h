//
// SVSkinNode.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_SKINNODE_H
#define SV_SKINNODE_H

#include "SVNode.h"
#include "../base/SVMap.h"


namespace sv {
    
    namespace node{
        
        //骨骼动画节点3d
        
        class SVSkinNode : public SVNode {
        public:
            SVSkinNode(SVInst *_app);
            
            ~SVSkinNode();
            
            //更新
            virtual void update(f32 dt);

            //渲染
            virtual void render();
            
        protected:
            //动画数据
            //SVSkinAniPtr m_pSkinAni;
            
            //模型 动态数据model
            
            
        };
        
    }//!namespace node
    
}//!namespace sv

//
#endif //SV_SKINNODE_H


