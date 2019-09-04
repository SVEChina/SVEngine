//
// SVFaceCoord.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_FACECOORD_H
#define SV_FACECOORD_H

#include "SVNode.h"
#include "../base/SVVec2.h"

namespace sv {
    
    namespace node{
        
        class SVFaceCoord : public SVNode {
        public:
            SVFaceCoord(SVInst *_app);
            
            virtual ~SVFaceCoord();
            
            virtual void update(f32 dt);
            
            virtual void render();
            
            bool loadCoord(cptr8 _fname);
            
            void refresh();
            
            void setPointSize(f32 _size);
            
            void active(s32 _pt);
            
            void unActive();
            
        protected:
            //激活点
            s32 m_activePt;
            //
            SVArray<FVec2> m_ptList;
            //
            SVRenderObjectPtr m_pRObjNor;
            SVRenderObjectPtr m_pRObjAct;
            //
            SVRenderMeshPtr m_pMesh;
            SVRenderMeshPtr m_pMeshAct;
            //
            SVLockPtr m_meshLock;
            //
            f32 m_size;
            //
            f32 m_width;
            f32 m_height;
            //
            bool m_isScreen;    //是否在屏幕空间
        };
        
    }//!namespace node
    
}//!namespace sv



#endif //SV_FACECOORD_H
