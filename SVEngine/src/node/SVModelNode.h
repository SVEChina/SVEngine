//
// SVSkinNode.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MODELNODE_H
#define SV_MODELNODE_H

#include "SVNode.h"

namespace sv {
    
    namespace node{
        //
        class SVModelNode : public SVNode {
        public:
            SVModelNode(SVInst *_app);
            
            ~SVModelNode();

            //更新
            virtual void update(f32 dt);
   
            //渲染
            virtual void render();
            
            void enableDebugNormal(bool _enable);
            
            bool getDebugNormalEnable();
            
            void setModel(SVModelPtr _model);
            
            SVModelPtr getModel();
            
        protected:
            void _showDebugNormalLines();
            
            SVMultMeshMtlRenderObjectPtr m_pRObj;
            
            bool m_enableDebugNormal;
            
            SVModelPtr m_pModel;
        };
        
    }//!namespace node
    
}//!namespace sv



//
#endif //SV_MODELNODE_H


