//
// SVFaceImageUsingMoveNode.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//
#ifndef SVFACEIMAGEUSINGMOVENODE_H
#define SVFACEIMAGEUSINGMOVENODE_H

#include "SVNode.h"
#include "../base/SVMap.h"


namespace sv {
    
    namespace node {
        
        class SVFaceImageUsingMoveNode : public SVNode {
        public:
            SVFaceImageUsingMoveNode(SVInst *_app);
            
            virtual ~SVFaceImageUsingMoveNode();
            
            void init();
            
            void init(SVTexturePtr _tex,s32 _w,s32 _h,SVTexturePtr _outTex);
            
            void destroy();
            
            void update(f32 dt);
            
            void render();
            
            void addPass(SVPassPtr _pass);
            
            void clearPass();
            
            void setTagPoint(u32 _postion,V2 _point);
            
            void pointMove(V2 *_data);
            
            void setPoint(V2 *_data);
            
            V2 MSL(V2 point);
            
            void updatePointMSL();
            
        protected:
            SVRenderObjectPtr m_pRenderObj;
            SVRenderMeshPtr m_pMesh;
            SVMtlCorePtr m_pMtl;
            SVTexturePtr m_tex;
            SVImageUsingMovePtr m_iump;
            V2 m_pointScreen[20*30];
            s32 m_iIndexCount;
            u16 m_dataIndex[20*30*2*3];
            SVMap<u32, V2> m_pointMap;
            
            V2 *m_dataPoint;
            s32 m_wPointCount;
            s32 m_hPointCont;
            
            s32 m_width;
            s32 m_height;
            bool m_flip;
            
            void createScreenRectMesh(V2 *t_data,V2 *t_targetData);
            void initPoint();
            
        protected:
            typedef SVArray<SVPassPtr> PASSPOOL;
            PASSPOOL m_passPool;
            SVRenderTexturePtr m_fbo;
            
        };
        
    }//!namespace node
    
}//!namespace sv

#endif /* SVFACEIMAGEUSINGMOVENODE_H */
