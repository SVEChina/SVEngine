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
            
            SVModelPtr getModel();
            
            void setModel(SVModelPtr _model);
            
            void clearModel();
            
            //
            void setSke(SVSkeletonPtr _ske);
            
            void clearSke();
            
            //动画操作
            void addAni(SVAnimateSkinPtr _ani);
            
            void delAni(cptr8 _name);
            
            void clearAni();
            

        protected:
            SVModelPtr m_pModel;
            
            SVSkeletonPtr m_pSke;
            
            typedef SVMap<SVString,SVAnimateSkinPtr> ANIPOOL;
            ANIPOOL m_aniPool;
        };
        
    }//!namespace node
    
}//!namespace sv

//
#endif //SV_SKINNODE_H


