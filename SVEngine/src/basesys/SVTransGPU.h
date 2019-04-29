//
// SVTransGPU.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_TRANS_GPU_H
#define SV_TRANS_GPU_H

#include "SVTrans.h"
#include "../mtl/SVMtlDef.h"
#include "../node/SVNodeDeclare.h"

namespace sv {

    namespace logic {
        
        class SVTransGPU : public SVTrans {
        public:
            SVTransGPU(SVInst *_app,
                       SVTexturePtr _t0,
                       SVTexturePtr _t1,
                       SVTexturePtr _t2,
                       SVTexturePtr _tOut);
            
            virtual ~SVTransGPU();
            
            void init(s32 _w, s32 _h,f32 _angle,PICFORMATE _formate,SVTEXTYPE _tt);
            
            void destroy();
            
            void pushData(u8 *_srcPtr);
            
            void update(f32 dt);

            void setAngle(f32 _angle);
            
        protected:
            SVTexturePtr m_tex0;
            SVTexturePtr m_tex1;
            SVTexturePtr m_tex2;
            SVTexturePtr m_texOut;
            f32 m_angle;
            SVMultPassNodePtr m_passNode;
            SVRenderObjectPtr m_pRenderObj;
            SVRenderMeshPtr m_pMesh;
            bool  m_beload;
            SVMtlCorePtr m_pMtl;

            void createPass(s32 _w, s32 _h, f32 _angle, SVTEXTYPE _tt);
        };
        
    }
    
}//!namespace sv

#endif //SV_TRANS_GPU_H
