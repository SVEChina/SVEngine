//
// SVDeformImageMove.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_DEFORMIMAGEMOVE_H
#define SV_DEFORMIMAGEMOVE_H

#include "../base/SVGBase.h"
#include "../base/SVMap.h"
#include "../core/SVImageUsingMove.h"
#include "../mtl/SVTexture.h"
#include "../base/SVArray.h"

namespace sv {
    
    namespace util{
        //deform imagemove 算法
        class SVDeformImageMove : public SVGBase {
        public:
            SVDeformImageMove(SVInst *_app);
            
            ~SVDeformImageMove();
            
            void init(SVTexturePtr _tex,SVTexturePtr _texout);

            void update(f32 dt);
            
            void render();
            
            void setTagPoint(u32 _postion,V2 _point);
            
            void pointMove(V2 *_data);
            
            void setPoint(V2 *_data);
            
            s32 getWidth();
            
            s32 getHeight();
            
            V2 MSL(V2 point);
            
            void updatePointMSL();
            
            void setDeformSwitch(bool _swith);
            
        protected:
            SVTexturePtr m_pTexBg;
            SVMtlCorePtr m_pMtlBg;
            SVRenderMeshPtr m_pMeshBg;
            s32 m_tt_w;
            s32 m_tt_h;
            
            SVImageUsingMovePtr m_iump;
            V2 m_pointScreen[2400];//30*80
            s32 m_iIndexCount;
            u16 m_dataIndex[14400];//30*80*2*3
            SVMap<u32, V2> m_pointMap;
            V2 *m_dataPoint;
            s32 m_wPointCount;
            s32 m_hPointCont;
            
            s32 m_inw;
            s32 m_inh;
            bool m_flip;
            
            void _createScreenRectMesh(V2 *t_data,V2 *t_targetData);
            
            void _initPoint();
            
            void addPass(SVPassPtr _pass);
            
            void clearPass();
            
        protected:
            typedef SVArray<SVPassPtr> PASSPOOL;
            PASSPOOL m_passPool;
            //
            SVRenderTexturePtr m_fbo;
            bool is_swith;
            SVPassPtr m_pass1;
            SVPassPtr m_pass2;
        };

    }//!namespace util
    
}//!namespace sv

#endif /* SV_DEFORMIMAGEMOVE_H */
