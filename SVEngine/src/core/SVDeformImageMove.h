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
            
            void updatePointMesh( V2* _facepoint);
            
            void reset();
            
            void setvisiPoint(bool _ispoint){
                m_is_point = _ispoint;
            }
            
            virtual void toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator,
                                RAPIDJSON_NAMESPACE::Value &_objValue);
            
            virtual void fromJSON(RAPIDJSON_NAMESPACE::Value &item);
            
        protected:
            void _refreshScreenRectMesh(V2 *t_data,V2 *t_targetData);
            
            void _initPoint();
            
            SVTexturePtr m_pTexBg;
            SVMtlCorePtr m_pMtlBg;
            SVRenderMeshPtr m_pMeshBg;
            s32 m_tt_w;
            s32 m_tt_h;
            
            SVImageUsingMovePtr m_pIUMP;
            
            V2 m_pointScreen[2400];     //30*80
            
            SVMap<u32, V2> m_pointMap;  //人脸偏移值
            
            V2 *m_dataPoint;
            s32 m_wPointCount;
            s32 m_hPointCont;
            
            SVPassPtr m_passDeform;
            SVPassPtr m_passPoint;
            //
            SVRenderTexturePtr m_fbo;
            bool is_swith;
            bool m_flip;
            bool m_is_point;
            
            SVTexturePtr m_pPointTex;
            SVRenderMeshPtr m_pMeshPoint;
            s32 m_inw;
            s32 m_inh;
       
        };

    }//!namespace util
    
}//!namespace sv

#endif /* SV_DEFORMIMAGEMOVE_H */
